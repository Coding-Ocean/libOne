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
void present();

