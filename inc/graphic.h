#pragma once
#include"common.h"
#include"mathUtil.h"
#include"COLOR.h"
void initGraphic(int baseWidth, int baseHeight);
void freeGraphic();
void changeSize();
void clear(const COLOR& c = COLOR(0.0f, 0.0f, 0.0f));
void clear(float r, float g, float b);
void clear(float c);
void present();
void stroke(const COLOR& c = COLOR(0.0f, 0.0f, 0.0f));
void stroke(float r, float g, float b, float a=255);
void stroke(float c);
void strokeWeight(float weight);
void noStroke();
void fill(float r, float g, float b, float a = 255);
void fill(const COLOR& c);
void fill(float c);
void meshColor(float r, float g, float b, float a=255);
void meshColor(float c);
void line(float sx, float sy, float ex, float ey);
void point(float x, float y);
void rect(float x, float y, float w, float h);
void rect(float x, float y, float w, float h, float angle);
void circle(float x, float y, float diameter);
struct SHAPE_VERTEX { float x, y; };
int createShape(struct SHAPE_VERTEX* vertices, int numVertices);
int createShape(struct SHAPE_VERTEX* vertices, int numVertices, long* indices, int numIndices);
void shape(int idx, float x, float y, float angle = 0, float size = 1);
void shape(int idx, const class MATRIX& m);
enum COLOR_MODE { RGB, HSV };
void colorMode(COLOR_MODE mode);
enum RECT_MODE { CENTER, CORNER };
void rectMode(RECT_MODE mode);
extern float Width;
extern float Height;
#define width Width
#define height Height
int loadImage(const char* filename);
int cutImage(int idx, int left, int top, int w, int h);
void image(int idx, float x, float y, float angle = 0, float size = 1);
void font(const char* fontname);
void textSize(int size);
void text(const char* str, float x, float y);
void text(float n, float x, float y);
