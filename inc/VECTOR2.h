#pragma once

class VECTOR2{
public:
	float x,y;
	VECTOR2( float x=0, float y=0 );
    VECTOR2 operator+ ( const VECTOR2& v ) const;
    VECTOR2 operator- ( const VECTOR2& v ) const;
    VECTOR2 operator* ( float f ) const;
    VECTOR2 operator/ ( float f ) const;
    VECTOR2 operator- () const;
    void operator+= ( const VECTOR2& v );
    void operator-= ( const VECTOR2& v );
    void operator*= ( float f );
    void operator/= ( float f );
};

//VECTOR2ÇégópÇµÇΩàÍî ä÷êî
VECTOR2 operator* ( float f, const VECTOR2& v );// VECTOR2 = float * VECTOR2
float length( const VECTOR2& a );
float sqLength( const VECTOR2& a );
VECTOR2 normalize( const VECTOR2& a );
float dot( const VECTOR2& a, const VECTOR2& b );
float cross( const VECTOR2& a, const VECTOR2& b );
