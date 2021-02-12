#include "common.h"
#include "sound.h"
#include "window.h"
#include "package.h"
#include <dsound.h>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

static IDirectSound8* Ds = 0;
static vector<IDirectSoundBuffer*>* Dsb = 0;

//DirectSound初期化
void initSound(){
    HRESULT hr;
	CoInitialize( 0 );
    hr = DirectSoundCreate8(0, &Ds, 0 );
    WARNING( FAILED( hr ), "サウンドカードが無い","" );
    // 協調レベルの設定
    HWND hwnd = FindWindow( CLASS_NAME, 0 );
    hr = Ds->SetCooperativeLevel( hwnd, DSSCL_EXCLUSIVE );
    WARNING( FAILED( hr ), "協調レベル設定失敗","" );
	//データ
    Dsb = new vector<IDirectSoundBuffer*>;
}

void freeSound(){
    if( Dsb ){
        for( unsigned i = 0; i < Dsb->size(); i++ ){
            SAFE_RELEASE( Dsb->at( i ) );
        }
        SAFE_DELETE( Dsb );
    }
	SAFE_RELEASE( Ds );
	CoUninitialize();
}

int loadSound(const char* fileName) {
    if (packageData()) {
        return loadSoundFromPak(fileName);
    }
    else {
        //①まず使用できるWAVデータかチェックする。結構長い。
        HMMIO hmmio = 0;//WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPIでありDirectXではない）
        DWORD waveSize = 0;//WAVファイル内　WAVデータのサイズ（WAVファイルはWAVデータで占められているので、ほぼファイルサイズと同一）
        WAVEFORMATEX* waveFormat = 0;//WAVのフォーマット 例）16ビット、44100Hz、ステレオなど
        MMCKINFO ckInfo;//　チャンク情報
        MMCKINFO riffCkInfo;// 最上部チャンク（RIFFチャンク）保存用
        PCMWAVEFORMAT pcmWaveFormat;

        //  WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
        hmmio = mmioOpenA((char*)fileName, 0, MMIO_ALLOCBUF | MMIO_READ);
        if (hmmio == 0) {
            string msg = "WAVロードエラー\n";
            msg += fileName;
            WARNING(1, msg.c_str(), "");
        }
        //  ファイルポインタをRIFFチャンクの先頭にセットする
        if ((MMSYSERR_NOERROR != mmioDescend(hmmio, &riffCkInfo, NULL, 0))) {
            WARNING(1, "WAV 最上部チャンクに進入出来ません", "");
        }
        //  チャンク情報によりWAVファイルかどうか確認する
        if ((riffCkInfo.ckid != mmioFOURCC('R', 'I', 'F', 'F')) ||
            (riffCkInfo.fccType != mmioFOURCC('W', 'A', 'V', 'E'))) {
            WARNING(1, "これはWAVファイルではありません", "");
        }
        //  ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
        ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
        if (MMSYSERR_NOERROR != mmioDescend(hmmio, &ckInfo, &riffCkInfo, MMIO_FINDCHUNK)) {
            WARNING(1, "fmt チャンクが見つかりません", "");
        }
        //  フォーマットを読み込む
        if (mmioRead(hmmio, (HPSTR)&pcmWaveFormat, sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat)) {
            WARNING(1, "WAVフォーマットの読み込み失敗", "");
        }
        //  リニアPCMで、かつ、マルチチャンネルWAVは想定外
        if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM) {
            waveFormat = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
            if (NULL == waveFormat) {
                WARNING(1, "想定外WAVです", "");
            }
            memcpy(waveFormat, &pcmWaveFormat, sizeof(pcmWaveFormat));
            waveFormat->cbSize = 0;
        }
        else {
            WARNING(1, "標準のリニアPCMフォーマットを想定しています", "");
        }
        if (MMSYSERR_NOERROR != mmioAscend(hmmio, &ckInfo, 0)) {
            SAFE_DELETE(waveFormat);
            WARNING(1, "", "");
        }
        //  WAVファイル内の音データの読み込み
        ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
        if (MMSYSERR_NOERROR != mmioDescend(hmmio, &ckInfo, &riffCkInfo, MMIO_FINDCHUNK)) {
            WARNING(1, "dataチャンクが見つかりません", "");
        }
        waveSize = ckInfo.cksize;
        mmioClose(hmmio, 0);
        //②やっとこさDirectSoundセカンダリーバッファー作成
        IDirectSoundBuffer* dsb;//push_back用テンポラリィ
        DSBUFFERDESC dsbd;
        ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
        dsbd.dwSize = sizeof(DSBUFFERDESC);
        dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
        dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
        dsbd.dwBufferBytes = waveSize;
        dsbd.lpwfxFormat = waveFormat;
        if (FAILED(Ds->CreateSoundBuffer(&dsbd, &dsb, NULL))) {
            WARNING(1, "セカンダリバッファー作成失敗", "");
        }
        SAFE_DELETE(waveFormat);
        //波形データを書き込む準備として、セカンダリーバッファーをロックする
        void* buffer = 0;
        DWORD bufferSize = 0;
        if (FAILED(dsb->Lock(0, waveSize, &buffer, &bufferSize, NULL, NULL, 0))) {
            WARNING(1, "サウンドバッファーロックできません", "");
        }
        //ロックしたバッファーサイズが実際の波形データより大きい場合。クラッシュ防止。
        DWORD size = bufferSize;
        if (size > ckInfo.cksize) {
            size = ckInfo.cksize;
        }
        //バッファーに音データを書き込む   
        FILE* fp;
        fopen_s(&fp, fileName, "rb");
        fseek(fp, riffCkInfo.dwDataOffset + sizeof(FOURCC), SEEK_SET);
        BYTE* waveData = new BYTE[size];
        fread(waveData, 1, size, fp);//まずは、一時的な場所(waveData)に波形データを読み込み
        for (DWORD i = 0; i < size; i++) {
            *((BYTE*)buffer + i) = *((BYTE*)waveData + i); //一時的な場所の波形データをセカンダリバッファーにコピーする
        }
        fclose(fp);
        SAFE_DELETE(waveData);//一時的な波形データを開放する
        dsb->Unlock(buffer, bufferSize, 0, 0);
        //soundバッファの追加または使いまわし
        unsigned idx = 0;
        for (idx = 0; idx < Dsb->size(); idx++) {
            if (Dsb->at(idx) == 0) {
                break;
            }
        }
        if (idx == Dsb->size()) {
            Dsb->push_back(dsb);
        }
        else {
            Dsb->at(idx) = dsb;
        }
        return idx;
    }
}

