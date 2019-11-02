// SpinButtonCtrl.h …スピンボタンコントロールのサブクラス化
#ifndef __WGC_SPINBUTTONCTRL__
#define __WGC_SPINBUTTONCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class spinbutton_ctrl : public control
{
public:
	////////////////////////////////////////////////////////////////
	// コンストラクタ／デストラクタ
	explicit spinbutton_ctrl(void);
	virtual ~spinbutton_ctrl(void);

	// アクセラレーション
	UINT GetAccel(int nAccel, UDACCEL *pAccel) const;
	BOOL SetAccel(int nAccel, UDACCEL *pAccel);

	// 基数値
	int GetBase(void) const;
	int SetBase(int nBase);

	// バディウインドウ
	HWND GetBuddy(void) const;
	HWND SetBuddy(HWND hWndBuddy);

	// 位置
	int GetPos(void) const;
	int SetPos(int nPos);

	// 範囲
	DWORD GetRange(void) const;
	void GetRange(int &lower, int &upper) const;
	void GetRange32(int &lower, int &upper) const;

	void SetRange(int nLower, int nUpper);
	void SetRange32(int nLower, int nUpper);
};

_WGC_END                                // }

#endif // __WGC_SPINBUTTONCTRL__
