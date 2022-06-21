#include <math.h>
#include "VECTOR.h"
VECTOR::VECTOR( float x, float y, float z ){
    this->x = x;    this->y = y;    this->z = z;
}
void VECTOR::set(float x, float y, float z){
    this->x = x;    this->y = y;    this->z = z;
}
VECTOR VECTOR::operator-( const VECTOR& v ) const{
    return VECTOR( x - v.x,  y - v.y,  z - v.z );
}
VECTOR VECTOR::operator+( const VECTOR& v ) const{
    return VECTOR( x + v.x,  y + v.y,  z + v.z );
}
VECTOR VECTOR::operator*( float f ) const{
    return VECTOR( x * f, y * f, z * f );
}
VECTOR VECTOR::operator/( float f ) const{
    return VECTOR( x / f, y / f, z / f );
}
VECTOR VECTOR::operator-() const{
    return VECTOR( -x, -y, -z );
}
void VECTOR::operator+=( const VECTOR& v ){
    x += v.x;   y += v.y;   z += v.z;
}
void VECTOR::operator-=( const VECTOR& v ){
    x -= v.x;    y -= v.y;    z -= v.z;
}
void VECTOR::operator*=( float f ){
    x *= f;    y *= f;    z *= f;
}
void VECTOR::operator/=( float f ){
    x /= f;    y /= f;    z /= f;
}
float VECTOR::sqMag() const{
    return x * x + y * y + z * z;
}
float VECTOR::magSq() const{
    return x * x + y * y + z * z;
}
float VECTOR::mag() const{
    return sqrtf(x * x + y * y + z * z);
}
VECTOR VECTOR::setMag(float mag){
    float len = sqrtf(x * x + y * y + z * z);
    if (len > 0) {
        mag /= len;
        x *= mag;
        y *= mag;
        z *= mag;
    }
    return VECTOR(x, y, z);
}
VECTOR VECTOR::limmit(float maxMag) {
    float len = sqrtf(x * x + y * y + z * z);
    if (len > maxMag) {
        maxMag /= len;
        x *= maxMag; 
        y *= maxMag; 
        z *= maxMag;
    }
    return VECTOR(x, y, z);
}
VECTOR VECTOR::normalize(){
    float len = sqrtf(x * x + y * y + z * z);
    if (len > 0) {
        x /= len;
        y /= len;
        z /= len;
    }
    return VECTOR(x, y, z);
}
float VECTOR::dot(const VECTOR& v) const{
    return x * v.x + y * v.y + z * v.z;
}
VECTOR VECTOR::cross(const VECTOR& v) const{
    return VECTOR(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}
float VECTOR::crossZ(const VECTOR& v) const {
    return x * v.y - y * v.x;
}
float VECTOR::crossY(const VECTOR& v) const {
    return x * v.z - z * v.x;
}

//‚RŽŸŒ³ƒxƒNƒgƒ‹‚Ìˆê”ÊŠÖ”-------------------------------------------------------
VECTOR operator*(float f, const VECTOR& v) {
    return VECTOR(f * v.x, f * v.y, f * v.z);
}
//”ñ”j‰ónoramalize
VECTOR normalize( const VECTOR& a ){
    float l = sqrtf( a.x * a.x + a.y * a.y + a.z * a.z );
    if (l > 0) {
        return VECTOR( a.x / l, a.y / l, a.z / l);
    }
    return a;
}
float dot( const VECTOR& a, const VECTOR& b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
VECTOR cross( const VECTOR& a, const VECTOR& b ){
    return VECTOR(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}
float crossZ(const VECTOR& a, const VECTOR& b) {
    return a.x * b.y - a.y * b.x;
}
float crossY(const VECTOR& a, const VECTOR& b) {
    return a.x * b.z - a.z * b.x;
}



