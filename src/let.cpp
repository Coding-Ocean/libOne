#include<stdio.h>
#include <string.h>
#include "let.h"
let::let() {}
let::let(float v) {
    V = v;
}
let::operator float() {
    return V;
}
float let::operator+(int v) {
    return V + v;
}
float let::operator+(float v) {
    return V + v;
}
int let::operator%(int i) {
    return V - (int(V) / i) * i;
}
int let::operator/(int i) {
    return int(V) / i;
}
void let::operator+=(float v) {
    V += v;
}
void let::operator*=(float v) {
    V *= v;
}
let& let::operator++() {
    V++;
    return *this;
}
let let::operator++(int) {
    let pre = *this;
    V++;
    return pre;
}

char s1[128];
char s2[64];
char* operator+(const char* str, let v) 
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