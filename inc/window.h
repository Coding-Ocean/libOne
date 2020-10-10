#include<tchar.h>
#include<Windows.h>
extern HWND HWnd;
extern LPCTSTR CLASS_NAME;
extern int ClientWidth;
extern int ClientHeight;
extern unsigned ActiveWindow;
extern int MouseDelta;
extern bool Windowed;
unsigned getTime();
//clientWidthとclientHeightを省略するとフルスクリーンモードになる
void initWindow(LPCTSTR caption, int clientWidth = 0, int clientHeight = 0);


