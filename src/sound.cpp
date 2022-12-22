#include <dsound.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <memory.h>
#include "common.h"
#include "window.h"
#include "package.h"
#include "sound.h"
using namespace std;

static IDirectSound8* Ds = 0;
static vector<IDirectSoundBuffer*>* Dsb = 0;
static vector<string>* Filenames = 0;

//DirectSound������
void initSound(){
    HRESULT hr;
	CoInitialize( 0 );
    hr = DirectSoundCreate8(0, &Ds, 0 );
    WARNING( FAILED( hr ), "�T�E���h�J�[�h������","" );
    // �������x���̐ݒ�
    HWND hwnd = FindWindow( CLASS_NAME, 0 );
    hr = Ds->SetCooperativeLevel( hwnd, DSSCL_EXCLUSIVE );
    WARNING( FAILED( hr ), "�������x���ݒ莸�s","" );
	//�f�[�^
    Dsb = new vector<IDirectSoundBuffer*>;
    Filenames = new vector<string>;
}

void freeSound(){
    SAFE_DELETE(Filenames);
    if( Dsb ){
        for( unsigned i = 0; i < Dsb->size(); i++ ){
            SAFE_RELEASE( Dsb->at( i ) );
        }
        SAFE_DELETE( Dsb );
    }
	SAFE_RELEASE( Ds );
	CoUninitialize();
}

