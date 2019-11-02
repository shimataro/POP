// CheckBoxCtrl.h …チェックボックスコントロールのサブクラス化
#ifndef __WGC_CHECKBOXCTRL__
#define __WGC_CHECKBOXCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class checkbox_ctrl : public control
{
public:
	// コンストラクタ／デストラクタ
	explicit checkbox_ctrl(void);
	virtual ~checkbox_ctrl(void);

	// 状態
	UINT GetState(void) const;
	void SetState(BOOL bHighlight);

	// オン／オフ
	int GetCheck(void) const;
	void SetCheck(int nCheck);

	// ボタンスタイル
	void SetStyle(UINT nStyle, BOOL bRedraw = TRUE);
};

_WGC_END                                // }

#endif // __WGC_CHECKBOXCTRL__
