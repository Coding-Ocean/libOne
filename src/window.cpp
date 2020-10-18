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

//メッセージ処理
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
	//ウィンドウクラスを登録する
	HINSTANCE hinst = GetModuleHandle( 0 );
    WNDCLASS wc = {0, winProc, 0, 0, hinst,
        LoadIcon( hinst, _T("MYICON") ),
        LoadCursor( 0, IDC_ARROW ),
		( HBRUSH )2, 0, CLASS_NAME };
    RegisterClass( &wc );

	//ウィンドウを作る
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
        CLASS_NAME, // 登録されているクラス名
        caption, // ウィンドウ名
        style,// ウィンドウスタイル
        0, // ウィンドウの横方向の位置
        0, // ウィンドウの縦方向の位置
        ClientWidth, // ウィンドウの幅（ウィンドウ枠の幅を含む）
        ClientHeight, // ウィンドウの高さ（タイトルバーとウィンドウ枠の高さを含む）
        0, // 親ウィンドウまたはオーナーウィンドウのハンドル
        0, // メニューハンドルまたは子ウィンドウ ID
        wc.hInstance, // アプリケーションインスタンスのハンドル
        0 // ウィンドウ作成データ
    );

    if (style == WS_OVERLAPPEDWINDOW) {
        //ウィンドウが中央に表示されるようなwxとwyを計算する。
        int desktopWidth = GetSystemMetrics(SM_CXSCREEN);//デスクトップの幅
        int desktopHeight = GetSystemMetrics(SM_CYSCREEN);//デスクトップの高さ
        int wx = (desktopWidth - ClientWidth) / 2;//ウィンドウ表示位置x
        int wy = (desktopHeight - ClientHeight) / 2 - 32;//ウィンドウ表示位置y
        //クライアント領域の大きさがclientWidth,clientHeightになるようなwwとwhを計算する。
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
        //ウィンドウの位置と大きさを設定しなおす
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

