#pragma once
class VECTOR2{
public:
	float x,y;
	VECTOR2( float x=0, float y=0 );
    void set(float x, float y);
    VECTOR2 operator+ ( const VECTOR2& v ) const;
    VECTOR2 operator- ( const VECTOR2& v ) const;
    VECTOR2 operator* ( float f ) const;
    VECTOR2 operator/ ( float f ) const;
    VECTOR2 operator- () const;
    void operator+= ( const VECTOR2& v );
    void operator-= ( const VECTOR2& v );
    void operator*= ( float f );
    void operator/= ( float f );
    float sqMag() const;
    float magSq() const;
    float mag() const;
    VECTOR2 normalize();
    VECTOR2 setMag(float mag);
    VECTOR2 limmit(float limmit);
    float dot(const VECTOR2& v)const;
    float cross(const VECTOR2& v)const;
    float heading() const;
};

//VECTOR2���g�p������ʊ֐�
VECTOR2 operator* ( float f, const VECTOR2& v );// VECTOR2 = float * VECTOR2
VECTOR2 normalize( const VECTOR2& a );
float dot( const VECTOR2& a, const VECTOR2& b );
float cross( const VECTOR2& a, const VECTOR2& b );

float length( const VECTOR2& a );
float sqLength( const VECTOR2& a );
