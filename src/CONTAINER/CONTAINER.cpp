#include<stdio.h>
#include"common.h"
#include"FILE_BUFFER.h"
#include"CONTAINER/VERTEX_BUFFER.h"
#include"CONTAINER/INDEX_BUFFER.h"
#include"CONTAINER/TEXTURE.h"
#include"CONTAINER/BATCH.h"
//#include"CONTAINER/IMAGE.h"
#include"CONTAINER/TREE_ORIGIN.h"
#include"CONTAINER/ANIMATION.h"
#include"CONTAINER/DATA.h"
#include"CONTAINER/CONTAINER.h"
//#include"SOUND.h"

CONTAINER::CONTAINER( HOW_TO_LOAD howToLoad, const char* fileName ){
    switch( howToLoad ){
    case LOAD_FROM_FILE:
        Create( fileName ); break;
    case LOAD_FROM_FILE_THREAD:
        FileName = fileName; break;
    }
}

CONTAINER::CONTAINER( const char* fileName )
{
    Create( fileName );
}

CONTAINER::CONTAINER()
{
}

void CONTAINER::operator=( const char* fileName ){
    Destroy();
    Create( fileName );
}

CONTAINER::~CONTAINER(){
    Destroy();
}

void CONTAINER::Create( const char* fileName ){
    //ファイルをバッファにロードする
    FILE_BUFFER fb( fileName );
    //まずバッファを読んで、各オブジェクトの数を数える（確保する配列数となる)
    while( fb.readString() ){
        if(       fb == "v" ){
            NumVertexBuffers++;
        }else if( fb == "i" ){
            NumIndexBuffers++;
        }else if( fb == "x" ){
            NumTextures++;
        }else if (fb == "b"){
			NumBatches++;
		}else if (fb == "img"){
			NumImages++;
		}else if( fb == "t" ){
            NumTreeOrigins++;
        }else if( fb == "a" ){
            NumAnimations++;
        }else if( fb == "s" ){
            NumSounds++;
        }else if( fb == "d" ){
            NumData++;
        }else if( fb == "{" ){
            fb.skipNode();
        }
    }
    //各オブジェクトの数が分かったのでポインタ配列確保
    VertexBuffers = new VERTEX_BUFFER*[ NumVertexBuffers ]();
    IndexBuffers  = new INDEX_BUFFER*[ NumIndexBuffers ]();
    Textures      = new TEXTURE*[ NumTextures ]();
    Batches       = new BATCH*[ NumBatches ]();
	//Images        = new IMAGE*[ NumImages ]();
    TreeOrigins   = new TREE_ORIGIN*[ NumTreeOrigins ]();
    Animations    = new ANIMATION*[ NumAnimations ]();
    //Sounds        = new SOUND*[ NumSounds ]();
    if( NumData )Data = new DATA*[ NumData ]();
    //バッファからコンテナの各オブジェクトにデータを格納していく
    //　まず、独立している「 VertexBuffer, IndexBuffer, Texture, Animation, Sound, Data 」の格納
    int vertexIdx = 0; //現在の添え字
    int indexIdx = 0;
    int textureIdx = 0;
    int animationIdx = 0;
    int soundIdx = 0;
    int dataIdx = 0;
    fb.restart();//バッファを最初から読み直す
    while( fb.readString() ){
        if(       fb == "v" ){
            VertexBuffers[ vertexIdx ] = new VERTEX_BUFFER( fb );
            vertexIdx++;
        }else if( fb == "i" ){
            IndexBuffers[ indexIdx ] = new INDEX_BUFFER( fb );
            indexIdx++;
        }else if( fb == "x" ){
            Textures[ textureIdx ] = new TEXTURE( fb );
            textureIdx++;
        }else if( fb == "a" ){
            Animations[ animationIdx ] = new ANIMATION( fb );
            animationIdx++;
        }else if( fb == "s" ){
            //Sounds[ soundIdx ] = new SOUND( fb, this );
            soundIdx++;
        }else if( fb == "d" ){
            Data[ dataIdx ] = new DATA( fb );
            dataIdx++;
    	}else if( fb == "{" ){
            fb.skipNode();
        }
    }
    //　次は、VERTEX_BUFFER,INDEX_BUFFER,TEXTUREに依存している「 BATCH 」と「 IMAGE 」の格納
    int batchIdx = 0;
	int imageIdx = 0;
    fb.restart(); //再び最初から読み直し
    while( fb.readString() ){
        if(       fb == "b" ){
            Batches[ batchIdx ] = new BATCH( fb, this );//thisで上のv,i,tを渡す
            batchIdx++;
        }else if( fb == "img" ){
			//Images[imageIdx] = new IMAGE(fb, this);//thisで上のv,tを渡す
			imageIdx++;
		}else if( fb == "{" ){
            fb.skipNode();
        }
    }
    //　最後に、BATCHに依存している「 TREE_ORIGIN 」の格納
    int treeOriginIdx = 0;
    fb.restart(); //再び最初から読み直し
    while( fb.readString() ){
        if(       fb == "t" ){
            TreeOrigins[ treeOriginIdx ] = new TREE_ORIGIN( fb, this );//thisで上のbを渡す
            treeOriginIdx++;
            if( treeOriginIdx >= NumTreeOrigins ){
                break; //残りのバッファをreadする必要なし
            }
        }else if( fb == "{" ){
            fb.skipNode();
        }
    }
}

