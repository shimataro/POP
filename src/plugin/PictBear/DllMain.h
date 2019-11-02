// DllMain.h
#ifndef __DLLMAIN__
#define __DLLMAIN__

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <assert.h>

#include "resource.h"


// 警告の無効化
#pragma warning(disable:4786)           // 「識別子が○○文字に切り捨てられました」


// 関数のプロトタイプ宣言
HINSTANCE GetInstanceHandle(void);
int MessageBox(HWND hWnd, UINT uTextID, UINT uCaptionID, UINT uType);

#endif // __DLLMAIN__
