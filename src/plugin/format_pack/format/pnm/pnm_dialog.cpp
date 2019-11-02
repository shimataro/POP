// pnm_dialog.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "pnm_dialog.h"


// コンストラクタ
pnm_dialog::pnm_dialog(const PNMPARAM &param, HWND hWndParent /* = NULL */)
	: dialog(GetInstanceHandle(), MAKEINTRESOURCE(IDD), hWndParent)
{
	m_param = param;
}

// コントロールをオブジェクトに関連付け
void pnm_dialog::AssociateDialogControls(void)
{
}

// ダイアログパラメータを更新
void pnm_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	if(bUpdate)
	{
		// ダイアログ→変数
		const UINT nSaveFormat = GetCheckedRadioButton(IDC_PNM_SETTING_RADIO_FORMAT_ASCII, IDC_PNM_SETTING_RADIO_FORMAT_BINARY);

		m_param.save_format = nSaveFormat - IDC_PNM_SETTING_RADIO_FORMAT_ASCII;
	}
	else
	{
		// 変数→ダイアログ
		const UINT nSaveFormat = m_param.save_format + IDC_PNM_SETTING_RADIO_FORMAT_ASCII;

		CheckRadioButton(IDC_PNM_SETTING_RADIO_FORMAT_ASCII, IDC_PNM_SETTING_RADIO_FORMAT_BINARY, nSaveFormat);
	}
}

// ダイアログ初期化
BOOL pnm_dialog::OnInitDialog(HWND hWndFocus, LPARAM dwInitParam)
{
	dialog::OnInitDialog(hWndFocus, dwInitParam);

	// コントロールの状態を設定
	UpdateData(FALSE);

	// ツールチップを設定
	const UINT arID[] =
	{
		IDC_PNM_SETTING_RADIO_FORMAT_ASCII,
		IDC_PNM_SETTING_RADIO_FORMAT_BINARY,
	};

	SetToolTips(arID, SIZE_OF_ARRAY(arID));

	// OKボタンにフォーカスをあわせる
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}
