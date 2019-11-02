// winutil.cpp
#include "stdafx.h"
#include "winutil.h"

#include <stdio.h>
#include <stdarg.h>
#include <vector>

#include <shlwapi.h>                    // ::PathRemoveFileSpec(), ::PathAppend()

#pragma comment(lib, "version.lib")
#pragma comment(lib, "shlwapi.lib")

#define VERSION_ROOT        _T("\\")
#define VERSION_TRANSLATE   _T("\\VarFileInfo\\Translation")
#define VERSION_FORMAT      _T("\\StringFileInfo\\%04x%04x\\%s")


// 実行モジュールのバージョンを取得
BOOL GvxGetFileVersion(LPCTSTR lpszPathName, VS_FIXEDFILEINFO *lpvffi, LPTSTR lpszLanguage, const int nLanguageSize, ...)
{
	LPTSTR lptstrPathName = const_cast<LPTSTR>(lpszPathName);

	// バージョン情報のサイズを取得
	DWORD dwHandle = 0;
	const DWORD dwVersionSize = ::GetFileVersionInfoSize(lptstrPathName, &dwHandle);
	if(dwVersionSize == 0)
	{
		return FALSE;
	}

	// バージョン情報を取得
	std::vector<BYTE> buffer(dwVersionSize);
	::GetFileVersionInfo(lptstrPathName, dwHandle, dwVersionSize, &buffer[0]);

	// 構造体にバージョン情報を格納
	{
		LPVOID lpBuffer = NULL;
		UINT   uLen = 0;
		::VerQueryValue(&buffer[0], VERSION_ROOT, &lpBuffer, &uLen);
		::CopyMemory(lpvffi, lpBuffer, sizeof(VS_FIXEDFILEINFO));
	}

	// 言語名を格納
	if(lpszLanguage != NULL)
	{
		LPDWORD lpdwTranslate = NULL;
		UINT    uLen = 0;
		::VerQueryValue(&buffer[0], VERSION_TRANSLATE, reinterpret_cast<LPVOID *>(&lpdwTranslate), &uLen);
		::VerLanguageName(LOWORD(*lpdwTranslate), lpszLanguage, nLanguageSize);

		// 可変個引数の処理
		va_list va = NULL;
		va_start(va, nLanguageSize);

		// StringNameを取得
		LPCTSTR lpszStringName = NULL;
		while((lpszStringName = va_arg(va, LPCTSTR)) != NULL)
		{
			// サブブロックを作成
			TCHAR szSubBlock[128];
			wsprintf(szSubBlock, VERSION_FORMAT, LOWORD(*lpdwTranslate), HIWORD(*lpdwTranslate), lpszStringName);

			// 値を取得
			LPTSTR lptstrValue = NULL;
			::VerQueryValue(&buffer[0], szSubBlock, reinterpret_cast<LPVOID *>(&lptstrValue), &uLen);

			// 値をバッファに格納
			LPTSTR     lpszValue  = va_arg(va, LPTSTR);
			const UINT nValueSize = va_arg(va, UINT);

			_tcsncpy(lpszValue, lptstrValue, nValueSize);
			lpszValue[nValueSize - 1] = _T('\0');
		}
		va_end(va);
	}

	return TRUE;
}


// 指定されたメニューIDを子に持つメニューハンドルと場所を取得
HMENU GvxFindMenu(HMENU hMenu, UINT nFindID, int *pIndex)
{
	int tmp;
	if(pIndex == NULL)
	{
		pIndex = &tmp;
	}

	// メニューを検索
	const int item_count = GetMenuItemCount(hMenu);
	for(int i = 0; i < item_count; i++)
	{
		UINT nSubID = GetMenuItemID(hMenu, i);

		// ポップアップメニューの場合
		if(nSubID == -1)
		{ 
			// 再帰検索
			HMENU hFindResult = GvxFindMenu(GetSubMenu(hMenu, i), nFindID, pIndex);
			if(hFindResult != NULL)
			{
				return hFindResult;
			}
		}
		// メニューアイテムがある場合
		else
		{
			// 探していたIDが見つかったら終了
			if(nSubID == nFindID)
			{
				*pIndex = i;
				return hMenu;
			}
		}
	}

	// 見つからなかったらNULLを返す
	return NULL;
}


// TCHAR文字列→ANSI文字列
void GvxTCHAR2CHAR(LPCTSTR lpszTcharString, LPSTR lpszAnsiString, int nBufferMax)
{
#ifdef _UNICODE
	::WideCharToMultiByte(
		CP_ACP,
		0,
		lpszTcharString, -1,
		lpszAnsiString, nBufferMax,
		NULL, NULL);
#else
	strncpy(lpszAnsiString, lpszTcharString, nBufferMax - 1);
	lpszAnsiString[nBufferMax - 1] = '\0';
#endif
}

// ANSI文字列→TCHAR文字列
void GvxCHAR2TCHAR(LPCSTR lpszAnsiString, LPTSTR lpszTcharString, int nBufferMax)
{
#ifdef _UNICODE
	::MultiByteToWideChar(
		CP_ACP,
		0,
		lpszAnsiString, -1,
		lpszTcharString, nBufferMax);
#else
	strncpy(lpszTcharString, lpszAnsiString, nBufferMax - 1);
	lpszTcharString[nBufferMax - 1] = '\0';
#endif
}


// モジュールのあるディレクトリ名にパスを追加
void GvxPathAppendToModuleDir(HMODULE hModule, LPTSTR lpPathName, DWORD dwSize, ...)
{
	// モジュールのパス名からファイル名を削除
	::GetModuleFileName(hModule, lpPathName, dwSize);
	::PathRemoveFileSpec(lpPathName);

	LPCTSTR lpszAppendName = NULL;

	va_list va = NULL;
	va_start(va, dwSize);
	while((lpszAppendName = va_arg(va, LPCTSTR)) != NULL)
	{
		::PathAppend(lpPathName, lpszAppendName);
	}
	va_end(va);
}
