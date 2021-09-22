extern float ColorDenominator;//graphic.cpp
#include "COLOR.h"
COLOR::COLOR() {
    r = g = b = a = ColorDenominator;
}
COLOR::COLOR(float r, float g, float b) {
    this->r = r; this->g = g; this->b = b; this->a = ColorDenominator;
}
COLOR::COLOR(float r, float g, float b, float a) {
    this->r = r; this->g = g; this->b = b; this->a = a;
}
COLOR::COLOR(unsigned c) {
    a = float(c >> 24);
    r = float((c & 0x00ff0000) >> 16);
    g = float((c & 0x0000ff00) >> 8);
    b = float(c & 0x000000ff);
}
COLOR COLOR::operator*(float f)const {
    return(COLOR(r * f, g * f, b * f, a));//ƒAƒ‹ƒtƒ@‚Í‚»‚Ì‚Ü‚Ü
}
void COLOR::set(float r, float g, float b) {
    this->r = r; this->g = g; this->b = b; this->a = ColorDenominator;
}
void COLOR::set(float r, float g, float b, float a) {
    this->r = r; this->g = g; this->b = b; this->a = a;
}
