// StatusBar.h …スライダコントロールのサブクラス化
#ifndef __WGC_STATUSBAR__
#define __WGC_STATUSBAR__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class statusbar : public control
{
public:
	////////////////////////////////////////////////////////////////
	// コンストラクタ／デストラクタ
	explicit statusbar(void);
	virtual ~statusbar(void);

	BOOL Create(
		HWND hWndParent,
		LPCTSTR lpszWindowName,
		const DWORD dwStyle,
		const UINT nID);

	// 操作
	BOOL SetText(const int iPart, const int uType, LPCTSTR lpszText);
};

_WGC_END                                // }

#endif // __WGC_STATUSBAR__
