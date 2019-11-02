#if !defined(AFX_FILEIMPORTDISPLAYDLG_H__AFDB77A1_287D_4E98_AAFA_58595CD498D9__INCLUDED_)
#define AFX_FILEIMPORTDISPLAYDLG_H__AFDB77A1_287D_4E98_AAFA_58595CD498D9__INCLUDED_

#include "ToolTipDialog.h"	// ClassView �ɂ���Ēǉ�����܂����B
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileImportDisplayDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CFileImportDisplayDlg �_�C�A���O

class CFileImportDisplayDlg : public CToolTipDialog
{
// �R���X�g���N�V����
public:
	CFileImportDisplayDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CFileImportDisplayDlg)
	enum { IDD = IDD_FILE_IMPORT_DISPLAY };
	CSpinButtonCtrl	m_spinTiming;
	int		m_nArea;
	UINT	m_nTiming;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFileImportDisplayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CFileImportDisplayDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FILEIMPORTDISPLAYDLG_H__AFDB77A1_287D_4E98_AAFA_58595CD498D9__INCLUDED_)
