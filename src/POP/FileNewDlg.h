#if !defined(AFX_FILENEWDLG_H__07E38982_643E_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_FILENEWDLG_H__07E38982_643E_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileNewDlg.h : �w�b�_�[ �t�@�C��
//
#include "ToolTipDialog.h"
#include "ColorStatic.h"
#include "operation.h"

/////////////////////////////////////////////////////////////////////////////
// FileNewDlg �_�C�A���O

class CFileNewDlg : public CToolTipDialog
{
// �R���X�g���N�V����
public:
	CFileNewDlg(NEWIMAGEPARAM &param, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CFileNewDlg)
	enum { IDD = IDD_FILE_NEW };
	CColorStatic	m_staticColor;
	UINT  m_nWidth;
	UINT  m_nHeight;
	CSpinButtonCtrl  m_spinWidth;
	CSpinButtonCtrl  m_spinHeight;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFileNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CFileNewDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	NEWIMAGEPARAM &m_param;

	void UpdateColorSample(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FILENEWDLG_H__07E38982_643E_11D5_8205_08004605D14C__INCLUDED_)
