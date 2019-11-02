#if !defined(AFX_TRIMMINGDLG_H__7CA8C684_A4EB_49CC_836D_063A86C1255B__INCLUDED_)
#define AFX_TRIMMINGDLG_H__7CA8C684_A4EB_49CC_836D_063A86C1255B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrimmingDlg.h : ヘッダー ファイル
//
#include "ToolTipDialog.h"
#include "operation.h"

/////////////////////////////////////////////////////////////////////////////
// CTrimmingDlg ダイアログ

class CTrimmingDlg : public CToolTipDialog
{
// コンストラクション
public:
	TRIMMINGPARAM m_param;
	UINT m_nHeight;
	UINT m_nWidth;
	CTrimmingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
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


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTrimmingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTrimmingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TRIMMINGDLG_H__7CA8C684_A4EB_49CC_836D_063A86C1255B__INCLUDED_)
