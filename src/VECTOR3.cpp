#include <math.h>
#include "VECTOR3.h"

VECTOR3::VECTOR3( float x, float y, float z ){
    this->x = x;
    this->y = y;
    this->z = z;
}

VECTOR3 VECTOR3::operator-( const VECTOR3& v ) const{
    return VECTOR3( x - v.x,  y - v.y,  z - v.z );
}

VECTOR3 VECTOR3::operator+( const VECTOR3& v ) const{
    return VECTOR3( x + v.x,  y + v.y,  z + v.z );
}

VECTOR3 VECTOR3::operator*( float f ) const{
    return VECTOR3( x * f, y * f, z * f );
}

VECTOR3 VECTOR3::operator/( float f ) const{
    return VECTOR3( x / f, y / f, z / f );
}

VECTOR3 VECTOR3::operator-() const{
    return VECTOR3( -x, -y, -z );
}

void VECTOR3::operator+=( const VECTOR3& v ){
    x += v.x;
    y += v.y;
    z += v.z;
}

void VECTOR3::operator-=( const VECTOR3& v ){
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void VECTOR3::operator*=( float f ){
    x *= f;
    y *= f;
    z *= f;
}

void VECTOR3::operator/=( float f ){
    x /= f;
    y /= f;
    z /= f;
}

//ÇRéüå≥ÉxÉNÉgÉãÇÃàÍî ä÷êî-------------------------------------------------------
float length( const VECTOR3& a ){
    return sqrtf( a.x * a.x + a.y * a.y + a.z * a.z );
}

float length2(const VECTOR3& a){
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

float sqLength(const VECTOR3& a){
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

VECTOR3 normalize( const VECTOR3& a ){
    float l = sqrtf( a.x * a.x + a.y * a.y + a.z * a.z );
    return VECTOR3( a.x / l, a.y / l, a.z / l);
}

float dot( const VECTOR3& a, const VECTOR3& b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

VECTOR3 cross( const VECTOR3& a, const VECTOR3& b ){
    return VECTOR3( a.y * b.z  -  a.z * b.y,
                    a.z * b.x  -  a.x * b.z,
                    a.x * b.y  -  a.y * b.x );
}




