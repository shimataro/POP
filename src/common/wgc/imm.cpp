// imm.cpp …インプリメンテーションファイル
#include "imm.h"


// ライブラリのリンク
#pragma comment(lib, "imm32.lib")       // IMM制御用ライブラリ


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// 構築

// コンストラクタ
imm::imm(HWND hWnd)
{
	m_hWnd = hWnd;
	m_hIMC = ::ImmGetContext(m_hWnd);
}

// デストラクタ
imm::~imm(void)
{
	::ImmReleaseContext(m_hWnd, m_hIMC);
	m_hIMC = NULL;
}


////////////////////////////////////////////////////////////////////////////////
// 状態

BOOL imm::GetOpenStatus(void) const
{
	return ::ImmGetOpenStatus(m_hIMC);
}

BOOL imm::SetOpenStatus(const BOOL bOpen /* = TRUE */)
{
	return ::ImmSetOpenStatus(m_hIMC, bOpen);
}


////////////////////////////////////////////////////////////////////////////////
// 操作

// IMEのカレット座標を設定
BOOL imm::SetPos(const POINT &point)
{
	COMPOSITIONFORM cf;
	cf.dwStyle = CFS_POINT;
	cf.ptCurrentPos = point;

	return ::ImmSetCompositionWindow(m_hIMC, &cf);
}


// フォント操作
BOOL imm::GetCompositionFont(LPLOGFONT lplf)
{
	return ::ImmGetCompositionFont(m_hIMC, lplf);
}

BOOL imm::SetCompositionFont(LPLOGFONT lplf)
{
	return ::ImmSetCompositionFont(m_hIMC, lplf);
}

BOOL imm::SetCompositionFont(HFONT hFont)
{
	LOGFONT lf;
	::GetObject(hFont, sizeof(LOGFONT), &lf);
	return SetCompositionFont(&lf);
}


// 変換
LONG imm::GetCompositionString(const DWORD dwIndex, LPVOID lpBuf, const DWORD dwBufLen)
{
	return ::ImmGetCompositionString(m_hIMC, dwIndex, lpBuf, dwBufLen);
}

LONG imm::GetCompositionStringResultStr(tstring_t &tstr)
{
	// バッファサイズを取得
	const int size = GetCompositionString(GCS_RESULTSTR, NULL, 0);
	if(size < 0)
	{
		return size;
	}

	// バッファメモリを確保
	const size_t length = size / sizeof(TCHAR);
	TCHAR *buf = new TCHAR[length + 1];

	// 変換文字列を取得
	const LONG lResult = ::ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, buf, size);
	buf[length] = _T('\0');
	tstr = buf;

	// 後処理
	delete []buf;
	return lResult;
}

// UNICODE文字列で変換後の値を取得
LONG imm::GetCompositionStringResultStrW(wstring_t &wstr)
{
	// バッファサイズを取得
	const int size = ::ImmGetCompositionStringW(m_hIMC, GCS_RESULTSTR, NULL, 0);
	if(size < 0)
	{
		return size;
	}

	// バッファメモリを確保
	const size_t length = size / sizeof(wchar_t);
	wchar_t *buf = new wchar_t[length + 1];

	// 変換文字列を取得
	const LONG lResult = ::ImmGetCompositionStringW(m_hIMC, GCS_RESULTSTR, buf, size);
	buf[length] = L'\0';
	wstr = buf;

	// 後処理
	delete []buf;
	return lResult;
}

_WGC_END                                // }
