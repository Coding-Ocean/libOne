#include "mathUtil.h"
#include "MATRIX.h"

void MATRIX::identity() {
    _11 = 1;     _12 = 0;     _13 = 0;     _14 = 0;    
    _21 = 0;     _22 = 1;     _23 = 0;     _24 = 0;    
    _31 = 0;     _32 = 0;     _33 = 1;     _34 = 0;    
    _41 = 0;     _42 = 0;     _43 = 0;     _44 = 1;    
}
void MATRIX::translate(float x, float y, float z) {
    _11 = 1;     _12 = 0;     _13 = 0;     _14 = x;    
    _21 = 0;     _22 = 1;     _23 = 0;     _24 = y;    
    _31 = 0;     _32 = 0;     _33 = 1;     _34 = z;    
    _41 = 0;     _42 = 0;     _43 = 0;     _44 = 1;    
}
void MATRIX::scaling(float x, float y, float z) {
    _11 = x;     _12 = 0;     _13 = 0;     _14 = 0;    
    _21 = 0;     _22 = y;     _23 = 0;     _24 = 0;    
    _31 = 0;     _32 = 0;     _33 = z;     _34 = 0;    
    _41 = 0;     _42 = 0;     _43 = 0;     _44 = 1;    
}
void MATRIX::rotateZ(float angle){
    float c = cos(angle);    
    float s = sin(angle);    
    _11 = c;     _12 =-s;     _13 = 0;     _14 = 0;    
    _21 = s;     _22 = c;     _23 = 0;     _24 = 0;    
    _31 = 0;     _32 = 0;     _33 = 1;     _34 = 0;    
    _41 = 0;     _42 = 0;     _43 = 0;     _44 = 1;    
}
void MATRIX::rotateY(float angle) {
    float c = cos(angle);    
    float s = sin(angle);    
    _11 = c;     _12 = 0;     _13 = s;     _14 = 0;    
    _21 = 0;     _22 = 1;     _23 = 0;     _24 = 0;    
    _31 =-s;     _32 = 0;     _33 = c;     _34 = 0;    
    _41 = 0;     _42 = 0;     _43 = 0;     _44 = 1;    
}
void MATRIX::rotateX(float angle) {
    float c = cos(angle);    
    float s = sin(angle);    
    _11 = 1;     _12 = 0;     _13 = 0;     _14 = 0;    
    _21 = 0;     _22 = c;     _23 =-s;     _24 = 0;    
    _31 = 0;     _32 = s;     _33 = c;     _34 = 0;    
    _41 = 0;     _42 = 0;     _43 = 0;     _44 = 1;    
}
void MATRIX::camera(const VECTOR& camera, const VECTOR& lookat, const VECTOR& up) {
    //カメラのローカル軸座標を求める(正規直交基底ベクトル）
    VECTOR z = normalize(camera - lookat);
    VECTOR x = normalize(cross(up, z));
    VECTOR y = cross(z, x);
    _11 = x.x;    _12 = x.y;    _13 = x.z;    _14 = x.x * -camera.x + x.y * -camera.y + x.z * -camera.z;
    _21 = y.x;    _22 = y.y;    _23 = y.z;    _24 = y.x * -camera.x + y.y * -camera.y + y.z * -camera.z;
    _31 = z.x;    _32 = z.y;    _33 = z.z;    _34 = z.x * -camera.x + z.y * -camera.y + z.z * -camera.z;
    _41 = 0;      _42 = 0;      _43 = 0;      _44 = 1;
}
void MATRIX::pers(float angle, float aspect, float n, float f) {
    float s = 1.0f / tan(angle * 0.5f);
    float a = f / (-f + n);
    float b = a * n;
    _11 = s / aspect;   _12 = 0;        _13 = 0;            _14 = 0;
    _21 = 0;            _22 = s;        _23 = 0;            _24 = 0;
    _31 = 0;            _32 = 0;        _33 = a;            _34 = b;
    _41 = 0;            _42 = 0;        _43 = -1;           _44 = 0;
}
void MATRIX::ortho(float l, float r, float b, float t, float n, float f) {
    _11 = 2.0f / (r - l);   _12 = 0.0f;             _13 = 0.0f;               _14 = -(r + l) / (r - l);
    _21 = 0.0f;             _22 = 2.0f / (t - b);   _23 = 0.0f;               _24 = -(t + b) / (t - b);
    _31 = 0.0f;             _32 = 0.0f;             _33 = -2.0f / (f - n);    _34 = -(f + n) / (f - n);
    _41 = 0.0f;             _42 = 0.0f;             _43 = 0.0f;               _44 = 1.0f;
}

