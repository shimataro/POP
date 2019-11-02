// wrench_dialog.h
#ifndef __WRENCH_DIALOG__
#define __WRENCH_DIALOG__

#include "../PreviewDialog.h"
#include "wrench.h"


class wrench_dialog : public preview_dialog
{
public:
	explicit wrench_dialog(const WRENCHPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data__ptr, HWND hWndParent /* = NULL */);
	~wrench_dialog(void);

	// �˂���p�����[�^
	WRENCHPARAM m_param;

protected:
	enum { IDD = IDD_OPTION_WRENCH };

	// �R���g���[���I�u�W�F�N�g
	wgc::scrollbar_ctrl m_ctrlCenterX;          // �˂��蒆�S��X���W
	wgc::scrollbar_ctrl m_ctrlCenterY;          // �˂��蒆�S��Y���W
	wgc::static_ctrl    m_ctrlStaticCenterX;
	wgc::static_ctrl    m_ctrlStaticCenterY;

	void AssociateDialogControls(void);
	void UpdateData(const BOOL bUpdate = TRUE);
	void UpdatePreview(void);

	BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);
	void OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);
};


/*
/////////////////////////////////////////////////////////////////////////////
// CFilterWrenchDlg �_�C�A���O

class CFilterWrenchDlg : public CToolTipDialog
{
// �R���X�g���N�V����
public:
	WRENCHDLGPARAM * m_pParam;
	_INT32 nHeight;
	_INT32 nWidth;
	CFilterWrenchDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CFilterWrenchDlg)
	enum { IDD = IDD_FILTER_WRENCH };
	CString	m_strRadius;
	CString	m_strX;
	CString	m_strY;
	int		m_nAngle;
	int		m_nRadius;
	int		m_nX;
	int		m_nY;
	CString	m_valAngle;
	CString	m_valRadius;
	CString	m_valX;
	CString	m_valY;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFilterWrenchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CFilterWrenchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
*/
#endif // __WRENCH_DIALOG__
