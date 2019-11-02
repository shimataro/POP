// Window.cpp …インプリメンテーションファイル
#include "Window.h"
#include "wgfunc.h"                     // wgc::wgfLoadString()

#include <windowsx.h>                   // GET_WM_COMMAND_ID()
#include <commctrl.h>                   // WM_MOUSELEAVE
#include <tchar.h>                      // TCHAR
#include <assert.h>                     // assert()


////////////////////////////////////////////////////////////////////////////////
// マクロ定義

////////////////////////////////////////////////////////////////////////////////
// メッセージ（from winuser.h）
#define WM_MOUSEWHEEL     0x020A        // マウスホイール回転

#define WM_XBUTTONDOWN    0x020B        // 5ボタンマウスのエクストラボタン押下
#define WM_XBUTTONUP      0x020C        // 5ボタンマウスのエクストラボタン開放
#define WM_XBUTTONDBLCLK  0x020D        // 5ボタンマウスのエクストラボタンダブルクリック
#define XBUTTON1       0x0001           // 5ボタンマウスのエクストラボタン（大）
#define XBUTTON2       0x0002           // 5ボタンマウスのエクストラボタン（小）


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// static変数
LONG  window::m_lRefCount = 0;                    // 参照カウンタ
DWORD window::m_dwTlsIndex = TLS_OUT_OF_INDEXES;  // Thread Local Storage のインデックス


////////////////////////////////////////////////////////////////////////////////
// コンストラクタ
window::window(HWND hWnd /* = NULL */)
{
	// 複数スレッドからの参照
	::InterlockedIncrement(&m_lRefCount);
	if(m_lRefCount == 1)
	{
		assert(m_dwTlsIndex == TLS_OUT_OF_INDEXES);
		m_dwTlsIndex = ::TlsAlloc();
	}

	// Thread Local Storage の作成
	thread_data_t *pThreadData = _GetThreadData();
	if(pThreadData == NULL)
	{
		// スレッド情報を格納
		pThreadData = new thread_data_t;
		pThreadData->ref_count  = 0;
		pThreadData->hHook      = NULL;
		pThreadData->window_ptr = NULL;
		_SetThreadData(pThreadData);

		// フックプロシージャをインストール
		pThreadData->hHook = ::SetWindowsHookEx(
			WH_CBT,
			_CBTProc,
			NULL,
			::GetCurrentThreadId());;
	}
	pThreadData->ref_count++;

	// メンバ変数の初期化
	m_hWnd           = hWnd;
	m_bOwner         = FALSE;
	m_lpfnOldWndProc = NULL;
}

// デストラクタ
window::~window(void)
{
	// 所有権を持ったままデストラクタが呼び出された場合、
	// ウインドウが破棄される（OnNcDestroy()が呼び出される）前に
	// ウインドウオブジェクトが破棄されることになる。
	// このような状態は、スタック上で Create() するなど、
	// オブジェクトの管理ミス以外にありえないので、
	// エラーを捕捉できるようにアサーションをかける
	assert(!m_bOwner);

	// スレッドの参照カウンタをデクリメント
	thread_data_t *pThreadData = _GetThreadData();
	assert(pThreadData != NULL);
	pThreadData->ref_count--;
	if(pThreadData->ref_count == 0)
	{
		// フックプロシージャをアンインストール
		::UnhookWindowsHookEx(pThreadData->hHook);

		// Thread Local Storage をクリア
		_SetThreadData(NULL);
		assert(pThreadData->handlemap.empty());
		delete pThreadData;
		pThreadData = NULL;
	}

	// 参照カウンタをデクリメント
	::InterlockedDecrement(&m_lRefCount);
	if(m_lRefCount == 0)
	{
		// Thread Local Storage を開放
		::TlsFree(m_dwTlsIndex);
		m_dwTlsIndex = TLS_OUT_OF_INDEXES;
	}
}


window::operator HWND(void) const
{
	return m_hWnd;
}


