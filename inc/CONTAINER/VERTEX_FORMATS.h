#pragma once

#include"VECTOR.h"
#include"VECTOR2.h"

struct VERTEX_PNT{
    VECTOR pos;
	VECTOR normal;
    VECTOR2 tex;
};

struct VERTEX_PNTWW{
    VECTOR pos;
	VECTOR normal;
    VECTOR2 tex;
    short worldIndices[ 4 ];
    float weights[ 4 ];
    VERTEX_PNTWW():worldIndices(),weights(){}
};

struct VERTEX_PT{
	VECTOR pos;
	VECTOR2 tex;
    VERTEX_PT(){}
    VERTEX_PT(VECTOR p, VECTOR2 t){pos=p;tex=t;}
};
