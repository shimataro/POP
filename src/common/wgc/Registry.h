// Registry.h …レジストリ操作クラス
#ifndef __WGC_REGISTRY__
#define __WGC_REGISTRY__

#include "wgc.h"
#include <windows.h>


_WGC_BEGIN                              // namespace wgc {

class registry
{
public:
	registry(HKEY hKeyRoot = HKEY_CURRENT_USER);
	registry(HKEY hKeyRoot, LPCTSTR lpszSubName, const BOOL bCreate = FALSE, const REGSAM samDesired = KEY_ALL_ACCESS);
	~registry(void);

	operator HKEY(void)
	{return m_hKeyRoot;}
	LONG GetErrorCode(void)
	{return m_error;}

	HKEY CreateSubKey(LPCTSTR lpszSubName, const DWORD dwOptions = REG_OPTION_NON_VOLATILE, const REGSAM samDesired = KEY_ALL_ACCESS);
	HKEY OpenSubKey(LPCTSTR lpszSubName, const REGSAM samDesired = KEY_ALL_ACCESS);
	LONG DeleteSubKey(LPCTSTR lpszSubName);
	LONG EnumerateSubKey(DWORD dwIndex, LPTSTR lpName, DWORD cbName);

	// 書き込み
	LONG SetValue(LPCTSTR lpszValueName, DWORD dwReserved, DWORD dwType, CONST BYTE *lpData, DWORD cbData);
	LONG SetValue(LPCTSTR lpszValueName, CONST BYTE *lpData, DWORD cbData);
	LONG SetValue(LPCTSTR lpszValueName, DWORD dwData);
	LONG SetValue(LPCTSTR lpszValueName, LPCTSTR lpszData);

	// 読み込み
	LONG GetValue(LPCTSTR lpszValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	LONG GetValue(LPCTSTR lpszValueName, LPBYTE lpData, DWORD cbData);
	LONG GetValue(LPCTSTR lpszValueName, LPDWORD lpdwData);
	LONG GetValue(LPCTSTR lpszValueName, LPCTSTR lpszData, DWORD cbData);

private:
	HKEY m_hKeyRoot;
	LONG m_error;
};

_WGC_END                                // }

#endif // __WGC_REGISTRY__
