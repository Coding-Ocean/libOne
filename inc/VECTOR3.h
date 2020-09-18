#ifndef VECTOR3_HD
#define VECTOR3_HD

#include "VECTOR2.h"

class VECTOR3{
public:
    float x, y, z;
    VECTOR3( float x=0.0f, float y=0.0f, float z=0.0f );
    VECTOR3 operator+( const VECTOR3& v ) const;
    VECTOR3 operator-( const VECTOR3& v ) const;
    VECTOR3 operator*( float f ) const;
    VECTOR3 operator/( float f ) const;
    VECTOR3 operator-() const;
    void operator+=( const VECTOR3& v );
    void operator-=( const VECTOR3& v );
    void operator*=( float f );
    void operator/=( float f );
    VECTOR2 xy(){ return VECTOR2( x, y ); }
    VECTOR2 xz(){ return VECTOR2( x, z ); }
};

float length( const VECTOR3& a );

float sqLength(const VECTOR3& a);
float length2(const VECTOR3& a);

VECTOR3 normalize( const VECTOR3& a );

float dot( const VECTOR3& a, const VECTOR3& b );

VECTOR3 cross( const VECTOR3& a, const VECTOR3& b );

#endif