int duplicateSound( int i ){
    IDirectSoundBuffer* dsb;
    Ds->DuplicateSoundBuffer( Dsb->at( i ), &dsb );
    //soundバッファの追加または使いまわし
    unsigned idx = 0;
    for( idx = 0; idx < Dsb->size(); idx++ ){
        if( Dsb->at( idx ) == 0 ){
            break;
        }
    }
    if( idx == Dsb->size() ){
        Dsb->push_back( dsb );
    }
    else{
        Dsb->at( idx ) = dsb;
    }
    return idx;
}

void playSound( int i ){
    Dsb->at( i )->SetCurrentPosition( 40 );
    Dsb->at( i )->Play( 0, 0, 0 );
}

void playLoopSound( int i ){
    Dsb->at( i )->SetCurrentPosition( 40 );
    Dsb->at( i )->Play( 0, 0, DSBPLAY_LOOPING );
}

void stopSound( int i ){
    Dsb->at( i )->Stop();
}

void setVolume( int i, long volume ){
    Dsb->at( i )->SetVolume( volume );
}

void setPan( int i, int pan ){
    Dsb->at( i )->SetPan( pan );
}

void releaseSound( int i ){
    SAFE_RELEASE( Dsb->at( i ) );
}

int loadSoundFromPak( const char* fileName ){
    //データぱっけいじから先頭アドレスを取得する
    int waveDataSize = 0;
    const char* waveData = (const char*)getData(fileName, &waveDataSize);
    const char* pWaveData = waveData;
    //riffChunk----------------------------------
    char fourChar[ 5 ] = "1234";
    unsigned riffChunkSize = 0;
    memcpy( fourChar, pWaveData, 1*4 ); pWaveData+=4;
    WARNING( strcmp( fourChar, "RIFF" ) != 0, "RIFF形式でない", "" );
    //WARNING( checkFourChar( fourChar, "RIFF" ), "RIFF形式でない", "" );
    memcpy( &riffChunkSize, pWaveData, 4*1 ); pWaveData+=4;
    memcpy( fourChar, pWaveData, 1*4 ); pWaveData += 4;
    WARNING( strcmp( fourChar, "WAVE" ) != 0, "WAVE形式でない", "" );
    //fmtChunk-----------------------------------
    while((unsigned int)(pWaveData - waveData) < riffChunkSize+8 ){
        if( *(pWaveData+0) == 'f' &&
            *(pWaveData+1) == 'm' &&
            *(pWaveData+2) == 't' &&
            *(pWaveData+3) == ' ' ){
            break;
        }
        pWaveData++;
    }
    WARNING((unsigned int)(pWaveData - waveData) >= riffChunkSize+8, "fmt chunkがない", "" );
    memcpy( fourChar, pWaveData, 1*4 ); pWaveData += 4;
    WARNING( strcmp( fourChar, "fmt " ) != 0, "fmt chunkでない", "" );
    unsigned fmtChunkSize = 0;
    memcpy( &fmtChunkSize, pWaveData, 4*1 ); pWaveData += 4;
	WAVEFORMATEX waveFormat;
    memcpy( &waveFormat.wFormatTag, pWaveData, 2*1 ); pWaveData += 2;
    WARNING( waveFormat.wFormatTag != 1, "標準ＰＣＭデータのみ有効", "formatが1でない" );
    memcpy( &waveFormat.nChannels, pWaveData, 2*1 ); pWaveData += 2;
    memcpy( &waveFormat.nSamplesPerSec, pWaveData, 4*1 ); pWaveData += 4;
    memcpy( &waveFormat.nAvgBytesPerSec, pWaveData, 4*1 ); pWaveData += 4;
    memcpy( &waveFormat.nBlockAlign, pWaveData, 2*1 ); pWaveData += 2;
    memcpy( &waveFormat.wBitsPerSample, pWaveData, 2*1 ); pWaveData += 2;
    waveFormat.cbSize = 0;
    //dataChunk----------------------------------
    while( (unsigned int)(pWaveData - waveData) < riffChunkSize+8 ){
        if( *(pWaveData+0) == 'd' &&
            *(pWaveData+1) == 'a' &&
            *(pWaveData+2) == 't' &&
            *(pWaveData+3) == 'a' ){
            break;
        }
        pWaveData++;
    }
    WARNING( (unsigned int)(pWaveData - waveData) >= riffChunkSize+8, "data chunkがない", "" );
    memcpy( fourChar, pWaveData, 1*4 ); pWaveData += 4;
    WARNING( strcmp( fourChar, "data" ) != 0, "data chunkでない", "" );
    unsigned long dataChunkSize = 0;
    memcpy( &dataChunkSize, pWaveData, 4*1 ); pWaveData += 4;
    //DirectSoundセカンダリーバッファー作成
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof( DSBUFFERDESC ) );
    dsbd.dwSize = sizeof( DSBUFFERDESC );
    dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
    dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
    dsbd.dwBufferBytes = dataChunkSize;
    dsbd.lpwfxFormat = &waveFormat;
    IDirectSoundBuffer* dsb;//push_back用テンポラリィ
    HRESULT hr = Ds->CreateSoundBuffer( &dsbd, &dsb, 0 );
    WARNING( FAILED( hr ), "セカンダリバッファー作成失敗","" );
    //波形データを書き込む準備として、セカンダリーバッファーをロックする
    void* buffer = 0;
    DWORD bufferSize = 0;
    hr = dsb->Lock( 0, dataChunkSize, &buffer, &bufferSize, 0, 0, 0 );
    WARNING( FAILED( hr ), "サウンドバッファをロックできません","" );
    //ロックしたバッファーサイズが実際の波形データより大きい場合。クラッシュ防止。
    DWORD tempBufferSize = bufferSize;
    if( bufferSize > dataChunkSize ) tempBufferSize = dataChunkSize;
    //バッファに音データを流し込む
    memcpy( buffer, pWaveData, tempBufferSize );
    dsb->Unlock( buffer, bufferSize, 0, 0 );
    //soundバッファの追加または使いまわし
    unsigned idx = 0;
    for( idx = 0; idx < Dsb->size(); idx++ ){
        if( Dsb->at( idx ) == 0 ){
            break;
        }
    }
    if( idx == Dsb->size() ){
        Dsb->push_back( dsb );
    }
    else{
        Dsb->at( idx ) = dsb;
    }
    return idx;
}

