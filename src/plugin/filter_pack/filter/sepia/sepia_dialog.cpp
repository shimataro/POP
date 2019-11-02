// sepia_dialog.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "sepia_dialog.h"


// コンストラクタ
sepia_dialog::sepia_dialog(const SEPIAPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: preview_dialog(IDD, info_ptr, data_ptr, hWndParent)
{
	// パラメータを保存
	m_param = param;
}

// デストラクタ
sepia_dialog::~sepia_dialog(void)
{
}

// コントロールをオブジェクトに関連付け
void sepia_dialog::AssociateDialogControls(void)
{
	m_ctrlStrength.SubclassDlgItem(IDC_OPTION_SEPIA_SLIDER_STRENGTH, this); // セピアの強度
	m_ctrlPreview .SubclassDlgItem(IDC_OPTION_SEPIA_STATIC_PREVIEW , this); // プレビュー
}

// ダイアログパラメータを更新
void sepia_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	m_bUpdating = TRUE;
	if(bUpdate)
	{
		// ダイアログ→変数
		m_param.strength = m_ctrlStrength.GetPos();
	}
	else
	{
		// 変数→ダイアログ
		m_ctrlStrength.SetPos(m_param.strength);
	}
	m_bUpdating = FALSE;
}

// プレビューを更新
void sepia_dialog::UpdatePreview(void)
{
	// 現在の設定でセピアフィルタをかける
	UpdateData(TRUE);
	effect_sepia(&m_info_preview, &m_data_preview_before, &m_data_preview_after, &m_param, NULL, 0);

	preview_dialog::UpdatePreview();
}

// ダイアログ初期化
BOOL sepia_dialog::OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam)
{
	preview_dialog::OnInitDialog(hWndFocus, dwInitParam);

	// コントロールの初期設定
	m_ctrlStrength.SetRange(SEPIA_STRENGTH_MIN, SEPIA_STRENGTH_MAX, TRUE);  // スライダコントロールの設定
	m_ctrlPreview.SetBitmap(m_hDIBSection);                     // ピクチャーコントロールのビットマップをDIBセクションに設定

	UpdateData(FALSE);
	UpdatePreview();

	// ツールチップを設定
	const UINT arIDs[] =
	{
		IDC_OPTION_SEPIA_SLIDER_STRENGTH,
		IDC_OPTION_SEPIA_STATIC_PREVIEW,
	};

	SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));

	// OKボタンにフォーカスをあわせる
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}

// 水平スクロールバー
void sepia_dialog::OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar)
{
	preview_dialog::OnHScroll(nSBCode, nPos, hWndScrollBar);
	UpdatePreview();
}
