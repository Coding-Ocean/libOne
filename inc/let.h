#pragma once
class let{
    float V=0;
public:
    let();
    let(float v);
    operator float();
    float operator+(int v);
    float operator+(float v);
    int operator%(int);
    int operator/(int);
    void operator+=(float v);
    void operator*=(float v);
    let& operator++();
    let operator++(int);
};
char* operator+(const char* str, let v);


