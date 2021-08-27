#pragma once
#include"VECTOR.h"

class MATRIX {
public:
    float   _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44;
    //�s�������
    //MATRIX();
    void identity();
    void translate(float x, float y, float z=0);
    void scaling(float x, float y, float z=1);
    void rotateZ(float angle);
    void rotateY(float angle);
    void rotateX(float angle);
    void camera(const VECTOR& camPos, const VECTOR& lookatPos, const VECTOR& upVec);
    void pers(float angle, float aspect, float n, float f);
    void ortho(float l, float r, float b, float t, float n, float f);
    //�s�񓯎m�̊|���Z
    void mulTranslate(float x, float y, float z = 0);
    void mulScaling(float x, float y, float z = 1);
    void mulRotateZ(float angle);
    void mulRotateY(float angle);
    void mulRotateX(float angle);
    MATRIX operator*(const MATRIX& m) const;
    //�s��ƃx�N�g���̊|���Z
    VECTOR operator*(const VECTOR& v) const;
};