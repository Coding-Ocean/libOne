#pragma once
#include"COLOR.h"
void initGraphic(int baseWidth, int baseHeight, bool windowed);
void freeGraphic();
void createRenderTarget();
void setViewport();
void changeSize();
void clearTarget(const COLOR& c = COLOR(0.2f, 0.2f, 0.2f, 1));
void present();

