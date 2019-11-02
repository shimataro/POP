// SpinButtonCtrl.cpp …インプリメンテーションファイル
#include "SpinButtonCtrl.h"


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// 構築

// コンストラクタ
spinbutton_ctrl::spinbutton_ctrl(void)
{
}

// デストラクタ
spinbutton_ctrl::~spinbutton_ctrl(void)
{
}


////////////////////////////////////////////////////////////////////////////////
// アクセラレーション

UINT spinbutton_ctrl::GetAccel(int nAccel, UDACCEL *pAccel) const
{
	return SendMessage(
		UDM_GETACCEL,
		static_cast     <WPARAM>(nAccel),
		reinterpret_cast<LPARAM>(pAccel));
}

BOOL spinbutton_ctrl::SetAccel(int nAccel, UDACCEL *pAccel)
{
	return SendMessage(
		UDM_SETACCEL,
		static_cast     <WPARAM>(nAccel),
		reinterpret_cast<LPARAM>(pAccel));
}


////////////////////////////////////////////////////////////////////////////////
// 基数値

int spinbutton_ctrl::GetBase(void) const
{
	return SendMessage(UDM_GETBASE);
}

int spinbutton_ctrl::SetBase(int nBase)
{
	return SendMessage(UDM_SETBASE, (WPARAM)nBase);
}


////////////////////////////////////////////////////////////////////////////////
// バディウインドウ

HWND spinbutton_ctrl::GetBuddy(void) const
{
	LRESULT result = SendMessage(UDM_GETBUDDY);
	return reinterpret_cast<HWND>(result);
}

HWND spinbutton_ctrl::SetBuddy(HWND hWndBuddy)
{
	LRESULT result = SendMessage(UDM_SETBUDDY, (WPARAM)hWndBuddy);
	return reinterpret_cast<HWND>(result);
}


////////////////////////////////////////////////////////////////////////////////
// 位置

int spinbutton_ctrl::GetPos(void) const
{
	return SendMessage(UDM_GETPOS);
}

int spinbutton_ctrl::SetPos(int nPos)
{
	return SendMessage(UDM_SETPOS, 0, (LPARAM)MAKELONG((short)nPos, 0));
}


////////////////////////////////////////////////////////////////////////////////
// 範囲

DWORD spinbutton_ctrl::GetRange(void) const
{
	return SendMessage(UDM_GETRANGE);
}

void spinbutton_ctrl::GetRange(int &lower, int &upper) const
{
	const DWORD dwRange = GetRange();

	lower = HIWORD(dwRange);
	upper = LOWORD(dwRange);
}

void spinbutton_ctrl::GetRange32(int &lower, int &upper) const
{
	SendMessage(UDM_GETRANGE32, (WPARAM)&lower, (LPARAM)&upper);
}

void spinbutton_ctrl::SetRange(int nLower, int nUpper)
{
	SendMessage(UDM_SETRANGE, 0, (LPARAM)MAKELONG((short)nUpper, (short)nLower));
}

void spinbutton_ctrl::SetRange32(int nLower, int nUpper)
{
	SendMessage(UDM_SETRANGE32, nLower, nUpper);
}

_WGC_END                                // }
