#if !defined(AFX_TOOLBAREX_H__C13B4108_0476_4D61_A452_E5ACB3A4B29D__INCLUDED_)
#define AFX_TOOLBAREX_H__C13B4108_0476_4D61_A452_E5ACB3A4B29D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarEx.h : ヘッダー ファイル
//
#include <AfxTempl.h>

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx ウィンドウ

class CToolBarEx : public CToolBar
{
// コンストラクション
public:
	CToolBarEx();
	BOOL LoadToolBar(LPCTSTR lpszResourceName);
	BOOL LoadToolBar(UINT nIDResource)
	{return LoadToolBar(MAKEINTRESOURCE(nIDResource));}

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CToolBarEx)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void SetContextMenu(CMenu *pContextMenu, CWnd *pWndOwner, const BOOL bAutoDestroy = FALSE);
	void ReplaceIcons(const UINT nIDNormal, const UINT nIDDisabled, const UINT nIDHot);
	BOOL DeleteButtons(const UINT nIDDelete[], const int count);

	virtual ~CToolBarEx();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CToolBarEx)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg void OnNotifyQueryInsert(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyQueryDelete(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyBeginAdjust(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyEndAdjust(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyReset(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyToolBarChange(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyGetButtonInfo(LPNMHDR lpNotifyStruct, LRESULT *result);
	DECLARE_MESSAGE_MAP()

private:
	int m_nItemCount;                       // アイテムの総数
	int m_nButtonCount;                     // セパレータを除いたボタン数

	// コンテキストメニュー用
	CMenu *m_pContextMenu;
	CWnd  *m_pWndOwner;
	BOOL   m_bAutoDestroy;

	// カスタマイズ用
	typedef struct tagTOOLBARINFO
	{
		TBBUTTON tbButton;
		CString  strText;
	} TOOLBARINFO;
	CArray<TOOLBARINFO, TOOLBARINFO &> m_toolbarinfos;

	// 背景ブラシ用
	CBrush m_brushBackGround;

	// フルカラーイメージ用
	typedef struct tagIMAGELISTINFO
	{
		int width, height;
		UINT flags;
		COLORREF mask;
	} IMAGELISTINFO;
	CImageList m_ilToolbar, m_ilToolbarDisabled, m_ilToolbarHot;
	BOOL CreateToolbarImageList(const UINT nIDBitmap, CImageList &il);
	BOOL GetImageListInfo(CBitmap &rBitmap, IMAGELISTINFO &info);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TOOLBAREX_H__C13B4108_0476_4D61_A452_E5ACB3A4B29D__INCLUDED_)
