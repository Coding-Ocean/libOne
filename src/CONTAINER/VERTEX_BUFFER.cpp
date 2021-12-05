#include<d3d11.h>
#include"common.h"
#include"FILE_BUFFER.h"
#include"CONTAINER/VERTEX_FORMATS.h"
#include"CONTAINER/VERTEX_BUFFER.h"

VERTEX_BUFFER::VERTEX_BUFFER( FILE_BUFFER& fb ):
    Obj( 0 ),
    Name( fb.readString() ){
    NAME type = fb.readString();
    Num = fb.readInt();
    if( type == "pnt" ){
		Type = VERTEX_TYPE::PNT;
        VERTEX_PNT* vertices = new VERTEX_PNT[ Num ];
        for( int i = 0; i < Num; i++ ){
            vertices[ i ].pos.x = fb.readFloat();
            vertices[ i ].pos.y = fb.readFloat();
            vertices[ i ].pos.z = fb.readFloat();
            vertices[ i ].normal.x = fb.readFloat();
            vertices[ i ].normal.y = fb.readFloat();
            vertices[ i ].normal.z = fb.readFloat();
            vertices[ i ].tex.x = fb.readFloat();
            vertices[ i ].tex.y = fb.readFloat();
        }
        createVertexBuffer( sizeof(VERTEX_PNT)*Num, vertices, &Obj );
		SAFE_DELETE_ARRAY( vertices );
    }
    else if( type == "pntww" ){
		Type = VERTEX_TYPE::PNTWW;
        VERTEX_PNTWW* vertices = new VERTEX_PNTWW[ Num ];
        for( int i = 0; i < Num; i++ ){
            vertices[ i ].pos.x = fb.readFloat();
            vertices[ i ].pos.y = fb.readFloat();
            vertices[ i ].pos.z = fb.readFloat();
            vertices[ i ].normal.x = fb.readFloat();
            vertices[ i ].normal.y = fb.readFloat();
            vertices[ i ].normal.z = fb.readFloat();
            vertices[ i ].tex.x = fb.readFloat();
            vertices[ i ].tex.y = fb.readFloat();
            for( int j = 0; j < 4; j++ ){
                vertices[ i ].worldIndices[ j ] = fb.readInt();
            }
            for( int j = 0; j < 4; j++ ){
                vertices[ i ].weights[ j ] = fb.readFloat();
            }        
        }
        createVertexBuffer( sizeof(VERTEX_PNTWW)*Num, vertices, &Obj );    
		SAFE_DELETE_ARRAY( vertices );
    }
	else if (type == "pt") {
		Type = VERTEX_TYPE::PT;
		VERTEX_PT* vertices = new VERTEX_PT[Num];
		for (int i = 0; i < Num; i++) {
			vertices[i].pos.x = fb.readFloat();
			vertices[i].pos.y = fb.readFloat();
			vertices[i].pos.z = fb.readFloat();
			vertices[i].tex.x = fb.readFloat();
			vertices[i].tex.y = fb.readFloat();
		}
        OffsetX = vertices[3].pos.x;
        OffsetY = vertices[3].pos.y;
	    createVertexBuffer(sizeof(VERTEX_PT)*Num, vertices, &Obj);
		SAFE_DELETE_ARRAY(vertices);
	}
}

VERTEX_BUFFER::~VERTEX_BUFFER(){
    SAFE_RELEASE( Obj );
}

VERTEX_BUFFER_OBJ** VERTEX_BUFFER::obj(){
    return &Obj;
}

int VERTEX_BUFFER::num() const{
    return Num;
}

const NAME& VERTEX_BUFFER::name() const{
    return Name;
}

const VERTEX_TYPE VERTEX_BUFFER::type() const{
    return Type;
}


/*
VERTEX_BUFFER::VERTEX_BUFFER( float w, float h ):
    Obj( 0 ){
    float tw = w;//texture width
    float th = h;//texture height
    float hw;//half width(vertex position)
    float hh;//half height(vertex position)
    float l=0;//tex coord left
    float t=0;//tex coord top
    float r=1;//tex coord right
    float b=1;//tex coord bottom

    hw = tw/2.0f;// / g->width();
    hh = th/2.0f;// / g->width();
    VERTEX_PT vertices[ 4 ] = {
        VERTEX_PT( VECTOR( -hw, -hh, 0.0f ), VECTOR2( l, t ) ),
        VERTEX_PT( VECTOR( -hw, hh, 0.0f ), VECTOR2( l, b ) ),
        VERTEX_PT( VECTOR( hw, -hh, 0.0f ), VECTOR2( r, t ) ),
        VERTEX_PT( VECTOR( hw, hh, 0.0f ), VECTOR2( r, b ) )
    };
    Type = VERTEX_TYPE::PT;
    createVertexBuffer( 4, vertices, &Obj );
    OffsetX = vertices[3].pos.x;
    OffsetY = vertices[3].pos.y;// * g->aspect();
}
*/
