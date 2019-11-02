// DllMain.cpp …DllMain関数
#include "DllMain.h"


// anonymous namespace
namespace
{
	// インスタンスハンドル
	HINSTANCE g_hInstance = NULL;
}


// DllMain関数
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD /* dwReason */, LPVOID /* lpvReserved */)
{
	g_hInstance = hInstance;

	return TRUE;
}

// インスタンスハンドルを取得
HINSTANCE GetInstanceHandle(void)
{
	return g_hInstance;
}

// メッセージボックスを表示
int MessageBox(HWND hWnd, UINT uTextID, UINT uCaptionID, UINT uType)
{
	TCHAR szText[128];
	::LoadString(GetInstanceHandle(), uTextID, szText, sizeof(szText) / sizeof(szText[0]));

	TCHAR szCaption[128];
	::LoadString(GetInstanceHandle(), uCaptionID, szCaption, sizeof(szCaption) / sizeof(szCaption[0]));

	return ::MessageBox(hWnd, szText, szCaption, uType);
}
