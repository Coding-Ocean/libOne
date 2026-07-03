#pragma once
#include<tchar.h>
#include<Windows.h>
extern HWND HWnd;
extern LPCTSTR CLASS_NAME;
extern int ClientWidth;
extern int ClientHeight;
extern unsigned ActiveWindow;
extern int MouseDelta;
extern bool Windowed;
extern float DeltaTime;
extern float FixedDeltaTime;

//clientWidthとclientHeightを省略するとフルスクリーンモードになる
void initWindow(const char* caption, int clientWidth = 0, int clientHeight = 0);

void closeWindow();

#define delta DeltaTime
void initDeltaTime();
void setDeltaTime();
unsigned getTime();

#define fixedDelta FixedDeltaTime
void setFixedDeltaTime();

bool intervalTimer(float& waitTime, float intervalTime);

bool isMainThread();
void hideCursor();
void showCursor();

void printInfo();