//�p�b�P�[�W�ǂݍ��ݑΉ���
int loadSound(const char* fileName) {
    //�ǂݍ��݂̏d����������
    unsigned i;
    for (i = 0; i<Filenames->size(); i++) {
        if (Filenames->at(i) == fileName) {
            return i;
        }
    }
    //�t�@�C���܂��̓p�b�P�[�W����ǂݍ���
    int waveDataSize = 0;
    char* waveData = 0;
    if (packageDataExists() == false) {
        //�p�b�P�[�W�f�[�^���Ȃ��̂ŁA���ʂɃt�@�C������ǂݍ���
        FILE* fp=0;
        fopen_s(&fp, fileName, "rb");
        WARNING(fp == 0, "���[�h�G���[", fileName);
        struct stat info;
        fstat(_fileno(fp), &info);
        waveDataSize = info.st_size;
        waveData = new char[waveDataSize];
        fread(waveData, 1, waveDataSize, fp);//�܂��́A�ꎞ�I�ȏꏊ(waveData)�ɔg�`�f�[�^��ǂݍ���
        fclose(fp);
    }
    else {
        //�f�[�^�p�b�P�[�W����擪�A�h���X���擾����
        waveData = (char*)getPackageData(fileName, &waveDataSize);
    }

    //pWaveData���ǂݍ��݈ʒu��i�߂邽�߂̃|�C���^
    const char* pWaveData = waveData;

    //riffChunk----------------------------------
    unsigned riffChunkSize = 0;
    WARNING(strncmp(pWaveData, "RIFF", 4) != 0, "RIFF�`���łȂ�", ""); pWaveData += 4;
    memcpy(&riffChunkSize, pWaveData, 4 * 1); pWaveData += 4;
    WARNING(strncmp(pWaveData, "WAVE", 4) != 0, "WAVE�`���łȂ�", ""); pWaveData += 4;

    //fmtChunk-----------------------------------
    //Search "fmt "
    while ((unsigned int)(pWaveData - waveData) < riffChunkSize + 8) {
        if (*(pWaveData + 0) == 'f' &&
            *(pWaveData + 1) == 'm' &&
            *(pWaveData + 2) == 't' &&
            *(pWaveData + 3) == ' ') {
            break;
        }
        pWaveData++;
    }
    WARNING((unsigned int)(pWaveData - waveData) >= riffChunkSize + 8, "fmt chunk���Ȃ�", "");
    pWaveData += 4;

    unsigned fmtChunkSize = 0;
    memcpy(&fmtChunkSize, pWaveData, 4 * 1); pWaveData += 4;
    WAVEFORMATEX waveFormat;
    memcpy(&waveFormat.wFormatTag, pWaveData, 2 * 1); pWaveData += 2;
    WARNING(waveFormat.wFormatTag != 1, "�W���o�b�l�f�[�^�̂ݗL��", "format��1�łȂ�");
    memcpy(&waveFormat.nChannels, pWaveData, 2 * 1); pWaveData += 2;
    memcpy(&waveFormat.nSamplesPerSec, pWaveData, 4 * 1); pWaveData += 4;
    memcpy(&waveFormat.nAvgBytesPerSec, pWaveData, 4 * 1); pWaveData += 4;
    memcpy(&waveFormat.nBlockAlign, pWaveData, 2 * 1); pWaveData += 2;
    memcpy(&waveFormat.wBitsPerSample, pWaveData, 2 * 1); pWaveData += 2;
    waveFormat.cbSize = 0;

    //dataChunk----------------------------------
    //Serch "data"
    while ((unsigned int)(pWaveData - waveData) < riffChunkSize + 8) {
        if (*(pWaveData + 0) == 'd' &&
            *(pWaveData + 1) == 'a' &&
            *(pWaveData + 2) == 't' &&
            *(pWaveData + 3) == 'a') {
            break;
        }
        pWaveData++;
    }
    WARNING((unsigned int)(pWaveData - waveData) >= riffChunkSize + 8, "data chunk���Ȃ�", "");
    pWaveData += 4;

    unsigned long dataChunkSize = 0;
    memcpy(&dataChunkSize, pWaveData, 4 * 1); pWaveData += 4;

    //DirectSound�Z�J���_���[�o�b�t�@�[�쐬
    DSBUFFERDESC dsbd;
    ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
    dsbd.dwSize = sizeof(DSBUFFERDESC);
    dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
    dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
    dsbd.dwBufferBytes = dataChunkSize;
    dsbd.lpwfxFormat = &waveFormat;
    IDirectSoundBuffer* dsb;//push_back�p�e���|�����B
    HRESULT hr = Ds->CreateSoundBuffer(&dsbd, &dsb, 0);
    WARNING(FAILED(hr), "�Z�J���_���o�b�t�@�[�쐬���s", "");
    //�g�`�f�[�^���������ޏ����Ƃ��āA�Z�J���_���[�o�b�t�@�[�����b�N����
    void* buffer = 0;
    DWORD bufferSize = 0;
    hr = dsb->Lock(0, dataChunkSize, &buffer, &bufferSize, 0, 0, 0);
    WARNING(FAILED(hr), "�T�E���h�o�b�t�@�����b�N�ł��܂���", "");
    //���b�N�����o�b�t�@�[�T�C�Y�����ۂ̔g�`�f�[�^���傫���ꍇ�B�N���b�V���h�~�B
    DWORD tempBufferSize = bufferSize;
    if (bufferSize > dataChunkSize) tempBufferSize = dataChunkSize;
    //�o�b�t�@�ɉ��f�[�^�𗬂�����
    memcpy(buffer, pWaveData, tempBufferSize);
    dsb->Unlock(buffer, bufferSize, 0, 0);

    //�t�@�C������ǂݍ��񂾏ꍇ�͂�����waveData�J��
    if (packageDataExists() == false) {
        SAFE_DELETE_ARRAY(waveData);//�ꎞ�I�Ȕg�`�f�[�^���J������
    }

    //sound�o�b�t�@�̒ǉ��܂��͎g���܂킵
    unsigned idx = 0;
    for (idx = 0; idx < Dsb->size(); idx++) {
        if (Dsb->at(idx) == 0) {
            break;
        }
    }
    if (idx == Dsb->size()) {
        Dsb->push_back(dsb);
        Filenames->push_back(fileName);
    }
    else {
        Dsb->at(idx) = dsb;
        Filenames->at(idx) = fileName;
    }
    return idx;
}

