#pragma once
void window(int width, int height, bool fullscreenMode=false);
void window(const char* caption, int width, int height, bool fullscreenMode=false);
#define repeat while(msgProc())
#define program void gmain()
#define notQuit msgProc()
#define pause() while(msgProc()){}
#define full true