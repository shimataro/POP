// FileImportDisplayDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "pop.h"
#include "FileImportDisplayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileImportDisplayDlg �_�C�A���O


CFileImportDisplayDlg::CFileImportDisplayDlg(CWnd* pParent /*=NULL*/)
	: CToolTipDialog(CFileImportDisplayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileImportDisplayDlg)
	m_nArea = -1;
	m_nTiming = 0;
	//}}AFX_DATA_INIT
}


void CFileImportDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CToolTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileImportDisplayDlg)
	DDX_Control(pDX, IDC_FILE_IMPORT_DISPLAY_SPIN_TIMING, m_spinTiming);
	DDX_Radio(pDX, IDC_FILE_IMPORT_DISPLAY_AREA_DESKTOP, m_nArea);
	DDX_Text(pDX, IDC_FILE_IMPORT_DISPLAY_TIMING, m_nTiming);
	DDV_MinMaxUInt(pDX, m_nTiming, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileImportDisplayDlg, CToolTipDialog)
	//{{AFX_MSG_MAP(CFileImportDisplayDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileImportDisplayDlg ���b�Z�[�W �n���h��

BOOL CFileImportDisplayDlg::OnInitDialog() 
{
	CToolTipDialog::OnInitDialog();

	// �c�[���`�b�v��ǉ�
	if(TRUE)
	{
		const UINT arIDs[] =
		{
			IDC_FILE_IMPORT_DISPLAY_AREA_DESKTOP,
			IDC_FILE_IMPORT_DISPLAY_AREA_WINDOW_ALTOGETHER,
			IDC_FILE_IMPORT_DISPLAY_AREA_WINDOW_CLIENT,
			IDC_FILE_IMPORT_DISPLAY_TIMING,
		};
		SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));
	}

	// �X�s���R���g���[���̏���E������ݒ�
	m_spinTiming.SetRange(0, 100);

	// OK�{�^���Ƀt�H�[�J�X�����킹��
	GetDlgItem(IDOK)->SetFocus();
	return FALSE;
}
