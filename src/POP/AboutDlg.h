// AboutDlg.h: CAboutDlg �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABOUTDLG_H__48EA2E4F_0BFE_4A57_A8F2_21E0F0600005__INCLUDED_)
#define AFX_ABOUTDLG_H__48EA2E4F_0BFE_4A57_A8F2_21E0F0600005__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// �A�v���P�[�V�����̃o�[�W�������Ŏg���� CAboutDlg �_�C�A���O
#include "ToolTipDialog.h"
#include "HyperLink.h"	// ClassView �ɂ���Ēǉ�����܂����B

class CAboutDlg : public CToolTipDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strDescription;
	//}}AFX_DATA

	// ClassWizard ���z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CHyperLink m_ctrlHyperLink;
};

#endif // !defined(AFX_ABOUTDLG_H__48EA2E4F_0BFE_4A57_A8F2_21E0F0600005__INCLUDED_)
