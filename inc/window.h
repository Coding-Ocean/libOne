#include<tchar.h>
#include<Windows.h>
bool msgProc();
unsigned getTime();
void initWindow(LPCTSTR caption, int clientWidth = 0, int clientHeight = 0);

extern HWND HWnd;
extern LPCTSTR CLASS_NAME;
extern int ClientWidth;
extern int ClientHeight;
extern float Aspect;
extern unsigned ActiveWindow;
extern int MouseDelta;

