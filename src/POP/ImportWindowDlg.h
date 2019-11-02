#if !defined(AFX_IMPORTWINDOWDLG_H__6634E813_DBBF_4B9A_ACF3_790962F3A842__INCLUDED_)
#define AFX_IMPORTWINDOWDLG_H__6634E813_DBBF_4B9A_ACF3_790962F3A842__INCLUDED_

#include "ToolTipDialog.h"	// ClassView �ɂ���Ēǉ�����܂����B
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportWindowDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CImportWindowDlg �_�C�A���O

class CImportWindowDlg : public CToolTipDialog
{
// �R���X�g���N�V����
public:
	BOOL m_bAltogether;
	DWORD m_dwMilliSeconds;
	CImportWindowDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CImportWindowDlg)
	enum { IDD = IDD_IMPORT_WINDOW };
	CStatic	m_staticCrosshair;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CImportWindowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HWND m_hWndCapture;
	HCURSOR m_hCursorDefault;
	HCURSOR m_hCursorDrag;
	BOOL m_bCaptured;
	BOOL InvertFrame(HWND hWnd);
	void Restore(void);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CImportWindowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnStaticCrosshair();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_IMPORTWINDOWDLG_H__6634E813_DBBF_4B9A_ACF3_790962F3A842__INCLUDED_)
