// devicecontext.cpp …インプリメンテーションファイル
#include "devicecontext.h"


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// コンストラクション

devicecontext::devicecontext(HDC hDC)
{
	m_hDC = hDC;
	m_bOwner = FALSE;
}

devicecontext::devicecontext(
	LPCTSTR lpszDriver,
	LPCTSTR lpszDevice,
	CONST DEVMODE *lpInitData)
{
	m_hDC = ::CreateDC(
		lpszDriver,
		lpszDevice,
		NULL,
		lpInitData);

	m_bOwner = TRUE;
}

devicecontext::~devicecontext(void)
{
	if(m_hDC == NULL)
	{
		return;
	}

	if(m_bOwner)
	{
		Delete();
	}
}

BOOL devicecontext::Delete(void)
{
	HDC hTmp = m_hDC;
	m_hDC = NULL;

	return ::DeleteDC(hTmp);
}


////////////////////////////////////////////////////////////////////////////////
// オペレーション

HGDIOBJ devicecontext::SelectObject(HGDIOBJ hNewObject)
{
	return ::SelectObject(m_hDC, hNewObject);
}


////////////////////////////////////////////////////////////

COLORREF devicecontext::GetTextColor(void) const
{
	return ::GetTextColor(m_hDC);
}

COLORREF devicecontext::GetBkColor(void) const
{
	return ::GetBkColor(m_hDC);
}

COLORREF devicecontext::SetTextColor(const COLORREF color)
{
	return ::SetTextColor(m_hDC, color);
}

COLORREF devicecontext::SetBkColor(const COLORREF color)
{
	return ::SetBkColor(m_hDC, color);
}

int devicecontext::GetBkMode(void)
{
	return ::GetBkMode(m_hDC);
}

int devicecontext::SetBkMode(const int iBkMode)
{
	return ::SetBkMode(m_hDC, iBkMode);
}


////////////////////////////////////////////////////////////
// クリッピング領域

// クリッピング領域をクリア
int devicecontext::ClearClip(void)
{
	return ::SelectClipRgn(m_hDC, NULL);
}

// 現在のクリッピング領域と rRect から新しいクリッピング領域を作成
int devicecontext::IntersectClipRect(const RECT &rRect)
{
	return ::IntersectClipRect(m_hDC, rRect.left, rRect.top, rRect.right, rRect.bottom);
}

// 新しいクリッピング領域を作成
int devicecontext::SetClipRect(const RECT &rRect)
{
	ClearClip();
	return IntersectClipRect(rRect);
}


////////////////////////////////////////////////////////////
// 塗りつぶし

BOOL devicecontext::FillRect(const RECT &rect, HBRUSH hBrush)
{
	return ::FillRect(m_hDC, &rect, hBrush);
}

BOOL devicecontext::FillSolidRect(const RECT &rect, const COLORREF color)
{
	SetBkColor(color);
	return ExtTextOut(0, 0, ETO_OPAQUE, rect, tstring_t());
}


////////////////////////////////////////////////////////////
// 描画

BOOL devicecontext::DrawFocusRect(const RECT &rect)
{
	return ::DrawFocusRect(m_hDC, &rect);
}

BOOL devicecontext::DrawEdge(
	const RECT &rect,
	const UINT edge     /* = EDGE_ETCHED */,
	const UINT grfFlags /* = BF_RECT */)
{
	RECT tmp = rect;
	return ::DrawEdge(m_hDC, &tmp, edge, grfFlags);
}


////////////////////////////////////////////////////////////

BOOL devicecontext::MoveToEx(
	const int x,
	const int y,
	LPPOINT lpPoint)
{
	return ::MoveToEx(m_hDC, x, y, lpPoint);
}

BOOL devicecontext::MoveTo(
	const int x,
	const int y)
{
	return MoveToEx(x, y, NULL);
}

BOOL devicecontext::LineTo(
	const int x,
	const int y)
{
	return ::LineTo(m_hDC, x, y);
}


////////////////////////////////////////////////////////////
// テキスト描画

BOOL devicecontext::TextOut(
	const int x,
	const int y,
	const tstring_t &tstr,
	const int length /* = -1 */)
{
	return ::TextOut(
		m_hDC,
		x, y,
		tstr.c_str(),
		(length == -1) ? tstr.length() : length);
}

BOOL devicecontext::TextOutW(
	const int x,
	const int y,
	const wstring_t &wstr,
	const int length /* = -1 */)
{
	return ::TextOutW(
		m_hDC,
		x, y,
		wstr.c_str(),
		(length == -1) ? wstr.length() : length);
}

BOOL devicecontext::ExtTextOut(
	const int x,
	const int y,
	const UINT fuOptions,
	const RECT &rect,
	const tstring_t &tstr,
	const UINT length /* = -1 */,
	const int *lpDx   /* = NULL */)
{
	return ::ExtTextOut(
		m_hDC,
		x, y,
		fuOptions,
		&rect,
		tstr.c_str(),
		(length == -1) ? tstr.length() : length,
		lpDx);
}

BOOL devicecontext::ExtTextOutW(
	const int x,
	const int y,
	const UINT fuOptions,
	const RECT &rect,
	const wstring_t &wstr,
	const UINT length /* = -1 */,
	const int *lpDx   /* = NULL */)
{
	return ::ExtTextOutW(
		m_hDC,
		x, y,
		fuOptions,
		&rect,
		wstr.c_str(),
		(length == -1) ? wstr.length() : length,
		lpDx);
}

BOOL devicecontext::GetTextExtent(const tstring_t &tstr, SIZE &size) const
{
	return ::GetTextExtentPoint32(
		m_hDC,
		tstr.c_str(),
		tstr.length(),
		&size);
}


////////////////////////////////////////////////////////////////////////////////
// 派生DC

// クライアントDC
devicecontext_client::devicecontext_client(HWND hWnd)
	: devicecontext(::GetDC(hWnd))
{
	m_hWnd = hWnd;
}

devicecontext_client::~devicecontext_client(void)
{
	::ReleaseDC(m_hWnd, m_hDC);
	m_hDC = NULL;
}


// ウインドウDC
devicecontext_window::devicecontext_window(HWND hWnd)
	: devicecontext(::GetWindowDC(hWnd))
{
	m_hWnd = hWnd;
}

devicecontext_window::~devicecontext_window(void)
{
	::ReleaseDC(m_hWnd, m_hDC);
	m_hDC = NULL;
}


// ペイントDC
devicecontext_paint::devicecontext_paint(HWND hWnd)
	: devicecontext(::BeginPaint(hWnd, &m_ps))
{
	m_hWnd = hWnd;
}

devicecontext_paint::~devicecontext_paint(void)
{
	::EndPaint(m_hWnd, &m_ps);
	m_hDC = NULL;
}


// メモリDC
devicecontext_memory::devicecontext_memory(HDC hDC)
	: devicecontext(::CreateCompatibleDC(hDC))
{
}

devicecontext_memory::~devicecontext_memory(void)
{
	Delete();
}

_WGC_END                                // }
