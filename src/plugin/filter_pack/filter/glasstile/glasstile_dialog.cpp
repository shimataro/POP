// glasstile_dialog.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "glasstile_dialog.h"


// コンストラクタ
glasstile_dialog::glasstile_dialog(const GLASSTILEPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: m_width(info_ptr->width), m_height(info_ptr->height),
	preview_dialog(IDD, info_ptr, data_ptr, hWndParent)
{
	// パラメータを保存
	m_param = param;
}

// デストラクタ
glasstile_dialog::~glasstile_dialog(void)
{
}

// コントロールをオブジェクトに関連付け
void glasstile_dialog::AssociateDialogControls(void)
{
	m_ctrlSpinWidth .SubclassDlgItem(IDC_OPTION_GLASSTILE_SPIN_WIDTH    , this);
	m_ctrlSpinHeight.SubclassDlgItem(IDC_OPTION_GLASSTILE_SPIN_HEIGHT   , this);
	m_ctrlPreview   .SubclassDlgItem(IDC_OPTION_GLASSTILE_STATIC_PREVIEW, this);
}

// ダイアログパラメータを更新
void glasstile_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	m_bUpdating = TRUE;
	if(bUpdate)
	{
		// ダイアログ→変数
		m_param.width  = m_ctrlSpinWidth.GetPos();
		m_param.height = m_ctrlSpinHeight.GetPos();
	}
	else
	{
		// 変数→ダイアログ
		m_ctrlSpinWidth.SetPos(m_param.width);
		m_ctrlSpinHeight.SetPos(m_param.height);
	}
	m_bUpdating = FALSE;
}

// プレビューを更新
void glasstile_dialog::UpdatePreview(void)
{
	// 現在の設定でガラスタイルフィルタをかける
	UpdateData(TRUE);

	// プレビューサイズに合わせてパラメータを調整
	GLASSTILEPARAM param = m_param;
	param.width  = saturate<int>(param.width  * m_info_preview.width  / m_width , TILESIZE_MIN, TILESIZE_MAX);
	param.height = saturate<int>(param.height * m_info_preview.height / m_height, TILESIZE_MIN, TILESIZE_MAX);
	effect_glasstile(&m_info_preview, &m_data_preview_before, &m_data_preview_after, &param, NULL, 0);

	preview_dialog::UpdatePreview();
}

// ダイアログ初期化
BOOL glasstile_dialog::OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam)
{
	preview_dialog::OnInitDialog(hWndFocus, dwInitParam);

	// コントロールの初期設定
	m_ctrlSpinWidth .SetRange(TILESIZE_MIN, TILESIZE_MAX);
	m_ctrlSpinHeight.SetRange(TILESIZE_MIN, TILESIZE_MAX);
	m_ctrlPreview.SetBitmap(m_hDIBSection);                     // ピクチャーコントロールのビットマップをDIBセクションに設定

	UpdateData(FALSE);
	UpdatePreview();

	// ツールチップを設定
	const UINT arIDs[] =
	{
		IDC_OPTION_GLASSTILE_EDIT_WIDTH,
		IDC_OPTION_GLASSTILE_EDIT_HEIGHT,
		IDC_OPTION_GLASSTILE_STATIC_PREVIEW,
	};

	SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));

	// OKボタンにフォーカスをあわせる
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}
