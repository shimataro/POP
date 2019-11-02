// winutil.h …Windowsの拡張関数
#ifndef __WINUTIL__
#define __WINUTIL__


#define VERSION_SUB_COMMENTS            _T("Comments")
#define VERSION_SUB_COMPANYNAME         _T("CompanyName")
#define VERSION_SUB_FILEDESCRIPTION     _T("FileDescription")
#define VERSION_SUB_FILEVERSION         _T("FileVersion")
#define VERSION_SUB_INTERNALNAME        _T("InternalName")
#define VERSION_SUB_LEGALCOPYRIGHT      _T("LegalCopyright")
#define VERSION_SUB_LEGALTRADEMARKS     _T("LegalTrademarks")
#define VERSION_SUB_ORIGINALFILENAME    _T("OriginalFilename")
#define VERSION_SUB_PRIVATEBUILD        _T("PrivateBuild")
#define VERSION_SUB_PRODUCTNAME         _T("ProductName")
#define VERSION_SUB_PRODUCTVERSION      _T("ProductVersion")
#define VERSION_SUB_SPECIALBUILD        _T("SpecialBuild")

// バージョン
BOOL GvxGetFileVersion(LPCTSTR lpszPathName, VS_FIXEDFILEINFO *lpvffi, LPTSTR lpszLanguage, const int nLanguageSize, ...);

// メニュー
HMENU GvxFindMenu(HMENU hMenu, UINT nFindID, int *pIndex);

// 文字列
void GvxTCHAR2CHAR(LPCTSTR lpszTcharString, LPSTR lpszAnsiString, int nBufferMax);
void GvxCHAR2TCHAR(LPCSTR lpszAnsiString, LPTSTR lpszTcharString, int nBufferMax);

void GvxPathAppendToModuleDir(HMODULE hModule, LPTSTR lpPathName, DWORD dwSize, ...);

#endif // __WINUTIL__
