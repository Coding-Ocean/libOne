#pragma once
void window(float width, float height, bool fullscreenMode=false);
void window(const char* caption, float width, float height, bool fullscreenMode=false);
bool msgProc();
bool msgProcDelta();
bool quit();
void disableEscapeKey();
void background(float r, float g, float b);
bool noPresent();
constexpr auto full = true;
//for beginner
#define notQuit msgProc()
#define program() void gmain()
#define loop() while(msgProc())
#define repeat() while(msgProcDelta())
#define pause() present();while(noPresent()){Sleep(1);}