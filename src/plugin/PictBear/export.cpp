// export.cpp �c�C���v�������e�[�V�����t�@�C��
#include "stdafx.h"
#include "export.h"
#include "filter/PictBear.h"

#include <shlobj.h>                     // ::SHBrowseForFolder(), ::SHGetDataFromIDList()
#include <objbase.h>                    // ::CoTaskMemFree()


#define TCHAR_REG_SUBKEY            _T("PictBear")
#define TCHAR_REG_NAME_DIRECTORY    _T("Plugin Folder")

// �o�[�W�����ԍ�
#define VERSION_MAJOR       1           // ���W���[�ԍ��i1�o�C�g�j
#define VERSION_MINOR       0           // �}�C�i�[�ԍ��i1�o�C�g�j
#define VERSION_BUILD       0           // �r���h�ԍ��i2�o�C�g�j


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	HKEY g_hKeyRoot = NULL;
	TCHAR g_directory[_MAX_PATH];           // PictBear�p�v���O�C���̓����Ă���f�B���N�g��

	int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
END_NAMESPACE()


// �v���O�C���̏��
EXTERN_C
DWORD WINAPI pop_get_product(LPSTR author, UINT author_max, LPSTR description, UINT description_max)
{
	// �X�g�����O���\�[�X���當�����ǂݍ���
	if(TRUE)
	{
		HINSTANCE hInstance = GetInstanceHandle();
		::LoadStringA(hInstance, IDS_PRODUCT_AUTHOR     , author     , author_max);
		::LoadStringA(hInstance, IDS_PRODUCT_DESCRIPTION, description, description_max);
	}

	// �o�[�W��������Ԃ�
	return MAKELONG(VERSION_BUILD, MAKEWORD(VERSION_MINOR, VERSION_MAJOR));
}

// ���W�X�g���L�[�̐ݒ�
EXTERN_C
void WINAPI pop_set_registry(HKEY hKey)
{
	DWORD dwDisposition;
	::RegCreateKeyEx(hKey, TCHAR_REG_SUBKEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &g_hKeyRoot, &dwDisposition);

	// PictBear�v���O�C���̃f�B���N�g�����擾
	DWORD dbData = SIZE_OF_ARRAY(g_directory);
	if(::RegQueryValueEx(g_hKeyRoot, TCHAR_REG_NAME_DIRECTORY, NULL, NULL, reinterpret_cast<LPBYTE>(g_directory), &dbData) != ERROR_SUCCESS)
	{
		// ���W�X�g���ɒl���Ȃ�������J�����g�f�B���N�g����I��
		::GetCurrentDirectory(SIZE_OF_ARRAY(g_directory), g_directory);
	}
	_tcsncpy(g_directory, g_directory, _MAX_PATH);
	g_directory[_MAX_PATH - 1] = _T('\0');

	// �v���O�C�����̊֐����擾
	load_PictBear_plugin(g_directory);
}

// �v���O�C���̃Z�b�g�A�b�v
EXTERN_C
BOOL WINAPI pop_setup(HWND hWnd)
{
	// �����f�B���N�g�����擾
	TCHAR directory[_MAX_PATH];
	_tcsncpy(directory, g_directory, _MAX_PATH);
	directory[_MAX_PATH - 1] = _T('\0');

	// �^�C�g�����擾
	TCHAR title[256];
	::LoadString(GetInstanceHandle(), IDS_FOLDERTITLE, title, SIZE_OF_ARRAY(title));

	TCHAR buffer[_MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner      = hWnd;
	bi.pidlRoot       = NULL;
	bi.pszDisplayName = buffer;
	bi.lpszTitle      = title;
	bi.ulFlags        = BIF_EDITBOX;
	bi.lpfn           = BrowseCallbackProc;
	bi.lParam         = reinterpret_cast<LPARAM>(directory);
	bi.iImage         = 0;

	// �t�H���_�����擾
	LPITEMIDLIST idlist = ::SHBrowseForFolder(&bi);
	if(idlist == NULL)
	{
		return FALSE;
	}

	// ��n��
	::SHGetPathFromIDList(idlist, g_directory);
	::CoTaskMemFree(idlist);
	return TRUE;
}


void attach_process(void)
{
}

void detach_process(void)
{
	free_PictBear_plugin();

	::RegSetValueEx(g_hKeyRoot, TCHAR_REG_NAME_DIRECTORY, 0, REG_SZ, reinterpret_cast<CONST BYTE *>(g_directory), _tcsclen(g_directory) + 1);
	::RegCloseKey(g_hKeyRoot);
	g_hKeyRoot = NULL;
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// �R�[���o�b�N�֐�
int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM /* lParam */, LPARAM lpData)
{
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	}
	return 0;
}

END_NAMESPACE()
