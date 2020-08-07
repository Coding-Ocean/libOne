#include <cmath>
#include "MATRIX.h"
#include "VECTOR3.h"

MATRIX::MATRIX(){
    _11 = 1;            _12 = 0;            _13 = 0;            _14 = 0;  
    _21 = 0;            _22 = 1;            _23 = 0;            _24 = 0;  
    _31 = 0;            _32 = 0;            _33 = 1;            _34 = 0;  
    _41 = 0;            _42 = 0;            _43 = 0;            _44 = 1;
}


/*
//無駄なく移動行列を乗算。
void MATRIX::mulTranslate( const VECTOR3& a ){
    _14 += _11 * a.x + _12 * a.y + _13 * a.z;
    _24 += _21 * a.x + _22 * a.y + _23 * a.z;
    _34 += _31 * a.x + _32 * a.y + _33 * a.z;
}
void MATRIX::mulTranslate( float x, float y, float z ){
    _14 += _11 * x + _12 * y + _13 * z;
    _24 += _21 * x + _22 * y + _23 * z;
    _34 += _31 * x + _32 * y + _33 * z;
}
//無駄なくZ回転行列を乗算 1,2列しか変わらない。
void MATRIX::mulRotateZ( float r ){
    float c = cos( r );
    float s = sin( r );
    float tmp;
    //1行目
    tmp = c * _11 + s * _12;
    _12 = -s * _11 +  c * _12;
    _11 = tmp;
    //2行目
    tmp = c * _21 + s * _22;
    _22 = -s * _21 +  c * _22;
    _21 = tmp;
    //1行目
    tmp = c * _31 + s * _32;
    _32 = -s * _31 +  c * _32;
    _31 = tmp;
}
//無駄なくX回転行列を乗算 2,3列しか変わらない。
void MATRIX::mulRotateX( float r ){
    float c = cos( r );
    float s = sin( r );
    float tmp;
    //1行目
    tmp = c * _12 + s * _13;
    _13 = -s * _12 + c * _13;
    _12 = tmp;
    //2行目
    tmp = c * _22 + s * _23;
    _23 = -s * _22 + c * _23;
    _22 = tmp;
    //3行目
    tmp = c * _32 + s * _33;
    _33 = -s * _32 + c * _33;
    _32 = tmp;
}
//無駄なくY回転行列を乗算 1,3列しか変わらない。
void MATRIX::mulRotateY( float r ){
    float c = cos( r );
    float s = sin( r );
    float tmp;
    //1行目
    tmp =  c * _11 - s * _13;
    _13 = s * _11 + c * _13;
    _11 = tmp;
    //2行目
    tmp =  c * _21 + -s * _23;
    _23 = s * _21 + c * _23;
    _21 = tmp;
    //3行目
    tmp =  c * _31 - s * _33;
    _33 = s * _31 + c * _33;
    _31 = tmp;
}
//無駄なく拡大縮小行列を乗算
void MATRIX::mulScale( const VECTOR3& a ){
    _11 *= a.x;
    _12 *= a.y;
    _13 *= a.z;
    _21 *= a.x;
    _22 *= a.y;
    _23 *= a.z;
    _31 *= a.x;
    _32 *= a.y;
    _33 *= a.z;
}
void MATRIX::mulScale( float x, float y, float z ){
    _11 *= x;
    _12 *= y;
    _13 *= z;
    _21 *= x;
    _22 *= y;
    _23 *= z;
    _31 *= x;
    _32 *= y;
    _33 *= z;
}
*/