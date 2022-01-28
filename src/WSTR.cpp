#include <windows.h>
#include "WSTR.h"
WSTR::WSTR(const char* str)
{
    int dataSize = (int)strlen(str) + 1;
    Wstr = new wchar_t[dataSize];
    int ret = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
        str, dataSize,
        Wstr, dataSize);
}
WSTR::~WSTR() 
{
    if (Wstr)delete[] Wstr;
}
const wchar_t* WSTR::wstr() 
{
    return Wstr;
}
WSTR::operator const wchar_t* ()
{
    return Wstr;
}
