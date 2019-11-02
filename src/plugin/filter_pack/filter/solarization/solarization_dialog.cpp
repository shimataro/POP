// solarization_dialog.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "solarization_dialog.h"


// コンストラクタ
solarization_dialog::solarization_dialog(const SOLARIZATIONPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: preview_dialog(IDD, info_ptr, data_ptr, hWndParent)
{
	// パラメータを保存
	m_param = param;
}

// デストラクタ
solarization_dialog::~solarization_dialog(void)
{
}

// コントロールをオブジェクトに関連付け
void solarization_dialog::AssociateDialogControls(void)
{
	m_ctrlThreshold.SubclassDlgItem     (IDC_OPTION_SOLARIZATION_SCROLL_THRESHOLD, this);
	m_ctrlThresholdValue.SubclassDlgItem(IDC_OPTION_SOLARIZATION_STATIC_THRESHOLD, this);
	m_ctrlChannelRed.SubclassDlgItem    (IDC_OPTION_SOLARIZATION_CHANNEL_RED     , this);
	m_ctrlChannelGreen.SubclassDlgItem  (IDC_OPTION_SOLARIZATION_CHANNEL_GREEN   , this);
	m_ctrlChannelBlue.SubclassDlgItem   (IDC_OPTION_SOLARIZATION_CHANNEL_BLUE    , this);
	m_ctrlPreview.SubclassDlgItem       (IDC_OPTION_SOLARIZATION_STATIC_PREVIEW  , this);
}

// ダイアログパラメータを更新
void solarization_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	m_bUpdating = TRUE;
	if(bUpdate)
	{
		// ダイアログ→変数
		m_param.threshold = m_ctrlThreshold.GetPos();
		m_param.channel_red   = (m_ctrlChannelRed  .GetCheck() == BST_CHECKED) ? TRUE : FALSE;
		m_param.channel_green = (m_ctrlChannelGreen.GetCheck() == BST_CHECKED) ? TRUE : FALSE;
		m_param.channel_blue  = (m_ctrlChannelBlue .GetCheck() == BST_CHECKED) ? TRUE : FALSE;
	}
	else
	{
		// 変数→ダイアログ
		m_ctrlThreshold.SetPos(m_param.threshold);
		m_ctrlChannelRed  .SetCheck(m_param.channel_red   ? BST_CHECKED : BST_UNCHECKED);
		m_ctrlChannelGreen.SetCheck(m_param.channel_green ? BST_CHECKED : BST_UNCHECKED);
		m_ctrlChannelBlue .SetCheck(m_param.channel_blue  ? BST_CHECKED : BST_UNCHECKED);
	}
	// 閾値を出力
	SetDlgItemInt(IDC_OPTION_SOLARIZATION_STATIC_THRESHOLD, m_param.threshold, FALSE);

	m_bUpdating = FALSE;
}

// プレビューを更新
void solarization_dialog::UpdatePreview(void)
{
	// 現在の設定でソラリゼーションフィルタをかける
	UpdateData(TRUE);
	effect_solarization(&m_info_preview, &m_data_preview_before, &m_data_preview_after, &m_param, NULL, 0);

	preview_dialog::UpdatePreview();
}

// ダイアログ初期化
BOOL solarization_dialog::OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam)
{
	preview_dialog::OnInitDialog(hWndFocus, dwInitParam);

	// コントロールの初期設定
	m_ctrlThreshold.SetRange(0, 256, TRUE);
	m_ctrlPreview.SetBitmap(m_hDIBSection); // ピクチャーコントロールのビットマップをDIBセクションに設定

	UpdateData(FALSE);
	UpdatePreview();

	// ツールチップを設定
	const UINT arIDs[] =
	{
		IDC_OPTION_SOLARIZATION_SCROLL_THRESHOLD,
		IDC_OPTION_SOLARIZATION_CHANNEL_RED,
		IDC_OPTION_SOLARIZATION_CHANNEL_GREEN,
		IDC_OPTION_SOLARIZATION_CHANNEL_BLUE,
		IDC_OPTION_SOLARIZATION_STATIC_PREVIEW,
	};

	SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));

	// OKボタンにフォーカスをあわせる
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}

// 水平スクロールバー
void solarization_dialog::OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar)
{
	preview_dialog::OnHScroll(nSBCode, nPos, hWndScrollBar);

	const int nPageSize = 32;

	// スクロール情報を取得
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask  = SIF_RANGE | SIF_POS;
	::GetScrollInfo(hWndScrollBar, SB_CTL, &si);

	// 現在位置を更新
	int position = si.nPos;
	switch(nSBCode)
	{
	case SB_LEFT:  position = si.nMin; break;
	case SB_RIGHT: position = si.nMax; break;

	case SB_PAGELEFT:  position -= nPageSize; break;
	case SB_PAGERIGHT: position += nPageSize; break;

	case SB_LINELEFT:  position--; break;
	case SB_LINERIGHT: position++; break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		position = nPos;
		break;

	case SB_ENDSCROLL: break;
	}

	// スクロール位置を変更
	::SetScrollPos(hWndScrollBar, SB_CTL, position, TRUE);

	// プレビューを更新
	UpdatePreview();
}
