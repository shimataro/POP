// Window.h …ウインドウのサブクラス化
#ifndef __WGC_WINDOW__
#define __WGC_WINDOW__

#include "wgc.h"
#include <map>


// from winuser.h
#ifndef WHEEL_DELTA
#define WHEEL_DELTA       120           // Value for rolling one detent
#endif // WHEEL_DELTA


_WGC_BEGIN                              // namespace wgc {

class window
{
public:
	////////////////////////////////////////////////////////////////
	// コンストラクタ／デストラクタ
	explicit window(HWND hWnd = NULL);
	virtual ~window(void);

	operator HWND(void) const;

	// トップモスト表示
	void SetTopmost(const BOOL bTopmost);

	////////////////////////////////////////////////////////////////////////////////
	// ラッパ関数

	BOOL IsWindow(void) const;
	BOOL IsChild(HWND hWnd) const;

	// GetWindowLong() / SetWindowLong() 関係
	DWORD GetStyle(void) const;
	void  SetStyle(const DWORD dwStyle);
	BOOL  ModifyStyle(const DWORD dwRemove, const DWORD dwAdd, const BOOL bRedraw = TRUE);

	// タイマ関数
	UINT SetTimer (const UINT uIDEvent, const UINT uElapse, TIMERPROC lpTimerFunc = NULL);
	BOOL KillTimer(const UINT uIDEvent);

	// 描画関数
	BOOL InvalidateRect(LPCRECT lpRect = NULL, const BOOL bErase = TRUE);
	HDC  BeginPaint(LPPAINTSTRUCT ps);
	BOOL EndPaint(const PAINTSTRUCT *ps);

	// フォーカス関数
	HWND SetFocus(void);
	BOOL SetForegroundWindow(void);

	// カレット関数
	BOOL CreateCaret(HBITMAP hBitmap, const int nWidth, const int nHeight) const;
	BOOL CreateSolidCaret(const int nWidth, const int nHeight) const;
	BOOL ShowCaret(void) const;
	BOOL HideCaret(void) const;

	// 親ウインドウ関数
	HWND GetParent(void) const;
	HWND SetParent(HWND hWndNewParent);

	// メニュー関数
	HMENU GetMenu(void) const;
	BOOL  SetMenu(HMENU hMenu);

	// フォント関数
	HFONT GetFont(void) const;
	void  SetFont(HFONT hFont, const BOOL bRedraw = TRUE);

	// スクロール関数
	int  SetHScrollInfo(const SCROLLINFO &si, const BOOL bRedraw = TRUE);
	int  SetVScrollInfo(const SCROLLINFO &si, const BOOL bRedraw = TRUE);
	BOOL GetHScrollInfo(SCROLLINFO &si);
	BOOL GetVScrollInfo(SCROLLINFO &si);

	// ウインドウ操作関数
	BOOL SetWindowText(const tstring_t &tstr);
	int  GetWindowText(tstring_t &tstr) const;
	int  GetWindowTextLength(void) const;
	BOOL SetWindowPos(HWND hWndInsertAfter, const int x, const int y, const int cx, const int cy, const UINT uFlags);
	BOOL MoveWindow(const int x, const int y, const int nWidth, int nHeight, const BOOL bRepaint = TRUE);
	BOOL MoveWindow(const RECT &rect, const BOOL bRepaint = TRUE);
	BOOL MoveWindow(const POINT &point, const SIZE &size, const BOOL bRepaint = TRUE);
	BOOL ShowWindow(const int nCmdShow);
	BOOL UpdateWindow(void);
	BOOL DestroyWindow(void);

	// ウインドウ領域関数
	BOOL GetClientRect(RECT &rRect) const;
	BOOL GetWindowRect(RECT &rRect) const;