void CONTAINER::addData( const char* fileName ){
    if( Data ){ WARNING( 1, "Dataは既に読み込んでいる", "CONTAINER" ) }
    //ファイルをバッファにロードする
    FILE_BUFFER fb( fileName );
    //まずバッファを読んで、オブジェクトの数を数える（確保する配列数となる)
    while( fb.readString() ){
        if( fb == "d" ){
            NumData++;
        }else if( fb == "{" ){
            fb.skipNode();
        }
    }
    //オブジェクトの数が分かったのでポインタ配列確保
    Data = new DATA*[ NumData ]();
    //バッファからコンテナの各オブジェクトにデータを格納していく
    //　「 Data 」の格納
    int dataIdx = 0;
    fb.restart();//バッファを最初から読み直す
    while( fb.readString() ){
        if( fb == "d" ){
            Data[ dataIdx ] = new DATA( fb );
            dataIdx++;
        }else if( fb == "{" ){
            fb.skipNode();
        }
    }
}

void CONTAINER::Destroy(){
    for( int i = 0; i < NumData; i++ ){
        SAFE_DELETE( Data[ i ] );
    }
    SAFE_DELETE_ARRAY( Data );
    NumData = 0;

    for( int i = 0; i < NumSounds; i++ ){
        //SAFE_DELETE( Sounds[ i ] );
    }
    //SAFE_DELETE_ARRAY( Sounds );
    NumSounds = 0;

    for( int i = 0; i < NumTreeOrigins; i++){
        SAFE_DELETE( TreeOrigins[ i ] ); 
    }
    SAFE_DELETE_ARRAY( TreeOrigins );
    NumTreeOrigins = 0;
     
    for( int i = 0; i < NumBatches; i++){
        SAFE_DELETE( Batches[ i ] ); 
    }
    SAFE_DELETE_ARRAY( Batches );
    NumBatches = 0;

	for (int i = 0; i < NumImages; i++) {
		//SAFE_DELETE(Images[i]);
	}
	//SAFE_DELETE_ARRAY(Images);
    NumImages = 0;

    for( int i = 0; i < NumAnimations; i++){
        SAFE_DELETE( Animations[ i ] );
    }
    SAFE_DELETE_ARRAY( Animations );
    NumAnimations = 0;

    for( int i = 0; i < NumVertexBuffers; i++){
        SAFE_DELETE( VertexBuffers[ i ] );
    }
    SAFE_DELETE_ARRAY( VertexBuffers );
    NumVertexBuffers = 0;

    for( int i = 0; i < NumIndexBuffers; i++){
        SAFE_DELETE( IndexBuffers[ i ] );
    }
    SAFE_DELETE_ARRAY( IndexBuffers );
    NumIndexBuffers = 0;

    for( int i = 0; i < NumTextures; i++){
        SAFE_DELETE( Textures[ i ] ); 
    }
    SAFE_DELETE_ARRAY( Textures );
    NumTextures = 0;
}

