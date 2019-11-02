// export.cpp …インプリメンテーションファイル
#include "stdafx.h"
#include "export.h"
#include "filter/PictBear.h"

#include <shlobj.h>                     // ::SHBrowseForFolder(), ::SHGetDataFromIDList()
#include <objbase.h>                    // ::CoTaskMemFree()


#define TCHAR_REG_SUBKEY            _T("PictBear")
#define TCHAR_REG_NAME_DIRECTORY    _T("Plugin Folder")

// バージョン番号
#define VERSION_MAJOR       1           // メジャー番号（1バイト）
#define VERSION_MINOR       0           // マイナー番号（1バイト）
#define VERSION_BUILD       0           // ビルド番号（2バイト）


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	HKEY g_hKeyRoot = NULL;
	TCHAR g_directory[_MAX_PATH];           // PictBear用プラグインの入っているディレクトリ

	int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
END_NAMESPACE()


// プラグインの情報
EXTERN_C
DWORD WINAPI pop_get_product(LPSTR author, UINT author_max, LPSTR description, UINT description_max)
{
	// ストリングリソースから文字列を読み込む
	if(TRUE)
	{
		HINSTANCE hInstance = GetInstanceHandle();
		::LoadStringA(hInstance, IDS_PRODUCT_AUTHOR     , author     , author_max);
		::LoadStringA(hInstance, IDS_PRODUCT_DESCRIPTION, description, description_max);
	}

	// バージョン情報を返す
	return MAKELONG(VERSION_BUILD, MAKEWORD(VERSION_MINOR, VERSION_MAJOR));
}

// レジストリキーの設定
EXTERN_C
void WINAPI pop_set_registry(HKEY hKey)
{
	DWORD dwDisposition;
	::RegCreateKeyEx(hKey, TCHAR_REG_SUBKEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &g_hKeyRoot, &dwDisposition);

	// PictBearプラグインのディレクトリを取得
	DWORD dbData = SIZE_OF_ARRAY(g_directory);
	if(::RegQueryValueEx(g_hKeyRoot, TCHAR_REG_NAME_DIRECTORY, NULL, NULL, reinterpret_cast<LPBYTE>(g_directory), &dbData) != ERROR_SUCCESS)
	{
		// レジストリに値がなかったらカレントディレクトリを選択
		::GetCurrentDirectory(SIZE_OF_ARRAY(g_directory), g_directory);
	}
	_tcsncpy(g_directory, g_directory, _MAX_PATH);
	g_directory[_MAX_PATH - 1] = _T('\0');

	// プラグイン内の関数を取得
	load_PictBear_plugin(g_directory);
}

// プラグインのセットアップ
EXTERN_C
BOOL WINAPI pop_setup(HWND hWnd)
{
	// 初期ディレクトリを取得
	TCHAR directory[_MAX_PATH];
	_tcsncpy(directory, g_directory, _MAX_PATH);
	directory[_MAX_PATH - 1] = _T('\0');

	// タイトルを取得
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

	// フォルダ名を取得
	LPITEMIDLIST idlist = ::SHBrowseForFolder(&bi);
	if(idlist == NULL)
	{
		return FALSE;
	}

	// 後始末
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

// コールバック関数
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
