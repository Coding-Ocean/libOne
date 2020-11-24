#pragma once
#include"common.h"
#include"mathUtil.h"
#include"COLOR.h"
#include"let.h"
//for users
extern float Width;
extern float Height;
#define width Width
#define height Height
enum COLOR_MODE { RGB, HSV };
void colorMode(COLOR_MODE mode);
enum RECT_MODE { CENTER, CORNER };
void rectMode(RECT_MODE mode);
enum TEXT_MODE { BOTTOM, TOP };
void textMode(TEXT_MODE mode);
TEXT_MODE getTextMode();
void clear(const COLOR& c);
void clear(float r, float g, float b);
void clear(float c=0);
void stroke(const COLOR& c);
void stroke(float r, float g, float b, float a=255);
void stroke(float c);
void strokeWeight(float weight);
void noStroke();
void fill(const COLOR& c);
void fill(float r, float g, float b, float a = 255);
void fill(float c);
void imageColor(float r, float g, float b, float a=255);
void imageColor(float c);
void point(float x, float y);
void line(float sx, float sy, float ex, float ey);
void rect(float x, float y, float w, float h);
void rect(float x, float y, float w, float h, float angle);
void circle(float x, float y, float diameter);
struct SHAPE_VERTEX { float x, y; };
int createShape(struct SHAPE_VERTEX* vertices, int numVertices);
int createShape(struct SHAPE_VERTEX* vertices, int numVertices, long* indices, int numIndices);
void shape(int idx, float x, float y, float angle = 0, float size = 1);
void shape(int idx, const class MATRIX& m);
int loadImage(const char* filename);
int cutImage(int idx, int left, int top, int w, int h);
void image(int idx, float x, float y, float angle = 0, float size = 1);
void font(const char* fontname);
void textSize(float size);
void text(const char* str, float x, float y);
void text(double n, float x, float y);
void text(int n, float x, float y);
void text(let n, float x, float y);
//for framework
void initGraphic(int baseWidth, int baseHeight);
void freeGraphic();
void changeSize();
void present();
