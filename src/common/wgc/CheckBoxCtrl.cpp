// CheckBoxCtrl.cpp …インプリメンテーションファイル
#include "CheckBoxCtrl.h"


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// 構築

// コンストラクタ
checkbox_ctrl::checkbox_ctrl(void)
{
}

// デストラクタ
checkbox_ctrl::~checkbox_ctrl(void)
{
}


////////////////////////////////////////////////////////////////////////////////
// 状態

UINT checkbox_ctrl::GetState(void) const
{
	return SendMessage(BM_GETSTATE);
}

void checkbox_ctrl::SetState(BOOL bHighlight)
{
	SendMessage(BM_SETSTATE, bHighlight);
}


////////////////////////////////////////////////////////////////////////////////
// オン／オフ

int checkbox_ctrl::GetCheck(void) const
{
	return SendMessage(BM_GETCHECK);
}

void checkbox_ctrl::SetCheck(int nCheck)
{
	SendMessage(BM_SETCHECK, nCheck);
}


////////////////////////////////////////////////////////////////////////////////
// ボタンスタイル

void checkbox_ctrl::SetStyle(UINT nStyle, BOOL bRedraw /* = TRUE */)
{
	SendMessage(BM_SETSTYLE, LOWORD(nStyle), MAKELPARAM(bRedraw, 0));
}

_WGC_END                                // }
