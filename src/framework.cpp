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

//window�i�V�X�e���j������
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

//�L���v�V�����w��window�i�V�X�e���j������
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
bool msgProc() {
    present();
    printStart();
    getInputState();
    if (EscapeKeyValid) {
        if (isTrigger(KEY_ESCAPE)) {
            closeWindow();
        }
    }
    MSG msg;
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.message != WM_QUIT);
}

//�G�X�P�[�v�L�[�ɂ��E�B���h�E�I���𖳌���
void disableEscapeKey() 
{
    EscapeKeyValid = false;
}
