// wgfunc.cpp …インプリメンテーションファイル
#include "wgfunc.h"


_WGC_BEGIN                              // namespace wgc {


////////////////////////////////////////////////////////////////////////////////
// 文字列

BOOL wgfExtractSubString(tstring_t &tstr, const tstring_t &tstr_full, const int substring_no, const TCHAR separator)
{
	tstring_t::size_type last_find_pos = 0;

	for(int i = 0; i < substring_no; i++)
	{
		tstring_t::size_type find_pos = tstr_full.find(separator, last_find_pos);
		if(find_pos == tstring_t::npos)
		{
			return FALSE;
		}
		last_find_pos = find_pos + 1;
	}

	tstring_t::size_type find_pos = tstr_full.find(separator, last_find_pos);
	tstr = tstr_full.substr(last_find_pos, find_pos - last_find_pos);
	return TRUE;
}

// ストリングリソースから文字列のロード
int wgfLoadString(HINSTANCE hResource, const UINT uID, tstring_t &tstr)
{
	TCHAR buffer[128];
	const int result = ::LoadString(hResource, uID, buffer, SIZE_OF_ARRAY(buffer));

	tstr = buffer;
	return result;
}

// uID（通常は ::GetLastError() の戻り値の示すエラーメッセージを取得
int wgfGetErrorMessage(const UINT uID, tstring_t &tstr)
{
	LPTSTR lpText = NULL;
	const int result = ::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		uID, 0,
		reinterpret_cast<LPTSTR>(&lpText), 0,
		NULL);

	tstr = lpText;
	::LocalFree(lpText);

	return result;
}

// マルチバイト文字列をワイド文字列に変換
int wgfMultiByteToWideChar(
	const string_t &multibyte_str,
	wstring_t &widechar_str,
	const UINT uCodePage /* = CP_ACP */,
	const DWORD dwFlags  /* = 0 */)
{
	// 文字列の長さを取得
	const int length = ::MultiByteToWideChar(
		uCodePage, dwFlags,
		multibyte_str.c_str(), -1,
		NULL, 0);

	if(length <= 1)
	{
		return length;
	}

	// ワイド文字列に変換
	wchar_t *wstr = new wchar_t[length + 1];
	const int result = ::MultiByteToWideChar(
		uCodePage, dwFlags,
		multibyte_str.c_str(), -1,
		wstr, length + 1);
	widechar_str = wstr;
	delete []wstr;

	return result;
}

// ワイド文字列をマルチバイト文字列に変換
int wgfWideCharToMultiByte(
	const wstring_t &widechar_str,
	string_t &multibyte_str,
	const UINT uCodePage /* = CP_ACP */,
	const DWORD dwFlags  /* = 0 */)
{
	// 文字列の長さを取得
	const int length = ::WideCharToMultiByte(
		uCodePage, dwFlags,
		widechar_str.c_str(), -1,
		NULL, 0,
		NULL, NULL);

	if(length <= 1)
	{
		return length;
	}

	// ワイド文字列に変換
	char *str = new char[length + 1];
	const int result = ::WideCharToMultiByte(
		uCodePage, dwFlags,
		widechar_str.c_str(), -1,
		str, length + 1,
		NULL, NULL);
	multibyte_str = str;
	delete []str;

	return result;
}

_WGC_END                                // }
