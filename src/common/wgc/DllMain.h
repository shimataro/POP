// DllMain.h
#ifndef __DLLMAIN__
#define __DLLMAIN__

#include "wgc.h"


// �֐��̃v���g�^�C�v�錾
HINSTANCE GetInstanceHandle(void);
int MessageBox(HWND hWnd, UINT uTextID, UINT uCaptionID, UINT uType);

#endif // __DLLMAIN__