void MATRIX::mulTranslate( float x, float y, float z ){
	_14 += _11 * x + _12 * y + _13 * z;
	_24 += _21 * x + _22 * y + _23 * z;
	_34 += _31 * x + _32 * y + _33 * z;
}
void MATRIX::mulScaling( float x, float y, float z ){
	_11 *= x;    _12 *= y;    _13 *= z;
	_21 *= x;    _22 *= y;    _23 *= z;
	_31 *= x;    _32 *= y;    _33 *= z;
}
void MATRIX::mulRotateX(float r) {
    float c = cos(r);
    float s = sin(r);
    float tmp;
    //1行目
    tmp = _12 * c + _13 * s;
    _13 = _12 * -s + _13 * c;
    _12 = tmp;
    //2行目
    tmp = _22 * c + _23 * s;
    _23 = _22 * -s + _23 * c;
    _22 = tmp;
    //3行目
    tmp = _32 * c + _33 * s;
    _33 = _32 * -s + _33 * c;
    _32 = tmp;
}
void MATRIX::mulRotateY(float r) {
    float c = cos(r);
    float s = sin(r);
    float tmp;
    //1行目
    tmp = _11 * c - _13 * s;
    _13 = _11 * s + _13 * c;
    _11 = tmp;
    //2行目
    tmp = _21 * c - _23 * s;
    _23 = _21 * s + _23 * c;
    _21 = tmp;
    //3行目
    tmp = _31 * c - _33 * s;
    _33 = _31 * s + _33 * c;
    _31 = tmp;
}
void MATRIX::mulRotateZ(float r) {
    float c = cos(r);
    float s = sin(r);
    float tmp;
    //1行目
    tmp = _11 * c + _12 * s;
    _12 = _11 * -s + _12 * c;
    _11 = tmp;
    //2行目
    tmp = _21 * c + _22 * s;
    _22 = _21 * -s + _22 * c;
    _21 = tmp;
    //3行目
    tmp = _31 * c + _32 * s;
    _32 = _31 * -s + _32 * c;
    _31 = tmp;
}
MATRIX MATRIX::operator*(const MATRIX& m) const {
    MATRIX tmp;

    tmp._11 = _11 * m._11 + _12 * m._21 + _13 * m._31 +_14 * m._41;
    tmp._21 = _21 * m._11 + _22 * m._21 + _23 * m._31 +_24 * m._41;
    tmp._31 = _31 * m._11 + _32 * m._21 + _33 * m._31 +_34 * m._41;
    tmp._41 = _41 * m._11 + _42 * m._21 + _43 * m._31 + _44 * m._41;

    tmp._12 = _11 * m._12 + _12 * m._22 + _13 * m._32 +_14 * m._42;
    tmp._22 = _21 * m._12 + _22 * m._22 + _23 * m._32 +_24 * m._42;
    tmp._32 = _31 * m._12 + _32 * m._22 + _33 * m._32 +_34 * m._42;
    tmp._42 = _41 * m._12 + _42 * m._22 + _43 * m._32 +_44 * m._42;

    tmp._13 = _11 * m._13 + _12 * m._23 + _13 * m._33 + _14 * m._43;
    tmp._23 = _21 * m._13 + _22 * m._23 + _23 * m._33 + _24 * m._43;
    tmp._33 = _31 * m._13 + _32 * m._23 + _33 * m._33 + _34 * m._43;
    tmp._43 = _41 * m._13 + _42 * m._23 + _43 * m._33 + _44 * m._43;

    tmp._14 = _11 * m._14 + _12 * m._24 + _13 * m._34 + _14 * m._44;
    tmp._24 = _21 * m._14 + _22 * m._24 + _23 * m._34 + _24 * m._44;
    tmp._34 = _31 * m._14 + _32 * m._24 + _33 * m._34 + _34 * m._44;
    tmp._44 = _41 * m._14 + _42 * m._24 + _43 * m._34 + _44 * m._44;

    return tmp;
}

VECTOR MATRIX::operator*(const VECTOR& v) const {
    VECTOR tmp;
    tmp.x   = _11 * v.x + _12 * v.y + _13 * v.z + _14;
    tmp.y   = _21 * v.x + _22 * v.y + _23 * v.z + _24;
    tmp.z   = _31 * v.x + _32 * v.y + _33 * v.z + _34;
    float w = _43 * v.z + _44;
    if (w < 0) w *= -1;
    return  tmp / w;
}

