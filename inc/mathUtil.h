#pragma once
#include"let.h"
float maxScaleX();
float minScaleX();
float maxScaleY();
enum AXIS_MODE { DRAW, NODRAW };
void axisMode(AXIS_MODE mode);
void mathAxis(float maxScaleX=1.1f, float bright=0);
void mathAxis(float ox, float oy, float unit, float bright=0);
void mathPoint(float x, float y);
void mathCircle(float x, float y, float diameter);
void mathRect(float x, float y, float w, float h, float angle = 0);
void mathLine(float sx, float sy, float ex, float ey);
void mathImage(int img, float x, float y, float angle = 0, float scale=1);
void mathArrow(float sx, float sy, float ex, float ey, float size = 30.0f);
void mathText(class let l, float x, float y);
void mathArc(float startAngle, float centralAngle, float radius);
//positionÇVECTORÇ≈éÛÇØéÊÇÈ,êFÇ∆ê¸ÇÃëæÇ≥Ç‡åàÇﬂÇÈÅB
void mathPoint(const class VECTOR& p, const struct COLOR& c, float strokeW);
void mathCircle(const class VECTOR& p, float diameter, 
	const struct COLOR& fillC, const struct COLOR& c, float strokeW);
void mathRect(const class VECTOR& p, float w, float h, float angle,
	enum RECT_MODE mode, const struct COLOR& fillC, const struct COLOR& strokeC, float strokeW);
void mathLine(const class VECTOR& sp, const class VECTOR& ep, const struct COLOR& c, float strokeW);
void mathImage(int img, const class VECTOR& p, float angle = 0, float scale = 1);
void mathArrow(const class VECTOR& sp, const class VECTOR& ep, 
	const struct COLOR& c, float strokeW, float size = 30.0f);
void mathText(class let l, const class VECTOR& p, enum TEXT_MODE mode, const struct COLOR& c, float size=30);
void mathBracket(const class VECTOR& sp, const class VECTOR& ep, float r,
	const struct COLOR& c, float strokeW);
void mathArc(float startAngle, float centralAngle, float radius, const struct COLOR& c, float strokeW);
extern float mathMouseX;
extern float mathMouseY;

extern const float M_PI;
extern const float M_2PI;
extern const float TO_RAD;
extern const float TO_DEG;

enum ANGLE_MODE { RADIANS, DEGREES };
extern ANGLE_MODE AngleMode;
void angleMode(ANGLE_MODE mode);
float Sin(float angle); 
float Cos(float angle);
float Tan(float angle);
float Asin(float sinAngle);
float Acos(float cosAngle);
float Atan(float tanAngle);
float Atan2(float y, float x);
float Sqrt(float square);
float Pow(float a, float b);
float Abs(float a);
float Round(float v);
float Map(float v, float a1, float a2, float b1, float b2);
float lerp(float a, float b, float f);
