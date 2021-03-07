#include<stdio.h>
#include <string>
#include "let.h"
char let::StaticStr[NUM_STR];
let::let() :Str(0) {
}
let::let(int v):Str(0) {
    V = (double)v;
}
let::let(float v) : Str(0) {
    V = v;
}
let::let(double v) : Str(0) {
    V = v;
}
let::let(size_t v) : Str(0) {
    V = (double)v;
}
let::let(const char* s) : Str(s) {
}

let::operator int() {
    return (int)V;
}
let::operator float() {
    return (float)V;
}
let::operator double() {
    return V;
}
const char* let::str() {
    return Str;
}
char* let::staticStr() {
    return StaticStr;
}
int let::length() {
    return (int)strlen(Str);
}

let let::operator+(int v) const{
    if (Str)
        return Str + (let)v;
    else
        return V + v;
}
let let::operator+(float v) const {
    if (Str)
        return Str + (let)v;
    else
        return V + v;
}
let let::operator+(double v) const{
    if (Str)
        return Str + (let)v;
    else
        return V + v;
}
let let::operator+(let v) const {
    if (Str) {
        return Str + v;
    }
    else if (v.Str) {
        int iv = (int)V;
        if (iv == V) {
            sprintf_s(StaticStr, "%d", iv);
        }
        else {
            sprintf_s(StaticStr, "%lf", V);
        }
        strcat_s(StaticStr, v.Str);
        return StaticStr;
    }
    return V + v.V;
}
let let::operator+(const char* v)const {
    if (Str) {
        return Str + (let)v;
    }
    else {
        int iv = (int)V;
        if (iv == V) {
            sprintf_s(StaticStr, "%d", iv);
        }
        else {
            sprintf_s(StaticStr, "%lf", V);
        }
        strcat_s(StaticStr, v);
        return StaticStr;
    }
}

let let::operator-(int v) const {
    return V - v;
}
let let::operator-(float v) const {
    return V - v;
}
let let::operator-(double v) const {
    return V - v;
}
let let::operator-(let v) const {
    return V - v.V;
}
let let::operator-()const {
    return -V;
}

let let::operator*(int v) const{
    return V * v;
}
let let::operator*(float v) const {
    return V * v;
}
let let::operator*(double v) const{
    return V * v;
}
let let::operator*(let v) const{
    return V * v.V;
}

let let::operator/(int i) const{
    return V / i;
}
let let::operator/(float i) const {
    return V / i;
}
let let::operator/(double f) const{
    return V / f;
}
let let::operator/(let v) const{
    return V / v.V;
}

int let::operator%(int i) {
    return int(V) - (int(V) / i) * i;
}

void let::operator+=(int v) {
    V = V + v;
}
void let::operator+=(float v) {
    V = V + v;
}
void let::operator+=(double v) {
    V = V + v;
}
void let::operator+=(let v) {
    V = V + v;
}

void let::operator-=(int v) {
    V = V - v;
}
void let::operator-=(float v) {
    V = V - v;
}
void let::operator-=(double v) {
    V = V - v;
}
void let::operator-=(let v) {
    V = V - v;
}

void let::operator*=(int v) {
    V = V * v;
}
void let::operator*=(float v) {
    V = V * v;
}
void let::operator*=(double v) {
    V = V * v;
}
void let::operator*=(let v) {
    V = V * v;
}

void let::operator/=(int v) {
    V = V / v;
}
void let::operator/=(float v) {
    V = V / v;
}
void let::operator/=(double v) {
    V = V / v;
}
void let::operator/=(let v) {
    V = V / v;
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
let& let::operator--() {
    V--;
    return *this;
}
let let::operator--(int) {
    let pre = *this;
    V--;
    return pre;
}

bool let::operator<(int v) {
    return V < v;
}
bool let::operator<(float v) {
    return V < v;
}
bool let::operator<(double v) {
    return V < v;
}
bool let::operator<(let v) {
    return V < v.V;
}

bool let::operator<=(int v) {
    return V <= v;
}
bool let::operator<=(float v) {
    return V <= v;
}
bool let::operator<=(double v) {
    return V <= v;
}
bool let::operator<=(let v) {
    return V <= v.V;
}

bool let::operator>(int v) {
    return V > v;
}
bool let::operator>(float v) {
    return V > v;
}
bool let::operator>(double v) {
    return V > v;
}
bool let::operator>(let v) {
    return V > v.V;
}

bool let::operator>=(int v) {
    return V >= v;
}
bool let::operator>=(float v) {
    return V >= v;
}
bool let::operator>=(double v) {
    return V >= v;
}
bool let::operator>=(let v) {
    return V >= v.V;
}

bool let::operator!=(int v) {
    return V != v;
}
bool let::operator!=(float v) {
    return V != v;
}
bool let::operator!=(double v) {
    return V != v;
}
bool let::operator!=(let v) {
    return V != v.V;
}

bool let::operator==(int v) {
    return V == v;
}
bool let::operator==(float v) {
    return V == v;
}
bool let::operator==(double v) {
    return V == v;
}
bool let::operator==(let v) {
    return V == v.V;
}
//à»â∫letÇÃÉÅÉìÉoÇ≈Ç»Ç¢ä÷êî--------------------------------
let operator+(const char* str, let v) {
    strcpy_s(let::staticStr(), NUM_STR, str);
    if (v.str()) {
        strcat_s(let::staticStr(), NUM_STR, v.str());
    }
    else {
        char addStr[64];
        int iv = (int)v;
        if (iv == (double)v) {
            sprintf_s(addStr, "%d", iv);
        }
        else {
            sprintf_s(addStr, "%lf", (double)v);
        }
        strcat_s(let::staticStr(), NUM_STR, addStr);
    }
    return let::staticStr();
}
let operator+(int i, let v) {
    return v + i;
}
let operator+(float f, let v) {
    return v + f;
}
let operator+(double d, let v) {
    return v + d;
}
let operator-(int i, let v) {
    return (let)i - v;
}
let operator-(float f, let v) {
    return (let)f - v;
}
let operator-(double d, let v) {
    return (let)d - v;
}
let operator*(int i, let v) {
    return v*i;
}
let operator*(float f, let v) {
    return v*f;
}
let operator*(double d, let v) {
    return v*d;
}
let operator/(int i, let v) {
    return (let)i / v;
}
let operator/(float f, let v) {
    return (let)f / v;
}
let operator/(double d, let v) {
    return (let)d / v;
}
