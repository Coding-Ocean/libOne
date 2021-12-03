#include<d3d11.h>
#include"common.h"
#include"graphic.h"
#include"FILE_BUFFER.h"
#include"CONTAINER/INDEX_BUFFER.h"

INDEX_BUFFER::INDEX_BUFFER( FILE_BUFFER& fb ):
    DxObj( 0 ),
    Name( fb.readString() ){//インデックス名
    Num = fb.readInt();
    unsigned short* indices = new unsigned short[ Num ];
    for( int i = 0; i < Num; i++ ){
        indices[ i ] = fb.readInt();
    }
    createIndexBuffer( Num, indices, &DxObj );
    SAFE_DELETE_ARRAY( indices );
}

INDEX_BUFFER::~INDEX_BUFFER(){
    SAFE_RELEASE( DxObj );
}

ID3D11Buffer* INDEX_BUFFER::dxObj() const{
    return DxObj;
}

int INDEX_BUFFER::num() const{
    return Num;
}

const NAME& INDEX_BUFFER::name() const{
    return Name;
}


