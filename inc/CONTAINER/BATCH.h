#pragma once
#include"graphic.h"
#include"NAME.h"
#include"VERTEX_FORMATS.h"
struct ID3D11Buffer;
class CONTAINER;
class FILE_BUFFER;
class VERTEX_BUFFER;
class INDEX_BUFFER;
class TEXTURE;
class MATRIX;
struct SUBSET;
class SHADER;

class BATCH{
public:
    BATCH( FILE_BUFFER& fb, CONTAINER* c );
    BATCH();
    void operator=( BATCH* batch );
    ~BATCH();
    void draw( SHADER* shader, const MATRIX& world ) const;
    void drawSkin( SHADER* shader ) const;
    const VERTEX_TYPE vertexType() const;
    //CONTAINERからのコピーで使用
    const NAME& name() const;
    int numSubsets(){ return NumSubsets; }
    VERTEX_BUFFER* vertexBuffer(){ return VertexBuffer; }
    INDEX_BUFFER* indexBuffer(){ return IndexBuffer; }
    SUBSET* subsets(){ return Subsets; }
private:
    VERTEX_BUFFER* VertexBuffer;
    INDEX_BUFFER* IndexBuffer;
    SUBSET* Subsets;
    int NumSubsets;
    const NAME Name;
};

struct SUBSET{
    TEXTURE* texture;
    int startIdx; //描画開始インデックスの番号
    int numDrawPolygons; //三角形描画枚数
};