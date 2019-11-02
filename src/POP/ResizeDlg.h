#if !defined(AFX_OPERATIONRESIZEDLG_H__B69714C0_36B6_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_OPERATIONRESIZEDLG_H__B69714C0_36B6_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResizeDlg.h : ヘッダー ファイル
//
#include "ToolTipDialog.h"
#include "operation.h"

/////////////////////////////////////////////////////////////////////////////
// CResizeDlg ダイアログ

class CResizeDlg : public CToolTipDialog
{
// コンストラクション
public:
	RESIZEPARAM m_param;
	BOOL m_bIsAbleToSmooth;
	UINT nHeight;
	UINT nWidth;
	CResizeDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
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


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CResizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
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
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPERATIONRESIZEDLG_H__B69714C0_36B6_11D5_8205_08004605D14C__INCLUDED_)
