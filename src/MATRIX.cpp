#include "mathUtil.h"
#include "QUATERNION.h"
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
void MATRIX::translate(VECTOR& t)
{
    _11 = 1;     _12 = 0;     _13 = 0;     _14 = t.x;
    _21 = 0;     _22 = 1;     _23 = 0;     _24 = t.y;
    _31 = 0;     _32 = 0;     _33 = 1;     _34 = t.z;
    _41 = 0;     _42 = 0;     _43 = 0;     _44 = 1;
}
void MATRIX::scaling(float x, float y, float z) {
    _11 = x;     _12 = 0;     _13 = 0;     _14 = 0;    
    _21 = 0;     _22 = y;     _23 = 0;     _24 = 0;    
    _31 = 0;     _32 = 0;     _33 = z;     _34 = 0;    
    _41 = 0;     _42 = 0;     _43 = 0;     _44 = 1;    
}
void MATRIX::rotateZ(float angle){
    float c = Cos(angle);    
    float s = Sin(angle);    
    _11 = c;     _12 =-s;     _13 = 0;     _14 = 0;    
    _21 = s;     _22 = c;     _23 = 0;     _24 = 0;    
    _31 = 0;     _32 = 0;     _33 = 1;     _34 = 0;    
    _41 = 0;     _42 = 0;     _43 = 0;     _44 = 1;    
}
void MATRIX::rotateY(float angle) {
    float c = Cos(angle);    
    float s = Sin(angle);    
    _11 = c;     _12 = 0;     _13 = s;     _14 = 0;    
    _21 = 0;     _22 = 1;     _23 = 0;     _24 = 0;    
    _31 =-s;     _32 = 0;     _33 = c;     _34 = 0;    
    _41 = 0;     _42 = 0;     _43 = 0;     _44 = 1;    
}
void MATRIX::rotateX(float angle) {
    float c = Cos(angle);    
    float s = Sin(angle);    
    _11 = 1;     _12 = 0;     _13 = 0;     _14 = 0;    
    _21 = 0;     _22 = c;     _23 =-s;     _24 = 0;    
    _31 = 0;     _32 = s;     _33 = c;     _34 = 0;    
    _41 = 0;     _42 = 0;     _43 = 0;     _44 = 1;    
}
void MATRIX::camera(const VECTOR& camPos, const VECTOR& lookat, const VECTOR& up) {
    //カメラのローカル軸座標を求める(正規直交ベクトル）
    VECTOR z = normalize(camPos - lookat);
    VECTOR x = normalize(cross(up, z));
    VECTOR y = cross(z, x);
    _11 = x.x;    _12 = x.y;    _13 = x.z;    _14 = x.x * -camPos.x + x.y * -camPos.y + x.z * -camPos.z;
    _21 = y.x;    _22 = y.y;    _23 = y.z;    _24 = y.x * -camPos.x + y.y * -camPos.y + y.z * -camPos.z;
    _31 = z.x;    _32 = z.y;    _33 = z.z;    _34 = z.x * -camPos.x + z.y * -camPos.y + z.z * -camPos.z;
    _41 = 0;      _42 = 0;      _43 = 0;      _44 = 1;
}
void MATRIX::pers(float angle, float aspect, float n, float f) {
    float s = 1.0f / Tan(angle * 0.5f);
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
void MATRIX::mulTranslate(const VECTOR& t)
{
    _14 += _11 * t.x + _12 * t.y + _13 * t.z;
    _24 += _21 * t.x + _22 * t.y + _23 * t.z;
    _34 += _31 * t.x + _32 * t.y + _33 * t.z;
}
void MATRIX::mulScaling( float x, float y, float z ){
	_11 *= x;    _12 *= y;    _13 *= z;
	_21 *= x;    _22 *= y;    _23 *= z;
	_31 *= x;    _32 *= y;    _33 *= z;
}
void MATRIX::mulScaling(const VECTOR& s)
{
    _11 *= s.x;    _12 *= s.y;    _13 *= s.z;
    _21 *= s.x;    _22 *= s.y;    _23 *= s.z;
    _31 *= s.x;    _32 *= s.y;    _33 *= s.z;
}
void MATRIX::mulRotateX(float r) {
    float c = Cos(r);
    float s = Sin(r);
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
    float c = Cos(r);
    float s = Sin(r);
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
    float c = Cos(r);
    float s = Sin(r);
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
void MATRIX::rotateYXZ(const VECTOR& r)
{
    rotateY(r.y);
    mulRotateX(r.x);
    mulRotateZ(r.z);
}
void MATRIX::mulRotateYXZ(const VECTOR& r)
{
    mulRotateY(r.y);
    mulRotateX(r.x);
    mulRotateZ(r.z);
}
MATRIX MATRIX::operator*(const MATRIX& m) const {
    MATRIX tmp;

    tmp._11 = _11 * m._11 + _12 * m._21 + _13 * m._31 + _14 * m._41;
    tmp._21 = _21 * m._11 + _22 * m._21 + _23 * m._31 + _24 * m._41;
    tmp._31 = _31 * m._11 + _32 * m._21 + _33 * m._31 + _34 * m._41;
    tmp._41 = _41 * m._11 + _42 * m._21 + _43 * m._31 + _44 * m._41;

    tmp._12 = _11 * m._12 + _12 * m._22 + _13 * m._32 + _14 * m._42;
    tmp._22 = _21 * m._12 + _22 * m._22 + _23 * m._32 + _24 * m._42;
    tmp._32 = _31 * m._12 + _32 * m._22 + _33 * m._32 + _34 * m._42;
    tmp._42 = _41 * m._12 + _42 * m._22 + _43 * m._32 + _44 * m._42;

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
    tmp.x = _11 * v.x + _12 * v.y + _13 * v.z + _14;
    tmp.y = _21 * v.x + _22 * v.y + _23 * v.z + _24;
    tmp.z = _31 * v.x + _32 * v.y + _33 * v.z + _34;
    //projとの掛け算
    if (_43 < 0) {//応用がきかないが、これでproj matと判断
        if (v.z != 0) {
            return tmp / (v.z < 0 ? -v.z : v.z);
        }
    }
    //proj以外の掛け算
    return tmp;
}
void MATRIX::rotateQuaternion(const class QUATERNION& q)
{
    *this = createMatFromQtn(q);
}
void MATRIX::mulRotateQuaternion(const QUATERNION& q)
{
    MATRIX t = *this;
    MATRIX m = createMatFromQtn(q);

    _11 = t._11 * m._11 + t._12 * m._21 + t._13 * m._31;
    _21 = t._21 * m._11 + t._22 * m._21 + t._23 * m._31;
    _31 = t._31 * m._11 + t._32 * m._21 + t._33 * m._31;
    //_41 = t._41 * m._11 + t._42 * m._21 + t._43 * m._31;

    _12 = t._11 * m._12 + t._12 * m._22 + t._13 * m._32;
    _22 = t._21 * m._12 + t._22 * m._22 + t._23 * m._32;
    _32 = t._31 * m._12 + t._32 * m._22 + t._33 * m._32;
    //_42 = t._41 * m._12 + t._42 * m._22 + t._43 * m._32;

    _13 = t._11 * m._13 + t._12 * m._23 + t._13 * m._33;
    _23 = t._21 * m._13 + t._22 * m._23 + t._23 * m._33;
    _33 = t._31 * m._13 + t._32 * m._23 + t._33 * m._33;
    //_43 = t._41 * m._13 + t._42 * m._23 + t._43 * m._33;

    //_14 = t._14;
    //_24 = t._24;
    //_34 = t._34;
    //_44 = t._44;

    /*
    _11 = t._11 * m._11 + t._12 * m._21 + t._13 * m._31 + t._14 * m._41;
    _21 = t._21 * m._11 + t._22 * m._21 + t._23 * m._31 + t._24 * m._41;
    _31 = t._31 * m._11 + t._32 * m._21 + t._33 * m._31 + t._34 * m._41;
    _41 = t._41 * m._11 + t._42 * m._21 + t._43 * m._31 + t._44 * m._41;

    _12 = t._11 * m._12 + t._12 * m._22 + t._13 * m._32 + t._14 * m._42;
    _22 = t._21 * m._12 + t._22 * m._22 + t._23 * m._32 + t._24 * m._42;
    _32 = t._31 * m._12 + t._32 * m._22 + t._33 * m._32 + t._34 * m._42;
    _42 = t._41 * m._12 + t._42 * m._22 + t._43 * m._32 + t._44 * m._42;

    _13 = t._11 * m._13 + t._12 * m._23 + t._13 * m._33 + t._14 * m._43;
    _23 = t._21 * m._13 + t._22 * m._23 + t._23 * m._33 + t._24 * m._43;
    _33 = t._31 * m._13 + t._32 * m._23 + t._33 * m._33 + t._34 * m._43;
    _43 = t._41 * m._13 + t._42 * m._23 + t._43 * m._33 + t._44 * m._43;

    _14 = t._11 * m._14 + t._12 * m._24 + t._13 * m._34 + t._14 * m._44;
    _24 = t._21 * m._14 + t._22 * m._24 + t._23 * m._34 + t._24 * m._44;
    _34 = t._31 * m._14 + t._32 * m._24 + t._33 * m._34 + t._34 * m._44;
    _44 = t._41 * m._14 + t._42 * m._24 + t._43 * m._34 + t._44 * m._44;
*/
}

MATRIX createMatFromQtn(const QUATERNION& q)
{
    MATRIX m;
    m._11 = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
    m._12 = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
    m._13 = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
    m._14 = 0.0f;

    m._21 = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
    m._22 = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
    m._23 = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
    m._24 = 0.0f;

    m._31 = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
    m._32 = 2.0f * q.y * q.z + 2.0f * q.w * q.x;
    m._33 = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
    m._34 = 0.0f;

    m._41 = 0.0f;
    m._42 = 0.0f;
    m._43 = 0.0f;
    m._44 = 1.0f;

    return m;
}
