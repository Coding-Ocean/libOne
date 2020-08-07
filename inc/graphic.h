#pragma once
#include"common.h"
#include"COLOR.h"
void initGraphic(int baseWidth, int baseHeight);
void freeGraphic();
void createDevice();
void createRenderTarget();
void setViewport();
void changeSize();
void clear(const COLOR& c = COLOR(0.0f, 0.0f, 0.0f, 1.0f));
void clear(float r, float g, float b);
void present();
void stroke(float r, float g, float b, float a=255);
void fill(float r, float g, float b, float a=255);
void meshColor(float r, float g, float b, float a=255);
void strokeWeight(float weight);
void line(float sx, float sy, float ex, float ey);
void point(float x, float y);




