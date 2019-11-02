// Control.h …コントロールのサブクラス化
#ifndef __WGC_CONTROL__
#define __WGC_CONTROL__

#include "Window.h"
#include <commctrl.h>                   // InitCommonControls()


#define IDW_STD_REBAR       500
#define IDW_STD_TOOLBAR     501
#define IDW_STD_STATUSBAR   502


_WGC_BEGIN                              // namespace wgc {

class control : public window
{
public:
	// コンストラクタ／デストラクタ
	explicit control(void);
	virtual ~control(void);

	// サブクラス化
	BOOL SubclassDlgItem(const UINT uID, dialog *parent);

protected:
	BOOL Create(
		LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName,
		const DWORD dwStyle,
		const DWORD dwCtlStyle,
		const DWORD dwExStyle,
		window *parent,
		const UINT nID,
		HINSTANCE hInstance = NULL,
		const int x = CW_USEDEFAULT,
		const int y = CW_USEDEFAULT,
		const int nWidth  = CW_USEDEFAULT,
		const int nHeight = CW_USEDEFAULT);
};

_WGC_END                                // }

#endif // __WGC_CONTROL__
