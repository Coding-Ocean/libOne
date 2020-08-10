#pragma once
#include"common.h"
#include"COLOR.h"
void initGraphic(int baseWidth, int baseHeight);
void freeGraphic();
void changeSize();
void clear(const COLOR& c = COLOR(0.0f, 0.0f, 0.0f, 1.0f));
void clear(float r, float g, float b);
void present();
void stroke(float r, float g, float b, float a=255);
void strokeWeight(float weight);
void noStroke();
void fill(float r, float g, float b, float a=255);
void meshColor(float r, float g, float b, float a=255);
void line(float sx, float sy, float ex, float ey);
void point(float x, float y);
void rect(float x, float y, float w, float h);
void rect(float x, float y, float w, float h, float r);
void circle(float x, float y, float diameter);
extern float Width;
extern float Height;
enum RECT_MODE { CENTER, LEFTTOP };
void rectMode(RECT_MODE mode);
int loadImage(const char* filename);
int cutImage(int img, int left, int top, int width, int height);
void image(int img, float x, float y, float r=0);
void font(const char* fontname);
void textSize(int size);
void text(const char* str, float x, float y);
void text(int n, float x, float y);
