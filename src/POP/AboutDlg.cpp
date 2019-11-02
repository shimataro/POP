// AboutDlg.cpp : �C���v�������e�[�V���� �t�@�C��
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
// CAboutDlg �_�C�A���O


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

	// �c�[���`�b�v��ݒ�
	if(TRUE)
	{
		const UINT arIDs[] =
		{
			IDC_ABOUT_STATIC_URL,
		};
		SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));
	}

	// �A�v���P�[�V���������L�q
	m_strDescription.LoadString(IDS_ABOUT_DESCRIPTION);
	UpdateData(FALSE);

	// URL��ݒ�
	if(TRUE)
	{
		m_ctrlHyperLink.SetURL();
		m_ctrlHyperLink.SubclassDlgItem(IDC_ABOUT_STATIC_URL, this);
	}

	// OK�{�^���Ƀt�H�[�J�X�����킹��
	GetDlgItem(IDOK)->SetFocus();
	return FALSE;
}
