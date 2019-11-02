#if !defined(AFX_TRIMMINGDLG_H__7CA8C684_A4EB_49CC_836D_063A86C1255B__INCLUDED_)
#define AFX_TRIMMINGDLG_H__7CA8C684_A4EB_49CC_836D_063A86C1255B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrimmingDlg.h : �w�b�_�[ �t�@�C��
//
#include "ToolTipDialog.h"
#include "operation.h"

/////////////////////////////////////////////////////////////////////////////
// CTrimmingDlg �_�C�A���O

class CTrimmingDlg : public CToolTipDialog
{
// �R���X�g���N�V����
public:
	TRIMMINGPARAM m_param;
	UINT m_nHeight;
	UINT m_nWidth;
	CTrimmingDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTrimmingDlg)
	enum { IDD = IDD_OPERATION_TRIMMING };
	UINT	m_nClipBottom;
	UINT	m_nClipLeft;
	UINT	m_nClipRight;
	UINT	m_nClipTop;
	UINT	m_nMarginBottom;
	UINT	m_nMarginLeft;
	UINT	m_nMarginRight;
	UINT	m_nMarginTop;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTrimmingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTrimmingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TRIMMINGDLG_H__7CA8C684_A4EB_49CC_836D_063A86C1255B__INCLUDED_)
