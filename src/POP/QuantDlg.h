#if !defined(AFX_QUANTDLG_H__EA8AE821_C55D_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_QUANTDLG_H__EA8AE821_C55D_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuantDlg.h : �w�b�_�[ �t�@�C��
//
#include "ToolTipDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CQuantDlg �_�C�A���O

class CQuantDlg : public CToolTipDialog
{
// �R���X�g���N�V����
public:
	CQuantDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CQuantDlg)
	enum { IDD = IDD_OPERATION_QUANTIZE };
	CSpinButtonCtrl  m_spinColors;
	UINT  m_nColors;
	int    m_radioColors;
	int		m_radioDither;
	int		m_radioMethod;
	//}}AFX_DATA
	int m_nQuantColors;


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CQuantDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CQuantDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnQuantRadioColors(UINT nID);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_QUANTDLG_H__EA8AE821_C55D_11D5_8205_08004605D14C__INCLUDED_)
