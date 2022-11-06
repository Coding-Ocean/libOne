#include<math.h>
#include"graphic.h"
#include"window.h"
#include"input.h"
#include"VECTOR.h"
#include"mathUtil.h"
static float Cx = 0;
static float Cy = 0;
static float Unit = 0;
static float MaxScaleX = 0;
static float MinScaleX = 0;
extern float mathMouseX = 0;
extern float mathMouseY = 0;
float maxScaleX() {
    return MaxScaleX;
}
float minScaleX()
{
    return MinScaleX;
}
float maxScaleY() {
    return Cy / Unit;
}
void mathMouse() {
    float aspect = Width / Height;
    if ((float)ClientWidth / ClientHeight > aspect){
        mathMouseX =  (inValue(MOUSE_X) - ClientWidth  / 2.0f) / (ClientHeight / 2.0f / (Cy/Unit));
        mathMouseY = -(inValue(MOUSE_Y) - ClientHeight / 2.0f) / (ClientHeight / 2.0f / (Cy/Unit));
    }
    else {
        mathMouseX =  (inValue(MOUSE_X) - ClientWidth  / 2.0f) / (ClientWidth / 2.0f / MaxScaleX);
        mathMouseY = -(inValue(MOUSE_Y) - ClientHeight / 2.0f) / (ClientWidth / 2.0f / MaxScaleX);
    }
}
void mathMouse2() {
//    float aspect = Width / Height;
//    if ((float)ClientWidth / ClientHeight > aspect) {
//    }
//    else {
    mathMouseX =  (inValue(MOUSE_X) - Cx - (ClientWidth-Width)/2)/Unit;
    mathMouseY = -(inValue(MOUSE_Y) - Cy )/Unit;
//    }
}
AXIS_MODE AxisMode = DRAW;
void axisMode(AXIS_MODE mode) {
    AxisMode = mode;
}
void mathAxis(float maxScaleX, float bright){
    Cx = Width / 2;
    Cy = Height / 2;
    MaxScaleX = maxScaleX;
    Unit = Cx / MaxScaleX;

    if (AxisMode == DRAW) {
        strokeWeight(1);
        stroke(bright);
        fill(bright);
        line(0, Cy, Width, Cy);
        line(Cx, 0, Cx, Height);
        //scale x y
        int ofst[3][4] = {
            15,25,35,45,//MaxScaleX>=100
            10,20,25,35,//MaxScaleX>=10
            5,15,15,25//MaxScaleX<10;
        };
        float l = 5;
        int inc = 0;
        int idx = 0;
        if (MaxScaleX >= 1000) {
        }
        else if (MaxScaleX >= 100) {
            idx = 0; inc = 100;
        }
        else if (MaxScaleX >= 10) {
            idx = 1; inc = 10;
        }
        else {
            idx = 2; inc = 1;
        }
        TEXT_MODE preTextMode = getTextMode();
        textMode(TOP);
        textSize(20);
        if (MaxScaleX < 1000) {
            //scale x
            for (int i = inc; i < Cx / Unit; i += inc) {
                float sx = Unit * i;
                line(Cx + sx, Cy - l, Cx + sx, Cy + l);//x>0
                line(Cx - sx, Cy - l, Cx - sx, Cy + l);//x<0
                text((float)i, Cx + sx - ofst[idx][0], Cy + 7);
                text((float)-i, Cx - sx - ofst[idx][1], Cy + 7);
            }
            //scale y
            for (int i = inc; i < Cy / Unit; i += inc) {
                float sy = Unit * i;
                line(Cx - l, Cy - sy, Cx + l, Cy - sy);//y>0
                line(Cx - l, Cy + sy, Cx + l, Cy + sy);//y<0
                text((float)i, Cx - ofst[idx][2], Cy - sy - 10);
                text((float)-i, Cx - ofst[idx][3], Cy + sy - 10);
            }
        }
        text("0", Cx - 15, Cy + 7);
        textMode(preTextMode);
    }
    mathMouse();
}

void mathAxis(float ox, float oy, float unit, float bright) 
{
    Cx = ox;
    Cy = oy;

    if (unit < 1)unit = 1;
    Unit = unit;
    MaxScaleX = (Width-Cx)/Unit;
    MinScaleX = -Cx / Unit;

    if (AxisMode == DRAW) {
        strokeWeight(1);
        stroke(bright);
        fill(bright);
        //axis X
        line(0, Cy, Width, Cy);
        //axis y
        line(Cx, 0, Cx, Height);
        TEXT_MODE preTextMode = getTextMode();
        textMode(BCENTER);
        textSize(20);
        float l = 5;
        //scale x
        int cnt = 1,ofst=0;
        for (float x = Cx+Unit; x < Width; x += Unit) {
            ofst = cnt >= 10 ? 5 : 0;
            line(x, Cy - l, x, Cy + l);
            text(cnt, x - ofst, Cy + 15 );
            cnt++;
        }
        cnt = -1;
        for (float x = Cx - Unit; x > 0; x -= Unit) {
            ofst = cnt <= -10 ? 15 : 10;
            line(x, Cy - l, x, Cy + l);
            text(cnt, x-ofst, Cy + 15);
            cnt--;
        }
        //scale y
        cnt = 1;
        for (float y = Cy-Unit; y > 0; y -= Unit) {
            ofst = cnt >= 10 ? 25 : 15;
            line(Cx - l, y, Cx + l, y);
            text(cnt, Cx - ofst, y);
            cnt++;
        }
        cnt = -1;
        for (float y = Cy + Unit; y < height; y += Unit) {
            ofst = cnt <= -10 ? 35 : 25;
            line(Cx - l, y, Cx + l, y);
            text(cnt, Cx - ofst, y);
            cnt--;
        }
        text("0", Cx - 15, Cy + 15);
        textMode(preTextMode);
    }
    mathMouse2();
    fill(255);
}

