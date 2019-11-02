// FileNewDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "pop.h"
#include "FileNewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileNewDlg ダイアログ


CFileNewDlg::CFileNewDlg(NEWIMAGEPARAM &param, CWnd* pParent /*=NULL*/)
  : m_param(param), CToolTipDialog(CFileNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileNewDlg)
	m_nWidth = 0;
	m_nHeight = 0;
	//}}AFX_DATA_INIT
}


void CFileNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CToolTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileNewDlg)
	DDX_Text(pDX, IDC_FILE_NEW_EDIT_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_FILE_NEW_EDIT_HEIGHT, m_nHeight);
	DDX_Control(pDX, IDC_FILE_NEW_SPIN_WIDTH, m_spinWidth);
	DDX_Control(pDX, IDC_FILE_NEW_SPIN_HEIGHT, m_spinHeight);
	DDX_Control(pDX, IDC_FILE_NEW_STATIC_COLOR, m_staticColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileNewDlg, CToolTipDialog)
	//{{AFX_MSG_MAP(CFileNewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileNewDlg メッセージ ハンドラ

BOOL CFileNewDlg::OnInitDialog() 
{
	CToolTipDialog::OnInitDialog();

	// ツールチップコントロールを設定
	if(TRUE)
	{
		const UINT nIDs[]=
		{
			IDC_FILE_NEW_EDIT_WIDTH,
			IDC_FILE_NEW_EDIT_HEIGHT,
			IDC_FILE_NEW_STATIC_COLOR,
		};
		SetToolTips(nIDs, SIZE_OF_ARRAY(nIDs));
	}

	// 初期値の設定
	if(TRUE)
	{
		m_spinWidth .SetRange(1, 512);
		m_spinHeight.SetRange(1, 512);
		m_nWidth  = m_param.width;
		m_nHeight = m_param.height;
		m_staticColor.SetColor(m_param.rgb.value);
	}

	UpdateData(FALSE);

	// デフォルトのフォーカスを使う
	return TRUE;
}

void CFileNewDlg::OnOK() 
{
	if(UpdateData(TRUE))
	{
		// 幅、高さを取得
		if(TRUE)
		{
			m_param.width  = m_nWidth;
			m_param.height = m_nHeight;
		}

		// 色を取得
		if(TRUE)
		{
			const COLORREF color = m_staticColor.GetColor();
			m_param.rgb.el.r = GetRValue(color);
			m_param.rgb.el.g = GetGValue(color);
			m_param.rgb.el.b = GetBValue(color);
		}

		CToolTipDialog::OnOK();
	}
}
