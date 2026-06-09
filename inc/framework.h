#pragma once
void window(float width, float height, bool fullscreenMode=false);
void window(const char* caption, float width, float height, bool fullscreenMode=false);
bool msgProc();
bool msgProcDelta();
bool quit();
void disableEscapeKey();
bool msgProcPause();
constexpr auto full = true;
//for beginner
#define program() void gmain()
#define notQuit msgProc()
#define loop() while(msgProc())
#define repeat() while(msgProcDelta())
#define pause() present();while(msgProcPause()){Sleep(1);}