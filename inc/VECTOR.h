#pragma once
class VECTOR{
public:
    float x, y, z;
    VECTOR( float x=0, float y=0, float z=0 );
    void set(float x, float y, float z=0);
    VECTOR operator+( const VECTOR& v ) const;
    VECTOR operator-( const VECTOR& v ) const;
    VECTOR operator*( float f ) const;
    VECTOR operator/( float f ) const;
    VECTOR operator-() const;
    void operator+=( const VECTOR& v );
    void operator-=( const VECTOR& v );
    void operator*=( float f );
    void operator/=( float f );
    float sqMag() const;
    float magSq() const;//ã@î\ÇÕsqMagÇ∆ìØÇ∂
    float mag() const;
    VECTOR setMag(float mag);
    VECTOR limmit(float maxMag);
    VECTOR normalize();
    float dot(const VECTOR& v)const;
    VECTOR cross(const VECTOR& v)const;
    float crossZ(const VECTOR& v)const;//äOêœZê¨ï™ÇÃÇ›åvéZ(ÇQÇcóp)
    float crossY(const VECTOR& v)const;//äOêœYê¨ï™ÇÃÇ›åvéZ(ÇQÇcóp)
};

VECTOR operator* (float f, const VECTOR& v);// VECTOR = float * VECTOR
VECTOR normalize( const VECTOR& a );
float dot( const VECTOR& a, const VECTOR& b );
VECTOR cross( const VECTOR& a, const VECTOR& b );
float crossZ(const VECTOR& a, const VECTOR& b);
float crossY(const VECTOR& a, const VECTOR& b);
