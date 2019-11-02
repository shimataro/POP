// DllMain.cpp �cDllMain�֐�
#include "DllMain.h"


// anonymous namespace
namespace
{
	// �C���X�^���X�n���h��
	HINSTANCE g_hInstance = NULL;
}


// DllMain�֐�
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD /* dwReason */, LPVOID /* lpvReserved */)
{
	g_hInstance = hInstance;

	return TRUE;
}

// �C���X�^���X�n���h�����擾
HINSTANCE GetInstanceHandle(void)
{
	return g_hInstance;
}

// ���b�Z�[�W�{�b�N�X��\��
int MessageBox(HWND hWnd, UINT uTextID, UINT uCaptionID, UINT uType)
{
	TCHAR szText[128];
	::LoadString(GetInstanceHandle(), uTextID, szText, sizeof(szText) / sizeof(szText[0]));

	TCHAR szCaption[128];
	::LoadString(GetInstanceHandle(), uCaptionID, szCaption, sizeof(szCaption) / sizeof(szCaption[0]));

	return ::MessageBox(hWnd, szText, szCaption, uType);
}
