// QuantDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "pop.h"
#include "QuantDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	UINT  g_nColors     = 64;
	int   g_radioMethod = 0;
	int   g_radioColors = 0;
END_NAMESPACE()


/////////////////////////////////////////////////////////////////////////////
// CQuantDlg ダイアログ


CQuantDlg::CQuantDlg(CWnd* pParent /*=NULL*/)
	: CToolTipDialog(CQuantDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuantDlg)
	m_nColors = g_nColors;
	m_radioColors = g_radioColors;
	m_radioDither = -1;
	m_radioMethod = -1;
	//}}AFX_DATA_INIT
}


void CQuantDlg::DoDataExchange(CDataExchange* pDX)
{
	CToolTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuantDlg)
	DDX_Control(pDX, IDC_QUANT_COLORS_SPIN_ASSIGN, m_spinColors);
	DDX_Text(pDX, IDC_QUANT_COLORS_EDIT_ASSIGN, m_nColors);
	DDV_MinMaxUInt(pDX, m_nColors, 1, 256);
	DDX_Radio(pDX, IDC_QUANT_COLORS_RADIO_256, m_radioColors);
	DDX_Radio(pDX, IDC_QUANT_METHOD_RADIO_FREQ, m_radioMethod);
	DDX_Radio(pDX, IDC_QUANT_DITHER_RADIO_NONE, m_radioDither);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuantDlg, CToolTipDialog)
	//{{AFX_MSG_MAP(CQuantDlg)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_QUANT_COLORS_RADIO_256, IDC_QUANT_COLORS_RADIO_ASSIGN, OnQuantRadioColors)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuantDlg メッセージ ハンドラ

BOOL CQuantDlg::OnInitDialog() 
{
	CToolTipDialog::OnInitDialog();
	
	// ツールチップを設定
	if(TRUE)
	{
		const UINT arID[]=
		{
			IDC_QUANT_COLORS_RADIO_256,
			IDC_QUANT_COLORS_RADIO_16,
			IDC_QUANT_COLORS_RADIO_8,
			IDC_QUANT_COLORS_RADIO_ASSIGN,
			IDC_QUANT_COLORS_EDIT_ASSIGN,
			IDC_QUANT_METHOD_RADIO_FREQ,
			IDC_QUANT_METHOD_RADIO_MEDIANCUT,
			IDC_QUANT_METHOD_RADIO_PADIE,
			IDC_QUANT_METHOD_RADIO_PALETTE,
			IDC_QUANT_DITHER_RADIO_NONE,
			IDC_QUANT_DITHER_RADIO_BAYER,
			IDC_QUANT_DITHER_RADIO_RANDOM,
			IDC_QUANT_DITHER_RADIO_SPREAD,
		};
		SetToolTips(arID, SIZE_OF_ARRAY(arID));
	}

	// DDX変数を初期化
	if(TRUE)
	{
		m_nColors = g_nColors;
		m_radioMethod = g_radioMethod;
		m_radioColors = g_radioColors;
		m_spinColors.SetRange(1, 256);
	}

	UpdateData(FALSE);

	OnQuantRadioColors(m_radioColors + IDC_QUANT_COLORS_RADIO_256);

	// OKボタンにフォーカスをあわせる
	GetDlgItem(IDOK)->SetFocus();
	return FALSE;
}

void CQuantDlg::OnQuantRadioColors(UINT nID) 
{
	GetDlgItem(IDC_QUANT_COLORS_EDIT_ASSIGN)->EnableWindow(nID == IDC_QUANT_COLORS_RADIO_ASSIGN);
	m_spinColors.EnableWindow(nID == IDC_QUANT_COLORS_RADIO_ASSIGN);
}

// OKボタン
void CQuantDlg::OnOK() 
{
	if(UpdateData(TRUE))
	{
		g_nColors     = m_nColors;
		g_radioMethod = m_radioMethod;
		g_radioColors = m_radioColors;

		// 減色色数を決定
		if(TRUE)
		{
			const int arColors[] = {256, 16, 8, m_nColors};
			m_nQuantColors = arColors[m_radioColors];
		}

		CToolTipDialog::OnOK();
	}
}
