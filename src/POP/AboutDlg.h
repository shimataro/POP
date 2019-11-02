// AboutDlg.h: CAboutDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABOUTDLG_H__48EA2E4F_0BFE_4A57_A8F2_21E0F0600005__INCLUDED_)
#define AFX_ABOUTDLG_H__48EA2E4F_0BFE_4A57_A8F2_21E0F0600005__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ
#include "ToolTipDialog.h"
#include "HyperLink.h"	// ClassView によって追加されました。

class CAboutDlg : public CToolTipDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strDescription;
	//}}AFX_DATA

	// ClassWizard 仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CHyperLink m_ctrlHyperLink;
};

#endif // !defined(AFX_ABOUTDLG_H__48EA2E4F_0BFE_4A57_A8F2_21E0F0600005__INCLUDED_)
