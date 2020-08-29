#include<stdio.h>
#include <string.h>
#include "var.h"
var::var() {}
var::var(float v) {
    V = v;
}
var::operator float() {
    return V;
}
float var::operator+(int v) {
    return V + v;
}
float var::operator+(float v) {
    return V + v;
}
int var::operator%(int i) {
    return V - (int(V) / i) * i;
}
int var::operator/(int i) {
    return int(V) / i;
}
void var::operator+=(float v) {
    V += v;
}
void var::operator*=(float v) {
    V *= v;
}
var& var::operator++() {
    V++;
    return *this;
}
var var::operator++(int) {
    var pre = *this;
    V++;
    return pre;
}

char s1[128];
char s2[64];
char* operator+(const char* str, var v) 
{
    int iv = (int)v;
    if (iv == v) {
        sprintf_s(s2, "%d", iv);
    }
    else {
        sprintf_s(s2, "%f", (float)v);
    }
    strcpy_s(s1, str);
    strcat_s(s1, s2);
    return s1;
}