// ToolBar.h …ツールバーのサブクラス化
#ifndef __WGC_TOOLBAR__
#define __WGC_TOOLBAR__

#include "Control.h"
#include <vector>


_WGC_BEGIN                              // namespace wgc {

class toolbar : public control
{
public:
	////////////////////////////////////////////////////////////////
	// コンストラクタ／デストラクタ
	explicit toolbar(void);
	virtual ~toolbar(void);

	BOOL Create(
		window *parent,
		HINSTANCE hResource,
		const UINT nResourceID,
		const int cx,
		const int cy,
		const int arCommand[],
		const int nCommandCount,
		const DWORD dwStyle        = WS_CHILD | WS_VISIBLE,
		const DWORD dwCtlStyle     = CCS_TOP | CCS_ADJUSTABLE,
		const DWORD dwToolBarStyle = TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | TBSTYLE_ALTDRAG,
		const UINT  nID            = IDW_STD_TOOLBAR);

	// アトリビュート
	void GetButtonSize(SIZE &size) const;
	BOOL GetButton(const UINT iButton, TBBUTTON &tbButton) const;
	UINT GetButtonCount(void) const;

	// オペレーション
	void Customize(void);
	BOOL AddButtons(LPTBBUTTON lpButtons, const int nNumButtons);
	BOOL DeleteButton(const UINT iButton);

	// 通知メッセージの処理
	BOOL HandleNotifyMessage(LPNMHDR lpNmHeader, BOOL &bResult);

protected:
	typedef struct _button_info_t
	{
		TBBUTTON  button;
		tstring_t str;
	} button_info_t;
	std::vector<button_info_t> m_button_array;

	virtual void OnNotifyBeginAdjust(LPTBNOTIFY lpNotify);
	virtual void OnNotifyEndAdjust  (LPTBNOTIFY lpNotify);

	virtual BOOL OnNotifyQueryInsert(LPTBNOTIFY lpNotify);
	virtual BOOL OnNotifyQueryDelete(LPTBNOTIFY lpNotify);

	virtual void OnNotifyBeginDrag(LPTBNOTIFY lpNotify);
	virtual void OnNotifyEndDrag  (LPTBNOTIFY lpNotify);

	virtual BOOL OnNotifyGetButtonInfo(LPTBNOTIFY lpNotify);

	virtual void OnNotifyToolbarChange(LPTBNOTIFY lpNotify);
	virtual void OnNotifyReset   (LPTBNOTIFY lpNotify);
	virtual void OnNotifyCustHelp(LPTBNOTIFY lpNotify);
};

_WGC_END                                // }

#endif // __WGC_TOOLBAR__
