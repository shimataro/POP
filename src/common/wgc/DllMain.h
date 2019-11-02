// DllMain.h
#ifndef __DLLMAIN__
#define __DLLMAIN__

#include "wgc.h"


// 関数のプロトタイプ宣言
HINSTANCE GetInstanceHandle(void);
int MessageBox(HWND hWnd, UINT uTextID, UINT uCaptionID, UINT uType);

#endif // __DLLMAIN__