////////////////////////////////////////////////////////////////////////////////
// ウインドウを作成
BOOL window::Create(
	LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
	const DWORD dwStyle, const DWORD dwExStyle,
	window *parent,
	HMENU     hMenu     /* = NULL */,
	HINSTANCE hInstance /* = NULL */,
	const int x       /* = CW_USEDEFAULT */,
	const int y       /* = CW_USEDEFAULT */,
	const int nWidth  /* = CW_USEDEFAULT */,
	const int nHeight /* = CW_USEDEFAULT */,
	LPVOID lpParam /* = NULL */)
{
	// PreCreateWindow()を呼び出す
	CREATESTRUCT cs;
	cs.dwExStyle      = dwExStyle;
	cs.lpszClass      = lpszClassName;
	cs.lpszName       = lpszWindowName;
	cs.style          = dwStyle;
	cs.x              = x;
	cs.y              = y;
	cs.cx             = nWidth;
	cs.cy             = nHeight;
	cs.hwndParent     = (parent == NULL) ? NULL : parent->m_hWnd;
	cs.hMenu          = hMenu;
	cs.hInstance      = (hInstance == NULL) ? ::GetModuleHandle(NULL) : hInstance;
	cs.lpCreateParams = lpParam;
	if (!PreCreateWindow(cs))
	{
		PostNcDestroy();
		return FALSE;
	}

	// フックプロシージャをインストール（アンインストールはフックプロシージャ中で行う）
	_SetHookTarget(this);
	HWND hWnd = ::CreateWindowEx(
		cs.dwExStyle,
		cs.lpszClass, cs.lpszName,
		cs.style,
		cs.x, cs.y, cs.cx, cs.cy,
		cs.hwndParent, cs.hMenu, cs.hInstance,
		cs.lpCreateParams);

	if(hWnd == NULL)
	{
		_SetHookTarget(NULL);
		return FALSE;
	}

	assert(hWnd == m_hWnd);
	return TRUE;
}


