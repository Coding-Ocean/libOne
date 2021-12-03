#include"../NAME.h"
#include"THREAD.h"
#include"MUTEX.h"

class File{
public:
	bool isReady() const; //ロード終わってる？
	int size() const; //ファイルサイズ取得
	const char* data() const; //中身取得
    const NAME& name() const;
private:
	friend class LOADER;
	File( const char* filename );
	~File();
	NAME Name;
	char* Data;
	int Size;
};

//LOADER::updateを呼ぶだけの人
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
	void update( bool* endOut ); //終わる時はendOutにtrueを入れる
private:
	LOADER();
	LOADER( const LOADER& ); //封印
	~LOADER();

	//スレッドの都合により配列にする。
	static const int MAX_FILE_NUMBER = 100;
	File* Files[ MAX_FILE_NUMBER ];
	
	LOADING_THREAD* LoadingThread; //スレッド
	MUTEX Mutex;
	bool EndRequest; //終わるときにtrueにする。
	//static const int READ_UNIT = 1 * 1024 * 1024; //1MB

	static LOADER* Instance;
};