VERTEX_BUFFER* CONTAINER::vertexBuffer( const char* name ) const{
    for( int i = 0; i < NumVertexBuffers; i++ ){
        if( VertexBuffers[ i ]->name() == name ){
            return VertexBuffers[ i ];
        }
    }
    WARNING( 1, "VertexBuffer not found", name );
    return 0;
}

INDEX_BUFFER* CONTAINER::indexBuffer( const char* name ) const{
    for( int i = 0; i < NumIndexBuffers; i++ ){
        if( IndexBuffers[ i ]->name() == name ){
            return IndexBuffers[ i ];
        }
    }
    WARNING( 1, "IndexBuffer not found", name );
    return 0;
}

TEXTURE* CONTAINER::texture( const char* name, int idx ) const{
    if( idx == -1 ){
        for( int i = 0; i < NumTextures && Textures[ i ] != 0 ; i++ ){
            if( Textures[ i ]->name() == name ){
                return Textures[ i ];
            }
        }
        WARNING( 1, "Texture not found", name );
    }
    else{
        char _name[128];
        sprintf_s( _name, "%s%d", name, idx );
        for( int i = 0; i < NumTextures && Textures[ i ] != 0 ; i++ ){
            if( Textures[ i ]->name() == _name ){
                return Textures[ i ];
            }
        }
        WARNING( 1, "Texture not found", _name );
    }
    return 0;
}

BATCH* CONTAINER::batch( const char* name ) const{
    for( int i = 0; i < NumBatches; i++ ){
        if( Batches[ i ]->name() == name ){
            return Batches[ i ];
        }
    }
    WARNING( 1, "Batch not found", name );
    return 0;
}

IMAGE* CONTAINER::image(const char* name) const {
	for (int i = 0; i < NumImages; i++) {
		//if (Images[i]->name() == name) {
		//	return Images[i];
		//}
	}
	WARNING(1, "Image not found", name);
	return 0;
}

TREE_ORIGIN* CONTAINER::treeOrigin( const char* name ) const{
    for( int i = 0; i < NumTreeOrigins; i++ ){
        if( TreeOrigins[ i ]->name() == name ){
            return TreeOrigins[ i ];
        }
    }
    WARNING( 1, "TreeOrigin not found", name );
    return 0;
}

ANIMATION* CONTAINER::animation( const char* name ) const {
    for ( int i = 0; i < NumAnimations; ++i ){
        if ( Animations[ i ]->name() == name ){
            return Animations[ i ];
        }
    }
    WARNING( 1, "Animation not found", name );
    return 0;
}

SOUND* CONTAINER::sound( const char* name ) const{
    for( int i = 0; i < NumSounds; i++ ){
        //if( Sounds[ i ]->name() == name ){
        //    return Sounds[ i ];
        //}
    }
    WARNING( 1, "Sound not found", name );
    return 0;
}

DATA& CONTAINER::data( const char* name ){
    int i;
    for( i = 0; i < NumData; i++ ){
        if( Data[ i ]->name() == name ){
            break;
        }
    }
    WARNING( i == NumData, "Data not found", name );
    return *Data[ i ];
}

//-----------------------------------------------------------------------------
CONTAINER_LOAD_THREAD::CONTAINER_LOAD_THREAD( CONTAINER* c )
    :C( c )
{
    start();
}
CONTAINER_LOAD_THREAD::CONTAINER_LOAD_THREAD( CONTAINER* c, const char* filename )
    :C( c )
{
    C->FileName = filename;
    start();
}
void CONTAINER_LOAD_THREAD::operator()(){
    C->Create( C->FileName );
}
