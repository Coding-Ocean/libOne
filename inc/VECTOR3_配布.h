#pragma once

class VECTOR3{
public:
    float   x;
    float   y;
    float   z;
    VECTOR3( float x=0.0f, float y=0.0f, float z=0.0f );
    //以下、コメントを外しながら一つ一つ作っていこう
    //VECTOR3 operator+( const VECTOR3& v ) const;
    //VECTOR3 operator-( const VECTOR3& v ) const;
    //VECTOR3 operator*( float f ) const;
    //VECTOR3 operator/( float f ) const;
    //VECTOR3 operator-() const;
    //void operator+=( const VECTOR3& v );
    //void operator-=( const VECTOR3& v );
    //void operator*=( float f );
    //void operator/=( float f );
};

//VECTOR3を使用する一般関数　ここも一つ一つ作っていこう
//内積を求める
float dot( const VECTOR3& a, const VECTOR3& b );
//外積ベクトルを求める
VECTOR3 cross( const VECTOR3& a, const VECTOR3& b );
//ベクトルを正規化する
VECTOR3 normalize( const VECTOR3& a );
//ベクトルの大きさを求める
float length( const VECTOR3& a );



