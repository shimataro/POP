// DllMain.cpp …DllMain関数
#include "stdafx.h"
#include "export.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

	// インスタンスハンドル
	HINSTANCE g_hInstance = NULL;

END_NAMESPACE()


// DllMain関数
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /* lpvReserved */)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstance = hInstance;

		attach_process();
		break;

	case DLL_PROCESS_DETACH:
		detach_process();
		break;
	}

	return TRUE;
}

// インスタンスハンドルを取得
HINSTANCE GetInstanceHandle(void)
{
	return g_hInstance;
}
