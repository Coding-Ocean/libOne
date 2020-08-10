#include <math.h>
#include "MAT.h"

MAT::MAT(){
    _11 = 1;            _12 = 0;            _13 = 0;            _14 = 0;  
    _21 = 0;            _22 = 1;            _23 = 0;            _24 = 0;  
    _31 = 0;            _32 = 0;            _33 = 1;            _34 = 0;  
    _41 = 0;            _42 = 0;            _43 = 0;            _44 = 1;
}
void MAT::identity() {
    _11 = 1;            _12 = 0;            _13 = 0;            _14 = 0;
    _21 = 0;            _22 = 1;            _23 = 0;            _24 = 0;
    _31 = 0;            _32 = 0;            _33 = 1;            _34 = 0;
    _41 = 0;            _42 = 0;            _43 = 0;            _44 = 1;
}
void MAT::mulTranslate( float x, float y, float z ){
	_14 += _11 * x + _12 * y + _13 * z;
	_24 += _21 * x + _22 * y + _23 * z;
	_34 += _31 * x + _32 * y + _33 * z;
}
void MAT::mulScale( float x, float y, float z ){
	_11 *= x;    _12 *= y;    _13 *= z;
	_21 *= x;    _22 *= y;    _23 *= z;
	_31 *= x;    _32 *= y;    _33 *= z;
}
void MAT::mulRotateZ( float r ){
    float c = cosf( r );
    float s = sinf( r );
    float tmp;
    //1çsñ⁄
    tmp = c * _11 + s * _12;
    _12 = -s * _11 +  c * _12;
    _11 = tmp;
    //2çsñ⁄
    tmp = c * _21 + s * _22;
    _22 = -s * _21 +  c * _22;
    _21 = tmp;
    //1çsñ⁄
    tmp = c * _31 + s * _32;
    _32 = -s * _31 +  c * _32;
    _31 = tmp;
}
void MAT::ortho( float l, float r, float b, float t, float n, float f ){
    _11 = 2.0f / (r - l);   _12 = 0.0f;             _13 = 0.0f;               _14 = -(r + l) / (r - l); 
    _21 = 0.0f;             _22 = 2.0f / (t - b);   _23 = 0.0f;               _24 = -(t + b) / (t - b); 
    _31 = 0.0f;             _32 = 0.0f;             _33 = -2.0f / (f - n);    _34 = -(f + n) / (f - n);
    _41 = 0.0f;             _42 = 0.0f;             _43 = 0.0f;               _44 = 1.0f;
}
VECTOR3 MAT::operator*( const VECTOR3& v ) const{
    VECTOR3 tmp;
    tmp.x   = _11 * v.x + _12 * v.y + _13 * v.z + _14;
    tmp.y   = _21 * v.x + _22 * v.y + _23 * v.z + _24;
    tmp.z   = _31 * v.x + _32 * v.y + _33 * v.z + _34;
    //float w = _41 * v.x + _42 * v.y + _43 * v.z + _44;
    //if( w < 0 ){ tmp.z=-1; w=1; }
    //tmp = tmp / w;
    return  tmp;
}

