#include "window.h"
#include "graphic.h"
#include "input.h"

extern LPCTSTR CLASS_NAME = _T("GameWindow");
extern int ClientWidth = 0;
extern int ClientHeight = 0;
extern bool Windowed = true;
extern float Aspect = 0;
extern unsigned ActiveWindow = 0;
extern int MouseDelta = 0;
extern HWND HWnd = 0;

//���b�Z�[�W����
LRESULT CALLBACK winProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {	
    switch( msg ) {
    case WM_SIZE:
        if( wp == SIZE_RESTORED || wp == SIZE_MAXIMIZED ){
            ClientWidth  =  lp & 0xFFFF;
            ClientHeight = (lp >> 16) & 0xFFFF;
            changeSize();
        }
        return 0;
    case WM_KEYDOWN:
        if( wp == VK_ESCAPE ) {
            PostMessage( hwnd, WM_CLOSE, 0, 0 );
        }
        return 0;
    case WM_MOUSEWHEEL:
        MouseDelta = GET_WHEEL_DELTA_WPARAM( wp );
        return 0;
    case WM_ACTIVATE:
        ActiveWindow = wp & 0xFFFF;
        return 0;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    default:
        return DefWindowProc( hwnd, msg, wp, lp );
    }
}

void initWindow( LPCTSTR caption, int clientWidth, int clientHeight ){
	//�E�B���h�E�N���X��o�^����
	HINSTANCE hinst = GetModuleHandle( 0 );
    WNDCLASS wc = {0, winProc, 0, 0, hinst,
        LoadIcon( hinst, _T("MYICON") ),
        LoadCursor( 0, IDC_ARROW ),
		( HBRUSH )2, 0, CLASS_NAME };
    RegisterClass( &wc );

	//�E�B���h�E�����
    DWORD style;
    if (clientWidth != 0 && clientHeight != 0) {
        ClientWidth = clientWidth;
        ClientHeight = clientHeight;
        Aspect = (float)ClientWidth / ClientHeight;
        style = WS_OVERLAPPEDWINDOW;
        Windowed = true;
    }
    else {
        ClientWidth = GetSystemMetrics(SM_CXSCREEN);
        ClientHeight = GetSystemMetrics(SM_CYSCREEN);
        Aspect = (float)ClientWidth / ClientHeight;
        style = WS_POPUP;
        Windowed = false;
    }
    HWnd = CreateWindow(
        CLASS_NAME, // �o�^����Ă���N���X��
        caption, // �E�B���h�E��
        style,// �E�B���h�E�X�^�C��
        0, // �E�B���h�E�̉������̈ʒu
        0, // �E�B���h�E�̏c�����̈ʒu
        ClientWidth, // �E�B���h�E�̕��i�E�B���h�E�g�̕����܂ށj
        ClientHeight, // �E�B���h�E�̍����i�^�C�g���o�[�ƃE�B���h�E�g�̍������܂ށj
        0, // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
        0, // ���j���[�n���h���܂��͎q�E�B���h�E ID
        wc.hInstance, // �A�v���P�[�V�����C���X�^���X�̃n���h��
        0 // �E�B���h�E�쐬�f�[�^
    );

    if (style == WS_OVERLAPPEDWINDOW) {
        //�E�B���h�E�������ɕ\�������悤��wx��wy���v�Z����B
        int desktopWidth = GetSystemMetrics(SM_CXSCREEN);//�f�X�N�g�b�v�̕�
        int desktopHeight = GetSystemMetrics(SM_CYSCREEN);//�f�X�N�g�b�v�̍���
        int wx = (desktopWidth - ClientWidth) / 2;//�E�B���h�E�\���ʒux
        int wy = (desktopHeight - ClientHeight) / 2 - 32;//�E�B���h�E�\���ʒuy
        //�N���C�A���g�̈�̑傫����clientWidth,clientHeight�ɂȂ�悤��ww��wh���v�Z����B
        RECT rect;
        GetWindowRect(HWnd, &rect);
        int ww = rect.right - rect.left;
        int wh = rect.bottom - rect.top;
        GetClientRect(HWnd, &rect);
        int cw = rect.right - rect.left;
        int ch = rect.bottom - rect.top;
        int fw = ww - cw;
        int fh = wh - ch;
        ww = clientWidth + fw;
        wh = clientHeight + fh;
        //�E�B���h�E�̈ʒu�Ƒ傫����ݒ肵�Ȃ���
        SetWindowPos(HWnd, 0, wx, wy, ww, wh, 0);
    }

    //IME off
    HIMC hIMC = ImmGetContext( HWnd );
    ImmSetOpenStatus( hIMC, 0 );
    ImmReleaseContext( HWnd, hIMC );

    ShowWindow( HWnd, SW_SHOW );
}

unsigned getTime(){
    return timeGetTime();
}

