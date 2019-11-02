// StatusBar.cpp …インプリメンテーションファイル
#include "StatusBar.h"
#include "devicecontext.h"              // wgc::devicecontext
#include "wgfunc.h"                     // wgc::wgfLoadString()
#include <assert.h>                     // assert()


_WGC_BEGIN                              // namespace wgc {

// コンストラクタ
statusbar::statusbar(void)
{
}

// デストラクタ
statusbar::~statusbar(void)
{
}


// ステータスバーの作成
BOOL statusbar::Create(
	window *parent,
	LPCTSTR lpszWindowName,
	const DWORD dwStyle          /* = WS_CHILD | WS_VISIBLE */,
	const DWORD dwCtlStyle       /* = CCS_BOTTOM */,
	const DWORD dwStatusBarStyle /* = SBARS_SIZEGRIP */,
	const UINT  nID              /* = IDW_STD_STATUSBAR */)
{
	assert(parent != NULL);

	_SetHookTarget(this);
	HWND hWnd = ::CreateStatusWindow(
		dwStyle | dwCtlStyle | dwStatusBarStyle,
		lpszWindowName,
		*parent,
		nID);

	if(hWnd == NULL)
	{
		_SetHookTarget(NULL);
		return FALSE;
	}
	assert(hWnd == m_hWnd);
	return TRUE;
}


// インジケータを設定
BOOL statusbar::SetIndicators(HINSTANCE hResource, const UINT nIDArray[], const int nCount)
{
	m_pane_width.resize(nCount);

	// 各文字の幅を取得
	for(int i = 0; i < nCount; i++)
	{
		tstring_t tstr;
		wgfLoadString(hResource, nIDArray[i], tstr);

		SIZE size;
		devicecontext_client dc(m_hWnd);
		dc.GetTextExtent(tstr, size);

		m_pane_width[i] = size.cx;
	}

	return UpdatePanePosition();
}


// 境界情報を取得（全要素を同時に）
BOOL statusbar::GetBorders(int *pBorders) const
{
	assert(pBorders != NULL);
	return SendMessage(
		SB_GETBORDERS,
		0,
		reinterpret_cast<LPARAM>(pBorders));
}

// 境界情報を取得（一要素ずつ）
BOOL statusbar::GetBorders(int *pHorz, int *pVert, int *nSpacing) const
{
	int arBorders[3];
	if(!GetBorders(arBorders))
	{
		return FALSE;
	}

	if(pHorz    != NULL) { *pHorz    = arBorders[0]; }
	if(pVert    != NULL) { *pVert    = arBorders[1]; }
	if(nSpacing != NULL) { *nSpacing = arBorders[2]; }
	return TRUE;
}

// ペインを作成
BOOL statusbar::SetParts(const int nParts, int *pWidths)
{
	assert(pWidths != NULL);
	return SendMessage(
		SB_SETPARTS,
		nParts,
		reinterpret_cast<LPARAM>(pWidths));
}

// ペイン情報を取得
BOOL statusbar::GetParts(const int nParts, int *pWidths) const
{
	assert(pWidths != NULL);
	return SendMessage(
		SB_GETPARTS,
		nParts,
		reinterpret_cast<LPARAM>(pWidths));
}

// テキストを出力
BOOL statusbar::SetText(const int iPart, const int uType, const tstring_t &text)
{
	return SendMessage(
		SB_SETTEXT,
		iPart | uType,
		reinterpret_cast<LPARAM>(text.c_str()));
}


////////////////////////////////////////////////////////////
// メッセージハンドラ
void statusbar::OnSize(const UINT nType, const int cx, const int cy)
{
	control::OnSize(nType, cx, cy);
	UpdatePanePosition();
}


////////////////////////////////////////////////////////////
// private

// 内側の領域を計算
void statusbar::CalcInsideRect(RECT &rect) const
{
	GetClientRect(rect);

	// 最大表示でなければサイズ変更領域を考慮
	if(!::IsZoomed(GetParent()))
	{
		// サイズ変更領域は、ほぼクライアント領域の高さと等しい
		rect.right -= (rect.bottom - rect.top);
	}
}

// 各ペインの位置を更新
BOOL statusbar::UpdatePanePosition(void)
{
	// アイドル文字列表示領域＋インジケータ
	const size_t count = m_pane_width.size();
	intarray_t right_pos(count + 1);

	// [block]
	// 一番右のペインの座標を決定
	{
		RECT rect;
		CalcInsideRect(rect);
		right_pos[count] = rect.right;
	}

	// その他のペインの座標を決定
	for(int j = count - 1; j >= 0; j--)
	{
		right_pos[j] = right_pos[j + 1] - m_pane_width[j];
	}

	return SetParts(right_pos.size(), &right_pos[0]);
}

_WGC_END                                // }
