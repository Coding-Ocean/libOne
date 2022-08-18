#if( _DEBUG )

#include<fbxsdk.h>
void InitializeSdkObjects( FbxManager*& pManager, FbxScene*& pScene );
void DestroySdkObjects( FbxManager* pManager, bool pExitStatus );
bool LoadScene( FbxManager* pManager, FbxDocument* pScene, const char* pFilename );

#endif