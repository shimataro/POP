// AboutDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "pop.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg ダイアログ


CAboutDlg::CAboutDlg() : CToolTipDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strDescription = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CToolTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_ABOUT_DESCRIPTION, m_strDescription);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CToolTipDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CToolTipDialog::OnInitDialog();

	// ツールチップを設定
	if(TRUE)
	{
		const UINT arIDs[] =
		{
			IDC_ABOUT_STATIC_URL,
		};
		SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));
	}

	// アプリケーション情報を記述
	m_strDescription.LoadString(IDS_ABOUT_DESCRIPTION);
	UpdateData(FALSE);

	// URLを設定
	if(TRUE)
	{
		m_ctrlHyperLink.SetURL();
		m_ctrlHyperLink.SubclassDlgItem(IDC_ABOUT_STATIC_URL, this);
	}

	// OKボタンにフォーカスをあわせる
	GetDlgItem(IDOK)->SetFocus();
	return FALSE;
}
