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
#define delta DeltaTime
unsigned getTime();
void initDeltaTime();
void setDeltaTime();
void closeWindow();
//clientWidth��clientHeight���ȗ�����ƃt���X�N���[�����[�h�ɂȂ�
void initWindow(LPCTSTR caption, int clientWidth = 0, int clientHeight = 0);
