#pragma comment(lib, "winmm.lib" )
#pragma comment(lib, "imm32.lib" )
#pragma comment(lib, "d3d11.lib") 
#include<tchar.h>
#include<Windows.h>
#include"window.h"
#include"graphic.h"
#include"framework.h"
void gmain();

void window(const char* caption, int width, int height) 
{
#ifdef _UNICODE
    int dataSize = strlen(caption)+1;
    wchar_t* wCaption = new wchar_t[dataSize];
    int ret = MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        caption,
        dataSize,
        wCaption,
        dataSize);
	initWindow(wCaption, width, height);
    initGraphic(width, height, true);
    delete[] wCaption;
#else
    initWindow(caption, width, height);
#endif
}

void window(int width, int height)
{
	initWindow(_T("Coding Ocean"), width, height);
    initGraphic(width, height, true);
}

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, INT)
{
	gmain();
    freeGraphic();
}