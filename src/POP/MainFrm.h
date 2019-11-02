// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BF5410AE_332D_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_MAINFRM_H__BF5410AE_332D_11D5_8205_08004605D14C__INCLUDED_

#include "ToolBarEx.h"  // ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// アトリビュート
public:

// オペレーション
public:
	void CustomizeToolBar(const UINT nIDResource);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバ
	CReBar     m_wndReBar;
	CToolBarEx m_wndToolBarStandard;
	CToolBarEx m_wndToolBarSelect;
	CToolBarEx m_wndToolBarWindow;
	CStatusBar m_wndStatusBar;

// 生成されたメッセージ マップ関数
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

	// ツールバーのコンテキストメニュー
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
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINFRM_H__BF5410AE_332D_11D5_8205_08004605D14C__INCLUDED_)
