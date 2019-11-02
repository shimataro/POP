// Control.cpp …インプリメンテーションファイル
#include "Control.h"
#include "Dialog.h"
#include <assert.h>                     // assert()


// ライブラリのリンク
#pragma comment(lib, "comctl32.lib")    // コモンコントロール用ライブラリ


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// 構築

// コンストラクタ
control::control(void)
{
	// コモンコントロールを初期化
	::InitCommonControls();
}

// デストラクタ
control::~control(void)
{
}



// サブクラス化
BOOL control::SubclassDlgItem(const UINT uID, dialog *parent)
{
	assert(parent != NULL);

	// コントロールウインドウハンドルを取得
	HWND hWnd = parent->GetDlgItem(uID);
	if(hWnd == NULL)
	{
		return FALSE;
	}

	// サブクラス化
	return Subclass(hWnd);
}


BOOL control::Create(
	LPCTSTR lpszClassName,
	LPCTSTR lpszWindowName,
	const DWORD dwStyle,
	const DWORD dwCtlStyle,
	const DWORD dwExStyle,
	window *parent,
	const UINT nID,
	HINSTANCE hInstance /* = NULL */,
	const int x /* = CW_USEDEFAULT */,
	const int y /* = CW_USEDEFAULT */,
	const int nWidth  /* = CW_USEDEFAULT */,
	const int nHeight /* = CW_USEDEFAULT */)
{
	return window::Create(
		lpszClassName, lpszWindowName,
		dwStyle | dwCtlStyle,
		dwExStyle,
		parent,
		reinterpret_cast<HMENU>(nID),
		hInstance,
		x, y, nWidth, nHeight);
}

_WGC_END                                // }
