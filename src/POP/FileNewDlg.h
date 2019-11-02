#if !defined(AFX_FILENEWDLG_H__07E38982_643E_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_FILENEWDLG_H__07E38982_643E_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileNewDlg.h : ヘッダー ファイル
//
#include "ToolTipDialog.h"
#include "ColorStatic.h"
#include "operation.h"

/////////////////////////////////////////////////////////////////////////////
// FileNewDlg ダイアログ

class CFileNewDlg : public CToolTipDialog
{
// コンストラクション
public:
	CFileNewDlg(NEWIMAGEPARAM &param, CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CFileNewDlg)
	enum { IDD = IDD_FILE_NEW };
	CColorStatic	m_staticColor;
	UINT  m_nWidth;
	UINT  m_nHeight;
	CSpinButtonCtrl  m_spinWidth;
	CSpinButtonCtrl  m_spinHeight;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFileNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFileNewDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	NEWIMAGEPARAM &m_param;

	void UpdateColorSample(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FILENEWDLG_H__07E38982_643E_11D5_8205_08004605D14C__INCLUDED_)
