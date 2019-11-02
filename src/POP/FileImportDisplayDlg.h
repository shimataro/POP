#if !defined(AFX_FILEIMPORTDISPLAYDLG_H__AFDB77A1_287D_4E98_AAFA_58595CD498D9__INCLUDED_)
#define AFX_FILEIMPORTDISPLAYDLG_H__AFDB77A1_287D_4E98_AAFA_58595CD498D9__INCLUDED_

#include "ToolTipDialog.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileImportDisplayDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CFileImportDisplayDlg ダイアログ

class CFileImportDisplayDlg : public CToolTipDialog
{
// コンストラクション
public:
	CFileImportDisplayDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CFileImportDisplayDlg)
	enum { IDD = IDD_FILE_IMPORT_DISPLAY };
	CSpinButtonCtrl	m_spinTiming;
	int		m_nArea;
	UINT	m_nTiming;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFileImportDisplayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFileImportDisplayDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FILEIMPORTDISPLAYDLG_H__AFDB77A1_287D_4E98_AAFA_58595CD498D9__INCLUDED_)
