#if !defined(AFX_TOOLTIPDIALOG_H__D996FCA0_7852_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_TOOLTIPDIALOG_H__D996FCA0_7852_11D5_8205_08004605D14C__INCLUDED_

#include "POP.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTipDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CToolTipDialog ウィンドウ

class CToolTipDialog : public CDialog
{
// コンストラクション
public:
	CToolTipDialog();
	CToolTipDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	CToolTipDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

// インプリメンテーション
protected:
	void SetToolTips(const UINT arIDs[], const UINT nCount);
	void SetToolTip (const UINT arID)
	{SetToolTips(&arID, 1);}

	virtual ~CToolTipDialog();
	virtual BOOL OnInitDialog();

private:
	CToolTipCtrl m_ctrlToolTip;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TOOLTIPDIALOG_H__D996FCA0_7852_11D5_8205_08004605D14C__INCLUDED_)
