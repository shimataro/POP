// Registry.cpp …インプリメンテーションファイル
#include "Registry.h"
#include <tchar.h>


_WGC_BEGIN                              // namespace wgc {

// コンストラクタ（hKeyRootを管理する）
registry::registry(HKEY hKeyRoot /* = HKEY_CURRENT_USER */)
{
	m_hKeyRoot = hKeyRoot;
	m_error    = ERROR_SUCCESS;
}

// コンストラクタ2（hKeyRootのサブキーを管理する）
registry::registry(HKEY hKeyRoot, LPCTSTR lpszSubName, BOOL bCreate /* = FALSE */, REGSAM samDesired /* = KEY_ALL_ACCESS */)
{
	m_hKeyRoot = hKeyRoot;
	if(bCreate)
	{
		m_hKeyRoot = CreateSubKey(lpszSubName, REG_OPTION_NON_VOLATILE);
	}
	else
	{
		m_hKeyRoot = OpenSubKey(lpszSubName, samDesired);
	}
}

// デストラクタ
registry::~registry(void)
{
/*
	if(m_hKeyRoot != NULL)
	{
		::RegCloseKey(m_hKeyRoot);
	}
*/
}

// サブキーを作成
HKEY registry::CreateSubKey(LPCTSTR lpszSubName, const DWORD dwOptions /* = REG_OPTION_NON_VOLATILE */, const REGSAM samDesired /* = KEY_ALL_ACCESS */)
{
	HKEY hKeyResult;
	DWORD dwDisposition;

	m_error = ::RegCreateKeyEx(
		m_hKeyRoot,
		lpszSubName,
		NULL, NULL,
		dwOptions,
		samDesired,
		NULL,
		&hKeyResult,
		&dwDisposition);
	if(m_error != ERROR_SUCCESS)
	{
		return NULL;
	}
	return hKeyResult;
}

// サブキーをオープン
HKEY registry::OpenSubKey(LPCTSTR lpszSubName, const REGSAM samDesired /* = KEY_ALL_ACCESS */)
{
	HKEY hKey;

	m_error = ::RegOpenKeyEx(m_hKeyRoot, lpszSubName, 0, samDesired, &hKey);
	if(m_error != ERROR_SUCCESS)
	{
		return NULL;
	}
	return hKey;
}

// サブキーを削除
LONG registry::DeleteSubKey(LPCTSTR lpszSubName)
{
	// サブキーを開く
	WUCRegistry regSubKey = OpenSubKey(lpszSubName, KEY_ALL_ACCESS);
	if(regSubKey == NULL)
	{
		// 開けなかったらエラーコードを返す
		return m_error;
	}

	// サブキーのサブキーを列挙
	TCHAR szName[128];
	while(regSubKey.EnumerateSubKey(0, szName, sizeof(szName) / sizeof(szName[0])) != ERROR_NO_MORE_ITEMS)
		{
		// 見つかったキーを削除
		regSubKey.DeleteSubKey(szName);
	}

	// 最後にサブキーを削除
	m_error = ::RegDeleteKey(m_hKeyRoot, lpszSubName);
	return m_error;
}

// サブキーを列挙
LONG registry::EnumerateSubKey(DWORD dwIndex, LPTSTR lpName, DWORD cbName)
{
	m_error = ::RegEnumKey(m_hKeyRoot, dwIndex, lpName, cbName);
	return m_error;
}


////////////////////////////////////////
// 書き込み

// 任意のデータを書き込む
LONG registry::SetValue(LPCTSTR lpszValueName, DWORD dwReserved, DWORD dwType, CONST BYTE *lpData, DWORD cbData)
{
	m_error = ::RegSetValueEx(m_hKeyRoot, lpszValueName, dwReserved, dwType, lpData, cbData);
	return m_error;
}

// バイナリ値を書き込む
LONG registry::SetValue(LPCTSTR lpszValueName, CONST BYTE *lpData, DWORD cbData)
{
	return SetValue(lpszValueName, 0, REG_BINARY, lpData, cbData);
}

// 32bit値を書き込む
LONG registry::SetValue(LPCTSTR lpszValueName, DWORD dwData)
{
	return SetValue(lpszValueName, 0, REG_DWORD, (CONST BYTE *)&dwData, sizeof(dwData));
}

// 文字列を書き込む
LONG registry::SetValue(LPCTSTR lpszValueName, LPCTSTR lpszData)
{
	return SetValue(lpszValueName, 0, REG_SZ, (CONST BYTE *)lpszData, _tcsclen(lpszData)+1);
}


////////////////////////////////////////
// 読み込み

// 任意のデータを読み込む
LONG registry::GetValue(LPCTSTR lpszValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	m_error = ::RegQueryValueEx(m_hKeyRoot, lpszValueName, lpReserved, lpType, lpData, lpcbData);
	return m_error;
}

// バイナリ値を読み込む
LONG registry::GetValue(LPCTSTR lpszValueName, LPBYTE lpData, DWORD cbData)
{
	return GetValue(lpszValueName, NULL, NULL, lpData, &cbData);
}

// 32bit値を読み込む
LONG registry::GetValue(LPCTSTR lpszValueName, LPDWORD lpdwData)
{
	DWORD cbData;
	return GetValue(lpszValueName, NULL, NULL, (LPBYTE)lpdwData, &cbData);
}

// 文字列を読み込む
LONG registry::GetValue(LPCTSTR lpszValueName, LPCTSTR lpszData, DWORD cbData)
{
	return GetValue(lpszValueName, NULL, NULL, (LPBYTE)lpszData, &cbData);
}

_WGC_END                                // }
