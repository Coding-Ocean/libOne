#pragma comment(lib, "winmm.lib" )
#pragma comment(lib, "imm32.lib" )
#pragma comment(lib, "d3d11.lib") 
#include<tchar.h>
#include<Windows.h>
#include"window.h"
#include"graphic.h"
#include"rand.h"
#include"framework.h"

void gmain();
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, INT) {
    setRandSeed();
    gmain();
    freeGraphic();
}

void window(int width, int height, bool fullscreen){
    if (fullscreen) {
        initWindow(_T("Game"));
    }
    else {
        initWindow(_T("Game"), width, height);
    }
    initGraphic(width, height);
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

void window(const char* caption, int width, int height, bool fullscreen) {
#ifdef _UNICODE
    WSTR wCaption(caption);
    if (fullscreen) {
        initWindow(wCaption.wstr());
    }
    else {
        initWindow(wCaption.wstr(), width, height);
    }
    initGraphic(width, height);
#else
    if (fullscreen) {
        initWindow(caption);
    }
    else {
        initWindow(caption, width, height);
    }
    initGraphic(width, height);
#endif
}


