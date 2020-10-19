#include<math.h>
#include"graphic.h"
#include"window.h"
#include"input.h"
#include"mathUtil.h"
static float Cx = 0;
static float Cy = 0;
static float Unit = 0;
static float MaxScaleX = 0;
extern float MathMouseX = 0;
extern float MathMouseY = 0;
float maxScaleX() {
    return MaxScaleX;
}
float maxScaleY() {
    return Cy / Unit;
}
void mathMouse() {
    float aspect = Width / Height;
    if ((float)ClientWidth / ClientHeight > aspect){
    //if (Windowed) {
    //    MathMouseX = (inValue(MOUSE_X) - Cx) / (Cx / MaxScaleX);
    //    MathMouseY = -(inValue(MOUSE_Y) - Cy) / (Cy / MaxScaleX);
        MathMouseX =  (inValue(MOUSE_X) - ClientWidth  / 2.0f) / (ClientHeight / 2.0f / (Cy/Unit));
        MathMouseY = -(inValue(MOUSE_Y) - ClientHeight / 2.0f) / (ClientHeight / 2.0f / (Cy/Unit));
    }
    else {
        MathMouseX =  (inValue(MOUSE_X) - ClientWidth  / 2.0f) / (ClientWidth / 2.0f / MaxScaleX);
        MathMouseY = -(inValue(MOUSE_Y) - ClientHeight / 2.0f) / (ClientWidth / 2.0f / MaxScaleX);
    }
}
void mathAxis(float maxScaleX) {
    Cx = Width / 2;
    Cy = Height / 2;
    MaxScaleX = maxScaleX;
    Unit = Cx / MaxScaleX;
    strokeWeight(1);
    stroke(0, 0, 0);
    fill(0, 0, 0);
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
    textSize(20);
    if (MaxScaleX < 1000) {
        //scale x
        for (int i = inc; i < Cx / Unit; i += inc) {
            float sx = Unit * i;
            line(Cx + sx, Cy - l, Cx + sx, Cy + l);//x>0
            line(Cx - sx, Cy - l, Cx - sx, Cy + l);//x<0
            text((float)i, Cx + sx - ofst[idx][0], Cy + 25);
            text((float)-i, Cx - sx - ofst[idx][1], Cy + 25);
        }
        //scale y
        for (int i = inc; i < Cy / Unit; i += inc) {
            float sy = Unit * i;
            line(Cx - l, Cy - sy, Cx + l, Cy - sy);//y>0
            line(Cx - l, Cy + sy, Cx + l, Cy + sy);//y<0
            text((float)i, Cx - ofst[idx][2], Cy - sy + 10);
            text((float)-i, Cx - ofst[idx][3], Cy + sy + 10);
        }
    }
    text("0", Cx - 15, Cy + 25);
    mathMouse();
}
void mathPoint(float x, float y) {
    point(Cx + x * Unit, Cy - y * Unit);
}
void mathCircle(float x, float y, float r) {
    circle(Cx + x * Unit, Cy - y * Unit, r * Unit);
}
void mathLine(float sx, float sy, float ex, float ey) {
    line(Cx + sx * Unit, Cy - sy * Unit, Cx + ex * Unit, Cy - ey * Unit);
}
void mathImage(int img, float x, float y, float r) {
    image(img, Cx + x * Unit, Cy - y * Unit, r);
}
void mathText(float n, float x, float y) {
    text(n, Cx + x * Unit, Cy - y * Unit);
}
void mathText(const char* s, float x, float y) {
    text(s, Cx + x * Unit, Cy - y * Unit);
}


extern ANGLE_MODE AngleMode = RADIANS;
void angleMode(ANGLE_MODE angleMode) {
    AngleMode = angleMode;
}

const float TO_RAD = 3.1415926535897932384626433832795f / 180.f;
const float TO_DEG = 180.f / 3.1415926535897932384626433832795f;

float sin(float angle) {
    if (AngleMode == DEGREES) {
        angle *= TO_RAD;
    }
    return sinf(angle);
}
float cos(float angle) {
    if (AngleMode == DEGREES){
        angle *= TO_RAD;
    }
    return cosf(angle);
}
float acos(float cosAngle) {
    float angle = acosf(cosAngle);
    if (AngleMode == DEGREES) {
        angle *= TO_DEG;
    }
    return angle;
}
float atan2(float y, float x) {
    float angle = atan2f(y, x);
    if (AngleMode == DEGREES) {
        angle *= TO_DEG;
    }
    return angle;
}
float sqrt(float square) {
    return sqrtf(square);
}
float pow(float a, float b) {
    return powf(a, b);
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
    //底の変換公式を使う。
    //log_a(x) = ln(x) / ln(a)
    //今a=2なので、ln(x) / ln(2)を計算すればいい
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

