// DllMain.h
#ifndef __DLLMAIN__
#define __DLLMAIN__

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <assert.h>

#include "resource.h"


// �x���̖�����
#pragma warning(disable:4786)           // �u���ʎq�����������ɐ؂�̂Ă��܂����v


// �֐��̃v���g�^�C�v�錾
HINSTANCE GetInstanceHandle(void);
int MessageBox(HWND hWnd, UINT uTextID, UINT uCaptionID, UINT uType);

#endif // __DLLMAIN__
