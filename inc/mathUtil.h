#pragma once
float maxScaleX();
float maxScaleY();
void mathAxis(float maxScaleX=1.1f);
void mathPoint(float x, float y);
void mathCircle(float x, float y, float r);
void mathLine(float sx, float sy, float ex, float ey);
void mathImage(int img, float x, float y, float r = 0);
void mathText(float n, float x, float y);
void mathText(const char* s, float x, float y);
extern float MathMouseX;
extern float MathMouseY;

enum ANGLE_MODE { RADIANS, DEGREES };
extern ANGLE_MODE AngleMode;
extern const float TO_DEG;
void angleMode(ANGLE_MODE angleMode);
float sin(float angle); 
float cos(float angle);
float acos(float cosAngle);
float atan2(float y, float x);
float sqrt(float square);
float pow(float a, float b);