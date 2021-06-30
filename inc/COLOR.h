#pragma once
struct COLOR {
    float r, g, b, a;
    COLOR();
    COLOR(float r, float g, float b);
    COLOR(float r, float g, float b, float a);
    COLOR(unsigned c);
    COLOR operator*(float f)const;
};
