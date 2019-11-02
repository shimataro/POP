#if !defined(AFX_QUANTDLG_H__EA8AE821_C55D_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_QUANTDLG_H__EA8AE821_C55D_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuantDlg.h : ヘッダー ファイル
//
#include "ToolTipDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CQuantDlg ダイアログ

class CQuantDlg : public CToolTipDialog
{
// コンストラクション
public:
	CQuantDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CQuantDlg)
	enum { IDD = IDD_OPERATION_QUANTIZE };
	CSpinButtonCtrl  m_spinColors;
	UINT  m_nColors;
	int    m_radioColors;
	int		m_radioDither;
	int		m_radioMethod;
	//}}AFX_DATA
	int m_nQuantColors;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CQuantDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CQuantDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnQuantRadioColors(UINT nID);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_QUANTDLG_H__EA8AE821_C55D_11D5_8205_08004605D14C__INCLUDED_)
