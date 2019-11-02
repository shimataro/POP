#if !defined(AFX_HYPERLINK_H__70B57841_38FB_4ED8_967A_3EA91A45B39F__INCLUDED_)
#define AFX_HYPERLINK_H__70B57841_38FB_4ED8_967A_3EA91A45B39F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HyperLink.h : ヘッダー ファイル
//


// 基本色
#define RGB_BLACK     RGB(  0,   0,   0)  // 黒
#define RGB_RED       RGB(255,   0,   0)  // 赤
#define RGB_GREEN     RGB(  0, 255,   0)  // 緑
#define RGB_BLUE      RGB(  0,   0, 255)  // 青
#define RGB_YELLOW    RGB(255, 255,   0)  // 黄
#define RGB_MAGENTA   RGB(255,   0, 255)  // マゼンタ
#define RGB_CYAN      RGB(  0, 255, 255)  // シアン
#define RGB_WHITE     RGB(255, 255, 255)  // 白
#define RGB_ORANGE    RGB(255, 128,   0)  // オレンジ

/////////////////////////////////////////////////////////////////////////////
// CHyperLink ウィンドウ

class CHyperLink : public CStatic
{
// コンストラクション
public:
	CHyperLink(const UINT nIDResource = 0, const COLORREF colorNormal = RGB_BLUE, const COLORREF colorPoint = RGB_ORANGE, const COLORREF colorActive = RGB_RED);

// アトリビュート
public:

// オペレーション
public:
	BOOL SetURL(const UINT nIDResource = 0);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CHyperLink)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CHyperLink();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CHyperLink)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	typedef enum tagLINKSTATE
	{
		LS_NORMAL,
		LS_HOVER,
		LS_CLICKING,
		LS_DRAGGING,
	} LINKSTATE;

	// リンク色
	COLORREF m_colorNormal;                 // 通常のリンク色
	COLORREF m_colorPoint;                  // ポイント時
	COLORREF m_colorActive;                 // クリック時

	// リソース
	HCURSOR m_hCursorFinger;
	CBrush m_brush;
	CFont m_font;

	// リンク状態関連
	LINKSTATE m_status;
	void SetStatus(const LINKSTATE status);

	// URL関連
	CString m_url;
	void JumpToURL(void);

	// OLEドラッグ＆ドロップ関連
	BOOL m_bDragging;
	void OleDragDrop(void);
	static HGLOBAL CreateGlobalData(LPCVOID pData, const int size);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_HYPERLINK_H__70B57841_38FB_4ED8_967A_3EA91A45B39F__INCLUDED_)
