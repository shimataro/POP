#if !defined(AFX_IMPORTWINDOWDLG_H__6634E813_DBBF_4B9A_ACF3_790962F3A842__INCLUDED_)
#define AFX_IMPORTWINDOWDLG_H__6634E813_DBBF_4B9A_ACF3_790962F3A842__INCLUDED_

#include "ToolTipDialog.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportWindowDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CImportWindowDlg ダイアログ

class CImportWindowDlg : public CToolTipDialog
{
// コンストラクション
public:
	BOOL m_bAltogether;
	DWORD m_dwMilliSeconds;
	CImportWindowDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CImportWindowDlg)
	enum { IDD = IDD_IMPORT_WINDOW };
	CStatic	m_staticCrosshair;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CImportWindowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HWND m_hWndCapture;
	HCURSOR m_hCursorDefault;
	HCURSOR m_hCursorDrag;
	BOOL m_bCaptured;
	BOOL InvertFrame(HWND hWnd);
	void Restore(void);

	// 生成されたメッセージ マップ関数
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
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_IMPORTWINDOWDLG_H__6634E813_DBBF_4B9A_ACF3_790962F3A842__INCLUDED_)
