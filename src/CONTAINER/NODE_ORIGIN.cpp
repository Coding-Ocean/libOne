#include"common.h"
#include"CONTAINER/NODE_ORIGIN.h"

NODE_ORIGIN::NODE_ORIGIN():
Name(),
Batch( 0 ),
Scale( 1.0f, 1.0f, 1.0f ),
Rotate( 0.0f, 0.0f, 0.0f ),
Translate( 0.0f, 0.0f, 0.0f ),
ChounanIdx( -1 ),
OtoutoIdx( -1 ){
    ToOriginWorld.identity();
}

NODE_ORIGIN::~NODE_ORIGIN(){
}

const NAME& NODE_ORIGIN::name() const{
    return Name;
}

const BATCH* NODE_ORIGIN::batch() const{
    return Batch;
}

const VECTOR& NODE_ORIGIN::scale() const {
    return Scale;
}

const VECTOR& NODE_ORIGIN::rotate() const {
    return Rotate;
}

const VECTOR& NODE_ORIGIN::translate() const {
    return Translate;
}

const MATRIX& NODE_ORIGIN::toOriginWorld() const{
    return ToOriginWorld;
}

int NODE_ORIGIN::chounanIdx() const {
    return ChounanIdx;
}

int NODE_ORIGIN::otoutoIdx() const {
    return OtoutoIdx;
}



