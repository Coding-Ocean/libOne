#include <math.h>
#include "mathUtil.h"
#include "VECTOR2.h"
VECTOR2::VECTOR2( float x, float y ){
	this->x = x;	this->y = y;
}
void VECTOR2::set(float x, float y){
    this->x = x;    this->y = y;
}
VECTOR2 VECTOR2::operator+( const VECTOR2& v ) const {
    return VECTOR2( x + v.x, y + v.y );
}
VECTOR2 VECTOR2::operator-( const VECTOR2& v ) const{
	return VECTOR2( x - v.x, y - v.y );
}
VECTOR2 VECTOR2::operator*( float f ) const{
    return VECTOR2( x * f, y * f );
}
VECTOR2 VECTOR2::operator/( float f ) const{
	return VECTOR2( x / f, y / f );
}
VECTOR2 VECTOR2::operator-() const{
    return VECTOR2(-x, -y);
}
void VECTOR2::operator+=( const VECTOR2& v ) {
    x += v.x;
    y += v.y;
}
void VECTOR2::operator-=( const VECTOR2& v ) {
    x -= v.x;
    y -= v.y;
}
void VECTOR2::operator*=( float f ) {
    x *= f;
    y *= f;
}
void VECTOR2::operator/=( float f ) {
    x /= f;
    y /= f;
}
float VECTOR2::sqMag() const{
    return x * x + y * y;
}
float VECTOR2::magSq() const {
    return x * x + y * y;
}
float VECTOR2::mag() const{
    return sqrtf(x * x + y * y);
}
VECTOR2 VECTOR2::normalize() {
    float l = sqrtf(x * x + y * y);
    if (l != 0.0f) {
        x /= l;
        y /= l;
    }
    return VECTOR2(x, y);
}
VECTOR2 VECTOR2::setMag(float mag) {
    float l = sqrtf(x * x + y * y);
    if (l != 0.0f) {
        x /= l;
        y /= l;
        x *= mag;
        y *= mag;
    }
    return VECTOR2(x, y);
}
VECTOR2 VECTOR2::limmit(float lim){
    float l = sqrtf(x * x + y * y);
    if (l > lim) {
        x /= l;
        y /= l;
        x *= lim;
        y *= lim;
    }
    return VECTOR2(x, y);
}
float VECTOR2::dot(const VECTOR2& v)const{
    return x * v.x + y * v.y;
}
float VECTOR2::cross(const VECTOR2& v)const{
    return x * v.y - y * v.x;
}
float VECTOR2::heading() const {
    return Atan2(y, x);
}

//ÇQéüå≥ÉxÉNÉgÉãÇÃàÍî ä÷êî-------------------------------------------------------
VECTOR2 operator*( float f, const VECTOR2& v ){
    return VECTOR2( f * v.x, f * v.y );
}
VECTOR2 normalize( const VECTOR2& a ){
	float l = sqrtf( a.x * a.x + a.y * a.y );
    if( l != 0.0f )
	    return VECTOR2( a.x / l, a.y / l );
    else
        return a;
}
float dot( const VECTOR2& a, const VECTOR2& b ){
	return a.x * b.x + a.y * b.y;
}
float cross( const VECTOR2& a, const VECTOR2& b ){
	return a.x * b.y - a.y * b.x;
}

float length(const VECTOR2& a) {
    return sqrtf(a.x * a.x + a.y * a.y);
}
float sqLength(const VECTOR2& a) {
    return a.x * a.x + a.y * a.y;
}


