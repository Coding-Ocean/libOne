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
	return ( Data != 0 ); //���[�h���I���܂Ńf�[�^�ɒl�͓���Ȃ��̂ł���ŃI�[�P�[
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
//LOADER::update���ĂԂ����̐l
void LOADING_THREAD::operator()(){
	bool end = false;
	while ( !end ){ //�������[�v
		Sleep( 1 ); //������Ƃɂ��΂炭�Q��B�O���O�����̂�h�������B
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
	//0����
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		Files[ i ] = 0;
	}
}

LOADER::~LOADER(){
	//�X���b�h���~�߂�
	Mutex.lock();
	EndRequest = true;
	Mutex.unlock();
	//�X���b�h��delete�B����wait()���ďI����҂�
	SAFE_DELETE( LoadingThread );
	//���X�g�𖕎E
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
	//�󂢂Ă�ꏊ��T��
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		if ( !Files[ i ] ){
			*f = new File( filename );
			Mutex.lock(); //�����O�ɓ���
			Files[ i ] = *f;
			Mutex.unlock();
			break;
		}
	}
}

void LOADER::destroyFile( File** f ){
	if ( !( *f ) ){ //���ł�0�B��邱�ƂȂ��B
		return;
	}
	//�T��
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		if ( Files[ i ] == *f ){
			Mutex.lock(); //�����O�ɓ���
			SAFE_DELETE( Files[ i ] );
			Mutex.unlock();
			*f = 0; //�󂯎��|�C���^��0��
			break;
		}
	}
	WARNING( ( *f ), "can't find this pointer in the list. there must be a BUG!", "" );
}

void LOADER::update( bool* endOut ){
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		NAME filename; //�t�@�C���������o�����߂ɁB

		Mutex.lock();
		if ( EndRequest ){ //�I���ƌ����Ă���B���̃��[�v�Ŕ����܂��B
			*endOut = true;
		}
		if ( Files[ i ] && !Files[ i ]->isReady() ){
			filename = Files[ i ]->Name;
		}
		Mutex.unlock(); //���[�h�I�����ǂ����������Ĉ��o��

		if ( filename != "" ){ //�t�@�C�����������Ă�B���[�h����Ƃ������Ƃ��B
			int size;
			char* data = 0;
			//��U���[�J���̕ϐ��Ƀ��[�h����B��������΃N���e�B�J���Z�N�V�����ɓ��炸�ɍςށB
			ifstream in( (char*)filename, ifstream::binary );
			in.seekg( 0, ifstream::end );
			size = static_cast< int >( in.tellg() ); 
			in.seekg( 0, ifstream::beg );
			data = new char[ size ];
			in.read( data, size );
			//���[�h���I����Ă���N���e�B�J���Z�N�V�����ɓ���B������ςȂ����ƌ��������\��������B
			Mutex.lock();
			if ( Files[ i ] ){ //�����Ȃ���������Ȃ��B�`�F�b�N�B
				Files[ i ]->Data = data;
				data = 0; //�Ǘ�����n�����̂�����0�ɁB
				Files[ i ]->Size = size;
                cout << "loaded" << i << '\n';
			}
			Mutex.unlock();
			if ( data ){ //�����f�[�^���܂�����Ƃ������Ƃ́A���if�ɓ���Ȃ������Ƃ������Ƃ�����j���B
				//delete�͒x���̂ŃN���e�B�J���Z�N�V�����ɓ��ꂽ���Ȃ������̂��B
				SAFE_DELETE_ARRAY( data );
			}
		}
	}
}