int duplicateSound( int i ){
    IDirectSoundBuffer* dsb;
    Ds->DuplicateSoundBuffer( Dsb->at( i ), &dsb );
    //sound�o�b�t�@�̒ǉ��܂��͎g���܂킵
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

void releaseAllSound()
{
    for (unsigned i = 0; i < Dsb->size(); i++) {
        SAFE_RELEASE(Dsb->at(i));
    }
    Dsb->clear();
    Filenames->clear();
}

/*
//play( "xxxx.wav" );�Ƃ��邾���œǂݍ���ōĐ�����B
//���[�h�������B
//�Đ��I���O�ɓ�������play���悤�Ƃ���ƁA���ʂȃ��[�h�͂���dsb��duplicate����B
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

//DirectSound������
void init(){
    HRESULT hr;
    hr = DirectSoundCreate8( 0, &Ds, 0 );
    WARNING( FAILED( hr ), "�T�E���h�J�[�h�������H�H����Ȕn����" );
    // �������x���̐ݒ�
    HWND hwnd = FindWindow( CLASS_NAME, 0 );
    hr = Ds->SetCooperativeLevel( hwnd, DSSCL_EXCLUSIVE );
    WARNING( FAILED( hr ), "�������x���ݒ莸�s" );
}

void free(){
    for( unsigned i = 0; i < *(Dsb).size(); i++ ){
        SAFE_RELEASE( Dsb->at( i ) );
        SAFE_DELETE_ARRAY( FileName[ i ] );
    }
	SAFE_RELEASE( Ds );
}

//�����[�X�͂���ɔC����
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
        //�ǂݍ���łȂ��̂œǂݍ���
        load( fileName, &dsb );
    }
    else{
        //�ǂݍ��ݍς݂Ȃ̂ŃR�s�[
        duplicate( i, &dsb );
    }
    dsb->SetCurrentPosition( 40 );
    dsb->Play( 0, 0, 0 );
}

void duplicate( int i, IDirectSoundBuffer** dsb ){
    Ds->DuplicateSoundBuffer( Dsb->at( i ), &dsb );
    //sound�o�b�t�@�̒ǉ��܂��͎g���܂킵
    unsigned idx = 0;
    for( idx = 0; idx < *(Dsb).size(); idx++ ){
        if( Dsb[ idx ] == 0 ){
            break;
        }
    }
    if( idx == *(Dsb).size() ){
        *(Dsb).push_back( dsb ); 
        FileName.push_back( 0 );//�I���W�i���łȂ��̂Ŗ��O�͂Ȃ�
    }
    else{
        Dsb[ idx ] = dsb;
    }
}

void load( char* fileName, IDirectSoundBuffer** dsb ){
	HMMIO hmmio = 0;//Windows�}���`���f�B�AAPI�̃n���h��(Windows�}���`���f�B�AAPI��WAV�t�@�C���֌W�̑���p��API�ł���DirectX�ł͂Ȃ��j
	DWORD waveSize = 0;//WAV�t�@�C�����@WAV�f�[�^�̃T�C�Y�iWAV�t�@�C����WAV�f�[�^�Ő�߂��Ă���̂ŁA�قڃt�@�C���T�C�Y�Ɠ���j
	WAVEFORMATEX* waveFormat;//WAV�̃t�H�[�}�b�g ��j16�r�b�g�A44100Hz�A�X�e���I�Ȃ�
	MMCKINFO ckInfo;//�@�`�����N���
	MMCKINFO riffCkInfo;// �ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
	PCMWAVEFORMAT pcmWaveFormat;

    //�f�[�^�p�X�{�t�@�C���� �ҏW
    int l = strlen( DATA_SOUNDS_PATH ) + 1;
    l += strlen( fileName );
    char* pathFileName = new char[ l ];
    strcpy_s( pathFileName, l, DATA_SOUNDS_PATH );
    strcat_s( pathFileName, l, fileName );

    //WAV�t�@�C�����̃w�b�_�[���i���f�[�^�ȊO�j�̊m�F�Ɠǂݍ���
    hmmio = mmioOpen( pathFileName, 0, MMIO_ALLOCBUF | MMIO_READ );

    //�t�@�C���|�C���^��RIFF�`�����N�̐擪�ɃZ�b�g����
    if( ( MMSYSERR_NOERROR != mmioDescend( hmmio, &riffCkInfo, NULL, 0 ) ) ){
       WARNING( 1, "�ŏ��̃`�����N�i�ŏ㕔�`�����N)�ɐi���ł��܂���" );
    }
    //�`�����N���ɂ��WAV�t�@�C�����ǂ����m�F����
    if( (riffCkInfo.ckid != mmioFOURCC( 'R', 'I', 'F', 'F' ) ) || 
        (riffCkInfo.fccType != mmioFOURCC( 'W', 'A', 'V', 'E' ) ) ){
       WARNING( 1, "�����WAV�t�@�C���ł͂���܂���" );
    }
    // �t�@�C���|�C���^��'f' 'm' 't' ' ' �`�����N�ɃZ�b�g����
    ckInfo.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
    if( MMSYSERR_NOERROR != mmioDescend( hmmio, &ckInfo, &riffCkInfo, MMIO_FINDCHUNK ) ){
       WARNING( 1, "fmt �`�����N��������܂���" );	   
    }
    //�t�H�[�}�b�g��ǂݍ���
    if( mmioRead( hmmio, (HPSTR)&pcmWaveFormat, sizeof( pcmWaveFormat ) ) != sizeof( pcmWaveFormat ) ){
       WARNING( 1, "WAV�t�H�[�}�b�g�̓ǂݍ��ݎ��s" );
    }
    //���j�APCM�ŁA���A�}���`�`�����l��WAV�͑z��O
    if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM ){
        waveFormat = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) ];
        if( NULL == waveFormat ){
		    WARNING( 1, "�z��O�ł�" );
	    }  
        memcpy( waveFormat, &pcmWaveFormat, sizeof(pcmWaveFormat) );
        waveFormat->cbSize = 0;
    }
    else{
	    WARNING( 1, "�W���̃��j�APCM�t�H�[�}�b�g��z�肵�Ă��܂�" );
    }
    if( MMSYSERR_NOERROR != mmioAscend( hmmio, &ckInfo, 0 ) ){
        SAFE_DELETE( waveFormat );
        WARNING( 1, "" );
    }
    // WAV�t�@�C�����̉��f�[�^�̓ǂݍ���
    ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if( MMSYSERR_NOERROR != mmioDescend( hmmio, &ckInfo, &riffCkInfo, MMIO_FINDCHUNK ) ){
	    WARNING( 1, "data�`�����N��������܂���" );
    }
    waveSize = ckInfo.cksize;

    // DirectSound�Z�J���_���[�o�b�t�@�[�쐬
    DSBUFFERDESC dsbd;  
    ZeroMemory( &dsbd, sizeof( DSBUFFERDESC ) );
    dsbd.dwSize = sizeof( DSBUFFERDESC );
    dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
    dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
    dsbd.dwBufferBytes = waveSize;
    dsbd.lpwfxFormat = waveFormat;
    if( FAILED( Ds->CreateSoundBuffer( &dsbd, dsb, NULL ) ) ){
       WARNING( 1, "�Z�J���_���o�b�t�@�[�쐬���s" );
    }
    SAFE_DELETE( waveFormat );
    //dsb->QueryInterface();
    //�g�`�f�[�^���������ޏ����Ƃ��āA�Z�J���_���[�o�b�t�@�[�����b�N����
    VOID* buffer = 0;
    DWORD bufferSize = 0; 
    if( FAILED( (*dsb)->Lock( 0, waveSize, &buffer, &bufferSize, NULL, NULL, 0 ) ) ){
       WARNING( 1, "�T�E���h�o�b�t�@�[���b�N�ł��܂���" );
    }
    //���b�N�����o�b�t�@�[�T�C�Y�����ۂ̔g�`�f�[�^���傫���ꍇ�B�N���b�V���h�~�B
    DWORD size = bufferSize;
    if( size > ckInfo.cksize ){
       size = ckInfo.cksize;
    }
    //�o�b�t�@�[�ɉ��f�[�^����������   
    FILE* fp; 
    fopen_s( &fp, pathFileName, "rb" );
    fseek( fp, riffCkInfo.dwDataOffset + sizeof( FOURCC ), SEEK_SET );
    BYTE* waveData = new BYTE[ size ];
    fread( waveData, 1, size, fp );//�܂��́A�ꎞ�I�ȏꏊ(waveData)�ɔg�`�f�[�^��ǂݍ���
    for( DWORD i = 0; i < size; i++ ){
       *( (BYTE*)buffer + i) = *( (BYTE*) waveData + i ); //�ꎞ�I�ȏꏊ�̔g�`�f�[�^���Z�J���_���o�b�t�@�[�ɃR�s�[����
    }
    fclose( fp );
    SAFE_DELETE( waveData);//�ꎞ�I�Ȕg�`�f�[�^���J������
    SAFE_DELETE_ARRAY( pathFileName );
    (*dsb)->Unlock( buffer, bufferSize, 0, 0 );

    //sound�o�b�t�@�̒ǉ��܂��͎g���܂킵
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