void mathPoint(float x, float y) {
    point(Cx + x * Unit, Cy - y * Unit);
}
void mathCircle(float x, float y, float r) {
    circle(Cx + x * Unit, Cy - y * Unit, r * Unit);
}
void mathRect(float x, float y, float w, float h, float angle) {
    rect(Cx + x * Unit, Cy - y * Unit, w * Unit, h * Unit, -angle);
}
void mathLine(float sx, float sy, float ex, float ey) {
    line(Cx + sx * Unit, Cy - sy * Unit, Cx + ex * Unit, Cy - ey * Unit);
}
void mathImage(int img, float x, float y, float angle, float scale) {
    image(img, Cx + x * Unit, Cy - y * Unit, -angle, scale);
}
void mathText(let l, float x, float y) {
    text(l, Cx + x * Unit, Cy - y * Unit);
}
void mathArrow(float sx, float sy, float ex, float ey, float size) {
    if (sx == ex && sy == ey)return;
    arrow(Cx + sx * Unit, Cy - sy * Unit, Cx + ex * Unit, Cy - ey * Unit, size);
}
void mathArc(float startAngle, float centralAngle, float radius) {
    //ŒÊ‚Ì•ªŠ„” num 
    int num = int(centralAngle / 10);
    if (AngleMode == RADIANS) num = int(round(centralAngle * TO_DEG)) / 10;
    if (num < 0)num = -num;
    num += 1;
    //•ªŠ„‚µ‚½Šp‚Ì‘å‚«‚³
    float divAngle = centralAngle / num;
    //•ªŠ„”•ª‚Ìü‚ðˆø‚­
    float x1 = Cos(startAngle) * radius;
    float y1 = Sin(startAngle) * radius;
    for (int i = 1; i <= num; i++) {
        float angle = startAngle + divAngle * i;
        float x2 = Cos(angle) * radius;
        float y2 = Sin(angle) * radius;
        mathLine(x1, y1, x2, y2);
        x1 = x2;
        y1 = y2;
    }
}
void mathBracket(const VECTOR& sp,const VECTOR& ep, float r,
    const struct COLOR& c, float strokeW)
{
    strokeWeight(strokeW);
    stroke(c);
    VECTOR v = ep - sp;
    VECTOR nv = normalize(v);
    VECTOR sc = sp + nv * r;
    VECTOR ec = ep - nv * r;
    VECTOR mp = sp + v / 2 - VECTOR(-nv.y, nv.x) * r;
    angleMode(DEGREES);
    float angle = 90;
    float num = 10;
    //•ªŠ„‚µ‚½Šp‚Ì‘å‚«‚³
    float divAngle = angle / num;
    //•ªŠ„”•ª‚Ìü‚ðˆø‚­
    float x1, y1, x2, y2, a;
    x1 = sc.x + -nv.x * r * Cos(9) + -nv.y * r * -Sin(9);
    y1 = sc.y + -nv.x * r * Sin(9) + -nv.y * r * Cos(9);
    for (int i = 2; i <= num; i++) {
        a = 9.0f * i;
        x2 = sc.x + -nv.x * r * Cos(a) + -nv.y * r * -Sin(a);
        y2 = sc.y + -nv.x * r * Sin(a) + -nv.y * r * Cos(a);
        mathLine(x1, y1, x2, y2);
        x1 = x2;
        y1 = y2;
    }
    x1 = ec.x + -nv.x * r * Cos(a) + -nv.y * r * -Sin(a);
    y1 = ec.y + -nv.x * r * Sin(a) + -nv.y * r * Cos(a);
    mathLine(x2, y2, x1, y1);
    for (int i = 1; i < num; i++) {
        a = 90 + divAngle * i;
        x2 = ec.x + -nv.x * r * Cos(a) + -nv.y * r * -Sin(a);
        y2 = ec.y + -nv.x * r * Sin(a) + -nv.y * r * Cos(a);
        mathLine(x1, y1, x2, y2);
        x1 = x2;
        y1 = y2;
    }
    mathPoint(mp, c, 20);
}
void mathPoint(const class VECTOR& p, const struct COLOR& c, float strokeW){
    strokeWeight(strokeW);
    stroke(c);
    mathPoint(p.x, p.y);
}
void mathCircle(const class VECTOR& p, float r, 
    const COLOR& fillC, const struct COLOR& strokeC, float strokeW){
    strokeWeight(strokeW);
    stroke(strokeC);
    fill(fillC);
    mathCircle(p.x, p.y, r);
}
void mathRect(const class VECTOR& p, float w, float h, float angle,
    enum RECT_MODE mode, const struct COLOR& fillC, const struct COLOR& strokeC, float strokeW){
    strokeWeight(strokeW);
    stroke(strokeC);
    fill(fillC);
    rectMode(mode);
    mathRect(p.x, p.y, w, h, angle);
}
void mathLine(const class VECTOR& sp, const class VECTOR& ep, const struct COLOR& c, float strokeW){
    strokeWeight(strokeW);
    stroke(c);
    mathLine(sp.x, sp.y, ep.x, ep.y);
}
void mathImage(int img, const class VECTOR& p, float angle, float scale){}
void mathArrow(const class VECTOR& sp, const class VECTOR& ep,
    const struct COLOR& c, float strokeW, float size){
    strokeWeight(strokeW);
    stroke(c);
    mathArrow(sp.x, sp.y, ep.x, ep.y, size);
}
void mathText(class let l, const class VECTOR& p, TEXT_MODE mode,
    const struct COLOR& c, float size ){
    textMode(mode);
    textSize(size);
    fill(c);
    mathText(l, p.x, p.y);
}
void mathArc(float startAngle, float centralAngle, float radius, const struct COLOR& c, float strokeW){
    strokeWeight(strokeW);
    stroke(c);
    mathArc(startAngle, centralAngle, radius);
}


