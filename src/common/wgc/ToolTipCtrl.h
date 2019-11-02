// ToolTipCtrl.h …ツールチップコントロールのサブクラス化
#ifndef __WGC_TOOLTIPCTRL__
#define __WGC_TOOLTIPCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class tooltip_ctrl : public control
{
public:
	// コンストラクタ／デストラクタ
	explicit tooltip_ctrl(void);
	virtual ~tooltip_ctrl(void);

	// 作成
	BOOL Create(window *parent, const DWORD dwStyle = 0, HINSTANCE hInstance = NULL);

	////////////////////////////////////////
	// 操作

	// ツールチップを追加
	BOOL AddTool(const TOOLINFO &ti);
	BOOL AddTool(HWND hWnd, HINSTANCE hResource, const UINT nIDText);
	BOOL AddTool(HWND hWnd, HINSTANCE hResource, LPCTSTR lpszText);

	// 最大幅（Internet Explorer 3.0以降）
	int GetMaxTipWidth(void) const;
	int SetMaxTipWidth(const int iWidth);

private:
	HINSTANCE m_hInstance;
};

_WGC_END                                // }

#endif // __WGC_TOOLTIPCTRL__
