// ReBarBar.cpp …インプリメンテーションファイル
#include "ReBar.h"


_WGC_BEGIN                              // namespace wgc {

// コンストラクタ
rebar::rebar(void)
{
	INITCOMMONCONTROLSEX ic = {sizeof(ic)};
	ic.dwICC = ICC_COOL_CLASSES;
	::InitCommonControlsEx(&ic);
}

// デストラクタ
rebar::~rebar(void)
{
}

BOOL rebar::Create(
	window *parent,
	HINSTANCE hInstance,
	const DWORD dwStyle      /* = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_BORDER */,
	const DWORD dwCtlStyle   /* = CCS_NODIVIDER | CCS_NOPARENTALIGN */,
	const DWORD dwReBarStyle /* = RBS_BANDBORDERS | RBS_VARHEIGHT */,
	const UINT nID           /* = ID_STD_REBAR */)
{
	control::Create(
		REBARCLASSNAME, NULL,
		dwStyle,
		dwCtlStyle | dwReBarStyle,
		WS_EX_TOOLWINDOW | WS_EX_STATICEDGE,
		0, 0, 0, 0,
		parent, hInstance,
		nID);

	REBARINFO rbi;
	rbi.cbSize = sizeof(rbi);
    rbi.fMask  = 0;
    rbi.himl   = (HIMAGELIST)NULL;
    return SetBarInfo(rbi);
}


UINT rebar::GetBandCount(void) const
{
	return SendMessage(RB_GETBANDCOUNT);
}

BOOL rebar::GetBarInfo(REBARINFO &rbi) const
{
	return SendMessage(RB_GETBARINFO, 0, reinterpret_cast<LPARAM>(&rbi));
}

BOOL rebar::GetBandInfo(const UINT uBand, REBARBANDINFO &rbbi) const
{
	return SendMessage(RB_GETBANDINFO, uBand, reinterpret_cast<LPARAM>(&rbbi));
}


BOOL rebar::AddBar(
	HWND hWnd,
	const SIZE &size,
	const DWORD dwStyle /* = RBBS_GRIPPERALWAYS | RBBS_CHILDEDGE */,
	LPCTSTR lpszText    /* = NULL */,
	HBITMAP hBitmap     /* = NULL */)
{
	REBARBANDINFO rbbi = {sizeof(rbbi)};
	rbbi.fMask      = RBBIM_CHILD | RBBIM_STYLE | RBBIM_CHILDSIZE;
	rbbi.hwndChild  = hWnd;
	rbbi.fStyle     = dwStyle;
	rbbi.cxMinChild = size.cx;
	rbbi.cyMinChild = size.cy;

	if(lpszText != NULL)
	{
		rbbi.fMask |= RBBIM_TEXT;
		rbbi.lpText = const_cast<LPTSTR>(lpszText);
	}
	if(hBitmap != NULL)
	{
		rbbi.fMask |= RBBIM_BACKGROUND;
		rbbi.hbmBack = hBitmap;
	}
	return InsertBand(-1, rbbi);
}

BOOL rebar::SetBarInfo(const REBARINFO &rbi)
{
	REBARINFO tmp = rbi;
	return SendMessage(RB_SETBARINFO, 0, reinterpret_cast<LPARAM>(&tmp));
}

BOOL rebar::SetBandInfo(const UINT uBand, const REBARBANDINFO &rbbi)
{
	REBARBANDINFO tmp = rbbi;
	return SendMessage(RB_SETBANDINFO, uBand, reinterpret_cast<LPARAM>(&tmp));
}

BOOL rebar::InsertBand(const UINT uBand, const REBARBANDINFO &rbbi)
{
	REBARBANDINFO tmp = rbbi;
	return SendMessage(RB_INSERTBAND, uBand, reinterpret_cast<LPARAM>(&tmp));
}

BOOL rebar::DeleteBand(const UINT uBand)
{
	return SendMessage(RB_DELETEBAND, uBand);
}

_WGC_END                                // }
