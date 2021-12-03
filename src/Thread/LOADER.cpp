#include<iostream>
#include<fstream>
#include<Windows.h>
#include"common.h"
#include"thread/LOADER.h"
using namespace std;

File::File( const char* filename ) : 
    Name( filename ),
    Data( 0 ),
    Size( 0 ){
}

File::~File(){
	if ( Data ){
		SAFE_DELETE_ARRAY( Data );
	}
	Size = 0;
}

bool File::isReady() const {
	return ( Data != 0 ); //ロードが終わるまでデータに値は入らないのでこれでオーケー
}

int File::size() const {
	WARNING( Data == 0, "Loading is not finished. ", "" );
	return Size;
}

const char* File::data() const {
	WARNING( Data == 0, "Loading is not finished. ", "" );
	return Data;
}

const NAME& File::name() const {
	WARNING( Data == 0, "Loading is not finished. ", "" );
    return Name;
}

//-----------------------------------------------------------------------------
//LOADER::updateを呼ぶだけの人
void LOADING_THREAD::operator()(){
	bool end = false;
	while ( !end ){ //無限ループ
		Sleep( 1 ); //一周ごとにしばらく寝る。グルグル回るのを防ぎたい。
		LOADER::instance()->update( &end );
	}
}

//-----------------------------------------------------------------------------
LOADER* LOADER::Instance = 0;

LOADER::LOADER() : 
    LoadingThread( 0 ), 
    EndRequest( false ) {
	LoadingThread = new LOADING_THREAD;
	Mutex = MUTEX::create();
	//0うめ
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		Files[ i ] = 0;
	}
}

LOADER::~LOADER(){
	//スレッドを止める
	Mutex.lock();
	EndRequest = true;
	Mutex.unlock();
	//スレッドをdelete。中でwait()して終了を待つ
	SAFE_DELETE( LoadingThread );
	//リストを抹殺
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		if ( Files[ i ] ){
			SAFE_DELETE( Files[ i ] );
		}
	}
}

LOADER* LOADER::instance(){
	return Instance;
}

void LOADER::create(){
	WARNING( Instance, "already created.", "" );
	Instance = new LOADER();
}

void LOADER::destroy(){
	SAFE_DELETE( Instance );
}

void LOADER::createFile( File** f, const char* filename ){
	WARNING( ( *f ), "pointer must be NUL. ", "" );
	//空いてる場所を探す
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		if ( !Files[ i ] ){
			*f = new File( filename );
			Mutex.lock(); //足す前に入る
			Files[ i ] = *f;
			Mutex.unlock();
			break;
		}
	}
}

void LOADER::destroyFile( File** f ){
	if ( !( *f ) ){ //すでに0。やることない。
		return;
	}
	//探す
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		if ( Files[ i ] == *f ){
			Mutex.lock(); //消す前に入る
			SAFE_DELETE( Files[ i ] );
			Mutex.unlock();
			*f = 0; //受け取りポインタを0に
			break;
		}
	}
	WARNING( ( *f ), "can't find this pointer in the list. there must be a BUG!", "" );
}

void LOADER::update( bool* endOut ){
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		NAME filename; //ファイル名を取り出すために。

		Mutex.lock();
		if ( EndRequest ){ //終われと言われている。次のループで抜けます。
			*endOut = true;
		}
		if ( Files[ i ] && !Files[ i ]->isReady() ){
			filename = Files[ i ]->Name;
		}
		Mutex.unlock(); //ロード終了かどうかだけ見て一回出る

		if ( filename != "" ){ //ファイル名が入ってる。ロードしろということだ。
			int size;
			char* data = 0;
			//一旦ローカルの変数にロードする。そうすればクリティカルセクションに入らずに済む。
			ifstream in( (char*)filename, ifstream::binary );
			in.seekg( 0, ifstream::end );
			size = static_cast< int >( in.tellg() ); 
			in.seekg( 0, ifstream::beg );
			data = new char[ size ];
			in.read( data, size );
			//ロードが終わってからクリティカルセクションに入る。入りっぱなしだと激しく性能が落ちる。
			Mutex.lock();
			if ( Files[ i ] ){ //もうないかもしれない。チェック。
				Files[ i ]->Data = data;
				data = 0; //管理権を渡したのだから0に。
				Files[ i ]->Size = size;
                cout << "loaded" << i << '\n';
			}
			Mutex.unlock();
			if ( data ){ //もしデータがまだあるということは、上のifに入れなかったということだから破棄。
				//deleteは遅いのでクリティカルセクションに入れたくなかったのだ。
				SAFE_DELETE_ARRAY( data );
			}
		}
	}
}
