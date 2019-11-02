// ScrollBarCtrl.h …スクロールバーコントロールのサブクラス化
#ifndef __WGC_SCROLLBARCTRL__
#define __WGC_SCROLLBARCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class scrollbar_ctrl : public control
{
public:
	////////////////////////////////////////////////////////////////
	// コンストラクタ／デストラクタ
	explicit scrollbar_ctrl(void);
	virtual ~scrollbar_ctrl(void);

	// スクロール情報
	BOOL SetScrollInfo(const SCROLLINFO &si, BOOL bRedraw = TRUE);
	BOOL GetScrollInfo(SCROLLINFO &si, UINT nMask = SIF_ALL) const;

	// ページサイズ
	int GetPageSize(void) const;
	int SetPageSize(int nSize);

	// 範囲
	void GetRange(int &nMin, int &nMax) const;
	void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);

	// スクロール位置
	int GetPos(void) const;
	void SetPos(int nPos, BOOL bRedraw = TRUE);

	// 
	BOOL EnableArrow(UINT nArrowFlags = ESB_ENABLE_BOTH);

	int GetLimit(void) const;
};

_WGC_END                                // }

#endif // __WGC_SCROLLBARCTRL__
