// ToolTipDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "ToolTipDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipDialog

CToolTipDialog::CToolTipDialog()
	: CDialog()
{
}

CToolTipDialog::CToolTipDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd /*= NULL*/)
	: CDialog(lpszTemplateName, pParentWnd)
{
}

CToolTipDialog::CToolTipDialog(UINT nIDTemplate, CWnd* pParentWnd /*=NULL*/)
	: CDialog(nIDTemplate, pParentWnd)
{
}

CToolTipDialog::~CToolTipDialog()
{
}


/////////////////////////////////////////////////////////////////////////////
// CToolTipDialog ���b�Z�[�W �n���h��

// �_�C�A���O�̏������i�c�[���`�b�v�R���g���[���̏������j
BOOL CToolTipDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// �c�[���`�b�v�R���g���[���̏�����
	m_ctrlToolTip.Create(this, TTS_ALWAYSTIP);  // �c�[���`�b�v�R���g���[�����쐬
	m_ctrlToolTip.SetMaxTipWidth(INT_MAX);      // �c�[���`�b�v�̍ő啝��ݒ�i���ꂪ�Ȃ��Ɖ��s���ł��Ȃ��j

	return TRUE;                          // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ�
}

// �c�[���`�b�v�R���g���[���̍쐬
void CToolTipDialog::SetToolTips(const UINT arIDs[], const UINT nCount)
{
	TOOLINFO ti;
	ti.cbSize   = sizeof(TOOLINFO);
	ti.uFlags   = TTF_IDISHWND | TTF_SUBCLASS;
	ti.hwnd     = m_hWnd;
	ti.hinst    = AfxGetResourceHandle();

	for(UINT i=0; i<nCount; i++)
	{
		// �R���g���[���̃E�C���h�E�A�c�[���`�b�v�������ID���擾
		HWND hWnd    = GetDlgItem(arIDs[i])->GetSafeHwnd();
		UINT nTextID = arIDs[i];

		// �_�C�A���O���L���Ȃ�c�[���`�b�v��ǉ�
		if(hWnd != NULL)
		{
			ti.uId      = reinterpret_cast<UINT>(hWnd);
			ti.lpszText = MAKEINTRESOURCE(nTextID);
			m_ctrlToolTip.SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti);
		}
	}
}