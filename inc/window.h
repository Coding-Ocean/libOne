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

//clientWidth��clientHeight���ȗ�����ƃt���X�N���[�����[�h�ɂȂ�
void initWindow(const char* caption, int clientWidth = 0, int clientHeight = 0);

void closeWindow();

#define delta DeltaTime
void initDeltaTime();
void setDeltaTime();
unsigned getTime();

bool isMainThread();
void hideCursor();
void showCursor();