// トップモスト表示
void window::SetTopmost(const BOOL bTopmost)
{
	// トップモスト／非トップモスト表示
	SetWindowPos(
		bTopmost ? HWND_TOPMOST : HWND_NOTOPMOST,
		0, 0, 0, 0,
		SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
}


////////////////////////////////////////////////////////////////////////////////
// 診断関数

BOOL window::IsWindow(void) const
{
	return this != NULL && ::IsWindow(m_hWnd);
}

BOOL window::IsChild(HWND hWnd) const
{
	return ::IsChild(m_hWnd, hWnd);
}


////////////////////////////////////////////////////////////////////////////////
// スタイル関数

DWORD window::GetStyle(void) const
{
	return ::GetWindowLong(m_hWnd, GWL_STYLE);
}

void window::SetStyle(const DWORD dwStyle)
{
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
}

BOOL window::ModifyStyle(const DWORD dwRemove, const DWORD dwAdd, const BOOL bRedraw /* = TRUE */)
{
	// スタイルを変更
	DWORD dwStyle = GetStyle();
	dwStyle &= ~dwRemove;
	dwStyle |= dwAdd;
	SetStyle(dwStyle);

	// 再描画
	if(bRedraw)
	{
		InvalidateRect();
		UpdateWindow();
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
// タイマ関数

UINT window::SetTimer(const UINT uIDEvent, const UINT uElapse, TIMERPROC lpTimerFunc /* = NULL */)
{
	return ::SetTimer(m_hWnd, uIDEvent, uElapse, lpTimerFunc);
}

BOOL window::KillTimer(const UINT uIDEvent)
{
	return ::KillTimer(m_hWnd, uIDEvent);
}


////////////////////////////////////////////////////////////////////////////////
// 描画関数

BOOL window::InvalidateRect(LPCRECT lpRect /* = NULL */, const BOOL bErase /* = TRUE */)
{
	return ::InvalidateRect(m_hWnd, lpRect, bErase);
}

HDC window::BeginPaint(LPPAINTSTRUCT ps)
{
	return ::BeginPaint(m_hWnd, ps);
}

BOOL window::EndPaint(const PAINTSTRUCT *ps)
{
	return ::EndPaint(m_hWnd, ps);
}


////////////////////////////////////////////////////////////////////////////////
// フォーカス関数

HWND window::SetFocus(void)
{
	return ::SetFocus(m_hWnd);
}

BOOL window::SetForegroundWindow(void)
{
	return ::SetForegroundWindow(m_hWnd);
}


////////////////////////////////////////////////////////////////////////////////
// カレット関数
BOOL window::CreateCaret(HBITMAP hBitmap, const int nWidth, const int nHeight) const
{
	return ::CreateCaret(m_hWnd, hBitmap, nWidth, nHeight);
}

BOOL window::CreateSolidCaret(const int nWidth, const int nHeight) const
{
	return CreateCaret(NULL, nWidth, nHeight);
}

BOOL window::ShowCaret(void) const
{
	return ::ShowCaret(m_hWnd);
}

BOOL window::HideCaret(void) const
{
	return ::HideCaret(m_hWnd);
}


////////////////////////////////////////////////////////////////////////////////
// 親ウインドウ関数
HWND window::GetParent(void) const
{
	return ::GetParent(m_hWnd);
}

HWND window::SetParent(HWND hWndNewParent)
{
	return ::SetParent(m_hWnd, hWndNewParent);
}


////////////////////////////////////////////////////////////////////////////////
// メニュー関数
HMENU window::GetMenu(void) const
{
	return ::GetMenu(m_hWnd);
}

BOOL window::SetMenu(HMENU hMenu)
{
	return ::SetMenu(m_hWnd, hMenu);
}


////////////////////////////////////////////////////////////////////////////////
// フォント関数
HFONT window::GetFont(void) const
{
	const LRESULT lResult = SendMessage(WM_GETFONT);
	return reinterpret_cast<HFONT>(lResult);
}

void window::SetFont(HFONT hFont, const BOOL bRedraw /* = TRUE */)
{
	SendMessage(
		WM_SETFONT,
		reinterpret_cast<WPARAM>(hFont),
		MAKELPARAM(bRedraw, 0));
}


////////////////////////////////////////////////////////////////////////////////
// スクロール関数

int window::SetHScrollInfo(const SCROLLINFO &si, const BOOL bRedraw /* = TRUE */)
{
	return ::SetScrollInfo(m_hWnd, SB_HORZ, &si, bRedraw);
}

int window::SetVScrollInfo(const SCROLLINFO &si, const BOOL bRedraw /* = TRUE */)
{
	return ::SetScrollInfo(m_hWnd, SB_VERT, &si, bRedraw);
}

BOOL window::GetHScrollInfo(SCROLLINFO &si)
{
	return ::GetScrollInfo(m_hWnd, SB_HORZ, &si);
}

BOOL window::GetVScrollInfo(SCROLLINFO &si)
{
	return ::GetScrollInfo(m_hWnd, SB_VERT, &si);
}


////////////////////////////////////////////////////////////////////////////////
// ウインドウ操作

// ウインドウにテキストを出力
BOOL window::SetWindowText(const tstring_t &tstr)
{
	return ::SetWindowText(m_hWnd, tstr.c_str());
}

int window::GetWindowText(tstring_t &tstr) const
{
	// テキストの長さだけメモリを確保
	const int size  = GetWindowTextLength() + 1;
	LPTSTR    lpStr = new TCHAR[size];

	// テキストの内容を取得
	const int result = ::GetWindowText(m_hWnd, lpStr, size);
	tstr = lpStr;

	// メモリを開放
	delete []lpStr;
	lpStr = NULL;

	return result;
}

int window::GetWindowTextLength(void) const
{
	return ::GetWindowTextLength(m_hWnd);
}


// ウインドウの位置を設定
BOOL window::SetWindowPos(HWND hWndInsertAfter, const int x, const int y, const int cx, const int cy, const UINT uFlags)
{
	return ::SetWindowPos(m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
}

BOOL window::MoveWindow(const int x, const int y, const int nWidth, const int nHeight, const BOOL bRepaint /* = TRUE */)
{
	return ::MoveWindow(m_hWnd, x, y, nWidth, nHeight, bRepaint);
}

BOOL window::MoveWindow(const RECT &rect, const BOOL bRepaint /* = TRUE */)
{
	return MoveWindow(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, bRepaint);
}

BOOL window::MoveWindow(const POINT &point, const SIZE &size, const BOOL bRepaint /* = TRUE */)
{
	return MoveWindow(point.x, point.y, size.cx, size.cy, bRepaint);
}


BOOL window::ShowWindow(const int nCmdShow)
{
	return ::ShowWindow(m_hWnd, nCmdShow);
}

BOOL window::UpdateWindow(void)
{
	return ::UpdateWindow(m_hWnd);
}

BOOL window::DestroyWindow(void)
{
	return ::DestroyWindow(m_hWnd);
}


////////////////////////////////////////////////////////////////////////////////
// ウインドウ領域関数

BOOL window::GetClientRect(RECT &rRect) const
{
	return ::GetClientRect(m_hWnd, &rRect);
}

BOOL window::GetWindowRect(RECT &rRect) const
{
	return ::GetWindowRect(m_hWnd, &rRect);
}


////////////////////////////////////////////////////////////////////////////////
// メッセージ関数

// メッセージを送信
LRESULT window::SendMessage(UINT uMsg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */)
{
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
}

LRESULT window::SendMessage(UINT uMsg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */) const
{
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
}

// メッセージをポスト
BOOL window::PostMessage(UINT uMsg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */)
{
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

BOOL window::PostMessage(UINT uMsg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */) const
{
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

// メッセージを取得
BOOL window::GetMessage(LPMSG lpMsg, UINT wMsgFilterMin /* = 0 */, UINT wMsgFilterMax /* = 0 */) const
{
	return ::GetMessage(lpMsg, m_hWnd, wMsgFilterMin, wMsgFilterMax);
}

BOOL window::PeekMessage(LPMSG lpMsg, UINT wMsgFilterMin /* = 0 */, UINT wMsgFilterMax /* = 0 */, UINT wRemoveMsg /* = PM_REMOVE */) const
{
	return ::PeekMessage(lpMsg, m_hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

BOOL window::TranslateAccelerator(HACCEL hAccTable, LPMSG lpMsg)
{
	return ::TranslateAccelerator(m_hWnd, hAccTable, lpMsg);
}


// メッセージボックス関数
int window::MessageBox(HINSTANCE hResource, const UINT uText, const UINT uCaption, const UINT uType /* = MB_OK */)
{
	tstring_t text;
	wgfLoadString(hResource, uText, text);

	tstring_t caption;
	wgfLoadString(hResource, uCaption, caption);

	return MessageBox(text, caption, uType);
}

int window::MessageBox(const tstring_t &text, const tstring_t &caption, const UINT uType /* = MB_OK */)
{
	return ::MessageBox(m_hWnd, text.c_str(), caption.c_str(), uType);
}


////////////////////////////////////////////////////////////////////////////////
// マップ関連

window *window::_MapFind(HWND hWnd)
{
	thread_data_t *data = _GetThreadData();
	assert(data != NULL);

	handlemap_iterator_t p = data->handlemap.find(hWnd);
	if(p == data->handlemap.end())
	{
		return NULL;
	}
	return p->second;
}

BOOL window::_MapAttach(HWND hWnd, window *window_ptr)
{
	assert(hWnd       != NULL);
	assert(window_ptr != NULL);

	// ハンドルとオブジェクトを対応付ける
	thread_data_t *data = _GetThreadData();
	assert(data != NULL);

	data->handlemap[hWnd] = window_ptr;
	window_ptr->m_hWnd   = hWnd;
	window_ptr->m_bOwner = TRUE;
	return TRUE;
}

HWND window::_MapDetach(window *window_ptr)
{
	assert(window_ptr != NULL);

	HWND hWndTmp = window_ptr->m_hWnd;
	window_ptr->m_hWnd   = NULL;
	window_ptr->m_bOwner = FALSE;

	thread_data_t *data = _GetThreadData();
	assert(data != NULL);

	data->handlemap.erase(hWndTmp);
	return hWndTmp;
}


////////////////////////////////////////////////////////////////////////////////
// Thread Local Storage

window::thread_data_t *window::_GetThreadData(void)
{
	if(m_dwTlsIndex == TLS_OUT_OF_INDEXES)
	{
		return NULL;
	}

	LPVOID lpData = ::TlsGetValue(m_dwTlsIndex);
	return reinterpret_cast<thread_data_t *>(lpData);
}

BOOL window::_SetThreadData(thread_data_t *data)
{
	assert(m_dwTlsIndex != TLS_OUT_OF_INDEXES);
	return ::TlsSetValue(m_dwTlsIndex, data);
}


////////////////////////////////////////////////////////////////////////////////
// フック関連

// 関連付けのターゲットウインドウを設定
BOOL window::_SetHookTarget(window *pTargetWindow)
{
	thread_data_t *data = _GetThreadData();
	if(data == NULL)
	{
		return FALSE;
	}
	data->window_ptr = pTargetWindow;
	return TRUE;
}

// フックプロシージャ
LRESULT CALLBACK window::_CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	thread_data_t *data = _GetThreadData();
	assert(data != NULL);

	// フックハンドルを取得
	HHOOK hHook = data->hHook;
	assert(hHook != NULL);

	if(nCode == HCBT_CREATEWND)
	{
		// 関連付けのターゲットウインドウを取得
		window *pTargetWindow = data->window_ptr;
		if(pTargetWindow != NULL)
		{
			HWND hWnd = reinterpret_cast<HWND>(wParam);

			LPCBT_CREATEWND lpCreateWnd = reinterpret_cast<LPCBT_CREATEWND>(lParam);
			LPCREATESTRUCT  lpCreateStruct = lpCreateWnd->lpcs;

			if(lpCreateStruct->lpszClass == WC_DIALOG)
			{
				// ダイアログボックスはアタッチのみ
				_MapAttach(hWnd, pTargetWindow);
			}
			else
			{
				// ダイアログボックス以外はサブクラス化も行う
				pTargetWindow->Subclass(hWnd);
			}

			// ターゲットウインドウの設定をはずす
			_SetHookTarget(NULL);
		}
	}

	// フックチェイン中の次のフックプロシージャをコール
	return ::CallNextHookEx(hHook, nCode, wParam, lParam);
}


////////////////////////////////////////////////////////////////////////////////
// サブクラス化

// ウインドウをサブクラス化
BOOL window::Subclass(HWND hWnd)
{
	assert(m_hWnd == NULL);
	assert(hWnd   != NULL);
	assert(m_lpfnOldWndProc == NULL);

	// マッピングに失敗したらエラー
	if(!_MapAttach(hWnd, this))
	{
		return FALSE;
	}

	// ウインドウプロシージャを置き換え
	WNDPROC lpfnOldWndProc = SubclassWindow(m_hWnd, _WindowProcBase);
	if(lpfnOldWndProc == _WindowProcBase)
	{
		// （::RegisterClass() などで）すでに _WindowProcBase() が
		// ウインドウプロシージャとして割り当てられていれば失敗
		return FALSE;
	}

	m_lpfnOldWndProc = lpfnOldWndProc;
	return TRUE;
}

// サブクラス化を解除
BOOL window::Unsubclass(void)
{
	// ウインドウプロシージャを元に戻す
	if(m_lpfnOldWndProc == NULL)
	{
		return FALSE;
	}

	// アンサブクラス化
	SubclassWindow(m_hWnd, m_lpfnOldWndProc);
	m_lpfnOldWndProc = NULL;

	// ウインドウの対応を切る
	_MapDetach(this);
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
// メッセージ関連

// メッセージパラメータを取得
void window::GetMessageParam(MSGPARAM &rMsgParam) const
{
	rMsgParam = m_msgparam;
}


////////////////////////////////////////////////////////////////////////////////
// ウインドウプロシージャ

// メッセージ振り分けプロシージャ
LRESULT CALLBACK window::_WindowProcBase(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ウインドウと関連付けられたオブジェクトを取得
	window *pTargetWindow = _MapFind(hWnd);
	assert(pTargetWindow->IsWindow());

	// メッセージを保存
	pTargetWindow->m_msgparam.uMsg   = uMsg;
	pTargetWindow->m_msgparam.wParam = wParam;
	pTargetWindow->m_msgparam.lParam = lParam;

	// オブジェクトのプロシージャにメッセージを渡す
	return pTargetWindow->WindowProc(uMsg, wParam, lParam);
}

// ウインドウプロシージャ
LRESULT window::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// マウス座標
	const POINT point = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	// メッセージによって分岐
	switch(uMsg)
	{
	case WM_GETMINMAXINFO:
		OnGetMinMaxInfo(
			reinterpret_cast<LPMINMAXINFO>(lParam));    // lpMMI
		return 0;

	case WM_GETDLGCODE:
		return OnGetDlgCode(
			reinterpret_cast<LPMSG>(lParam));           // lpMsg

	case WM_NCCREATE:
		return OnNcCreate(
			reinterpret_cast<LPCREATESTRUCT>(lParam));  // lpCreateStruct

	case WM_CREATE:
		return OnCreate(
			reinterpret_cast<LPCREATESTRUCT>(lParam));  // lpCreateStruct

	case WM_CLOSE:
		OnClose();
		return 0;                               // 処理済み

	case WM_DESTROY:
		OnDestroy();
		return 0;                               // 処理済み

	case WM_NCDESTROY:
		OnNcDestroy();
		return 0;

	case WM_SIZE:
		OnSize(
			wParam,                                 // nType
			GET_X_LPARAM(lParam),                   // cx
			GET_Y_LPARAM(lParam));                   // cy
		return 0;

	case WM_SIZING:
		OnSizing(
			wParam,                                 // nSide
			reinterpret_cast<LPRECT>(lParam));      // lpRect
		return 0;


	case WM_ACTIVATE:
		OnActivate(
			LOWORD(wParam),                         // nState
			HIWORD(wParam),                         // bMinimized
			reinterpret_cast<HWND>(lParam));        // hWndPrevious
		return 0;

	case WM_SETFOCUS:
		OnSetFocus(
			reinterpret_cast<HWND>(wParam));        // hWndLoseFocus
		return 0;

	case WM_KILLFOCUS:
		OnKillFocus(
			reinterpret_cast<HWND>(wParam));        // hWndGetFocus
		return 0;


	case WM_SETFONT:
		OnSetFont(
			reinterpret_cast<HFONT>(wParam),        // hFont
			LOWORD(lParam));                        // bRedraw
		return 0;

	case WM_GETFONT:
		return reinterpret_cast<LRESULT>(
			OnGetFont());


	case WM_INITMENU:
		OnInitMenu(
			reinterpret_cast<HMENU>(wParam));       // hMenu
		return 0;

	case WM_INITMENUPOPUP:
		OnInitMenuPopup(
			reinterpret_cast<HMENU>(wParam),        // hMenu
			LOWORD(lParam),                         // uIndex
			HIWORD(lParam));                        // bSystemMenu
		return 0;

	case WM_MENUSELECT:
		OnMenuSelect(
			GET_WM_MENUSELECT_CMD  (wParam, lParam),    // nItemID
			GET_WM_MENUSELECT_FLAGS(wParam, lParam),    // nFlags
			GET_WM_MENUSELECT_HMENU(wParam, lParam));   // hSysMenu
		return 0;

	case WM_DROPFILES:
		OnDropFiles(
			reinterpret_cast<HDROP>(wParam));       // hDrop
		return 0;

	case WM_NOTIFY:
		return OnNotify(
			wParam,                                 // nIdCtrl
			reinterpret_cast<LPNMHDR>(lParam));     // lpNmHeader

	case WM_COMMAND:
		return OnCommand(
			GET_WM_COMMAND_CMD (wParam, lParam),    // wNotifyCode
			GET_WM_COMMAND_ID  (wParam, lParam),    // wID
			GET_WM_COMMAND_HWND(wParam, lParam));   // hWndCtrl


	case WM_NCHITTEST:
		return OnNcHitTest(
			point);                                 // point


	case WM_TIMER:
		OnTimer(wParam);                            // uIDEvent
		return 0;


	case WM_PAINT:
		OnPaint();
		return 0;

	case WM_NCPAINT:
		OnNcPaint();
		return 0;

	case WM_ERASEBKGND:
		return OnEraseBkgnd(
			reinterpret_cast<HDC>(wParam));         // hDC


	case WM_CTLCOLORSTATIC:
		return reinterpret_cast<LRESULT>(
			OnCtlColorStatic(
				GET_WM_CTLCOLOR_HDC (wParam, lParam, uMsg),
				GET_WM_CTLCOLOR_HWND(wParam, lParam, uMsg)));


	case WM_PARENTNOTIFY:
		OnParentNotify(
			GET_WM_PARENTNOTIFY_MSG(wParam, lParam),// message
			lParam);                                // lParam
		return TRUE;

	case WM_HSCROLL:
		OnHScroll(
			GET_WM_HSCROLL_CODE(wParam, lParam),    // nSBCode
			GET_WM_HSCROLL_POS (wParam, lParam),    // nPos
			GET_WM_HSCROLL_HWND(wParam, lParam));   // hWndScrollBar
		return TRUE;

	case WM_VSCROLL:
		OnVScroll(
			GET_WM_VSCROLL_CODE(wParam, lParam),    // nSBCode
			GET_WM_VSCROLL_POS (wParam, lParam),    // nPos
			GET_WM_VSCROLL_HWND(wParam, lParam));   // hWndScrollBar
		return TRUE;


	////////////////////////////////////////////////////////////
	// カーソルメッセージ
	case WM_SETCURSOR:
		return OnSetCursor(
			reinterpret_cast<HWND>(wParam),
			LOWORD(lParam),
			HIWORD(lParam));


	////////////////////////////////////////////////////////////
	// キーボードメッセージ
	case WM_KEYDOWN:
		OnKeyDown(
			wParam,                             // nChar
			LOWORD(lParam),                     // nRepeatCount
			HIWORD(lParam));                    // nFlags
		return 0;

	case WM_CHAR:
		OnChar(
			wParam,                             // nChar
			LOWORD(lParam),                     // nRepeatCount
			HIWORD(lParam));                    // nFlags
		return 0;


	////////////////////////////////////////////////////////////
	// IMEメッセージ
	case WM_IME_COMPOSITION:
		OnImeComposition(
			wParam,                             // chDBCS
			lParam);                            // nFlags
		return 0;


	////////////////////////////////////////////////////////////
	// マウスメッセージ
	case WM_MOUSEWHEEL:                     // マウスホイール回転
		return OnMouseWheel(
			LOWORD(wParam),                     // nFlags
			HIWORD(wParam),                     // zDelta
			point);                             // point

	case WM_MOUSELEAVE:                     // マウス離別
		OnMouseLeave();
		return 0;

	case WM_MOUSEMOVE:                      // マウス移動
		OnMouseMove(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_LBUTTONDOWN:                    // マウスの左ボタン押下
		OnLButtonDown(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_LBUTTONUP:                      // マウスの左ボタン開放
		OnLButtonUp(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_LBUTTONDBLCLK:                  // マウスの左ボタンダブルクリック
		OnLButtonDblClk(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_RBUTTONDOWN:                    // マウスの右ボタン押下
		OnRButtonDown(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_RBUTTONUP:                      // マウスの右ボタン開放
		OnRButtonUp(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_RBUTTONDBLCLK:                  // マウスの右ボタンダブルクリック
		OnRButtonDblClk(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_MBUTTONDOWN:                    // マウスの中ボタン押下
		OnMButtonDown(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_MBUTTONUP:                      // マウスの中ボタン開放
		OnMButtonUp(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_MBUTTONDBLCLK:                  // マウスの中ボタンダブルクリック
		OnMButtonDblClk(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_XBUTTONDOWN:                    // 5ボタンマウスのボタン押下
		switch(GET_WM_COMMAND_CMD(wParam, lParam))
		{
		case XBUTTON1:
			OnXButton1Down(
				wParam,                             // nFlags
				point);                             // point
			return 0;

		case XBUTTON2:
			OnXButton2Down(
				wParam,                             // nFlags
				point);                             // point
			return 0;
		}
		break;

	case WM_XBUTTONUP:                      // 5ボタンマウスのボタン開放
		switch(GET_WM_COMMAND_CMD(wParam, lParam))
		{
		case XBUTTON1:
			OnXButton1Up(
				wParam,                             // nFlags
				point);                             // point
			return 0;

		case XBUTTON2:
			OnXButton2Up(
				wParam,                             // nFlags
				point);                             // point
			return 0;
		}
		break;

	case WM_XBUTTONDBLCLK:                  // 5ボタンマウスのボタンダブルクリック
		switch(GET_WM_COMMAND_CMD(wParam, lParam))
		{
		case XBUTTON1:
			OnXButton1DblClk(
				wParam,                             // nFlags
				point);                             // point
			return 0;

		case XBUTTON2:
			OnXButton2DblClk(
				wParam,                             // nFlags
				point);                             // point
			return 0;
		}
		break;
	}

	// デフォルトのプロシージャを呼び出す
	return _Default();
}

// メッセージのデフォルト処理
LRESULT window::_Default(void)
{
	MSGPARAM param;
	GetMessageParam(param);

	// ウインドウプロシージャが保存されている場合
	if(m_lpfnOldWndProc != NULL)
	{
		return ::CallWindowProc(
			m_lpfnOldWndProc,
			m_hWnd,
			param.uMsg,
			param.wParam,
			param.lParam);
	}

	return ::DefWindowProc(
		m_hWnd,
		param.uMsg,
		param.wParam,
		param.lParam);
}


////////////////////////////////////////////////////////////////////////////////
// 仮想関数

// ウインドウが作成される前に呼び出される
BOOL window::PreCreateWindow(CREATESTRUCT & /* cs */)
{
	return TRUE;
}

// ウインドウが破棄された後に呼び出される
void window::PostNcDestroy(void)
{
}


////////////////////////////////////////////////////////////////////////////////
// メッセージハンドラ

void window::OnGetMinMaxInfo(LPMINMAXINFO /* lpMMI */)
{
	_Default();
}

UINT window::OnGetDlgCode(LPMSG /* lpMsg */)
{
	return _Default();
}

BOOL window::OnNcCreate(LPCREATESTRUCT /* lpCreateStruct */)
{
	return _Default();
}

// ウインドウ初期化
int window::OnCreate(LPCREATESTRUCT /* lpCreateStruct */)
{
	return _Default();
}

// ウインドウを閉じる
void window::OnClose(void)
{
	_Default();
}

// ウインドウの破棄
void window::OnDestroy(void)
{
	_Default();
}

// ウインドウのノンクライアント領域の破棄（ウインドウの破棄時に最後に呼び出される）
void window::OnNcDestroy(void)
{
	_Default();
	_MapDetach(this);
	PostNcDestroy();
}


// サイズ変更
void window::OnSize(const UINT /* nType */, const int /* cx */, const int /* cy */)
{
	_Default();
}

void window::OnSizing(const UINT /* nSide */, LPRECT /* lpRect */)
{
	_Default();
}


void window::OnActivate(const UINT /* nState */, const BOOL /* bMinimized */, HWND /* hWndPrevious */)
{
	_Default();
}

// フォーカス
void window::OnSetFocus(HWND /* hWndLoseFocus */)
{
	_Default();
}

void window::OnKillFocus(HWND /* hWndGetFocus */)
{
	_Default();
}


// フォント
void window::OnSetFont(HFONT /* hFont */, const BOOL /* bRedraw */)
{
	_Default();
}

HFONT window::OnGetFont(void)
{
	const LRESULT lResult = _Default();
	return reinterpret_cast<HFONT>(lResult);
}


// メニュー
void window::OnInitMenu(HMENU /* hMenu */)
{
	_Default();
}

void window::OnInitMenuPopup(HMENU /* hMenu */, const UINT /* uIndex */, const BOOL /* bSystemMenu */)
{
	_Default();
}

void window::OnMenuSelect(const UINT /* nItemID */, const UINT /* nFlags */, HMENU /* hSysMenu */)
{
	_Default();
}


// ファイルのドロップ
void window::OnDropFiles(HDROP /* hDrop */)
{
	_Default();
}


// 通知メッセージ
BOOL window::OnNotify(const int /* nIdCtrl */, LPNMHDR /* lpNmHeader */)
{
	return _Default();
}

// コマンドメッセージ
BOOL window::OnCommand(const WORD /* wNotifyCode */, const WORD /* wID */, HWND /* hWndCtrl */)
{
	return _Default();
}


// ヒットテスト
UINT window::OnNcHitTest(const POINT & /* point */)
{
	return _Default();
}


// タイマ
void window::OnTimer(const UINT /* uIDEvent */)
{
	_Default();
}


// ウインドウ描画
void window::OnPaint(void)
{
	_Default();
}

// ウインドウの非クライアント領域を描画
void window::OnNcPaint(void)
{
	_Default();
}

// 背景消去
BOOL window::OnEraseBkgnd(HDC /* hDC */)
{
	return _Default();
}


HBRUSH window::OnCtlColorStatic(HDC /* hDC */, HWND /* hWnd */)
{
	const LRESULT lResult = _Default();
	return reinterpret_cast<HBRUSH>(lResult);
}


// 通知メッセージ
void window::OnParentNotify(const UINT /* message */, const LPARAM /* lParam */)
{
	_Default();
}

// 水平スクロールバー
void window::OnHScroll(const UINT /* nSBCode */, const UINT /* nPos */, HWND /* hWndScrollBar */)
{
	_Default();
}

// 垂直スクロールバー
void window::OnVScroll(const UINT /* nSBCode */, const UINT /* nPos */, HWND /* hWndScrollBar */)
{
	_Default();
}


////////////////////////////////////////////////////////////
// カーソルメッセージ

BOOL window::OnSetCursor(HWND /* hWnd */, const UINT /* nHitTest */, const UINT /* message */)
{
	return _Default();
}


////////////////////////////////////////////////////////////
// キーボードメッセージ

void window::OnKeyDown(const UINT /* nChar */, const UINT /* nRepCnt */, const UINT /* nFlags */)
{
	_Default();
}

// キーボード押下
void window::OnChar(const UINT /* nChar */, const UINT /* nRepCnt */, const UINT /* nFlags */)
{
	_Default();
}


////////////////////////////////////////////////////////////
// IMEメッセージ

// IME変換
void window::OnImeComposition(const WORD /* chDBCS */, const UINT /* nFlags */)
{
	_Default();
}


////////////////////////////////////////////////////////////
// マウスメッセージ

// ホイール回転
BOOL window::OnMouseWheel(const UINT /* nFlags */, const short /* zDelta */, const POINT & /* point */)
{
	return _Default();
}

// マウス離別
void window::OnMouseLeave(void)
{
	_Default();
}

// マウス移動
void window::OnMouseMove(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// マウスの左ボタン押下
void window::OnLButtonDown(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// マウスの左ボタン開放
void window::OnLButtonUp(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// マウスの左ボタンダブルクリック
void window::OnLButtonDblClk(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// マウスの右ボタン押下
void window::OnRButtonDown(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// マウスの右ボタン開放
void window::OnRButtonUp(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// マウスの右ボタンダブルクリック
void window::OnRButtonDblClk(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// マウスの中ボタン押下
void window::OnMButtonDown(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// マウスの中ボタン開放
void window::OnMButtonUp(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// マウスの中ボタンダブルクリック
void window::OnMButtonDblClk(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5ボタンマウスの大ボタン押下
void window::OnXButton1Down(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5ボタンマウスの大ボタン開放
void window::OnXButton1Up(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5ボタンマウスの大ボタンダブルクリック
void window::OnXButton1DblClk(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5ボタンマウスの小ボタン押下
void window::OnXButton2Down(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5ボタンマウスの小ボタン開放
void window::OnXButton2Up(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5ボタンマウスの小ボタンダブルクリック
void window::OnXButton2DblClk(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

_WGC_END                                // }
