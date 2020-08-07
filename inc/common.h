#pragma once
#include<stdio.h>
#include<windows.h>

//#define DBG

//#define CLASS_NAME "GameClass"
//#define DATA_2DGRAPHICS_PATH "data/2DGraphics/"
//#define DATA_SOUNDS_PATH "data/sounds/"
//#define DATA_PATH "data/"

#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//#ifdef _DEBUG
  #define WARNING(flag,msg1,msg2)\
  { if(flag){\
      char buf[ 512 ];\
      sprintf_s( buf, "%s\n%s\n\n%s : %d", msg1, msg2, __FILE__, __LINE__ );\
      MessageBoxA( 0, buf, "WARNING!!", MB_OK | MB_ICONERROR );\
      exit(1);\
    }\
  }
//#else
//  #define WARNING(flag,msg1,msg2) {}
//#endif
