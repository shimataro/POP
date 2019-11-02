// MainFrm.h : CMainFrame �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BF5410AE_332D_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_MAINFRM_H__BF5410AE_332D_11D5_8205_08004605D14C__INCLUDED_

#include "ToolBarEx.h"  // ClassView �ɂ���Ēǉ�����܂����B
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	void CustomizeToolBar(const UINT nIDResource);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �R���g���[�� �o�[�p�����o
	CReBar     m_wndReBar;
	CToolBarEx m_wndToolBarStandard;
	CToolBarEx m_wndToolBarSelect;
	CToolBarEx m_wndToolBarWindow;
	CStatusBar m_wndStatusBar;

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	BOOL CreateToolBar(CToolBarEx &toolbar, const UINT nID, const UINT nIDResource, const UINT nIDCaption = 0, const DWORD dwReBarStyle = RBBS_GRIPPERALWAYS | RBBS_BREAK | RBBS_CHILDEDGE);
	BOOL CreateStatusBar(void);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnViewToolbarFix();
	afx_msg void OnViewTopmost();
	afx_msg void OnUpdateViewToolbarFix(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTopmost(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bTopmost;

	// �c�[���o�[�̃R���e�L�X�g���j���[
	CMenu m_menuContextToolBar;

	void LoadWindowPlacement(CREATESTRUCT &cs);
	void SaveWindowPlacement(void);
	void LoadState(void);
	void SaveState(void);
	void LoadCurrentDirectory(void);
	void SaveCurrentDirectory(void);
	void LoadControlBar(void);
	void SaveControlBar(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MAINFRM_H__BF5410AE_332D_11D5_8205_08004605D14C__INCLUDED_)
