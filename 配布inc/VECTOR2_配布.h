#pragma once

class VECTOR2{
public:
	float x,y;
	VECTOR2( float x=0, float y=0 ){
        this->x = x;
        this->y = y;
    }
    //以下、コメントを外しながら一つ一つ作っていこう
    //VECTOR2 operator+( const VECTOR2& v ) const;
    //VECTOR2 operator-( const VECTOR2& v ) const;
    //VECTOR2 operator*( float f );
    //VECTOR2 operator/( float f );
    //VECTOR2 operator-();
    //void operator+=( VECTOR2& v );
    //void operator-=( VECTOR2& v );
    //void operator*=( float f );
    //void operator/=( float f );
};

//VECTOR2を使用する一般関数　ここも一つ一つ作っていこう
//斜辺の長さ
float length( const VECTOR2& a );
//斜辺の長さの２乗
float sqLength( const VECTOR2& a );
//正規化
VECTOR2 normalize( const VECTOR2& a );
//内積
float dot( const VECTOR2& a, const VECTOR2& b );
//外積でzの値のみ求める
float cross( const VECTOR2& a, const VECTOR2& b );
