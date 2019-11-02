// Dialog.h …ダイアログボックスのサブクラス化とツールチップ
#ifndef __WGC_DIALOG__
#define __WGC_DIALOG__

#include "Window.h"
#include "Control.h"
#include "ToolTipCtrl.h"


_WGC_BEGIN                              // namespace wgc {

class dialog : public window
{
public:
	// コンストラクタ／デストラクタ
	dialog(HINSTANCE hResource, LPCTSTR lpTemplate, HWND hWndParent = NULL);
	~dialog(void);

	// ダイアログの表示／終了
	int  DoModal   (const LPARAM dwInitParam = 0);  // モーダル表示
	BOOL DoModeless(const LPARAM dwInitParam = 0);  // モードレス表示
	void EndModeless(void);                         // モードレスダイアログの終了

	// メッセージ関数
	LONG SendDlgItemMessage(const int nIDDlgItem, const UINT uMsg, const WPARAM wParam = 0, const LPARAM lParam = 0);
	BOOL IsDialogMessage(LPMSG lpMsg);

	// チェックボタン操作関数
	UINT IsDlgButtonChecked   (const int nIDButton) const;
	BOOL CheckDlgButton       (const int nIDButton, const UINT uCheck);
	BOOL CheckRadioButton     (const int nIDFirstButton, const int nIDLastButton, const int nIDCheckButton);
	int  GetCheckedRadioButton(const int nIDFirstButton, const int nIDLastButton);

	// ラッパ関数
	HWND GetDlgItem(const int nIDDlgItem);
	UINT GetDlgItemInt(const int nIDDlgItem, BOOL *lpTranslated, const BOOL bSigned = TRUE);
	BOOL SetDlgItemInt(const int nIDDlgItem, const UINT nValue , const BOOL bSigned = TRUE);
	UINT GetDlgItemText(const int nIDDlgItem, tstring_t &tstr);
	BOOL SetDlgItemText(const int nIDDlgItem, const tstring_t &tstr);

private:
	// ツールチップ
	tooltip_ctrl m_ctrlToolTip;
	void InitToolTips(void);

protected:
	HINSTANCE m_hResource;                  // ダイアログのリソースハンドル
	LPCTSTR   m_lpszTemplateName;           // ダイアログテンプレート
	HWND      m_hWndParent;                 // 親ウインドウ

	// ラッパ関数
	BOOL EndDialog(const int nResult);

	// 純粋仮想関数
	virtual void AssociateDialogControls(void) = 0;
	virtual void UpdateData(const BOOL bUpdate = TRUE) = 0;

	// ツールチップ関連
	void SetToolTips(const UINT arID[], const UINT nIDCount);
	void SetToolTip (const UINT nID);

	// ダイアログプロシージャ
	virtual LRESULT CALLBACK WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT _Default(void);


	// ダイアログメッセージハンドラ
	virtual BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);

	BOOL OnCommand(const WORD wNotifyCode, const WORD wID, HWND hWndCtrl);

	virtual void OnOK(void);                // OKボタン
	virtual void OnCancel(void);            // キャンセルボタン
};

_WGC_END                                // }

#endif // __WGC_DIALOG__
