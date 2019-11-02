#if !defined(AFX_OPERATIONRESIZEDLG_H__B69714C0_36B6_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_OPERATIONRESIZEDLG_H__B69714C0_36B6_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResizeDlg.h : �w�b�_�[ �t�@�C��
//
#include "ToolTipDialog.h"
#include "operation.h"

/////////////////////////////////////////////////////////////////////////////
// CResizeDlg �_�C�A���O

class CResizeDlg : public CToolTipDialog
{
// �R���X�g���N�V����
public:
	RESIZEPARAM m_param;
	BOOL m_bIsAbleToSmooth;
	UINT nHeight;
	UINT nWidth;
	CResizeDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CResizeDlg)
	enum { IDD = IDD_OPERATION_RESIZE };
	BOOL	m_bKeepAspect;
	UINT  m_nNewWidth;
	UINT  m_nNewHeight;
	UINT  m_nNewWidthRate;
	UINT  m_nNewHeightRate;
	int    m_nInterpolate;
	int    m_nSize;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CResizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CResizeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnResizeCheckKeepaspect();
	afx_msg void OnChangeResizePercentageNewwidth();
	afx_msg void OnChangeResizePercentageNewheight();
	afx_msg void OnChangeResizePixelNewwidth();
	afx_msg void OnChangeResizePixelNewheight();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnResizeRadioSize(UINT nID);
	DECLARE_MESSAGE_MAP()

private:
	UINT nHeightRate;
	UINT nWidthRate;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_OPERATIONRESIZEDLG_H__B69714C0_36B6_11D5_8205_08004605D14C__INCLUDED_)
