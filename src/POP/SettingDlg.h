#if !defined(AFX_SETTINGDLG_H__C1131BDB_73A1_4F4C_91DB_5FD0695D01D1__INCLUDED_)
#define AFX_SETTINGDLG_H__C1131BDB_73A1_4F4C_91DB_5FD0695D01D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDlg.h : ヘッダー ファイル
//
#include "ToolTipDialog.h"
#include "ColorStatic.h"
#include "POP.h"	// ClassView によって追加されました。
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg ダイアログ

class CSettingDlg : public CToolTipDialog
{
// コンストラクション
public:
	UINT     m_setting_category;
	COLORREF m_color_background1, m_color_background2;
	CString  m_resource_name;
	CSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSettingDlg)
	enum { IDD = IDD_SETTING };
	CListCtrl	m_listBasicPluginInstalled;
	CTreeCtrl	m_treeCategory;
	BOOL	m_bBasicImplantComment;
	CString	m_strBasicComment;
	BOOL	m_bBasicSaveWindowPlacement;
	UINT	m_nBasicMRUCount;
	CListCtrl	m_listDisplayLanguage;
	BOOL	m_bDisplayActivateGammaFile;
	BOOL	m_bDisplayCustomizeGammaDecode;
	double	m_dDisplayGammaDecode;
	BOOL	m_bDisplayCustomizeGammaDisplay;
	double	m_dDisplayGammaDisplay;
	int		m_nDisplayBackgroundGridSize;
	CColorStatic	m_staticDisplayBackgroundColor1;
	CColorStatic	m_staticDisplayBackgroundColor2;
	BOOL	m_bDisplayToolbarXP;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeCategory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkSettingBasicPluginListInstalled(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBasicCheckImplantComment();
	afx_msg void OnDisplayCheckCustomizeGammaDecode();
	afx_msg void OnDisplayCheckCustomizeGammaDisplay();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	// カテゴリ内のコントロールの表示/非表示
	void ShowCategoryControls(const UINT nCategoryIndex, const BOOL bShow = TRUE);

	// 各カテゴリの初期化
	void InitCategoryBasic(void);
	void InitCategoryBasicPlugin(void);
	void InitCategoryDisplay(void);
	void InitCategoryDisplayLanguage(void);

	// リストコントロールの設定
	static void ListCtrlSetHeader(CListCtrl &rListCtrl, CFont &rFont, const UINT title_array[], const int count);
	static BOOL ListCtrlSetItem(CListCtrl &rListCtrl, const int pos, const int sub_pos, LPCTSTR text);

	// 「基本」→「プラグイン」
	CFont m_fontBasicPluginTitle;
	void BasicPluginUpdateList(void);

	// 「表示」→「言語」
	CFont m_fontDisplayLanguageTitle;
	void DisplayLanguageUpdateList(void);
	int  DisplayLanguageInsertItem(const int pos);
	int  DisplayLanguageSelectItem(const CString &rString);
	BOOL DisplayLanguageGetSelectedResourceName(CString &rString);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SETTINGDLG_H__C1131BDB_73A1_4F4C_91DB_5FD0695D01D1__INCLUDED_)
