#pragma once
#include"let.h"
float maxScaleX();
float maxScaleY();
void mathAxis(float maxScaleX=1.1f, float bright=0);
void mathPoint(float x, float y);
void mathCircle(float x, float y, float r);
void mathRect(float x, float y, float w, float h);
void mathLine(float sx, float sy, float ex, float ey);
void mathImage(int img, float x, float y, float r = 0);
void mathText(class let l, float x, float y);
extern float mathMouseX;
extern float mathMouseY;

extern const float M_PI;
extern const float M_2PI;
extern const float TO_RAD;
extern const float TO_DEG;

enum ANGLE_MODE { RADIANS, DEGREES };
extern ANGLE_MODE AngleMode;
void angleMode(ANGLE_MODE mode);
float sin(float angle); 
float cos(float angle);
float asin(float sinAngle);
float acos(float cosAngle);
float atan2(float y, float x);
float sqrt(float square);
float pow(float a, float b);
float absolute(float a);

float map(float v, float a1, float a2, float b1, float b2);
