#pragma once
class var
{
    float V=0;
public:
    var();
    var(float v);
    operator float();
    float operator+(int v);
    float operator+(float v);
    int operator%(int);
    int operator/(int);
    void operator+=(float v);
    void operator*=(float v);
    var& operator++();
    var operator++(int);
};
char* operator+(const char* str, var v);


