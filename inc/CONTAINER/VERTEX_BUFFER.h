#pragma once
#include"graphic.h"
#include"NAME.h"

class VERTEX_BUFFER{
public:
    VERTEX_BUFFER( class FILE_BUFFER& fb );
    //VERTEX_BUFFER( float width, float height );
    ~VERTEX_BUFFER();
    //アクセッサ
    VERTEX_BUFFER_OBJ** obj();
    int num() const;
    const NAME& name() const;
    const VERTEX_TYPE type() const;
    float offsetX(){ return OffsetX; }
    float offsetY(){ return OffsetY; }
private:
    VERTEX_BUFFER_OBJ* Obj=0;
    int Num=0;
    NAME Name;
    VERTEX_TYPE Type;
    float OffsetX=0;
    float OffsetY=0;
};
