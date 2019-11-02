// DllMain.cpp �cDllMain�֐�
#include "stdafx.h"
#include "export.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

	// �C���X�^���X�n���h��
	HINSTANCE g_hInstance = NULL;

END_NAMESPACE()


// DllMain�֐�
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

// �C���X�^���X�n���h�����擾
HINSTANCE GetInstanceHandle(void)
{
	return g_hInstance;
}
