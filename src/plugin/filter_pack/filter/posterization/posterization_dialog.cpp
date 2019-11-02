// posterization_dialog.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "posterization_dialog.h"


// コンストラクタ
posterization_dialog::posterization_dialog(const POSTERIZATIONPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: preview_dialog(IDD, info_ptr, data_ptr, hWndParent)
{
	// パラメータを保存
	m_param = param;
}

// デストラクタ
posterization_dialog::~posterization_dialog(void)
{
}

// コントロールをオブジェクトに関連付け
void posterization_dialog::AssociateDialogControls(void)
{
	m_ctrlTone     .SubclassDlgItem(IDC_OPTION_POSTERIZATION_SCROLL_TONE   , this); // ポスタリゼーションの階調
	m_ctrlToneValue.SubclassDlgItem(IDC_OPTION_POSTERIZATION_STATIC_TONE   , this);
	m_ctrlPreview  .SubclassDlgItem(IDC_OPTION_POSTERIZATION_STATIC_PREVIEW, this); // プレビュー
}

// ダイアログパラメータを更新
void posterization_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	m_bUpdating = TRUE;
	if(bUpdate)
	{
		// ダイアログ→変数
		m_param.tone = m_ctrlTone.GetPos();
	}
	else
	{
		// 変数→ダイアログ
		m_ctrlTone.SetPos(m_param.tone);
	}
	// 階調を出力
	SetDlgItemInt(IDC_OPTION_POSTERIZATION_STATIC_TONE, m_param.tone, FALSE);

	m_bUpdating = FALSE;
}

// プレビューを更新
void posterization_dialog::UpdatePreview(void)
{
	// 現在の設定でポスタリゼーションフィルタをかける
	UpdateData(TRUE);
	effect_posterization(&m_info_preview, &m_data_preview_before, &m_data_preview_after, &m_param, NULL, 0);

	preview_dialog::UpdatePreview();
}

// ダイアログ初期化
BOOL posterization_dialog::OnInitDialog(HWND hWndFocus, LPARAM dwInitParam)
{
	preview_dialog::OnInitDialog(hWndFocus, dwInitParam);

	// コントロールの初期設定
	m_ctrlTone.SetRange(POSTERIZATION_TONE_MIN, POSTERIZATION_TONE_MAX, TRUE);  // スクロールバーコントロールの設定
	m_ctrlPreview.SetBitmap(m_hDIBSection);                     // ピクチャーコントロールのビットマップをDIBセクションに設定

	UpdateData(FALSE);
	UpdatePreview();

	// ツールチップを設定
	const UINT arIDs[] =
	{
		IDC_OPTION_POSTERIZATION_SCROLL_TONE,
		IDC_OPTION_POSTERIZATION_STATIC_PREVIEW,
	};

	SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));

	// OKボタンにフォーカスをあわせる
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}

// 水平スクロールバー
void posterization_dialog::OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar)
{
	preview_dialog::OnHScroll(nSBCode, nPos, hWndScrollBar);

	const int nPageSize = 8;

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
