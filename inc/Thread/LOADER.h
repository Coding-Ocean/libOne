#include"../NAME.h"
#include"THREAD.h"
#include"MUTEX.h"

class File{
public:
	bool isReady() const; //���[�h�I����Ă�H
	int size() const; //�t�@�C���T�C�Y�擾
	const char* data() const; //���g�擾
    const NAME& name() const;
private:
	friend class LOADER;
	File( const char* filename );
	~File();
	NAME Name;
	char* Data;
	int Size;
};

//LOADER::update���ĂԂ����̐l
class LOADING_THREAD : public THREAD{
public:
	LOADING_THREAD(){ start(); }
	~LOADING_THREAD(){ wait(); }
	void operator()();
};

class LOADER{
public:
	static LOADER* instance();
	static void create();
	static void destroy();

	void createFile( File**, const char* filename );
	void destroyFile( File** );
	void update( bool* endOut ); //�I��鎞��endOut��true������
private:
	LOADER();
	LOADER( const LOADER& ); //����
	~LOADER();

	//�X���b�h�̓s���ɂ��z��ɂ���B
	static const int MAX_FILE_NUMBER = 100;
	File* Files[ MAX_FILE_NUMBER ];
	
	LOADING_THREAD* LoadingThread; //�X���b�h
	MUTEX Mutex;
	bool EndRequest; //�I���Ƃ���true�ɂ���B
	//static const int READ_UNIT = 1 * 1024 * 1024; //1MB

	static LOADER* Instance;
};