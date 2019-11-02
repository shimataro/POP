// Dialog.cpp …インプリメンテーションファイル
#include "Dialog.h"
#include <windowsx.h>                   // GET_WM_COMMAND_ID()
#include <assert.h>                     // assert()
#include <limits.h>                     // INT_MAX


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// 構築

// コンストラクタ
dialog::dialog(HINSTANCE hResource, LPCTSTR lpszTemplateName, HWND hWndParent /* = NULL */)
{
	m_hResource        = hResource;
	m_lpszTemplateName = lpszTemplateName;
	m_hWndParent       = hWndParent;
}

// デストラクタ
dialog::~dialog(void)
{
}


////////////////////////////////////////////////////////////////////////////////
// 表示

// モーダルダイアログを表示
int dialog::DoModal(const LPARAM dwInitParam /* = 0 */)
{
	_SetHookTarget(this);
	const int nResult = ::DialogBoxParam(
		m_hResource,
		m_lpszTemplateName,
		m_hWndParent,
		reinterpret_cast<DLGPROC>(_WindowProcBase),
		dwInitParam);

	if(nResult == -1)
	{
		_SetHookTarget(NULL);
	}
	return nResult;
}

// ダイアログ作成
BOOL dialog::DoModeless(const LPARAM dwInitParam /* = 0 */)
{
	_SetHookTarget(this);
	HWND hWnd = ::CreateDialogParam(
		m_hResource,
		m_lpszTemplateName,
		m_hWndParent,
		reinterpret_cast<DLGPROC>(_WindowProcBase),
		dwInitParam);

	if(hWnd == NULL)
	{
		_SetHookTarget(NULL);
		return FALSE;
	}
	assert(hWnd == m_hWnd);
	return TRUE;
}

// ダイアログを閉じる（モードレスダイアログ）
void dialog::EndModeless(void)
{
	DestroyWindow();
}


////////////////////////////////////////////////////////////////////////////////
// メッセージ関数

// ダイアログアイテムへメッセージを送る
LONG dialog::SendDlgItemMessage(const int nIDDlgItem, const UINT uMsg, const WPARAM wParam /* = 0 */, const LPARAM lParam /* = 0*/)
{
	return ::SendDlgItemMessage(m_hWnd, nIDDlgItem, uMsg, wParam, lParam);
}

// モードレスダイアログ用メッセージの判定
BOOL dialog::IsDialogMessage(LPMSG lpMsg)
{
	return ::IsDialogMessage(m_hWnd, lpMsg);
}


////////////////////////////////////////////////////////////////////////////////
// チェックボタン操作関数

UINT dialog::IsDlgButtonChecked(const int nIDButton) const
{
	return ::IsDlgButtonChecked(m_hWnd, nIDButton);
}

BOOL dialog::CheckDlgButton(const int nIDButton, const UINT uCheck)
{
	return ::CheckDlgButton(m_hWnd, nIDButton, uCheck);
}

BOOL dialog::CheckRadioButton(const int nIDFirstButton, const int nIDLastButton, const int nIDCheckButton)
{
	return ::CheckRadioButton(m_hWnd, nIDFirstButton, nIDLastButton, nIDCheckButton);
}

int dialog::GetCheckedRadioButton(const int nIDFirstButton, const int nIDLastButton)
{
	for(int id = nIDFirstButton; id <= nIDLastButton; id++)
	{
		if(IsDlgButtonChecked(id) == BST_CHECKED)
		{
			return id;
		}
	}
	return -1;
}


////////////////////////////////////////////////////////////////////////////////
// ツールチップ関数

// ツールチップの初期化
void dialog::InitToolTips(void)
{
	m_ctrlToolTip.Create(this, WS_POPUP | TTS_ALWAYSTIP, m_hResource);
	m_ctrlToolTip.SetMaxTipWidth(INT_MAX);
}

// ツールチップを設定
void dialog::SetToolTips(const UINT arID[], const UINT nIDCount)
{
	// IDごとにツールチップを設定
	for(UINT i = 0; i < nIDCount; i++)
	{
		HWND hWnd = GetDlgItem(arID[i]);
		if(::IsWindow(hWnd))
		{
			// ツールチップを追加
			m_ctrlToolTip.AddTool(hWnd, m_hResource, arID[i]);
		}
	}
}

void dialog::SetToolTip(const UINT nID)
{
	SetToolTips(&nID, 1);
}


// ラッパ関数
HWND dialog::GetDlgItem(const int nIDDlgItem)
{
	return ::GetDlgItem(m_hWnd, nIDDlgItem);
}

UINT dialog::GetDlgItemInt(const int nIDDlgItem, BOOL *lpTranslated, const BOOL bSigned /* = TRUE */)
{
	return ::GetDlgItemInt(m_hWnd, nIDDlgItem, lpTranslated, bSigned);
}

BOOL dialog::SetDlgItemInt(const int nIDDlgItem, const UINT nValue, const BOOL bSigned /* = TRUE */)
{
	return ::SetDlgItemInt(m_hWnd, nIDDlgItem, nValue, bSigned);
}

UINT dialog::GetDlgItemText(const int nIDDlgItem, tstring_t &tstr)
{
	TCHAR buffer[256];
	const UINT result = ::GetDlgItemText(m_hWnd, nIDDlgItem, buffer, SIZE_OF_ARRAY(buffer));

	tstr = buffer;
	return result;
}

BOOL dialog::SetDlgItemText(const int nIDDlgItem, const tstring_t &tstr)
{
	return ::SetDlgItemText(m_hWnd, nIDDlgItem, tstr.c_str());
}

BOOL dialog::EndDialog(const int nResult)
{
	return ::EndDialog(m_hWnd, nResult);
}


////////////////////////////////////////////////////////////////////////////////
// ダイアログプロシージャ

LRESULT CALLBACK dialog::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		InitToolTips();
		return OnInitDialog(
			reinterpret_cast<HWND>(wParam),              // hWndFocus
			lParam);                                     // dwInitParam
	}
	return window::WindowProc(uMsg, wParam, lParam);
}

// ダイアログプロシージャのデフォルト処理
LRESULT dialog::_Default(void)
{
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
// ダイアログメッセージハンドラ

// ダイアログ初期化
BOOL dialog::OnInitDialog(HWND /* hWndFocus */, const LPARAM /* dwInitParam */)
{
	AssociateDialogControls();
	return _Default();
}

// コマンド
BOOL dialog::OnCommand(const WORD /* wNotifyCode */, const WORD wID, HWND /* hWndCtrl */)
{
	switch(wID)
	{
	case IDOK:
		OnOK();
		return FALSE;

	case IDCANCEL:
		OnCancel();
		return FALSE;
	}
	return TRUE;
}

// OK
void dialog::OnOK(void)
{
	UpdateData(TRUE);
	EndDialog(IDOK);
}

// キャンセル
void dialog::OnCancel(void)
{
	EndDialog(IDCANCEL);
}

_WGC_END                                // }
