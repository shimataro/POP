// TrimmingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "pop.h"
#include "TrimmingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrimmingDlg ダイアログ


CTrimmingDlg::CTrimmingDlg(CWnd* pParent /*=NULL*/)
	: CToolTipDialog(CTrimmingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrimmingDlg)
	m_nClipBottom = 0;
	m_nClipLeft = 0;
	m_nClipRight = 0;
	m_nClipTop = 0;
	m_nMarginBottom = 0;
	m_nMarginLeft = 0;
	m_nMarginRight = 0;
	m_nMarginTop = 0;
	//}}AFX_DATA_INIT
}


void CTrimmingDlg::DoDataExchange(CDataExchange* pDX)
{
	CToolTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrimmingDlg)
	DDX_Text(pDX, IDC_TRIMMING_EDIT_CLIP_BOTTOM, m_nClipBottom);
	DDX_Text(pDX, IDC_TRIMMING_EDIT_CLIP_LEFT, m_nClipLeft);
	DDX_Text(pDX, IDC_TRIMMING_EDIT_CLIP_RIGHT, m_nClipRight);
	DDX_Text(pDX, IDC_TRIMMING_EDIT_CLIP_TOP, m_nClipTop);
	DDX_Text(pDX, IDC_TRIMMING_EDIT_MARGIN_BOTTOM, m_nMarginBottom);
	DDX_Text(pDX, IDC_TRIMMING_EDIT_MARGIN_LEFT, m_nMarginLeft);
	DDX_Text(pDX, IDC_TRIMMING_EDIT_MARGIN_RIGHT, m_nMarginRight);
	DDX_Text(pDX, IDC_TRIMMING_EDIT_MARGIN_TOP, m_nMarginTop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrimmingDlg, CToolTipDialog)
	//{{AFX_MSG_MAP(CTrimmingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrimmingDlg メッセージ ハンドラ

BOOL CTrimmingDlg::OnInitDialog() 
{
	CToolTipDialog::OnInitDialog();
	
	// ツールチップを設定
	if(TRUE)
	{
		const UINT arToolTipIDs[]=
		{
			IDC_TRIMMING_EDIT_MARGIN_LEFT,
			IDC_TRIMMING_EDIT_MARGIN_RIGHT,
			IDC_TRIMMING_EDIT_MARGIN_TOP,
			IDC_TRIMMING_EDIT_MARGIN_BOTTOM,
			IDC_TRIMMING_EDIT_CLIP_TOP,
			IDC_TRIMMING_EDIT_CLIP_RIGHT,
			IDC_TRIMMING_EDIT_CLIP_BOTTOM,
			IDC_TRIMMING_EDIT_CLIP_LEFT, 
		};
		SetToolTips(arToolTipIDs, SIZE_OF_ARRAY(arToolTipIDs));
	}

	// 現在のサイズを初期化
	if(TRUE)
	{
		SetDlgItemInt(IDC_TRIMMING_STATIC_NOW_WIDTH , m_nWidth);
		SetDlgItemInt(IDC_TRIMMING_STATIC_NOW_HEIGHT, m_nHeight);
	}

	// スピンコントロールの上限・下限を設定
	if(TRUE)
	{
		const UINT arSpinIDs[]=
		{
			IDC_TRIMMING_SPIN_MARGIN_LEFT,
			IDC_TRIMMING_SPIN_MARGIN_RIGHT,
			IDC_TRIMMING_SPIN_MARGIN_TOP,
			IDC_TRIMMING_SPIN_MARGIN_BOTTOM,
			IDC_TRIMMING_SPIN_CLIP_TOP,
			IDC_TRIMMING_SPIN_CLIP_RIGHT,
			IDC_TRIMMING_SPIN_CLIP_BOTTOM,
			IDC_TRIMMING_SPIN_CLIP_LEFT, 
		};

		for(int i = 0; i < SIZE_OF_ARRAY(arSpinIDs); i++)
		{
			CSpinButtonCtrl ctrl;
			ctrl.Attach(GetDlgItem(arSpinIDs[i])->GetSafeHwnd());
			ctrl.SetRange32(0, 65535);
			ctrl.Detach();
		}
	}

	return FALSE;
}

// OKボタン
void CTrimmingDlg::OnOK() 
{
	if(!UpdateData(TRUE))
	{
		return;
	}

	// 値のチェック
	if(m_nClipLeft + m_nClipRight > m_nWidth)
	{
		return;
	}
	if(m_nClipTop + m_nClipBottom > m_nHeight)
	{
		return;
	}

	// パラメータをセット
	if(TRUE)
	{
		m_param.margin_left   = m_nMarginLeft;
		m_param.margin_right  = m_nMarginRight;
		m_param.margin_top    = m_nMarginTop;
		m_param.margin_bottom = m_nMarginBottom;

		m_param.clip_left   = m_nClipLeft;
		m_param.clip_right  = m_nClipRight;
		m_param.clip_top    = m_nClipTop;
		m_param.clip_bottom = m_nClipBottom;
	}

	CToolTipDialog::OnOK();
}
