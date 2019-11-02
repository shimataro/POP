// ScrollBarCtrl.cpp …インプリメンテーションファイル
#include "ScrollBarCtrl.h"


_WGC_BEGIN                              // namespace wgc {

// コンストラクタ
scrollbar_ctrl::scrollbar_ctrl(void)
{
}

// デストラクタ
scrollbar_ctrl::~scrollbar_ctrl(void)
{
}


// スクロール情報
BOOL scrollbar_ctrl::SetScrollInfo(const SCROLLINFO &si, BOOL bRedraw /* = TRUE */)
{
	return SendMessage(SBM_SETSCROLLINFO, bRedraw, (LPARAM)&si);
//	return ::SetScrollInfo(m_hWnd, SB_CTL, &si, bRedraw);
}

BOOL scrollbar_ctrl::GetScrollInfo(SCROLLINFO &si, UINT nMask /* = SIF_ALL */) const
{
	si.cbSize = sizeof(si);
	si.fMask  = nMask;
	return SendMessage(SBM_GETSCROLLINFO, 0, (LPARAM)&si);
//	return ::GetScrollInfo(m_hWnd, SB_CTL, &si);
}


// ページサイズ
int scrollbar_ctrl::GetPageSize(void) const
{
	SCROLLINFO si;
	GetScrollInfo(si, SIF_PAGE);
	return si.nPage;
}

int scrollbar_ctrl::SetPageSize(int nSize)
{
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask  = SIF_PAGE;
	si.nPage  = nSize;

	SetScrollInfo(si);
	return si.nPage;
}


// 範囲
void scrollbar_ctrl::GetRange(int &nMin, int &nMax) const
{
	SendMessage(SBM_GETRANGE, (WPARAM)&nMin, (LPARAM)&nMax);
}

void scrollbar_ctrl::SetRange(int nMin, int nMax, BOOL bRedraw /* = FALSE */)
{
	if(bRedraw)
	{
		SendMessage(SBM_SETRANGEREDRAW, nMin, nMax);
	}
	else
	{
		SendMessage(SBM_SETRANGE, nMin, nMax);
	}
}


// スクロール位置
int scrollbar_ctrl::GetPos(void) const
{
	return SendMessage(SBM_GETPOS);
}

void scrollbar_ctrl::SetPos(int nPos, BOOL bRedraw /* = TRUE */)
{
	SendMessage(SBM_SETPOS, nPos, bRedraw);
}


// 
BOOL scrollbar_ctrl::EnableArrow(UINT nArrowFlags /* = ESB_ENABLE_BOTH */)
{
	return TRUE;
}


int scrollbar_ctrl::GetLimit(void) const
{
	return 1;
}

_WGC_END                                // }
