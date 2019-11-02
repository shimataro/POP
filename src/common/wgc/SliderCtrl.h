// SliderCtrl.h …スライダコントロールのサブクラス化
#ifndef __WGC_SLIDERCTRL__
#define __WGC_SLIDERCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class slider_ctrl : public control
{
public:
	////////////////////////////////////////////////////////////////
	// コンストラクタ／デストラクタ
	explicit slider_ctrl(void);
	virtual ~slider_ctrl(void);

	// 行のサイズ
	int GetLineSize(void) const;
	int SetLineSize(int nSize);

	// ページサイズ
	int GetPageSize(void) const;
	int SetPageSize(int nSize);

	// 最小値
	int GetRangeMin(void) const;
	void SetRangeMin(int nMin, BOOL bRedraw = FALSE);

	// 最大値
	int GetRangeMax(void) const;
	void SetRangeMax(int nMax, BOOL bRedraw = FALSE);

	// 範囲
	void GetRange(int &nMin, int &nMax) const;
	void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);

	// 矩形
	void GetChannelRect(LPRECT lprc) const;
	void GetThumbRect(LPRECT lprc) const;

	// つまみの位置
	int GetPos(void) const;
	void SetPos(int nPos);

	// 目盛りマーク
	UINT GetNumTics(void) const;
	int  GetTic(int nTic) const;
	BOOL SetTic(int nTic);
	int  GetTicPos(int nTic) const;
	void SetTicFreq(int nFreq);

	// バディ
	HWND GetBuddy(BOOL fLocation = TRUE) const;
	HWND SetBuddy(HWND hWndBuddy, BOOL fLocation = TRUE);

	// 操作
	void ClearSel(BOOL bRedraw = FALSE);
	void ClearTics(BOOL bRedraw = FALSE);
};

_WGC_END                                // }

#endif // __WGC_SLIDERCTRL__
