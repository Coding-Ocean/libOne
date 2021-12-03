#include"common.h"
#include"FILE_BUFFER.h"
#include"MATRIX.h"
#include"CONTAINER/CONTAINER.h"
#include"CONTAINER/VERTEX_BUFFER.h"
#include"CONTAINER/INDEX_BUFFER.h"
#include"CONTAINER/TEXTURE.h"
#include"CONTAINER/BATCH.h"
#include"SHADER/SHADER.h"

//CONTAINERにBATCHを作るとき呼び出されるコンストラクタ
BATCH::BATCH( FILE_BUFFER& fb, CONTAINER* c ):
    Name( fb.readString() ){
    //名前が一致する各コンテナオブジェクトのポインタをBATCHにコピーする
    VertexBuffer = c->vertexBuffer( fb.readString() );
    IndexBuffer = c->indexBuffer( fb.readString() );
    NumSubsets = fb.readInt();
    Subsets = new SUBSET[ NumSubsets ];
    for( int i = 0; i < NumSubsets; i++ ){
        Subsets[ i ].startIdx = fb.readInt();
        Subsets[ i ].numDrawPolygons = fb.readInt();
        Subsets[ i ].texture = c->texture( fb.readString() );
    }
}

BATCH::~BATCH(){
    if( Name != "" ) SAFE_DELETE_ARRAY( Subsets );//Nameが""でないのはCONTAINER内のBATCH。
    NumSubsets = 0;
    VertexBuffer = 0;
    IndexBuffer = 0;
}

//ポインタでなくインスタンスを用意したコンストラクタ
BATCH::BATCH():
    Name(),
    VertexBuffer( 0 ),
    IndexBuffer( 0 ),
    NumSubsets( 0 ),
    Subsets( 0 ){
}
//上のインスタンスに代入する時に呼び出される
void BATCH::operator=( BATCH* batch ){
    VertexBuffer = batch->vertexBuffer();
    IndexBuffer = batch->indexBuffer();
    NumSubsets = batch->numSubsets();
    Subsets = batch->subsets();
}

void BATCH::draw( SHADER* shader, const MATRIX& world ) const{
    shader->setShader();
    shader->setWorld( world );
    draw3D( VertexBuffer, IndexBuffer, Subsets, NumSubsets, sizeof(VERTEX_PNT) );
}

void BATCH::drawSkin( SHADER* shader ) const{
    shader->setShaderSkin();
    draw3D( VertexBuffer, IndexBuffer, Subsets, NumSubsets, sizeof(VERTEX_PNTWW) );
}

const NAME& BATCH::name() const{
    return Name;
}

const VERTEX_TYPE BATCH::vertexType() const{
	return VertexBuffer->type();
}
