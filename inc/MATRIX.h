#pragma once
#include"VECTOR.h"

class MATRIX {
public:
    float   _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44;
    //�s�������
    void identity();
    void translate(float x, float y, float z = 0);
    void translate(class VECTOR& t);
    void scaling(float x, float y, float z=1);
    void rotateZ(float angle);
    void rotateY(float angle);
    void rotateX(float angle);
    void rotateYXZ(const VECTOR& r);
    void rotateQuaternion(const class QUATERNION& q);
    //�r���[�A�v���W�F�N�V����������
    void camera(const VECTOR& camPos, const VECTOR& lookatPos, const VECTOR& upVec);
    void pers(float angle, float aspect, float n, float f);
    void ortho(float l, float r, float b, float t, float n, float f);
    //�s�񓯎m�̊|���Z
    void mulTranslate(float x, float y, float z = 0);
    void mulTranslate(const VECTOR& t);
    void mulScaling(float x, float y, float z = 1);
    void mulRotateZ(float angle);
    void mulRotateY(float angle);
    void mulRotateX(float angle);
    void mulRotateYXZ(const VECTOR& r);
    void mulRotateQuaternion(const class QUATERNION& q);
    //�s��ƍs��̊|���Z
    MATRIX operator*(const MATRIX& m) const;
    //�s��ƃx�N�g���̊|���Z
    VECTOR operator*(const VECTOR& v) const;
};
MATRIX createMatFromQtn(const class QUATERNION& q);