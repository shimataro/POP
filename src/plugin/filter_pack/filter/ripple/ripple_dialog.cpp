// ripple_dialog.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "ripple_dialog.h"


// コンストラクタ
ripple_dialog::ripple_dialog(const RIPPLEPARAM_DIALOG &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: preview_dialog(IDD, info_ptr, data_ptr, hWndParent)
{
	// パラメータを保存
	m_param = param;
}

// デストラクタ
ripple_dialog::~ripple_dialog(void)
{
}

// コントロールをオブジェクトに関連付け
void ripple_dialog::AssociateDialogControls(void)
{
	m_ctrlScrollCenterX.SubclassDlgItem(IDC_OPTION_RIPPLE_SCROLL_CENTER_X, this);
	m_ctrlScrollCenterY.SubclassDlgItem(IDC_OPTION_RIPPLE_SCROLL_CENTER_Y, this);
	m_ctrlScrollAmplitude.SubclassDlgItem(IDC_OPTION_RIPPLE_SCROLL_AMPLITUDE, this);
	m_ctrlScrollCycle.SubclassDlgItem(IDC_OPTION_RIPPLE_SCROLL_CYCLE, this);
	m_ctrlScrollPhase.SubclassDlgItem(IDC_OPTION_RIPPLE_SCROLL_PHASE, this);
	m_ctrlScrollDistance.SubclassDlgItem(IDC_OPTION_RIPPLE_SCROLL_DISTANCE, this);
	m_ctrlPreview.SubclassDlgItem(IDC_OPTION_RIPPLE_STATIC_PREVIEW, this);
}

// ダイアログパラメータを更新
void ripple_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	m_bUpdating = TRUE;
	if(bUpdate)
	{
		// ダイアログ→変数
		m_param.center_x  = m_ctrlScrollCenterX.GetPos();
		m_param.center_y  = m_ctrlScrollCenterY.GetPos();
		m_param.amplitude = m_ctrlScrollAmplitude.GetPos();
		m_param.cycle     = m_ctrlScrollCycle.GetPos();
		m_param.phase     = m_ctrlScrollPhase.GetPos();
		m_param.distance  = m_ctrlScrollDistance.GetPos();
	}
	else
	{
		// 変数→ダイアログ
		m_ctrlScrollCenterX.SetPos  (m_param.center_x);
		m_ctrlScrollCenterY.SetPos  (m_param.center_y);
		m_ctrlScrollAmplitude.SetPos(m_param.amplitude);
		m_ctrlScrollCycle.SetPos    (m_param.cycle);
		m_ctrlScrollPhase.SetPos    (m_param.phase);
		m_ctrlScrollDistance.SetPos (m_param.distance);
	}
	SetDlgItemInt(IDC_OPTION_RIPPLE_STATIC_CENTER_X , m_param.center_x , FALSE);
	SetDlgItemInt(IDC_OPTION_RIPPLE_STATIC_CENTER_Y , m_param.center_y , FALSE);
	SetDlgItemInt(IDC_OPTION_RIPPLE_STATIC_AMPLITUDE, m_param.amplitude, FALSE);
	SetDlgItemInt(IDC_OPTION_RIPPLE_STATIC_CYCLE    , m_param.cycle    , FALSE);
	SetDlgItemInt(IDC_OPTION_RIPPLE_STATIC_PHASE    , m_param.phase    , FALSE);
	SetDlgItemInt(IDC_OPTION_RIPPLE_STATIC_DISTANCE , m_param.distance , FALSE);
	m_bUpdating = FALSE;
}

// プレビューを更新
void ripple_dialog::UpdatePreview(void)
{
	// 現在の設定で波紋フィルタをかける
	UpdateData(TRUE);
	effect_ripple(&m_info_preview, &m_data_preview_before, &m_data_preview_after, &m_param, NULL, 0);

	preview_dialog::UpdatePreview();
}

// ダイアログ初期化
BOOL ripple_dialog::OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam)
{
	preview_dialog::OnInitDialog(hWndFocus, dwInitParam);

	// コントロールの状態を設定
	m_ctrlScrollCenterX.SetRange  (0, 100);
	m_ctrlScrollCenterY.SetRange  (0, 100);
	m_ctrlScrollAmplitude.SetRange(0, 100);
	m_ctrlScrollCycle.SetRange    (1, 100);
	m_ctrlScrollPhase.SetRange    (0, 100);
	m_ctrlScrollDistance.SetRange (0, 100);
	m_ctrlPreview.SetBitmap(m_hDIBSection); // ピクチャーコントロールのビットマップをDIBセクションに設定

	UpdateData(FALSE);
	UpdatePreview();

	// ツールチップを設定
	const UINT arIDs[] =
	{
		IDC_OPTION_RIPPLE_SCROLL_CENTER_X,
		IDC_OPTION_RIPPLE_SCROLL_CENTER_Y,
		IDC_OPTION_RIPPLE_SCROLL_AMPLITUDE,
		IDC_OPTION_RIPPLE_SCROLL_CYCLE,
		IDC_OPTION_RIPPLE_SCROLL_PHASE,
		IDC_OPTION_RIPPLE_SCROLL_DISTANCE,
		IDC_OPTION_RIPPLE_STATIC_PREVIEW,
	};

	SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));

	// OKボタンにフォーカスをあわせる
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}

void ripple_dialog::OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar)
{
	preview_dialog::OnHScroll(nSBCode, nPos, hWndScrollBar);
	const int nPageSize = 10;

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
	UpdatePreview();
}
