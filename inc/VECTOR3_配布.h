#pragma once

class VECTOR3{
public:
    float   x;
    float   y;
    float   z;
    VECTOR3( float x=0.0f, float y=0.0f, float z=0.0f );
    //�ȉ��A�R�����g���O���Ȃ��������Ă�����
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

//VECTOR3���g�p�����ʊ֐��@������������Ă�����
//���ς����߂�
float dot( const VECTOR3& a, const VECTOR3& b );
//�O�σx�N�g�������߂�
VECTOR3 cross( const VECTOR3& a, const VECTOR3& b );
//�x�N�g���𐳋K������
VECTOR3 normalize( const VECTOR3& a );
//�x�N�g���̑傫�������߂�
float length( const VECTOR3& a );