/*
//play( "xxxx.wav" );とするだけで読み込んで再生する。
//ロードも自動。
//再生終了前に同じ音をplayしようとすると、無駄なロードはせずdsbをduplicateする。
#include"Sound.h"
#include"Common.h"
#include <windows.h>
#include <dsound.h>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

static IDirectSound8* Ds = 0;
static vector<IDirectSoundBuffer*> Dsb;
vector<char*> FileName;

//DirectSound初期化
void init(){
    HRESULT hr;
    hr = DirectSoundCreate8( 0, &Ds, 0 );
    WARNING( FAILED( hr ), "サウンドカードが無い？？そんな馬鹿な" );
    // 協調レベルの設定
    HWND hwnd = FindWindow( CLASS_NAME, 0 );
    hr = Ds->SetCooperativeLevel( hwnd, DSSCL_EXCLUSIVE );
    WARNING( FAILED( hr ), "協調レベル設定失敗" );
}

void free(){
    for( unsigned i = 0; i < *(Dsb).size(); i++ ){
        SAFE_RELEASE( Dsb->at( i ) );
        SAFE_DELETE_ARRAY( FileName[ i ] );
    }
	SAFE_RELEASE( Ds );
}

//リリースはこれに任せる
void autoReleaseSound(){
    for( unsigned i = 0; i < *(Dsb).size(); i++ ){
        if( Dsb->at( i ) ){
            DWORD status;
            Dsb->at( i )->GetStatus( &status );
            if( ( status & DSBSTATUS_PLAYING ) == 0 ){
                SAFE_RELEASE( Dsb->at( i ) );
                SAFE_DELETE_ARRAY( FileName[ i ] );
            }
        }
    }
}

void load( char* fileName, IDirectSoundBuffer** dsb );

void duplicate( int i, IDirectSoundBuffer** dsb );

void play( char* fileName ){
    IDirectSoundBuffer* dsb;
    unsigned i;
    for( i = 0; i < FileName.size(); i++ ){
        if( FileName[ i ] ){
            if( strcmp( FileName[ i ], fileName ) == 0 ){
                break;
            }
        }
    }
    if( i == FileName.size() ){
        //読み込んでないので読み込む
        load( fileName, &dsb );
    }
    else{
        //読み込み済みなのでコピー
        duplicate( i, &dsb );
    }
    dsb->SetCurrentPosition( 40 );
    dsb->Play( 0, 0, 0 );
}

void duplicate( int i, IDirectSoundBuffer** dsb ){
    Ds->DuplicateSoundBuffer( Dsb->at( i ), &dsb );
    //soundバッファの追加または使いまわし
    unsigned idx = 0;
    for( idx = 0; idx < *(Dsb).size(); idx++ ){
        if( Dsb[ idx ] == 0 ){
            break;
        }
    }
    if( idx == *(Dsb).size() ){
        *(Dsb).push_back( dsb ); 
        FileName.push_back( 0 );//オリジナルでないので名前はなし
    }
    else{
        Dsb[ idx ] = dsb;
    }
}

void load( char* fileName, IDirectSoundBuffer** dsb ){
	HMMIO hmmio = 0;//WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPIでありDirectXではない）
	DWORD waveSize = 0;//WAVファイル内　WAVデータのサイズ（WAVファイルはWAVデータで占められているので、ほぼファイルサイズと同一）
	WAVEFORMATEX* waveFormat;//WAVのフォーマット 例）16ビット、44100Hz、ステレオなど
	MMCKINFO ckInfo;//　チャンク情報
	MMCKINFO riffCkInfo;// 最上部チャンク（RIFFチャンク）保存用
	PCMWAVEFORMAT pcmWaveFormat;

    //データパス＋ファイル名 編集
    int l = strlen( DATA_SOUNDS_PATH ) + 1;
    l += strlen( fileName );
    char* pathFileName = new char[ l ];
    strcpy_s( pathFileName, l, DATA_SOUNDS_PATH );
    strcat_s( pathFileName, l, fileName );

    //WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
    hmmio = mmioOpen( pathFileName, 0, MMIO_ALLOCBUF | MMIO_READ );

    //ファイルポインタをRIFFチャンクの先頭にセットする
    if( ( MMSYSERR_NOERROR != mmioDescend( hmmio, &riffCkInfo, NULL, 0 ) ) ){
       WARNING( 1, "最初のチャンク（最上部チャンク)に進入できません" );
    }
    //チャンク情報によりWAVファイルかどうか確認する
    if( (riffCkInfo.ckid != mmioFOURCC( 'R', 'I', 'F', 'F' ) ) || 
        (riffCkInfo.fccType != mmioFOURCC( 'W', 'A', 'V', 'E' ) ) ){
       WARNING( 1, "これはWAVファイルではありません" );
    }
    // ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
    ckInfo.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
    if( MMSYSERR_NOERROR != mmioDescend( hmmio, &ckInfo, &riffCkInfo, MMIO_FINDCHUNK ) ){
       WARNING( 1, "fmt チャンクが見つかりません" );	   
    }
    //フォーマットを読み込む
    if( mmioRead( hmmio, (HPSTR)&pcmWaveFormat, sizeof( pcmWaveFormat ) ) != sizeof( pcmWaveFormat ) ){
       WARNING( 1, "WAVフォーマットの読み込み失敗" );
    }
    //リニアPCMで、かつ、マルチチャンネルWAVは想定外
    if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM ){
        waveFormat = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) ];
        if( NULL == waveFormat ){
		    WARNING( 1, "想定外です" );
	    }  
        memcpy( waveFormat, &pcmWaveFormat, sizeof(pcmWaveFormat) );
        waveFormat->cbSize = 0;
    }
    else{
	    WARNING( 1, "標準のリニアPCMフォーマットを想定しています" );
    }
    if( MMSYSERR_NOERROR != mmioAscend( hmmio, &ckInfo, 0 ) ){
        SAFE_DELETE( waveFormat );
        WARNING( 1, "" );
    }
    // WAVファイル内の音データの読み込み
    ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if( MMSYSERR_NOERROR != mmioDescend( hmmio, &ckInfo, &riffCkInfo, MMIO_FINDCHUNK ) ){
	    WARNING( 1, "dataチャンクが見つかりません" );
    }
    waveSize = ckInfo.cksize;

    // DirectSoundセカンダリーバッファー作成
    DSBUFFERDESC dsbd;  
    ZeroMemory( &dsbd, sizeof( DSBUFFERDESC ) );
    dsbd.dwSize = sizeof( DSBUFFERDESC );
    dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
    dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
    dsbd.dwBufferBytes = waveSize;
    dsbd.lpwfxFormat = waveFormat;
    if( FAILED( Ds->CreateSoundBuffer( &dsbd, dsb, NULL ) ) ){
       WARNING( 1, "セカンダリバッファー作成失敗" );
    }
    SAFE_DELETE( waveFormat );
    //dsb->QueryInterface();
    //波形データを書き込む準備として、セカンダリーバッファーをロックする
    VOID* buffer = 0;
    DWORD bufferSize = 0; 
    if( FAILED( (*dsb)->Lock( 0, waveSize, &buffer, &bufferSize, NULL, NULL, 0 ) ) ){
       WARNING( 1, "サウンドバッファーロックできません" );
    }
    //ロックしたバッファーサイズが実際の波形データより大きい場合。クラッシュ防止。
    DWORD size = bufferSize;
    if( size > ckInfo.cksize ){
       size = ckInfo.cksize;
    }
    //バッファーに音データを書き込む   
    FILE* fp; 
    fopen_s( &fp, pathFileName, "rb" );
    fseek( fp, riffCkInfo.dwDataOffset + sizeof( FOURCC ), SEEK_SET );
    BYTE* waveData = new BYTE[ size ];
    fread( waveData, 1, size, fp );//まずは、一時的な場所(waveData)に波形データを読み込み
    for( DWORD i = 0; i < size; i++ ){
       *( (BYTE*)buffer + i) = *( (BYTE*) waveData + i ); //一時的な場所の波形データをセカンダリバッファーにコピーする
    }
    fclose( fp );
    SAFE_DELETE( waveData);//一時的な波形データを開放する
    SAFE_DELETE_ARRAY( pathFileName );
    (*dsb)->Unlock( buffer, bufferSize, 0, 0 );

    //soundバッファの追加または使いまわし
    unsigned idx = 0;
    for( idx = 0; idx < *(Dsb).size(); idx++ ){
        if( Dsb[ idx ] == 0 ){
            break;
        }
    }
    if( idx == *(Dsb).size() ){
        *(Dsb).push_back( *dsb );
        int l = strlen( fileName ) + 1;
        char* name = new char[ l ];
        strcpy_s( name, l, fileName );
        FileName.push_back( name );
    }
    else{
        Dsb[ idx ] = *dsb;
        int l = strlen( fileName ) + 1;
        FileName[ idx ] = new char[ l ];
        strcpy_s( FileName[ idx ], l, fileName );
    }
}
*/
