#pragma once
class WSTR 
{
    wchar_t* Wstr = 0;
public:
    WSTR(const char* str);
    ~WSTR();
    const wchar_t* wstr();
    operator const wchar_t* ();
};
