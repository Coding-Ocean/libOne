#pragma once
#include<stdio.h>
#include<windows.h>

//#define DBG

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

void halt( const char* filename, int line, const char* message );
#define STRONG_ASSERT( exp ) ( ( !! ( exp ) ) || ( halt( __FILE__, __LINE__, #exp ), 0 ) ) //Ç±ÇÃçsÇÃà”ñ°ÇóùâÇ∑ÇÈÇÃÇÕëäìñìÔÇµÇ¢
#ifdef _DEBUG
    #define ASSERT( exp ) ( ( !! ( exp ) ) || ( halt( __FILE__, __LINE__, #exp ), 0 ) )
#else
    #define ASSERT( exp )
#endif
