// png_dialog.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "png_dialog.h"


// コンストラクタ
png_dialog::png_dialog(const PNGPARAM &param, HWND hWndParent /* = NULL */)
	: dialog(GetInstanceHandle(), MAKEINTRESOURCE(IDD), hWndParent)
{
	m_param = param;
}

// コントロールをオブジェクトに関連付け
void png_dialog::AssociateDialogControls(void)
{
	m_ctrlCompressText .SubclassDlgItem(IDC_PNG_CHECK_COMPRESS_TEXT, this);
	m_ctrlSaveGamma    .SubclassDlgItem(IDC_PNG_CHECK_GAMMA        , this);
	m_ctrlSaveTime     .SubclassDlgItem(IDC_PNG_CHECK_TIME         , this);
	m_ctrlCompressLevel.SubclassDlgItem(IDC_PNG_SLIDER_COMPRESS    , this);
}

// ダイアログパラメータを更新
void png_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	if(bUpdate)
	{
		// ダイアログ→変数
		const UINT uCompressText = m_ctrlCompressText.GetCheck();
		const UINT uGamma        = m_ctrlSaveGamma.GetCheck();
		const UINT uTime         = m_ctrlSaveTime.GetCheck();
		const UINT nInterlace    = GetCheckedRadioButton(IDC_PNG_RADIO_INTERLACE_NONE, IDC_PNG_RADIO_INTERLACE_ADAM7);
		const UINT nFilterTypeID = GetCheckedRadioButton(IDC_PNG_RADIO_FILTER_NONE   , IDC_PNG_RADIO_FILTER_PAETH);
		const UINT nCompress     = m_ctrlCompressLevel.GetPos();

		m_param.check.elements.compress_text = (uCompressText == BST_CHECKED) ? TRUE : FALSE;
		m_param.check.elements.save_gamma    = (uGamma        == BST_CHECKED) ? TRUE : FALSE;
		m_param.check.elements.save_time     = (uTime         == BST_CHECKED) ? TRUE : FALSE;
		m_param.interlace_type = nInterlace    - IDC_PNG_RADIO_INTERLACE_NONE;
		m_param.filter_type    = nFilterTypeID - IDC_PNG_RADIO_FILTER_NONE;
		m_param.compression_level = nCompress;
	}
	else
	{
		// 変数→ダイアログ
		const UINT uCompressText = m_param.check.elements.compress_text ? BST_CHECKED : BST_UNCHECKED;
		const UINT uGamma        = m_param.check.elements.save_gamma    ? BST_CHECKED : BST_UNCHECKED;
		const UINT uTime         = m_param.check.elements.save_time     ? BST_CHECKED : BST_UNCHECKED;
		const UINT nInterlace    = m_param.interlace_type + IDC_PNG_RADIO_INTERLACE_NONE;
		const UINT nFilterTypeID = m_param.filter_type    + IDC_PNG_RADIO_FILTER_NONE;
		const UINT nCompress     = m_param.compression_level;

		m_ctrlCompressText.SetCheck(uCompressText);
		m_ctrlSaveGamma.SetCheck(uGamma);
		m_ctrlSaveTime.SetCheck(uTime);
		CheckRadioButton(IDC_PNG_RADIO_INTERLACE_NONE , IDC_PNG_RADIO_INTERLACE_ADAM7, nInterlace);
		CheckRadioButton(IDC_PNG_RADIO_FILTER_NONE, IDC_PNG_RADIO_FILTER_PAETH, nFilterTypeID);
		m_ctrlCompressLevel.SetPos(nCompress);
	}
}

// ダイアログ初期化
BOOL png_dialog::OnInitDialog(HWND hWndFocus, LPARAM dwInitParam)
{
	dialog::OnInitDialog(hWndFocus, dwInitParam);

	// 圧縮率スライダコントロールの範囲設定
	m_ctrlCompressLevel.SetRange(0, 9, TRUE);

	UpdateData(FALSE);

	// ツールチップを設定
	const UINT arID[] =
	{
		IDC_PNG_CHECK_COMPRESS_TEXT,
		IDC_PNG_CHECK_GAMMA,
		IDC_PNG_CHECK_TIME,
		IDC_PNG_RADIO_INTERLACE_NONE,
		IDC_PNG_RADIO_INTERLACE_ADAM7,
		IDC_PNG_RADIO_FILTER_NONE,
		IDC_PNG_RADIO_FILTER_SUB,
		IDC_PNG_RADIO_FILTER_UP,
		IDC_PNG_RADIO_FILTER_AVERAGE,
		IDC_PNG_RADIO_FILTER_PAETH,
		IDC_PNG_SLIDER_COMPRESS,
	};

	SetToolTips(arID, SIZE_OF_ARRAY(arID));

	// OKボタンにフォーカスをあわせる
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}
