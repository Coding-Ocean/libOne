#include"common.h"

bool gThrowWhenHalt = false;
void halt(const char* filename, int line, const char* message) {
	char buf[8192];
	sprintf_s(buf, 8192, "%s:%d : %s", filename, line, message);
	if (gThrowWhenHalt) {
		::OutputDebugStringA(buf);
		::OutputDebugStringA("\n");
	}
	else {
		//メッセージボックスを出して
		MessageBoxA(0, buf, "Critical Error!", MB_OK | MB_ICONERROR);
		//即死
		*reinterpret_cast<int*>(0) = 0;
	}
}