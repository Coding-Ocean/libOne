#pragma comment(lib, "winmm.lib" )
#pragma comment(lib, "imm32.lib" )
#pragma comment(lib, "d3d11.lib") 
#pragma comment(lib, "dsound.lib" )
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

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, INT) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    setRandSeed(timeGetTime());
    timeBeginPeriod(1);
    gmain();
    timeEndPeriod(1);
    freeGraphic();
    freeSound();
}

void window(float width, float height, bool fullscreen){
    if (fullscreen) {
        initWindow(_T("Game"));
    }
    else {
        initWindow(_T("Game"), (int)width, (int)height);
    }
    initGraphic((int)width, (int)height);
    initSound();
}

class WSTR {
    wchar_t* Wstr = 0;
public:
    WSTR(const char* str)
    {
        int dataSize = (int)strlen(str) + 1;
        Wstr = new wchar_t[dataSize];
        int ret = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
            str, dataSize,
            Wstr, dataSize);
    }
    ~WSTR() {
        if (Wstr)delete[] Wstr;
    }
    const wchar_t* wstr() {
        return Wstr;
    }
};

void window(const char* caption, float width, float height, bool fullscreen) {
#ifdef _UNICODE
    WSTR wCaption(caption);
    if (fullscreen) {
        initWindow(wCaption.wstr());
    }
    else {
        initWindow(wCaption.wstr(), (int)width, (int)height);
    }
    initGraphic((int)width, (int)height);
    initSound();
#else
    if (fullscreen) {
        initWindow(caption);
    }
    else {
        initWindow(caption, (int)width, (int)height);
    }
    initGraphic((int)width, (int)height);
    initSound();
#endif
}

bool msgProc() {
    present();
    getInputState();
    printStart();
    MSG msg;
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.message != WM_QUIT);
}


