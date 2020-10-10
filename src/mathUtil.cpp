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
float sin(float angle) {
    if (AngleMode == DEGREES) {
        angle = angle * 3.141592f / 180.0f;
    }
    return sinf(angle);
}
float cos(float angle) {
    if (AngleMode == DEGREES){
        angle = angle * 3.141592f / 180.0f;
    }
    return cosf(angle);
}
float sqrt(float square) {
    return sqrtf(square);
}
float acos(float cosTheta) {
    return acosf(cosTheta);
}

