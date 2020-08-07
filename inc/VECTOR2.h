#pragma once

class VECTOR2{
public:
	float x,y;
	VECTOR2( float x=0, float y=0 ){
        this->x = x;
        this->y = y;
    }
    //�ȉ��A�R�����g���O���Ȃ��������Ă�����
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

//VECTOR2���g�p�����ʊ֐��@������������Ă�����
//�Εӂ̒���
float length( const VECTOR2& a );
//�Εӂ̒����̂Q��
float sqLength( const VECTOR2& a );
//���K��
VECTOR2 normalize( const VECTOR2& a );
//����
float dot( const VECTOR2& a, const VECTOR2& b );
//�O�ς�z�̒l�̂݋��߂�
float cross( const VECTOR2& a, const VECTOR2& b );