	// メッセージ関数
	LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
	BOOL PostMessage   (UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL PostMessage   (UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
	BOOL GetMessage (LPMSG lpMsg, UINT wMsgFilterMin = 0, UINT wMsgFilterMax = 0) const;
	BOOL PeekMessage(LPMSG lpMsg, UINT wMsgFilterMin = 0, UINT wMsgFilterMax = 0, UINT wRemoveMsg = PM_REMOVE) const;
	BOOL TranslateAccelerator(HACCEL hAccTable, LPMSG lpMsg);

	// メッセージボックス関数
	int MessageBox(HINSTANCE hResource, const UINT uText, const UINT uCaption, const UINT uType = MB_OK);
	int MessageBox(const tstring_t &text, const tstring_t &caption, const UINT uType = MB_OK);

protected:
	HWND m_hWnd;                            // ウインドウハンドル

	static BOOL _SetHookTarget(window *pTargetWindow);

	// ウインドウを作成
	BOOL Create(
		LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		const DWORD dwStyle, const DWORD dwExStyle,
		window *parent,
		HMENU     hMenu     = NULL,
		HINSTANCE hInstance = NULL,
		const int x       = CW_USEDEFAULT,
		const int y       = CW_USEDEFAULT,
		const int nWidth  = CW_USEDEFAULT,
		const int nHeight = CW_USEDEFAULT,
		LPVOID lpParam = NULL);

	// サブクラス化
	BOOL Subclass(HWND hWnd);
	BOOL Unsubclass(void);

	// メッセージ関連
	typedef struct _MSGPARAM
	{
		UINT uMsg;
		WPARAM wParam;
		LPARAM lParam;
	} MSGPARAM;
	void GetMessageParam(MSGPARAM &rMsgParam) const;

	// ウインドウプロシージャ
	static  LRESULT CALLBACK _WindowProcBase(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT CALLBACK WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT _Default(void);


	////////////////////////////////////////////////////////////////////////////////
	// 仮想関数
	virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
	virtual void PostNcDestroy(void);


	////////////////////////////////////////////////////////////////////////////////
	// メッセージハンドラ
	virtual void OnGetMinMaxInfo(LPMINMAXINFO lpMMI);
	virtual UINT OnGetDlgCode(LPMSG lpMsg);

	virtual BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	virtual int  OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual void OnClose(void);
	virtual void OnDestroy(void);
	virtual void OnNcDestroy(void);

	virtual void OnSize  (const UINT nType, const int cx, const int cy);
	virtual void OnSizing(const UINT nSide, LPRECT lpRect);

	virtual void OnActivate(const UINT nState, const BOOL bMinimized, HWND hWndPrevious);
	virtual void OnSetFocus (HWND hWndLoseFocus);
	virtual void OnKillFocus(HWND hWndGetFocus);

	virtual void  OnSetFont(HFONT hFont, const BOOL bRedraw);
	virtual HFONT OnGetFont(void);

	virtual void OnInitMenu(HMENU hMenu);
	virtual void OnInitMenuPopup(HMENU hMenu, const UINT uIndex, const BOOL bSystemMenu);
	virtual void OnMenuSelect(const UINT nItemID, const UINT nFlags, HMENU hSysMenu);

	virtual void OnDropFiles(HDROP hDrop);

	virtual BOOL OnNotify(const int nIdCtrl, LPNMHDR lpNmHeader);
	virtual BOOL OnCommand(const WORD wNotifyCode, const WORD wID, HWND hWndCtrl);

	virtual UINT OnNcHitTest(const POINT &point);

	virtual void OnTimer(const UINT uIDEvent);

	virtual void OnPaint(void);
	virtual void OnNcPaint(void);
	virtual BOOL OnEraseBkgnd(HDC hDC);

	virtual HBRUSH OnCtlColorStatic(HDC hDC, HWND hWnd);

	virtual void OnParentNotify(const UINT message, const LPARAM lParam);
	virtual void OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);
	virtual void OnVScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);

	virtual BOOL OnSetCursor(HWND hWnd, const UINT nHitTest, const UINT message);

	virtual void OnKeyDown(const UINT nChar, const UINT nRepeatCount, const UINT nFlags);
	virtual void OnChar   (const UINT nChar, const UINT nRepeatCount, const UINT nFlags);

	virtual void OnImeComposition(const WORD chDBCS, const UINT nFlags);

	virtual BOOL OnMouseWheel(const UINT nFlags, const short zDelta, const POINT &point);
	virtual void OnMouseLeave(void);
	virtual void OnMouseMove     (const UINT nFlags, const POINT &point);
	virtual void OnLButtonDown   (const UINT nFlags, const POINT &point);
	virtual void OnLButtonUp     (const UINT nFlags, const POINT &point);
	virtual void OnLButtonDblClk (const UINT nFlags, const POINT &point);
	virtual void OnRButtonDown   (const UINT nFlags, const POINT &point);
	virtual void OnRButtonUp     (const UINT nFlags, const POINT &point);
	virtual void OnRButtonDblClk (const UINT nFlags, const POINT &point);
	virtual void OnMButtonDown   (const UINT nFlags, const POINT &point);
	virtual void OnMButtonUp     (const UINT nFlags, const POINT &point);
	virtual void OnMButtonDblClk (const UINT nFlags, const POINT &point);
	virtual void OnXButton1Down  (const UINT nFlags, const POINT &point);
	virtual void OnXButton1Up    (const UINT nFlags, const POINT &point);
	virtual void OnXButton1DblClk(const UINT nFlags, const POINT &point);
	virtual void OnXButton2Down  (const UINT nFlags, const POINT &point);
	virtual void OnXButton2Up    (const UINT nFlags, const POINT &point);
	virtual void OnXButton2DblClk(const UINT nFlags, const POINT &point);

private:
	// ウインドウハンドルとオブジェクトの対応マップ
	typedef std::map<HWND, window *> handlemap_t;
	typedef handlemap_t::iterator    handlemap_iterator_t;

	// ウインドウの所有権インジケータ
	// タイミング
	// 　TRUE …_MapAttach()
	// 　FALSE…_MapDetach()、コンストラクタ
	BOOL m_bOwner;

	static window *_MapFind(HWND hWnd);
	static BOOL _MapAttach(HWND hWnd, window *window_ptr);
	static HWND _MapDetach(window *window_ptr);

	// Thread Local Storage
	typedef struct _thread_data_t
	{
		int         ref_count;
		handlemap_t handlemap;

		// フック用
		HHOOK   hHook;
		window *window_ptr;
	} thread_data_t;
	static LONG  m_lRefCount;
	static DWORD m_dwTlsIndex;

	static thread_data_t *_GetThreadData(void);
	static BOOL           _SetThreadData(thread_data_t *data);

	static LRESULT CALLBACK _CBTProc(int nCode, WPARAM wParam, LPARAM lParam);

	// メッセージ関連
	MSGPARAM m_msgparam;
	WNDPROC m_lpfnOldWndProc;               // 古いウインドウプロシージャ
};

_WGC_END                                // }

#endif // __WGC_WINDOW__
