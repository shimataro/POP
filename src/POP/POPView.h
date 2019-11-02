// POPView.h : CPOPView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_POPVIEW_H__BF5410B4_332D_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_POPVIEW_H__BF5410B4_332D_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// 5ボタンマウスのサイドボタン関係
// メッセージ
#define WM_XBUTTONDOWN    0x020B        // サイドボタン押下
#define WM_XBUTTONUP      0x020C        // サイドボタン開放
#define WM_XBUTTONDBLCLK  0x020D        // サイドボタンダブルクリック
// HIWORD(wParam)
#define XBUTTON1          0x0001        // サイドボタン大
#define XBUTTON2          0x0002        // サイドボタン小
// LOWORD(wParam)
#define MK_XBUTTON1       0x0020
#define MK_XBUTTON2       0x0040


class CPOPView : public CScrollView
{
protected: // シリアライズ機能のみから作成します。
	CPOPView();
	DECLARE_DYNCREATE(CPOPView)

// アトリビュート
public:
	CPOPDoc* GetDocument();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPOPView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	virtual void OnInitialUpdate(); // 構築後の最初の１度だけ呼び出されます。
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CPOPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	LRESULT OnXButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnXButtonUp(WPARAM wParam, LPARAM lParam);

	void OnXButton1Down(UINT nFlags, CPoint point);
	void OnXButton1Up(UINT nFlags, CPoint point);
	void OnXButton2Down(UINT nFlags, CPoint point);
	void OnXButton2Up(UINT nFlags, CPoint point);

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CPOPView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	// ビューの最大サイズ
	SIZE m_sizeViewMax;

	// マウスイベントをトラッキングしているか
	BOOL m_bTrackingMouseEvent;

	// マウスボタンの状態
	BOOL m_bXButton1Down;                   // エクストラボタン1（5ボタンマウスのサイドボタン大）が押されているか
	BOOL m_bXButton2Down;                   // エクストラボタン2（5ボタンマウスのサイドボタン小）が押されているか

	// ステータスバー、ペイン
	CStatusBar* m_pStatusBar;               // ステータスバー
	int m_nIndexColor;
	int m_nIndexPoint;

	// ガンマ
	uint8_t m_gamma_table[256];             // ガンマテーブル

	// DIBセクション
	CBitmap     m_dibsection;
	BITMAPINFO  m_bitmapinfo;
	RGBQUAD    *m_buffer;

	void ResizeWindow(void);
	void UpdateStatusPaneColor(const POINT &point);
	void UpdateStatusPanePoint(const POINT &point);

	void ConvertToLogicalPoint(POINT &point);
	void ConvertToLogicalPoint(RECT  &rect);
	void ConvertToLogicalPoint(SIZE  &size);

	void ConvertToDevicePoint(POINT &point);
	void ConvertToDevicePoint(RECT  &rect);
	void ConvertToDevicePoint(SIZE  &size);
};

#ifndef _DEBUG  // POPView.cpp ファイルがデバッグ環境の時使用されます。
inline CPOPDoc* CPOPView::GetDocument()
	 { return (CPOPDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POPVIEW_H__BF5410B4_332D_11D5_8205_08004605D14C__INCLUDED_)
