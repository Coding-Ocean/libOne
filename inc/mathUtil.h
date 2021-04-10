#pragma once
#include"let.h"
float maxScaleX();
float maxScaleY();
enum AXIS_MODE { DRAW, NODRAW };
void axisMode(AXIS_MODE mode);
void mathAxis(float maxScaleX=1.1f, float bright=0);
void mathPoint(float x, float y);
void mathCircle(float x, float y, float r);
void mathRect(float x, float y, float w, float h, float angle = 0);
void mathLine(float sx, float sy, float ex, float ey);
void mathImage(int img, float x, float y, float angle = 0, float scale=1);
void mathArrow(float sx, float sy, float ex, float ey, float size = 30.0f);
void mathText(class let l, float x, float y);
void mathArc(float startAngle, float centralAngle, float radius);
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
float tan(float angle);
float asin(float sinAngle);
float acos(float cosAngle);
float atan(float tanAngle);
float atan2(float y, float x);
float sqrt(float square);
float pow(float a, float b);
float absolute(float a);
float round(float v);
float map(float v, float a1, float a2, float b1, float b2);
