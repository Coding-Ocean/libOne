#pragma once
#include"common.h"
#include"COLOR.h"
#include"let.h"
//for users
extern float Width;
extern float Height;
#define width Width
#define height Height
enum COLOR_MODE { RGB, HSV };
void colorMode(COLOR_MODE mode, float colorDenominator=255);
enum RECT_MODE { CENTER, CORNER };
void rectMode(RECT_MODE mode);
enum TEXT_MODE { BOTTOM, TOP };
void textMode(TEXT_MODE mode);
TEXT_MODE getTextMode();
void clear(const struct COLOR& c);
void clear(float r, float g, float b);
void clear(float c=0);
void stroke(const struct COLOR& c);
void stroke(float r, float g, float b);
void stroke(float r, float g, float b, float a);
void stroke(float c);
void strokeWeight(float weight);
void noStroke();
void fill(const struct COLOR& c);
void fill(float r, float g, float b);
void fill(float r, float g, float b, float a);
void fill(float c);
void imageColor(const struct COLOR& c);
void imageColor(float r, float g, float b);
void imageColor(float r, float g, float b, float a);
void imageColor(float c);
void point(float x, float y);
void line(float sx, float sy, float ex, float ey);
void arrow(float sx, float sy, float ex, float ey, float size = 30.0f);
void arc(float x, float y, float startAngle, float angle, float radius);
void rect(float x, float y, float w, float h);
void rect(float x, float y, float w, float h, float angle);
void circle(float x, float y, float diameter);
struct SHAPE_VERTEX { float x, y; };
int createShape(struct SHAPE_VERTEX* vertices, int numVertices);
int createShape(struct SHAPE_VERTEX* vertices, int numVertices, long* indices, int numIndices);
void shape(int idx, float x, float y, float angle = 0, float size = 1);
void shape(int idx, const class MATRIX& m);
int loadImage(const char* filename);
int loadImageFromRes(const char* resname);
int cutImage(int idx, int left, int top, int w, int h);
void divideImage(int img, int cols, int rows, int w, int h, int* imgs);
void image(int idx, float x, float y, float angle = 0, float size = 1);
void triangle(const class VECTOR3& p0, const class VECTOR3& p1, const class VECTOR3& p2,
    const COLOR& c0, const COLOR& c1, const COLOR& c2);
void font(const char* fontname);
void textSize(float size);
void text(const char* str, float x, float y);
void text(double n, float x, float y);
void text(int n, float x, float y);
void text(let n, float x, float y);
void printStart();
void printSize(float size);
void printPosX(float x);
void printPosY(float y);
void printColor(float r, float g, float b);
void print(let textInfo);
//for framework
void initGraphic(int baseWidth, int baseHeight);
void freeGraphic();
void changeSize();
void present();
