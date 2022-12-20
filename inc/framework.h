#pragma once
void window(float width, float height, bool fullscreenMode=false);
void window(const char* caption, float width, float height, bool fullscreenMode=false);
bool msgProc();
bool quit();
void disableEscapeKey();
constexpr auto full = true;
//for beginner
#define program() void gmain()
#define loop() while(msgProc())
#define repeat() while(msgProc())
#define notQuit msgProc()
#define pause() while(msgProc()){}