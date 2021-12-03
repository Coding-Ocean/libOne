#include<d3d11.h>
//#include"SHADER.h"
#include"FILE_BUFFER.h"
#include"CONTAINER/TEXTURE.h"

//ID3D11Buffer* TEXTURE::VertexObj = 0;
//void TEXTURE::createVertex(){
//    float l=0;//tex coord left
//    float t=0;//tex coord top
//    float r=1;//tex coord right
//    float b=1;//tex coord bottom
//    VERTEX_IMAGE vertices[ 4 ] = {
//        VERTEX_IMAGE( VECTOR3( -0.5f,  0.5f, 0.0f ), VECTOR2( l, t ) ),
//        VERTEX_IMAGE( VECTOR3( -0.5f, -0.5f, 0.0f ), VECTOR2( l, b ) ),
//        VERTEX_IMAGE( VECTOR3(  0.5f,  0.5f, 0.0f ), VECTOR2( r, t ) ),
//        VERTEX_IMAGE( VECTOR3(  0.5f, -0.5f, 0.0f ), VECTOR2( r, b ) )
//    };
//    GRAPHIC* g = GRAPHIC::instance();
//    g->createVertexBuffer( 4, vertices, &VertexObj );
//}
//void TEXTURE::destroyVertex(){
//    SAFE_RELEASE( VertexObj );
//}

TEXTURE::TEXTURE( FILE_BUFFER& fb ):
    Name( fb.readString() ){
    createTexture( fb.readString(), &Obj, &W, &H );
}

//TEXTURE::TEXTURE( const char* fileName ){
//    GRAPHIC* g = GRAPHIC::instance();
//    g->createTexture( fileName, &Obj, &Width, &Height );
//}

//TEXTURE::TEXTURE( TEXTURE_OBJ* obj, int width, int height ){
//    Width = width;
//    Height = height;
//    Obj = obj;
//}

TEXTURE::~TEXTURE(){
    SAFE_RELEASE( Obj );
    W = 0;
    H = 0;
}

//void TEXTURE::create( const char* fileName ){
//    GRAPHIC* g = GRAPHIC::instance();
//    g->createTexture( fileName, &Obj, &Width, &Height );
//}

TEXTURE_OBJ** TEXTURE::obj(){
    return &Obj;
}

const NAME& TEXTURE::name() const{
    return Name;
}

////posで指定した位置に画像の左上が来る
//void TEXTURE::draw( SHADER* shader, const VECTOR2& pos, const COLOR& color ) {
//    GRAPHIC* g = GRAPHIC::instance();
//    //スクリーン座標をデカルト座標に変換
//    MATRIX world;
//    world.screen( g->baseWidth(), g->baseHeight() );
//    world.mulTranslate( pos.x, -pos.y, 0.0f );
//    world.mulScale( (float)Width, (float)Height, 1.0f );
//    world.mulTranslate( 0.5f, -0.5f, 0.0f );//Vertexが描く正方形の左上を原点に持ってくる
//    shader->setWorld( world );
//    shader->setDiffuse( color );
//    g->draw( VertexObj, Obj );
//}
//void TEXTURE::draw( SHADER* shader, const float x, float y, const COLOR& color ) {
//    draw( shader, VECTOR2(x,y), color );
//}
//
////posで指定した位置に画像の中央が来る
//void TEXTURE::draw( SHADER* shader, const VECTOR2& pos, float radian, const COLOR& color ){
//    GRAPHIC* g = GRAPHIC::instance();
//    //スクリーン座標をデカルト座標に変換
//    MATRIX world;
//    world.screen( g->baseWidth(), g->baseHeight() );
//    world.mulTranslate( pos.x, -pos.y, 0.0f );
//    world.mulRotateZ( radian );
//    world.mulScale( (float)Width, (float)Height, 1.0f );
//    shader->setWorld( world );
//    shader->setDiffuse( color );
//    g->draw( VertexObj, Obj );
//}
//void TEXTURE::draw( SHADER* shader, const float x, float y, float radian, const COLOR& color ){
//    draw( shader, VECTOR2(x,y), radian, color );
//}
//
////VertexをMATRIXで座標変換して、画像を３Ｄ空間に表示
//void TEXTURE::draw( SHADER* shader, const MATRIX& world, const COLOR& color ){
//    GRAPHIC* g = GRAPHIC::instance();
//    shader->setWorld( world );
//    shader->setDiffuse( color );
//    g->draw( VertexObj, Obj );
//}