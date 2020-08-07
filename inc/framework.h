#pragma once
void window(const char* caption, int width=0, int height=0);
void window(int width=0, int height=0);
#define repeat() while(msgProc())
#define program() void gmain()