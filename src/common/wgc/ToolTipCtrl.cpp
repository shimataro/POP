// ToolTipCtrl.cpp …インプリメンテーションファイル
#include "ToolTipCtrl.h"


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// 構築

// コンストラクタ
tooltip_ctrl::tooltip_ctrl(void)
{
}

// デストラクタ
tooltip_ctrl::~tooltip_ctrl(void)
{
}


////////////////////////////////////////////////////////////////////////////////
// 作成

BOOL tooltip_ctrl::Create(window *parent, const DWORD dwStyle /* = 0 */, HINSTANCE hInstance /* = NULL */)
{
	// ツールチップウインドウの作成
	return control::Create(
		TOOLTIPS_CLASS,
		NULL,
		dwStyle, 0, 0,
		parent,
		0,
		hInstance);
}


// ツールチップを追加（TOOLINFO構造体を使用）
BOOL tooltip_ctrl::AddTool(const TOOLINFO &ti)
{
	return SendMessage(TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&ti));
}

// 子ウインドウにツールチップを追加（ストリングリソース使用）
BOOL tooltip_ctrl::AddTool(HWND hWnd, HINSTANCE hResource, const UINT nIDText)
{
	return AddTool(hWnd, hResource, MAKEINTRESOURCE(nIDText));
}

// 子ウインドウにツールチップを追加（文字列を直接指定）
BOOL tooltip_ctrl::AddTool(HWND hWnd, HINSTANCE hResource, LPCTSTR lpszText)
{
	// TOOLINFO構造体の基本設定
	TOOLINFO ti;
	ti.cbSize = sizeof(ti);                 // TOOLINFO構造体のサイズ
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;// ID＝ウインドウハンドル ＋ サブクラス化
	ti.hwnd   = GetParent();                // ツールチップを所有するウインドウ＝親ウインドウ
	ti.hinst  = hResource;                  // ストリングリソースを持つインスタンス

	ti.uId      = reinterpret_cast<UINT>(hWnd);
	ti.lpszText = const_cast<LPTSTR>(lpszText);

	return AddTool(ti);
}


////////////////////////////////////////////////////////////////////////////////
// 最大幅（Internet Explorer 3.0以降）

// 最大幅を取得
int tooltip_ctrl::GetMaxTipWidth(void) const
{
#if (_WIN32_IE >= 0x0300)
	return SendMessage(TTM_GETMAXTIPWIDTH);
#else
	return -1;
#endif
}

// 最大幅を設定
int tooltip_ctrl::SetMaxTipWidth(const int iWidth)
{
#if (_WIN32_IE >= 0x0300)
	return SendMessage(TTM_SETMAXTIPWIDTH, 0, iWidth);
#else
	return -1;
#endif
}

_WGC_END                                // }
