// wgfunc.h ÅcWindows API
#ifndef __WGFUNC__
#define __WGFUNC__

#include "wgc.h"


_WGC_BEGIN                              // namespace wgc {

BOOL wgfExtractSubString(tstring_t &tstr, const tstring_t &tstr_full, const int substring_no, const TCHAR separator);
int  wgfLoadString(HINSTANCE hResource, const UINT uID, tstring_t &tstr);
int  wgfGetErrorMessage(const UINT uID, tstring_t &tstr);

int wgfMultiByteToWideChar(const string_t &multibyte_str, wstring_t &widechar_str, const UINT uCodePage = CP_ACP, const DWORD dwFlags = 0);
int wgfWideCharToMultiByte(const wstring_t &widechar_str, string_t &multibyte_str, const UINT uCodePage = CP_ACP, const DWORD dwFlags = 0);

_WGC_END                                // }

#endif // __WGFUNC__
