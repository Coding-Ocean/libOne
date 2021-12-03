#ifndef CONTAINER_HD
#define CONTAINER_HD

#include"NAME.h"
#include"Thread/THREAD.h"
#include"DATA.h"
class VERTEX_BUFFER;
class INDEX_BUFFER;
class TEXTURE;
class BATCH;
class IMAGE;
class TREE_ORIGIN;
class ANIMATION;
class SOUND;

class CONTAINER{
public:
    enum HOW_TO_LOAD{ NOT_LOAD_NOW, LOAD_FROM_FILE, LOAD_FROM_FILE_THREAD };
    //すべてのグラフィックデータを読み込み、各オブジェクトを作成する
    CONTAINER( HOW_TO_LOAD howToLoad, const char* fileName );
    CONTAINER( const char* fileName );
    CONTAINER();
    void operator=( const char* fileName );
    void addData( const char* fileName );
    //各オブジェクトはすべてここで開放する
    ~CONTAINER();
    //BATCH作成時に使用するアクセッサ
    VERTEX_BUFFER* vertexBuffer( const char* name ) const;
    INDEX_BUFFER* indexBuffer( const char* name ) const;
    TEXTURE* texture( const char* name, int i=-1 ) const;
    //アクセッサ
	BATCH* batch( const char* name ) const;
	IMAGE* image( const char* name ) const;
    TREE_ORIGIN* treeOrigin( const char* name ) const;
    ANIMATION* animation( const char* name ) const;
    SOUND* sound( const char* name ) const;
    DATA& data( const char* name );
private:
    friend class CONTAINER_LOAD_THREAD;
    void Create( const char* fileName );
    void Destroy();
    //ファイル名からパス名を取り出す
    //void GetPathName( char* pathName, int length, const char* fileName );
    NAME FileName;
    //各オブジェクトのポインタ配列と数
    VERTEX_BUFFER** VertexBuffers=0;
    INDEX_BUFFER** IndexBuffers=0;
    TEXTURE** Textures=0;
    BATCH** Batches=0;
	//IMAGE** Images=0;
    TREE_ORIGIN** TreeOrigins=0;
    ANIMATION** Animations=0;
    //SOUND** Sounds=0;
    DATA** Data=0;
    int NumVertexBuffers=0;
    int NumIndexBuffers=0;
    int NumTextures=0;
	int NumBatches=0;
	int NumImages=0;
    int NumTreeOrigins=0;
    int NumAnimations=0;
    int NumSounds=0;
    int NumData=0;
};

//-----------------------------------------------------------------------------
class CONTAINER_LOAD_THREAD : public THREAD{
public:
    CONTAINER_LOAD_THREAD( CONTAINER* c );
    CONTAINER_LOAD_THREAD( CONTAINER* c, const char* filename );
	void operator()();
private:
    CONTAINER* C;
};

#endif
