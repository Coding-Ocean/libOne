#pragma once
constexpr auto NUM_STR = 128;
class let{
    double V = 0;
    const char* Str;
    static char StaticStr[NUM_STR];
public:
    let();
    let(int v);
    let(float v);
    let(double v);
    let(size_t v);
    let(const char* v);

    operator int();
    operator float();
    operator double();
    const char* str();
    static char* staticStr();
    int length();

    let operator+(int v)const;
    let operator+(float v)const;
    let operator+(double v)const;
    let operator+(let v)const;
    let operator+(const char* v)const;
    
    let operator-(int v)const;
    let operator-(float v)const;
    let operator-(double v)const;
    let operator-(let v)const;
    let operator-()const;

    let operator*(int v)const;
    let operator*(float v)const;
    let operator*(double v)const;
    let operator*(let v)const;

    let operator/(int v)const;
    let operator/(float v)const;
    let operator/(double v)const;
    let operator/(let v)const;

    int operator%(int);
    
    void operator+=(int v);
    void operator+=(float v);
    void operator+=(double v);
    void operator+=(let v);

    void operator-=(int v);
    void operator-=(float v);
    void operator-=(double v);
    void operator-=(let v);

    void operator*=(int v);
    void operator*=(double v);
    void operator*=(float v);
    void operator*=(let v);
    
    void operator/=(int v);
    void operator/=(double v);
    void operator/=(float v);
    void operator/=(let v);

    let& operator++();
    let operator++(int);
    let& operator--();
    let operator--(int);

    bool operator<(int v);
    bool operator<(float v);
    bool operator<(double v);
    bool operator<(let v);

    bool operator<=(int v);
    bool operator<=(float v);
    bool operator<=(double v);
    bool operator<=(let v);

    bool operator>(int v);
    bool operator>(float v);
    bool operator>(double v);
    bool operator>(let v);

    bool operator>=(int v);
    bool operator>=(float v);
    bool operator>=(double v);
    bool operator>=(let v);

    bool operator!=(int v);
    bool operator!=(float v);
    bool operator!=(double v);
    bool operator!=(let v);

    bool operator==(int v);
    bool operator==(float v);
    bool operator==(double v);
    bool operator==(let v);
};
let operator+(const char* str, let v);
let operator+(int i, let v);
let operator+(float f, let v);
let operator+(double d, let v);
let operator-(int i, let v);
let operator-(float f, let v);
let operator-(double d, let v);
let operator*(int i, let v);
let operator*(float f, let v);
let operator*(double d, let v);
let operator/(int i, let v);
let operator/(float f, let v);
let operator/(double d, let v);


