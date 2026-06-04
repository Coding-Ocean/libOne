#pragma comment(lib, "winmm.lib" )
#pragma comment(lib, "imm32.lib" )
#pragma comment(lib, "d3d11.lib") 
#pragma comment(lib, "dsound.lib" )
#pragma comment(lib, "Xinput.lib" )
#include<crtdbg.h>
#include<tchar.h>
#include<Windows.h>
#include"window.h"
#include"graphic.h"
#include"input.h"
#include"sound.h"
#include"rand.h"
#include"framework.h"

void gmain();

//framework
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, INT) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    setRandSeed(timeGetTime());
    timeBeginPeriod(1);

    gmain();
    
    showCursor();
    timeEndPeriod(1);
    freeGraphic();
    freeSound();
    return 0;
}

//window（システム）初期化
void window(float width, float height, bool fullscreen){
    if (fullscreen) {
        initWindow("Game");
    }
    else {
        initWindow("Game", (int)width, (int)height);
    }
    initGraphic((int)width, (int)height);
    initSound();
}

//キャプション指定window（システム）初期化
void window(const char* caption, float width, float height, bool fullscreen) {
    if (fullscreen) {
        initWindow(caption);
    }
    else {
        initWindow(caption, (int)width, (int)height);
    }
    initGraphic((int)width, (int)height);
    initSound();
}

bool EscapeKeyValid = true;
//エスケープキーによるウィンドウ終了を無効化
void disableEscapeKey()
{
    EscapeKeyValid = false;
}
bool msgProc() {
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    present();
    printStart();
    getInputState();
    if (EscapeKeyValid) {
        if (isTrigger(KEY_ESCAPE)) {
            closeWindow();
        }
    }
    return true;
}

bool FirstTime = true;
bool msgProcDelta() {
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    present();
    printStart();
    getInputState();
    if (EscapeKeyValid) {
        if (isTrigger(KEY_ESCAPE)) {
            closeWindow();
        }
    }
    if (FirstTime) {
        initDeltaTime();
        FirstTime = false;
    }
    setDeltaTime();
    return true;
}
bool quit() {
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return true;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    present();
    printStart();
    getInputState();
    return false;
}
void background(float r, float g, float b){
    clear();
    noStroke();
    fill(r, g, b);
    rectMode(CORNER);
    rect(0, 0, width, height);
    strokeWeight(1.0f);
    fill(255);
}
bool noPresent()
{
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    getInputState();
    if (EscapeKeyValid) {
        if (isTrigger(KEY_ESCAPE)) {
            closeWindow();
        }
    }
    return true;
}