extern ANGLE_MODE AngleMode = RADIANS;
void angleMode(ANGLE_MODE mode) {
    AngleMode = mode;
}
extern const float M_PI = 3.1415926535897932384626433832795f;
extern const float M_2PI = 3.1415926535897932384626433832795f * 2;
extern const float TO_RAD = 3.1415926535897932384626433832795f / 180.f;
extern const float TO_DEG = 180.f / 3.1415926535897932384626433832795f;

float Sin(float angle) {
    if (AngleMode == DEGREES) {
        angle *= TO_RAD;
    }
    return sinf(angle);
}
float Cos(float angle) {
    if (AngleMode == DEGREES){
        angle *= TO_RAD;
    }
    return cosf(angle);
}
float Tan(float angle) {
    if (AngleMode == DEGREES) {
        angle *= TO_RAD;
    }
    return tanf(angle);
}
float Asin(float sinAngle) {
    float angle = asinf(sinAngle);
    if (AngleMode == DEGREES) {
        angle *= TO_DEG;
    }
    return angle;
}
float Acos(float cosAngle) {
    float angle = acosf(cosAngle);
    if (AngleMode == DEGREES) {
        angle *= TO_DEG;
    }
    return angle;
}
float Atan(float tanAngle) {
    float angle = atanf(tanAngle);
    if (AngleMode == DEGREES) {
        angle *= TO_DEG;
    }
    return angle;
}
float Atan2(float y, float x) {
    float angle = atan2f(y, x);
    if (AngleMode == DEGREES) {
        angle *= TO_DEG;
    }
    return angle;
}
float Sqrt(float square) {
    return sqrtf(square);
}
float Pow(float a, float b) {
    return powf(a, b);
}
float Abs(float a) {
    return a >= 0.0f ? a : -a;
}
float Round(float v) {
    return (float)roundf(v);
}
float Map(float value, float start1, float stop1, float start2, float stop2)
{
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}
float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}



/*
float sin(float a) {
    return sinf(a * TO_RAD);
}

float cos(float a) {
    return cosf(a * TO_RAD);
}

float tan(float a) {
    return tanf(a * TO_RAD);
}

float asin(float a) {
    ASSERT(a >= -1.f);
    ASSERT(a <= 1.f);
    return asinf(a) * TO_DEG;
}

float acos(float a) {
    ASSERT(a >= -1.f);
    ASSERT(a <= 1.f);
    return acosf(a) * TO_DEG;
}

float atan(float a) {
    return atanf(a) * TO_DEG;
}

float atan2(float y, float x) {
    return atan2f(y, x) * TO_DEG;
}

float pow(float a, float b) {
    return powf(a, b);
}

float log2(float a) {
    //’ê‚Ì•ÏŠ·ŒöŽ®‚ðŽg‚¤B
    //log_a(x) = ln(x) / ln(a)
    //¡a=2‚È‚Ì‚ÅAln(x) / ln(2)‚ðŒvŽZ‚·‚ê‚Î‚¢‚¢
    float logE = logf(a);
    static const float rcpLogE2 = 1.4426950408889634073599246810019f;
    return logE * rcpLogE2;
}

float log10(float a) {
    return log10f(a);
}

float abs(float a) {
    return (a < 0.f) ? -a : a;
}

float min(float a, float b) {
    return (a < b) ? a : b;
}

float max(float a, float b) {
    return (a > b) ? a : b;
}

float sqrt(float a) {
    ASSERT(a >= 0.f);
    return sqrtf(a);
}
*/

