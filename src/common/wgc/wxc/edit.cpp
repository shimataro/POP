// edit.cpp …インプリメンテーションファイル
// 　Windows 9x でも使用可能なUNICODE対応API ::TextOutW(), ::ExtTextOutW(), ::GetCharWidthW() を使用
#include "edit.h"
#include <windowsx.h>
#include <assert.h>                     // assert()
#include <wchar.h>                      // iswprint(), swprintf()
#include <algorithm>                    // std::max()

#include "../imm.h"                     // wgc::imm
#include "../wgfunc.h"                  // wgc::wgfMultiByteToWideChar()
#include "../../sgc/charcode.h"         // charcode_categorize_init(), charcode_categorize()


#define WXC_EDIT_CLASS  _T("wxc::edit")

#define is_keydown_control()    (::GetKeyState(VK_CONTROL) < 0)

// スクロールバー情報
#define VSCROLL_MIN     0
#define VSCROLL_MAX     65535
#define VSCROLL_PAGE    (VSCROLL_MAX - VSCROLL_MIN + 1)

// デフォルト値
#define DEFAULT_FONT    GetStockFont(ANSI_VAR_FONT)     // プロポーショナルフォント
//#define DEFAULT_FONT    GetStockFont(OEM_FIXED_FONT)    // 固定ピッチフォント

// 文字コード
#define CODE_NUL        L'\0'
#define CODE_TAB        L'\t'
#define CODE_SPACE      L' '
#define CODE_WIDESPACE  L'　'

#define SYMBOL_ARROW_DOWN   (wchar_t)7  // 半角の「↓」（UNICODEでは 0xffec）
#define SYMBOL_ARROW_UP     (wchar_t)28 // 半角の「↑」（UNICODEでは 0xffea）
#define SYMBOL_ARROW_LEFT   (wchar_t)31 // 半角の「←」（UNICODEでは 0xffe9）
#define SYMBOL_ARROW_RIGHT  (wchar_t)30 // 半角の「→」（UNICODEでは 0xffeb）
#define SYMBOL_LF           (wchar_t)27 // 改行マーク

// マーク
#define MARK_EOF            L"[EOF]"
#define MARK_EOF_LEN        5
#define MARK_WIDESPACE      L"□"
#define MARK_WIDESPACE_LEN  1
#define MARK_TAB            SYMBOL_ARROW_RIGHT
#define MARK_LF             SYMBOL_ARROW_DOWN


_WXC_BEGIN                              // namespace wxc {

const edit::text_info_t default_text_info =
{
	4,                                      // タブ幅
	1,                                      // 行間隔（≧１）

	RGB(  0,   0,   0),                     // 色
	RGB(255, 255, 255),                     // 背景色
	RGB(  0, 128, 255),                     // 特殊文字の色
	RGB(224, 224, 224),                     // 罫線の色
};

const edit::linenumber_info_t default_linenumber_info =
{
	5,                                      // 桁数
	8,                                      // 右側余白
	RGB(  0,   0,   0),                     // 色
	RGB(196, 196, 196),                     // 背景色
};

const edit::ruler_info_t default_ruler_info =
{
	8,                                      // 高さ
	10,                                     // 間隔
	RGB(  0,   0,   0),                     // 色
	RGB(196, 196, 196),                     // 背景色
};


////////////////////////////////////////////////////////////
// コンストラクション

// コンストラクタ
edit::edit(
	HCURSOR hCursorMargin /* = NULL */,
	HFONT   hFont         /* = NULL */)
{
	static BOOL bInitialized = FALSE;
	if(!bInitialized)
	{
		InitEditComponent();
		bInitialized = TRUE;
	}

	////////////////////////////////////////////////////////////
	// 論理情報の初期化

	m_ptCaretLogical.x = 0;
	m_ptCaretLogical.y = 0;
	m_nInternalX       = 0;


	////////////////////////////////////////////////////////////
	// 物理情報の初期化

	// 余白部のカーソル
	m_hCursorMargin = hCursorMargin;
	if(m_hCursorMargin == NULL)
	{
		m_hCursorMargin = ::LoadCursor(NULL, IDC_ARROW);
	}

	// フォント関連
	m_hFont = hFont;
	m_sizeFont.cx = 1;
	m_sizeFont.cy = 1;
	m_nCharWidth = new int[WCHAR_MAX - WCHAR_MIN + 1];

	m_sizeTextArea.cx = 1;
	m_sizeTextArea.cy = 1;

	m_bCaretVisible = FALSE;

	m_ptCaretPhysical.x = 0;
	m_ptCaretPhysical.y = 0;
	m_ptCaretPhysicalPrev = m_ptCaretPhysical;

	m_nColumnLeft     = 0;
	m_nColumnLeftPrev = m_nColumnLeft;

	m_nLineNumberTopPrev = 1;
	m_nLineCountPrev     = m_manager.get_line_count();

	// テキスト / 行番号 / ルーラー情報
	m_infoText       = default_text_info;
	m_infoLineNumber = default_linenumber_info;
	m_infoRuler      = default_ruler_info;

	m_dwXStyle = 0;
	m_nTextMarginLeft  = 0;
	m_nTextMarginRight = 0;
}

// デストラクタ
edit::~edit(void)
{
	delete []m_nCharWidth;
	m_nCharWidth = NULL;

	// リソースの開放
	// ・::LoadCursor() でロードしたカーソルは破棄しちゃいけないらしい
	// ・フォントの破棄義務は呼び出し側にある
	// （デフォルトではストックフォントを使用するので破棄する必要はない）
	// だから、現在は特にすることがない
}

BOOL edit::Create(
	const DWORD dwStyle, const DWORD dwExStyle,
	const DWORD dwXStyle,
	wgc::window *parent,
	const UINT nID,
	HINSTANCE hInstance /* = NULL */,
	const int x /* = CW_USEDEFAULT */,
	const int y /* = CW_USEDEFAULT */,
	const int nWidth  /* = CW_USEDEFAULT */,
	const int nHeight /* = CW_USEDEFAULT */)
{
	m_dwXStyle = dwXStyle;
	return window::Create(
		WXC_EDIT_CLASS, _T(""),
		dwStyle, dwExStyle,
		parent,
		reinterpret_cast<HMENU>(nID),
		hInstance,
		x, y, nWidth, nHeight);
}


////////////////////////////////////////////////////////////
// 環境設定

// 余白部のカーソルを設定
void edit::SetCursorMargin(HCURSOR hCursorMargin)
{
	assert(hCursorMargin != NULL);
	m_hCursorMargin = hCursorMargin;
}


////////////////////////////////////////////////////////////
// スタイル

// エディットコンポーネントの拡張スタイルを取得
DWORD edit::GetXStyle(void) const
{
	return m_dwXStyle;
}

// エディットコンポーネントの拡張スタイルを設定
DWORD edit::SetXStyle(const DWORD dwNewStyle)
{
	// 本当は GetStyle() / SetStyle() を使いたかったんだけど、
	// WS_xxx と ES_xxx でスタイルが32個近くあったから無理でした。。
	const DWORD dwOldStyle = m_dwXStyle;
	m_dwXStyle = dwNewStyle;

	UpdateTextAreaSize();
	InvalidateRect();
	return dwOldStyle;
}

// 行番号を表示するか？
BOOL edit::IsLinenumberVisible(void) const
{
	return m_dwXStyle & XES_LINENUMBER;
}

// ルーラーを表示するか？
BOOL edit::IsRulerVisible(void) const
{
	return m_dwXStyle & XES_RULER;
}

// カーソル位置に下線を引くか？
BOOL edit::IsCursorUnderline(void) const
{
	return m_dwXStyle & XES_CURSORUNDERLINE;
}

// 読み込み専用モードか？
BOOL edit::IsReadOnly(void) const
{
	return GetStyle() & ES_READONLY;
}


// 操作

// ダーティフラグを取得
BOOL edit::GetModify(void) const
{
	return SendMessage(EM_GETMODIFY);
}

// ダーティフラグを設定
void edit::SetModify(const BOOL bModified /* = TRUE */)
{
	SendMessage(EM_SETMODIFY, bModified);
}

// 読み取り専用属性の付加/解除
BOOL edit::SetReadOnly(const BOOL bReadOnly /* = TRUE */)
{
	return SendMessage(EM_SETREADONLY, bReadOnly);
}

void edit::SetMargins(const UINT nLeft, const UINT nRight)
{
	SendMessage(
		EM_SETMARGINS,
		EC_LEFTMARGIN | EC_RIGHTMARGIN,
		MAKELONG(nLeft, nRight));
}

DWORD edit::GetMargins(void) const
{
	return SendMessage(EM_GETMARGINS);
}

// バッファの行数を取得
int edit::GetLineCount(void) const
{
	return SendMessage(EM_GETLINECOUNT);
}

// アンドゥ
BOOL edit::Undo(void)
{
	return SendMessage(EM_UNDO);
}

// アンドゥできるか？
BOOL edit::CanUndo(void) const
{
	return SendMessage(EM_CANUNDO);
}

// リドゥ
BOOL edit::Redo(void)
{
	return SendMessage(EM_REDO);
}

// リドゥできるか？
BOOL edit::CanRedo(void) const
{
	return SendMessage(EM_CANREDO);
}

// 選択範囲を切り取り、クリップボードにコピー
void edit::Cut(void)
{
	SendMessage(WM_CUT);
}

// 選択範囲をクリップボードにコピー
void edit::Copy(void)
{
	SendMessage(WM_COPY);
}

// 選択範囲を削除
void edit::Clear(void)
{
	SendMessage(WM_CLEAR);
}

// クリップボード内のデータを貼り付け
void edit::Paste(void)
{
	SendMessage(WM_PASTE);
}

// 貼り付けできるか？
BOOL edit::CanPaste(const UINT uFormat /* = 0 */) const
{
	return SendMessage(EM_CANPASTE, uFormat);
}


// バッファの先頭を示すイテレータを取得
edit::const_iterator_t edit::GetIteratorBegin(void) const
{
	return m_manager.get_iterator_begin();
}

// バッファの末尾を示すイテレータを取得
edit::const_iterator_t edit::GetIteratorEnd(void) const
{
	return m_manager.get_iterator_end();
}

// 現在のバッファを破棄して指定バッファを入れる
void edit::SetData(const wchar_t *wstr)
{
	m_manager.set_data(wstr);

	_SyncLineCount();

	m_ptCaretLogical.x = 0;
	m_ptCaretLogical.y = 0;

	LogicalToPhysical();
	EnsureVisible();
	UpdateClient(1);
	MoveCaret();
}

// バッファを空にする
void edit::SetEmpty(void)
{
	m_manager.set_empty();

	_SyncLineCount();

	m_ptCaretLogical.x = 0;
	m_ptCaretLogical.y = 0;

	LogicalToPhysical();
	EnsureVisible();
	UpdateClient(1);
	MoveCaret();
}


////////////////////////////////////////////////////////////////////////////////
// protected 関数


////////////////////////////////////////////////////////////
// コンポーネントの初期化
BOOL edit::InitEditComponent(void)
{
	HINSTANCE hInstance = ::GetModuleHandle(NULL);
	HCURSOR   hCursor   = ::LoadCursor(NULL, IDC_IBEAM);

	// ウインドウクラスの登録
	WNDCLASS wc;
	wc.style         = CS_DBLCLKS | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
	wc.lpfnWndProc   = _WindowProcBase;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = NULL;
	wc.hCursor       = hCursor;
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = WXC_EDIT_CLASS;
	if(::RegisterClass(&wc) == 0)
	{
		return FALSE;
	}

	// 文字分類テーブルの初期化
	charcode_categorize_init();
	return TRUE;
}


////////////////////////////////////////////////////////////
// 論理/物理ルーチン

// 物理カレット位置→論理カレット位置
BOOL edit::PhysicalToLogical(void)
{
	const int linenumber_top = m_manager.get_linenumber_top();

	m_manager.set_position(linenumber_top + m_ptCaretPhysical.y, false, true);
	const_iterator_t pos_now = m_manager.get_iterator_now();

	m_ptCaretLogical.x = CalcLogicalLength(pos_now->c_str(), m_ptCaretPhysical.x + m_nColumnLeft * m_sizeFont.cx);
	m_ptCaretLogical.y = m_manager.get_linenumber_now() - 1;
	m_nInternalX = CalcPhysicalLength(pos_now->c_str(), m_ptCaretLogical.x);

	return TRUE;
}

// 論理カレット位置→物理カレット位置
BOOL edit::LogicalToPhysical(const BOOL bUpdateInternalX /* = TRUE */)
{
	const int linenumber_top = m_manager.get_linenumber_top();
	const_iterator_t pos_now = m_manager.get_iterator_now();

	// カーソルを移動
	m_ptCaretPhysical.x = CalcPhysicalLength(pos_now->c_str(), m_ptCaretLogical.x) - m_nColumnLeft * m_sizeFont.cx;
	m_ptCaretPhysical.y = m_ptCaretLogical.y - (linenumber_top - 1);

	if(bUpdateInternalX)
	{
		m_nInternalX = m_ptCaretPhysical.x;
	}

	return FALSE;
}


// 文字コード code の幅をキャラクタ単位で計算
int edit::CalcCodeLength(const wchar_t code) const
{
	// それ以外
	return m_nCharWidth[WCHAR_MIN + code];
}

// cols 桁目の位置にあるタブ幅をで計算
int edit::CalcTabLength(const int cols) const
{
	const int tabsize = m_infoText.nTabSize * m_sizeFont.cx;
	return tabsize - (cols % tabsize);
}


// 文字列 wstr の物理長を求める
int edit::CalcPhysicalLength(const wstring_t &wstr) const
{
	return CalcPhysicalLength(wstr.c_str(), wstr.length());
}

// 論理長 length の文字列 wstr の物理長を求める
int edit::CalcPhysicalLength(LPCWSTR wstr, const int length) const
{
	int pos = 0;
	for(int i = 0; i < length; i++)
	{
		const wchar_t code = wstr[i];
		if(code == CODE_TAB)
		{
			pos += CalcTabLength(pos);
		}
		else
		{
			pos += CalcCodeLength(code);
		}
	}
	return pos;
}

// 物理長 length を超えない文字列 wstr の最大位置を求める
int edit::CalcLogicalLength(LPCWSTR wstr, const int length) const
{
	int pos = 0;
	int i = 0;
	for(i = 0; wstr[i] != CODE_NUL; i++)
	{
		const wchar_t code = wstr[i];
		if(code == CODE_TAB)
		{
			pos += CalcTabLength(pos);
		}
		else
		{
			pos += CalcCodeLength(code);
		}

		// lengthを超えたら、その時点の位置を返す
		if(pos > length)
		{
			break;
		}
	}
	return i;
}

// 文字列 wstr 中で、pos に最も近い物理長を求める
int edit::CalcNearestPos(LPCWSTR wstr, const int pos) const
{
	int nearest_pos = 0;
	while(*wstr != CODE_NUL)
	{
		const wchar_t code = *wstr++;
		const int length = (code == CODE_TAB) ? CalcTabLength(nearest_pos) : CalcCodeLength(code);

		if(pos <= nearest_pos + (length + 1) / 2)
		{
			break;
		}
		nearest_pos += length;
	}
	return nearest_pos;
}


////////////////////////////////////////////////////////////
// スクロール

// columns 桁水平スクロール
void edit::ScrollHorz(const int columns, const BOOL bPhysical, const BOOL bLogical)
{
	const int dx = columns * m_sizeFont.cx;
	int total = 0;

	// 物理スクロール
	if(bPhysical)
	{
		m_nColumnLeft += columns;
		total -= dx;
	}

	// 論理スクロール
	if(bLogical)
	{
	}

	m_ptCaretPhysical.x += total;
	if(bLogical)
	{
		LogicalToPhysical(FALSE);
	}
	if(bPhysical)
	{
	}
}

// lines 行垂直スクロール
void edit::ScrollVert(const int dy, const BOOL bPhysical, const BOOL bLogical)
{
	int total = 0;

	// 物理スクロール
	if(bPhysical)
	{
		const int count = m_manager.scroll_top(dy);
		total -= count;
	}

	// 論理スクロール
	if(bLogical)
	{
		const int count = m_manager.scroll_now(dy);

		const_iterator_t pos_now = m_manager.get_iterator_now();
		m_ptCaretLogical.x  = CalcLogicalLength(pos_now->c_str(), m_nInternalX);
		m_ptCaretLogical.y += count;
		total += count;
	}

	m_ptCaretPhysical.y += total;
	if(bLogical)
	{
		LogicalToPhysical(FALSE);
	}
	if(bPhysical)
	{
		_SyncLineNumberTop();
	}
}


////////////////////////////////////////////////////////////
// 論理ルーチン

// 行番号の桁数を拡張
void edit::ExpandLineNumberCols(void)
{
	// 現在の桁数を数える
	int cols = 0;
	int line_count = m_manager.get_line_count();
	while(line_count > 0)
	{
		line_count /= 10;
		cols++;
	}

	// 設定してある桁数より大きければ拡張
	if(m_infoLineNumber.nCols < cols)
	{
		m_infoLineNumber.nCols = cols;
		InvalidateRect();
	}
}


////////////////////////////////////////////////////////////
// 物理ルーチン

// ヒットテスト
edit::client_hittest_t edit::ClientHitTest(const POINT &pos_display) const
{
	// 余白領域
	if(pos_display.x < CalcMarginLeft())
	{
		return CHT_MARGIN;
	}
	return CHT_TEXT;
}

// テキスト領域のサイズ（カレット単位）の更新
void edit::UpdateTextAreaSize(void)
{
	RECT rect;
	CalcRectText(rect, FALSE);

	const int cx = rect.right - rect.left;
	const int cy = (rect.bottom - rect.top) / (m_sizeFont.cy + m_infoText.nLineSpace);
	_SetTextAreaSize(
		(cx > 0) ? cx : 1,
		(cy > 0) ? cy : 1);
//	OnSyncTextAreaSize();
}


// 水平スクロールバーを同期
void edit::SyncHScroll(void)
{
}

// 垂直スクロールバーを同期
void edit::SyncVScroll(void)
{
	const LONG cy = m_sizeTextArea.cy;
	assert(cy > 0);

	const int line_count     = m_manager.get_line_count();
	const int linenumber_top = m_manager.get_linenumber_top();
	assert(line_count     > 0);
	assert(linenumber_top > 0);

	// [block]
	// スクロールバーのページサイズと位置を更新
	{
		SCROLLINFO si = {sizeof(si)};
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_DISABLENOSCROLL;
		si.nMin  = 1;
		si.nMax  = line_count + cy - 1;
		si.nPage = cy;
		si.nPos  = linenumber_top;
		SetVScrollInfo(si);
	}
}


// 変更を反映
void edit::UpdateClient(const int linenumber_modify)
{
	const int linecount      = m_manager.get_line_count();
	const int linenumber_top = m_manager.get_linenumber_top();

	int  linenumber_redraw  = linenumber_modify;
	BOOL bLinenumber        = FALSE;
	BOOL bRuler             = FALSE;

	// 画面左端の桁が変わっていたらテキスト領域（全部）とルーラーを再描画
	if(m_nColumnLeftPrev != m_nColumnLeft)
	{
		m_nColumnLeftPrev = m_nColumnLeft;
		linenumber_redraw = 1;
		bRuler = TRUE;
	}

	// 画面上端の行番号が変わっていたらテキスト領域（全部）と行番号を再描画
	if(linenumber_top != m_nLineNumberTopPrev)
	{
		m_nLineNumberTopPrev = linenumber_top;
		linenumber_redraw = 1;
		bLinenumber = TRUE;
	}

	// 行数が変更されていたらテキスト領域（変更分）と行番号を再描画
	if(linecount != m_nLineCountPrev)
	{
		m_nLineCountPrev = linecount;
		bLinenumber = TRUE;
	}

	// クライアント領域を描画
	if(linenumber_redraw > 0 || bLinenumber || bRuler)
	{
		wgc::devicecontext_client dc(m_hWnd);
		PaintClient(dc, linenumber_redraw, bLinenumber, bRuler);
	}
}

// カレットを画面内に表示
BOOL edit::EnsureVisible(const BOOL bByScroll /* = TRUE */)
{
	BOOL bMoved = FALSE;

	// 画面をスクロール
	if(bByScroll)
	{
		if(m_ptCaretPhysical.x < 0)
		{
			const int dx      = m_ptCaretPhysical.x;
			const int columns = (dx - (m_sizeFont.cx - 1)) / m_sizeFont.cx;
			ScrollHorz(columns, TRUE, FALSE);
			bMoved = TRUE;
		}
		if(m_ptCaretPhysical.x >= m_sizeTextArea.cx - 1)
		{
			const int dx      = m_ptCaretPhysical.x - (m_sizeTextArea.cx - 1);
			const int columns = (dx + (m_sizeFont.cx - 1)) / m_sizeFont.cx;
			ScrollHorz(columns, TRUE, FALSE);
			bMoved = TRUE;
		}

		if(m_ptCaretPhysical.y < 0)
		{
			ScrollVert(m_ptCaretPhysical.y, TRUE, FALSE);
			assert(m_ptCaretPhysical.y == 0);
			bMoved = TRUE;
		}
		if(m_ptCaretPhysical.y > m_sizeTextArea.cy - 1)
		{
			ScrollVert(m_ptCaretPhysical.y - (m_sizeTextArea.cy - 1), TRUE, FALSE);
			assert(m_ptCaretPhysical.y == m_sizeTextArea.cy - 1);
			bMoved = TRUE;
		}
	}
	// カーソルを移動
	else
	{
		if(m_ptCaretPhysical.y < 0)
		{
			m_ptCaretPhysical.y = 0;
			bMoved = TRUE;
		}
		if(m_ptCaretPhysical.y > m_sizeTextArea.cy - 1)
		{
			m_ptCaretPhysical.y = m_sizeTextArea.cy - 1;
			bMoved = TRUE;
		}

		if(m_ptCaretPhysical.x < 0)
		{
			m_ptCaretPhysical.x = 0;
			bMoved = TRUE;
		}
		if(m_ptCaretPhysical.x > m_sizeTextArea.cx)
		{
			m_ptCaretPhysical.x = m_sizeTextArea.cx;
			bMoved = TRUE;
		}

		if(bMoved)
		{
			PhysicalToLogical();
			LogicalToPhysical(FALSE);
		}
	}
	return bMoved;
}

// カレット位置、現在行を修正
void edit::AdjustPosition(void)
{
	const int line_count     = m_manager.get_line_count();
	const int linenumber_top = m_manager.get_linenumber_top();

	// Y座標のチェック（ファイルの末尾を越えていないか）
	if(m_ptCaretPhysical.y + linenumber_top > line_count)
	{
		// カーソル位置を修正
		m_ptCaretPhysical.y = line_count - linenumber_top;

		m_manager.set_position(0, false, true);
	}

	// X座標のチェック（行末を越えていないか）
	const_iterator_t pos_now = m_manager.get_iterator_now();

	const int length = CalcPhysicalLength(*pos_now);
	if(m_ptCaretPhysical.x > length)
	{
		m_ptCaretPhysical.x = length;
	}
}


////////////////////////////////////////////////////////////
// 領域計算ルーチン

// テキストの左側余白を計算
int edit::CalcMarginLeft(const BOOL bAddTextMargin /* = TRUE */) const
{
	int nMargin = 0;
	if(bAddTextMargin)
	{
		nMargin += m_nTextMarginLeft;
	}
	if(IsLinenumberVisible())
	{
		nMargin += m_infoLineNumber.nCols * m_sizeFont.cx + m_infoLineNumber.nMargin;
	}

	return nMargin;
}

// テキストの上側余白を計算
int edit::CalcMarginTop(void) const
{
	int nMargin = 0;
	if(IsRulerVisible())
	{
		nMargin += m_infoRuler.nHeight;
	}

	return nMargin;
}

// 行番号領域
void edit::CalcRectLineNumber(RECT &rRect) const
{
	GetClientRect(rRect);
	rRect.top   = CalcMarginTop();
	rRect.right = m_infoLineNumber.nCols * m_sizeFont.cx + m_infoLineNumber.nMargin;
}

// ルーラー領域
void edit::CalcRectRuler(RECT &rRect) const
{
	GetClientRect(rRect);
	rRect.bottom = CalcMarginTop();
}

// テキスト領域
void edit::CalcRectText(RECT &rRect, const BOOL bIncludeMargin /* = TRUE */) const
{
	// 余白領域を含むテキスト領域を取得
	GetClientRect(rRect);
	rRect.left = CalcMarginLeft(FALSE);
	rRect.top  = CalcMarginTop();

	// 余白領域を除く
	if(!bIncludeMargin)
	{
		rRect.left  += m_nTextMarginLeft;
		rRect.right -= m_nTextMarginRight;
	}
}


////////////////////////////////////////////////////////////
// カレットルーチン

// 参照カウンタを使わずにカレットの表示/非表示を切り替え
void edit::DisplayCaret(const BOOL bShow /* = TRUE */)
{
	// ShowCaret() でカレットを表示するには、
	// HideCaret() を呼び出した回数と同じだけ呼び出す必要がある。
	// DisplayCaret() を使えばその必要がなくなる。

	// 表示
	if(bShow)
	{
		// 現在不可視なら表示
		if(!m_bCaretVisible)
		{
			m_bCaretVisible = TRUE;
			ShowCaret();
		}
	}
	// 非表示
	else
	{
		// 現在可視なら非表示
		if(m_bCaretVisible)
		{
			m_bCaretVisible = FALSE;
			HideCaret();
		}
	}
}

// カレット位置からクライアント座標を計算
void edit::CalcDisplayPos(const POINT &pos_caret, POINT &pos_display) const
{
	// 基本座標
	pos_display.x = pos_caret.x;
	pos_display.y = pos_caret.y * (m_sizeFont.cy + m_infoText.nLineSpace);

	// 余白部を考慮
	pos_display.x += CalcMarginLeft();
	pos_display.y += CalcMarginTop();
}

// クライアント座標からカレット位置を計算
void edit::CalcCaretPos(const POINT &pos_display, POINT &pos_caret) const
{
	pos_caret.x = pos_display.x - CalcMarginLeft();
	if(pos_caret.x < 0) { pos_caret.x = 0; } else { pos_caret.x += m_nColumnLeft * m_sizeFont.cx; }

	pos_caret.y = pos_display.y - CalcMarginTop();
	if(pos_caret.y < 0) { pos_caret.y = 0; }

	// y座標をキャラクタ単位に補正
	pos_caret.y /= (m_sizeFont.cy + m_infoText.nLineSpace);

	// 指定位置のイテレータを取得
	sgc::edit_manager::const_iterator_t p = m_manager.get_iterator_top();
	for(int i = 0; i < pos_caret.y; i++)
	{
		sgc::edit_manager::const_iterator_t tmp = p;
		if(++tmp == m_manager.get_iterator_end())
		{
			pos_caret.y = i;
			break;
		}
		p = tmp;
	}

	// カレット座標を計算
	pos_caret.x = CalcNearestPos(p->c_str(), pos_caret.x) - m_nColumnLeft * m_sizeFont.cx;
}

// カレットを移動
void edit::MoveCaret(void)
{
	// テキストエリアの範囲内
	if(0 <= m_ptCaretPhysical.x && m_ptCaretPhysical.x <= m_sizeTextArea.cx
	&& 0 <= m_ptCaretPhysical.y && m_ptCaretPhysical.y <= m_sizeTextArea.cy)
	{
		// カレットを表示
		DisplayCaret();

		// カレットを移動
		POINT pos_display;
		CalcDisplayPos(m_ptCaretPhysical, pos_display);
		::SetCaretPos(pos_display.x, pos_display.y);

		// [block]
		// IMEのカレット座標を設定
		{
			wgc::imm immcontext(m_hWnd);
			immcontext.SetPos(pos_display);
		}
	}
	else
	{
		// カレットを非表示
		DisplayCaret(FALSE);
	}

	// 罫線を描画
	if(IsCursorUnderline() && ::GetFocus() == m_hWnd)
	{
		wgc::devicecontext_client dc(m_hWnd);
		PaintUnderline(dc, m_ptCaretPhysicalPrev, FALSE);
		PaintUnderline(dc, m_ptCaretPhysical    , TRUE);
	}

	// カレットの位置を更新
	m_ptCaretPhysicalPrev = m_ptCaretPhysical;
}


////////////////////////////////////////////////////////////
// キーボードルーチン

// [Page Up]
void edit::KeyPageUp(void)
{
	ScrollVert(-m_sizeTextArea.cy, TRUE, TRUE);
	EnsureVisible();
}

// [Page Down]
void edit::KeyPageDown(void)
{
	ScrollVert(m_sizeTextArea.cy, TRUE, TRUE);
	EnsureVisible();
}

// [Home]
void edit::KeyHome(void)
{
	// コントロールキーが押されている場合
	if(is_keydown_control())
	{
		// カーソルをバッファの先頭に移動
		m_manager.set_position(1, false, true);
		m_ptCaretLogical.y = 0;
	}

	// カーソルを画面左端に移動
	m_ptCaretLogical.x = 0;

	LogicalToPhysical();
	EnsureVisible();
}

// [End]
void edit::KeyEnd(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();

	// コントロールキーが押されている場合
	if(is_keydown_control())
	{
		// バッファの末尾にカーソルを移動
		m_manager.set_position(0, false, true);
		pos_now = m_manager.get_iterator_now();
		m_ptCaretLogical.y = m_manager.get_line_count() - 1;
	}

	// カーソルを画面右端に移動
	m_ptCaretLogical.x = pos_now->length();

	LogicalToPhysical();
	EnsureVisible();
}

// [←]
void edit::KeyLeft(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();

	// 診断
	const long length = pos_now->length();
	assert(m_ptCaretLogical.x <= length);

	// 行頭でない場合
	if(m_ptCaretLogical.x > 0)
	{
		LPCWSTR wstr = pos_now->c_str();

		// [Ctrl]が押されている場合
		if(is_keydown_control())
		{
			// 単語の区切りまで飛ばす
			const charcode_categorize_t char_type = charcode_categorize(wstr[--m_ptCaretLogical.x]);
			while(m_ptCaretLogical.x > 0)
			{
				if(charcode_categorize(wstr[m_ptCaretLogical.x - 1]) != char_type)
				{
					break;
				}
				m_ptCaretLogical.x--;
			}
		}
		// 押されていない場合
		else
		{
			// 1つ左へ移動
			m_ptCaretLogical.x--;
		}
	}
	// 行頭の場合
	else
	{
		// 現在行が先頭の場合
		if(m_manager.is_begin())
		{
			if(!EnsureVisible())
			{
				return;
			}
		}
		// 行頭で、上の行がある場合
		else
		{
			// 現在位置を更新
			m_manager.scroll_now(-1);
			pos_now--;
			m_ptCaretLogical.x = pos_now->length();
			m_ptCaretLogical.y--;
		}
	}

	LogicalToPhysical();
	EnsureVisible();
	UpdateClient(0);
}

// [→]
void edit::KeyRight(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();
	const long length = pos_now->length();

	// 行末でない場合
	if(m_ptCaretLogical.x < length)
	{
		LPCWSTR wstr = pos_now->c_str();

		// [Ctrl]が押されている場合
		if(is_keydown_control())
		{
			// 単語の区切りまで飛ばす
			const charcode_categorize_t char_type = charcode_categorize(wstr[m_ptCaretLogical.x]);
			while(m_ptCaretLogical.x < length)
			{
				m_ptCaretLogical.x++;
				if(charcode_categorize(wstr[m_ptCaretLogical.x]) != char_type)
				{
					break;
				}
			}
		}
		// 押されていない場合
		else
		{
			// 1つ右へ移動
			m_ptCaretLogical.x++;
		}
	}
	// 行末の場合
	else
	{
		// 現在行が末尾の場合
		if(m_manager.is_end())
		{
			m_nInternalX = 0;
			if(!EnsureVisible())
			{
				return;
			}
		}
		// 行末で、下の行がある場合
		else
		{
			// 現在位置を更新
			m_manager.scroll_now(1);
			pos_now++;
			m_ptCaretLogical.x = 0;
			m_ptCaretLogical.y++;
		}
	}

	LogicalToPhysical();
	EnsureVisible();
	UpdateClient(0);
}

// [↑]
void edit::KeyUp(void)
{
	// 論理スクロール用パラメータ
	BOOL bPhysical = FALSE;
	BOOL bLogical  = TRUE;
	BOOL bByScroll = TRUE;

	// [Ctrl]が押されている場合
	if(is_keydown_control())
	{
		// 物理スクロールのみ行う
		bPhysical = TRUE;
		bLogical  = FALSE;
		bByScroll = FALSE;
	}

	ScrollVert(-1, bPhysical, bLogical);
	EnsureVisible(bByScroll);
	UpdateClient(0);
}

// [↓]
void edit::KeyDown(void)
{
	// 論理スクロール用パラメータ
	BOOL bPhysical = FALSE;
	BOOL bLogical  = TRUE;
	BOOL bByScroll = TRUE;

	// [Ctrl]が押されている場合
	if(is_keydown_control())
	{
		// 物理スクロールのみ行う
		bPhysical = TRUE;
		bLogical  = FALSE;
		bByScroll = FALSE;
	}

	ScrollVert(1, bPhysical, bLogical);
	EnsureVisible(bByScroll);
	UpdateClient(0);
}

// [Enter]
int edit::KeyReturn(void)
{
	const int linenumber_modify = m_manager.get_linenumber_now();

	InsertLineFeed();

	LogicalToPhysical();
	EnsureVisible();

	// 変更箇所＝以前カーソルがあった行
	return linenumber_modify;
}

// [Backspace]
BOOL edit::KeyBack(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();

	// 診断
	const long length = pos_now->length();
	assert(m_ptCaretLogical.x <= length);

	// 行頭の場合
	if(m_ptCaretLogical.x == 0)
	{
		// バッファの先頭なら何もしない
		if(m_manager.is_begin())
		{
			EnsureVisible();
			return FALSE;
		}

		// 論理座標を更新
		pos_now--;
		m_ptCaretLogical.x = pos_now->length();
		m_ptCaretLogical.y--;

		// 現在行と1つ上の行を連結
		m_manager.combine_backword();
		_SyncLineCount();
	}
	// 行頭でない場合
	else
	{
		// 現在位置の左の文字を削除
		m_ptCaretLogical.x--;
		m_manager.delete_string(m_ptCaretLogical.x, 1);
	}
	LogicalToPhysical();
	EnsureVisible();

	// 変更箇所＝現在カーソルのある行
	return m_manager.get_linenumber_now();
}

// [Delete]
BOOL edit::KeyDelete(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();
	const long length = pos_now->length();

	// 行末の場合
	if(m_ptCaretLogical.x >= length)
	{
		m_ptCaretLogical.x = length;

		// バッファの末尾なら何もしない
		if(m_manager.is_end())
		{
			EnsureVisible();
			return 0;
		}

		// 現在行と1つ下の行を連結
		m_manager.combine_forward();
		_SyncLineCount();
	}
	// 行末でない場合
	else
	{
		// 現在位置の文字を削除
		m_manager.delete_string(m_ptCaretLogical.x, 1);
	}
	LogicalToPhysical();
	EnsureVisible();

	// 変更箇所＝現在カーソルのある行
	return m_manager.get_linenumber_now();
}

// 文字列の入力
void edit::KeyString(const wstring_t &wstr)
{
	if(IsReadOnly())
	{
		return;
	}

	const int linenumber_modify = m_manager.get_linenumber_now();

	InsertString(wstr);

	EnsureVisible();
	UpdateClient(linenumber_modify);
	MoveCaret();
}


// 現在位置にデータを挿入
void edit::InsertData(const wchar_t *wstr)
{
	const int linenumber_modify = m_manager.get_linenumber_now();

	int x = m_ptCaretLogical.x;
	const int lines = m_manager.insert_data(x, wstr);

	m_ptCaretLogical.x  = x;
	m_ptCaretLogical.y += lines;

	if(lines > 0)
	{
		_SyncLineCount();
	}

	LogicalToPhysical();
	EnsureVisible();
	UpdateClient(linenumber_modify);
	MoveCaret();
}


// 現在位置に文字列を挿入（改行なし）
void edit::InsertString(const wstring_t &wstr)
{
	m_manager.insert_string(m_ptCaretLogical.x, wstr);

	m_ptCaretLogical.x += wstr.length();

	LogicalToPhysical();
}

// 現在位置に改行を挿入
void edit::InsertLineFeed(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();

	// 診断
	const long length = pos_now->length();
	assert(m_ptCaretLogical.x <= length);

	// 現在行を現在位置で分割
	const int pos = m_ptCaretLogical.x;
	m_manager.insert_linefeed(pos);

	// 論理座標を更新
	m_ptCaretLogical.x = 0;
	m_ptCaretLogical.y++;
	m_nInternalX = 0;

	_SyncLineCount();
}


////////////////////////////////////////////////////////////
// 描画ルーチン

// クライアント領域を描画
void edit::PaintClient(
	wgc::devicecontext &dc,
	const int linenumber_redraw /* = 1 */,
	const BOOL bLinenumber      /* = TRUE */,
	const BOOL bRuler           /* = TRUE */) const
{
	// カーソルを非表示
	HideCaret();
	{
		// テキストを描画
		if(linenumber_redraw > 0)
		{
			PaintText(dc, linenumber_redraw);
		}

		// 行番号を描画
		if(bLinenumber && IsLinenumberVisible())
		{
			PaintLineNumber(dc);
		}

		// ルーラーを描画
		if(bRuler && IsRulerVisible())
		{
			PaintRuler(dc);
		}
	}
	// カーソルを復元
	ShowCaret();
}


// テキスト領域を描画
void edit::PaintText(wgc::devicecontext &dc, const int linenumber_redraw) const
{
	const_iterator_t p = m_manager.get_iterator_top();
	assert(p != m_manager.get_iterator_end());
	assert(linenumber_redraw > 0);

	// [block]
	// クリッピング領域を設定
	{
		RECT rect;
		CalcRectText(rect, FALSE);
		dc.SetClipRect(rect);
	}

	dc.SetBkColor(m_infoText.colorBack);
	HGDIOBJ hOldObject = dc.SelectObject(GetFont());
	{
		// 表示位置
		POINT pos = {0, 0};

		// [block]
		// 変更の不要な論理行数を計算
		{
			const int diff = linenumber_redraw - m_manager.get_linenumber_top();
			if(diff > m_sizeTextArea.cy)
			{
				// 描画不要
				goto end;
			}
			for(int i = 0; i < diff; i++)
			{
				p++;
				pos.y += 1;
				assert(p != m_manager.get_iterator_end());
			}
		}

		// 論理行単位のループ
		while(p != m_manager.get_iterator_end())
		{
			// 表示位置を計算
			pos.x = -m_nColumnLeft * m_sizeFont.cx;

			// [block]
			// 本当はここに折り返し処理のために物理行単位のループが入る
			{
				pos.x += DrawString(
					dc,
					pos,
					p->substr(0, m_nColumnLeft + (m_sizeTextArea.cx / m_sizeFont.cx) + 1),
					m_infoText.color, m_infoText.colorBack);

				// 行末に改行マークを追加
				PaintMarkLineFeed(dc, pos);

				if(pos.y > m_sizeTextArea.cy)
				{
					// 最下行まで描画完了
					goto end;
				}
				pos.y++;
			}
			p++;
		}

		// 行末をEOFで上書き
		pos.y--;
		PaintMarkEOF(dc, pos);

		// [block]
		// 以降のテキスト領域を背景色で塗りつぶす
		{
			RECT rect;
			CalcRectText(rect, FALSE);
			rect.top += (pos.y + 1) * (m_sizeFont.cy + m_infoText.nLineSpace);

			dc.FillSolidRect(rect, m_infoText.colorBack);
		}
	}
end:
	dc.SelectObject(hOldObject);
}

// テキスト領域の背景を描画
void edit::PaintTextBack(wgc::devicecontext &dc) const
{
	RECT rect;
	CalcRectText(rect);

	dc.FillSolidRect(rect, m_infoText.colorBack);
}


// 行番号を描画
void edit::PaintLineNumber(wgc::devicecontext &dc) const
{
	assert(IsLinenumberVisible());

	dc.ClearClip();

	dc.SetTextColor(m_infoLineNumber.color);
	dc.SetBkColor  (m_infoLineNumber.colorBack);
	HGDIOBJ hOldObject = dc.SelectObject(GetFont());
	{
		const int line_stride = m_sizeFont.cy + m_infoText.nLineSpace;
		const int line_count = m_manager.get_line_count();

		RECT rect;
		CalcRectLineNumber(rect);
		rect.bottom = rect.top + line_stride;
		rect.right -= m_infoLineNumber.nMargin / 2;

		int linenumber = m_manager.get_linenumber_top();
		for(int i = 0; i <= m_sizeTextArea.cy; i++)
		{
			// 行番号の文字列を取得
			// （wsprintf() を使いたかったけど、%*d って指定できないのね…）
			TCHAR line_str[128];
			_stprintf(
				line_str,
				_T("%*d"),
				m_infoLineNumber.nCols, linenumber);

			// 表示
			dc.ExtTextOut(
				rect.left, rect.top,
				ETO_OPAQUE | ETO_CLIPPED,
				rect,
				line_str);

			rect.top    += line_stride;
			rect.bottom += line_stride;

			if(linenumber++ == line_count)
			{
				// 以降の行番号領域を背景色で塗りつぶす
				rect.bottom += (m_sizeTextArea.cy - i - 1) * line_stride;
				dc.FillSolidRect(rect, m_infoLineNumber.colorBack);
				break;
			}
		}
	}
	dc.SelectObject(hOldObject);
}

// 行番号の背景を描画
void edit::PaintLineNumberBack(wgc::devicecontext &dc) const
{
	assert(IsLinenumberVisible());

	RECT rect;
	CalcRectLineNumber(rect);

	// 背景色で塗りつぶす
	dc.FillSolidRect(rect, m_infoLineNumber.colorBack);

	// 縦線を引く
	HPEN    hPen       = ::CreatePen(PS_SOLID, 1, m_infoLineNumber.color);
	HGDIOBJ hOldObject = dc.SelectObject(hPen);
	{
		const int x = rect.right - (m_infoLineNumber.nMargin / 2);
		dc.MoveTo(x, rect.top);
		dc.LineTo(x, rect.bottom);
	}
	dc.SelectObject(hOldObject);
	::DeleteObject(hPen);
}


// ルーラーを描画
void edit::PaintRuler(wgc::devicecontext &dc) const
{
	assert(IsRulerVisible());

	dc.ClearClip();

	PaintRulerBack(dc);

	HPEN    hPen       = ::CreatePen(PS_SOLID, 1, m_infoRuler.color);
	HGDIOBJ hOldObject = dc.SelectObject(hPen);
	{
		RECT rect;
		CalcRectRuler(rect);

		// [block]
		// 横線を引く
		{
			const int x1 = rect.left + CalcMarginLeft();
			const int x2 = rect.right;
			const int y  = m_infoRuler.nHeight - 1;

			dc.MoveTo(x1, y);
			dc.LineTo(x2, y);
		}

		POINT pos = {0, 0};
		CalcDisplayPos(pos, pos);
		int column = m_nColumnLeft;
		for(int pos_x = pos.x; pos_x < rect.right; pos_x += m_sizeFont.cx)
		{
			// 線の高さを決定
			int height = m_infoRuler.nHeight / 2;
			if((column % (m_infoRuler.nStride / 2)) == 0) { height = m_infoRuler.nHeight * 3 / 4; }
			if((column %  m_infoRuler.nStride     ) == 0) { height = m_infoRuler.nHeight; }

			// 線を引く
			dc.MoveTo(pos_x, m_infoRuler.nHeight - 1);
			dc.LineTo(pos_x, m_infoRuler.nHeight - height);

			column++;
		}
	}
	dc.SelectObject(hOldObject);
	::DeleteObject(hPen);
}

// ルーラーの背景を描画
void edit::PaintRulerBack(wgc::devicecontext &dc) const
{
	assert(IsRulerVisible());

	RECT rect;
	CalcRectRuler(rect);

	dc.FillSolidRect(rect, m_infoRuler.colorBack);
}


// 指定位置にアンダーラインを描画
void edit::PaintUnderline(wgc::devicecontext &dc, const POINT &pos, const BOOL bDraw /* = TRUE */) const
{
	// テキスト領域外なら何もしない
	if(pos.y < 0 || pos.y >= m_sizeTextArea.cy)
	{
		return;
	}

	// 罫線を表示する位置を計算
	POINT pos_display;
	CalcDisplayPos(pos, pos_display);
	pos_display.y += m_sizeFont.cy;

	// 罫線を表示する矩形を計算
	RECT rect;
	CalcRectText(rect);

	const COLORREF color = bDraw ? m_infoText.colorLine : m_infoText.colorBack;
	HPEN    hPen       = ::CreatePen(PS_SOLID, 1, color);
	HGDIOBJ hOldObject = dc.SelectObject(hPen);
	{
		// 罫線を描画
		dc.MoveTo(rect.left , pos_display.y);
		dc.LineTo(rect.right, pos_display.y);
	}
	dc.SelectObject(hOldObject);
	::DeleteObject(hPen);
}


int edit::DrawString(wgc::devicecontext &dc, const POINT &pos, const wstring_t &wstr, const COLORREF colorText, const COLORREF colorBack) const
{
	const wchar_t special[] =
	{
		CODE_TAB,
		CODE_WIDESPACE,
		CODE_NUL
	};

	wstring_t substring = wstr;
	int x = 0;
	for(;;)
	{
		dc.SetTextColor(colorText);
		dc.SetBkColor(colorBack);

		// 特殊文字を検索
		wstring_t::size_type found_pos = substring.find_first_of(special);

		// [block]
		// 特殊文字の前まで描画
		{
			const wstring_t tmp = substring.substr(0, found_pos);
			const POINT pos_draw = {pos.x + x, pos.y};
			PaintString(dc, pos_draw, tmp.c_str(), tmp.length());
			x += CalcPhysicalLength(tmp);
		}

		// 特殊文字が見つからなければループを抜ける
		if(found_pos == wstring_t::npos)
		{
			break;
		}

		const wchar_t code = substring[found_pos];
		switch(code)
		{
		case CODE_TAB:
			x += PaintMarkTab(dc, pos, x);
			break;

		case CODE_WIDESPACE:
			x += PaintMarkWideSpace(dc, pos, x);
			break;
		}

		substring = substring.substr(found_pos + 1);
	}

	// 移動した文字数（カレット単位）を返す
	return x;
}

// 文字列を描画
void edit::PaintString(wgc::devicecontext &dc, const POINT &pos, LPCWSTR wstr, const int length) const
{
	POINT pos_display;
	CalcDisplayPos(pos, pos_display);

	dc.TextOutW(pos_display.x, pos_display.y, wstr, length);
}

// 改行マークを描画
void edit::PaintMarkLineFeed(wgc::devicecontext &dc, const POINT &pos) const
{
	dc.SetTextColor(m_infoText.colorMark);

	const int fill_length = m_sizeTextArea.cx - pos.x + 2;
	if(fill_length > 0)
	{
		// 下向き矢印を描画
		wstring_t linefeed(fill_length, CODE_SPACE);
		linefeed[0] = MARK_LF;

		PaintString(dc, pos, linefeed.c_str(), linefeed.length());
	}
}

// EOFマークを描画
void edit::PaintMarkEOF(wgc::devicecontext &dc, const POINT &pos) const
{
	dc.SetTextColor(m_infoText.colorMark);
	PaintString(dc, pos, MARK_EOF, MARK_EOF_LEN);
}

// タブマークを描画
int edit::PaintMarkTab(wgc::devicecontext &dc, const POINT &pos_base, const int x) const
{
	dc.SetTextColor(m_infoText.colorMark);

	// 右向き矢印を描画
	const int length = CalcTabLength(x);
	wstring_t tab(length, CODE_SPACE);
	tab[0] = MARK_TAB;

	const POINT pos = {pos_base.x + x, pos_base.y};
	PaintString(dc, pos, tab.c_str(), tab.length());

	return length;
}

// 全角空白を描画
int edit::PaintMarkWideSpace(wgc::devicecontext &dc, const POINT &pos_base, const int x) const
{
	dc.SetTextColor(m_infoText.colorMark);

	const POINT pos = {pos_base.x + x, pos_base.y};
	PaintString(dc, pos, MARK_WIDESPACE, MARK_WIDESPACE_LEN);

	return CalcCodeLength(CODE_WIDESPACE);
}


////////////////////////////////////////////////////////////
// ウインドウメッセージ関連

// 親ウインドウに通知メッセージを送信
LRESULT edit::SendNotifyCommandToParent(const WORD wNotify)
{
	HWND hWndParent = GetParent();
	WORD wCtrlID    = GetDlgCtrlID(m_hWnd);

	return ::SendMessage(
		hWndParent,
		WM_COMMAND,
		GET_WM_COMMAND_MPS(wCtrlID, m_hWnd, wNotify));
}

// ウインドウプロシージャ
LRESULT edit::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case EM_GETMODIFY: return OnEmGetModify();
	case EM_SETMODIFY: OnEmSetModify(wParam); return 0;

	case EM_SETREADONLY: return OnEmSetReadOnly(wParam);

	case EM_GETMARGINS: return OnEmGetMargins();
	case EM_SETMARGINS: OnEmSetMargins(wParam, LOWORD(lParam), HIWORD(lParam)); return 0;

	case EM_GETLINECOUNT: return OnEmGetLineCount();

	case WM_UNDO:    // no break
	case EM_UNDO:    return OnEmUndo();
	case EM_CANUNDO: return OnEmCanUndo();

	case EM_REDO:    return OnEmRedo();
	case EM_CANREDO: return OnEmCanRedo();

	case WM_CUT:      OnCut  (); return 0;
	case WM_COPY:     OnCopy (); return 0;
	case WM_CLEAR:    OnClear(); return 0;
	case WM_PASTE:    OnPaste(); return 0;
	case EM_CANPASTE: return OnEmCanPaste(wParam);
	}

	// 基底クラスのプロシージャを呼び出す
	return window::WindowProc(uMsg, wParam, lParam);
}


// ウインドウ作成
int edit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(window::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// [block]
	// フォントの設定
	{
		HFONT hFont = (m_hFont != NULL) ? m_hFont : DEFAULT_FONT;
		SetFont(hFont, FALSE);
	}

	// [block]
	// スクロールバーの設定
	{
		SyncHScroll();
		SyncVScroll();
	}

	return 0;
}

UINT edit::OnGetDlgCode(LPMSG /* lpMsg */)
{
	return DLGC_WANTARROWS | DLGC_WANTCHARS;
}


// サイズ変更
void edit::OnSize(const UINT nType, const int cx, const int cy)
{
	window::OnSize(nType, cx, cy);
	UpdateTextAreaSize();
}


// フォーカスの設定
void edit::OnSetFocus(HWND /* hWndLoseFocus */)
{
	// カレットを移動
	CreateSolidCaret(2, m_sizeFont.cy);
	MoveCaret();

	// [block]
	// IMEのフォントを変更
	{
		wgc::imm immcontext(m_hWnd);
		immcontext.SetCompositionFont(GetFont());
	}
}

void edit::OnKillFocus(HWND /* hWndGetFocus */)
{
	// カレットを破棄
	DisplayCaret(FALSE);
	::DestroyCaret();

	// [block]
	// 罫線を消去
	{
		wgc::devicecontext_client dc(m_hWnd);
		PaintUnderline(dc, m_ptCaretPhysicalPrev, FALSE);
	}
}


// フォントの設定
void edit::OnSetFont(HFONT hFont, const BOOL bRedraw)
{
	// 古いフォントの破棄義務は呼び出し側
	m_hFont = hFont;

	// [block]
	{
		HDC hDC = GetDC(m_hWnd);
		HGDIOBJ hOldObject = ::SelectObject(hDC, m_hFont);

		// フォントの幅を取得
		::GetCharWidthW(
			hDC,
			WCHAR_MIN, WCHAR_MAX,
			m_nCharWidth);

		// [block]
		// フォントサイズを取得
		{
			TEXTMETRIC tm;
			::GetTextMetrics(hDC, &tm);

			_SetFontSize(tm.tmAveCharWidth, tm.tmHeight);
		}

		::SelectObject(hDC, hOldObject);
		ReleaseDC(m_hWnd, hDC);
	}

	// 再描画
	if(bRedraw)
	{
		InvalidateRect();
	}
}

HFONT edit::OnGetFont(void)
{
	return m_hFont;
}


// 水平スクロール
void edit::OnHScroll(const UINT /* nSBCode */, const UINT /* nPos */, HWND /* hWndScrollBar */)
{
}

// 垂直スクロール
void edit::OnVScroll(const UINT nSBCode, const UINT /* nPos */, HWND /* hWndScrollBar */)
{
	int dy = 0;                             // スクロール量

	// スクロール量を決定
	switch(nSBCode)
	{
	case SB_TOP:    dy = 1 - m_nLineNumberTopPrev; break;
	case SB_BOTTOM: dy = m_manager.get_line_count() - m_nLineNumberTopPrev; break;

	case SB_LINEUP:   dy = -1; break;
	case SB_LINEDOWN: dy = +1; break;

	case SB_PAGEUP:   dy = -m_sizeTextArea.cy; break;
	case SB_PAGEDOWN: dy = +m_sizeTextArea.cy; break;

	case SB_THUMBTRACK:
		{
			// nPos は 16bit なので、それを超える値は GetScrollInfo() を使って取得
			SCROLLINFO si = {sizeof(si), SIF_POS | SIF_TRACKPOS};
			GetVScrollInfo(si);
			dy = si.nTrackPos - si.nPos;
		}
		break;
	}

	// スクロール
	ScrollVert(dy, TRUE, FALSE);
	UpdateClient(0);
	MoveCaret();
}


// カーソル形状の変更
BOOL edit::OnSetCursor(const HWND hWnd, const UINT nHitTest, const UINT message)
{
	// クライアント領域内にある場合
	if(nHitTest == HTCLIENT)
	{
		// カーソルのクライアント座標を取得
		POINT point;
		::GetCursorPos(&point);
		::ScreenToClient(hWnd, &point);

		switch(ClientHitTest(point))
		{
		case CHT_MARGIN:
			::SetCursor(m_hCursorMargin);
			return TRUE;
		}
	}

	return window::OnSetCursor(hWnd, nHitTest, message);
}


////////////////////////////////////////////////////////////
// マウスメッセージ

BOOL edit::OnMouseWheel(const UINT /* nFlags */, const short zDelta, const POINT & /* point */)
{
	const int scroll = -zDelta / WHEEL_DELTA * 3;

	// 物理スクロールのみ行う
	ScrollVert(scroll, TRUE, FALSE);

	UpdateClient(0);
	MoveCaret();
	return TRUE;
}

void edit::OnLButtonDown(const UINT /* nFlags */, const POINT &point)
{
	SetFocus();

	// マウス座標からカレットの物理座標を計算
	CalcCaretPos(point, m_ptCaretPhysical);

	// 物理座標から論理座標を計算
	PhysicalToLogical();

	AdjustPosition();

	EnsureVisible();
	UpdateClient(0);
	MoveCaret();
}

void edit::OnRButtonDown(const UINT /* nFlags */, const POINT &point)
{
	SetFocus();

	// マウス座標からカレットの物理座標を計算
	CalcCaretPos(point, m_ptCaretPhysical);

	// 物理座標から論理座標を計算
	PhysicalToLogical();

	AdjustPosition();

	EnsureVisible();
	UpdateClient(0);
	MoveCaret();
}

void edit::OnMButtonDown(const UINT /* nFlags */, const POINT & /* point */)
{
	SetFocus();
}


////////////////////////////////////////////////////////////
// キーボードメッセージ

// キーボード押下
void edit::OnKeyDown(const UINT nChar, const UINT nRepeatCount, const UINT nFlags)
{
	int linenumber_modify = 0;

	switch(nChar)
	{
	case VK_PRIOR: KeyPageUp();   break;
	case VK_NEXT:  KeyPageDown(); break;
	case VK_HOME:  KeyHome();     break;
	case VK_END:   KeyEnd();      break;

	case VK_LEFT:  KeyLeft();  goto caret;
	case VK_RIGHT: KeyRight(); goto caret;
	case VK_UP:    KeyUp();    goto caret;
	case VK_DOWN:  KeyDown();  goto caret;

	case VK_RETURN: if(!IsReadOnly()) { linenumber_modify = KeyReturn(); } break;
	case VK_BACK:   if(!IsReadOnly()) { linenumber_modify = KeyBack();   } break;
	case VK_DELETE: if(!IsReadOnly()) { linenumber_modify = KeyDelete(); } break;
/*
	case VK_TAB:                                // VC++ 7 では WM_CHAR にも届く？
		KeyString(wstring_t(nRepeatCount, CODE_TAB));
		return;
*/
	default:
		window::OnKeyDown(nChar, nRepeatCount, nFlags);
		return;
	}

	UpdateClient(linenumber_modify);

caret:
	MoveCaret();
}

// 文字入力
void edit::OnChar(const UINT nChar, const UINT nRepeatCount, const UINT nFlags)
{
	const wchar_t code = nChar;
	if(iswprint(code))
	{
		KeyString(wstring_t(nRepeatCount, code));
	}

	window::OnChar(nChar, nRepeatCount, nFlags);
}

// IMEからの文字入力
void edit::OnImeComposition(const WORD chDBCS, const UINT nFlags)
{
	if((nFlags & GCS_RESULTSTR) != GCS_RESULTSTR)
	{
		window::OnImeComposition(chDBCS, nFlags);
		return;
	}

	wgc::imm immcontext(m_hWnd);
	wgc::wstring_t wstr;

	// UNICODE形式で変換後の文字列を取得（Windows 98以降）
	const LONG result = immcontext.GetCompositionStringResultStrW(wstr);

#ifndef UNICODE
	if(result == IMM_ERROR_GENERAL)
	{
		// Windows 95ではUNICODE形式で取得できないので、APIを使って変換
		wgc::string_t tstr;
		immcontext.GetCompositionStringResultStr(tstr);

		wgc::wgfMultiByteToWideChar(tstr, wstr);
	}
#endif

	KeyString(wstr);
}


// 画面描画
void edit::OnPaint(void)
{
	wgc::devicecontext_paint dc(m_hWnd);
	PaintClient(dc);
}

// 背景消去
BOOL edit::OnEraseBkgnd(HDC hDC)
{
	wgc::devicecontext dc = hDC;

	PaintTextBack(dc);
	if(IsLinenumberVisible())
	{
		PaintLineNumberBack(dc);
	}
	if(IsRulerVisible())
	{
		PaintRulerBack(dc);
	}

	// 罫線を描画
	if(IsCursorUnderline() && ::GetFocus() == m_hWnd)
	{
		PaintUnderline(dc, m_ptCaretPhysicalPrev, FALSE);
		PaintUnderline(dc, m_ptCaretPhysical    , TRUE);
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
// 編集メッセージハンドラ

// ダーティフラグを取得(EM_GETMODIFY)
BOOL edit::OnEmGetModify(void) const
{
	return FALSE;
}

// ダーティフラグを設定(EM_SETMODIFY)
void edit::OnEmSetModify(const BOOL /* bModified */)
{
}

// 読み取り専用属性を付加/解除（EM_SETREADONLY）
BOOL edit::OnEmSetReadOnly(const BOOL bReadOnly)
{
	if(bReadOnly)
	{
		// 読み取り専用属性を付加
		ModifyStyle(0, ES_READONLY, FALSE);
	}
	else
	{
		// 読み取り専用属性を解除
		ModifyStyle(ES_READONLY, 0, FALSE);
	}

	// 親ウインドウに XEN_READONLYCHANGED 通知メッセージを送る
	SendNotifyCommandToParent(XEN_READONLYCHANGED);

	// WM_KEYDOWN, WM_PASTE, KeyString() の3箇所でチェックを行う
	return TRUE;
}

// 左右の余白を設定（EM_SETMARGINS）
void edit::OnEmSetMargins(const WORD fwMargin, const WORD wLeft, const WORD wRight)
{
	// フォントの幅によって余白を決定
	if(fwMargin == EC_USEFONTINFO)
	{
		m_nTextMarginLeft  = CalcCodeLength(L'C');
		m_nTextMarginRight = CalcCodeLength(L'A');
	}
	else
	{
		if(fwMargin & EC_LEFTMARGIN ) { m_nTextMarginLeft  = wLeft;  }
		if(fwMargin & EC_RIGHTMARGIN) { m_nTextMarginRight = wRight; }
	}
	UpdateTextAreaSize();
}

// 左右の余白を取得（EM_GETMARGINS）
DWORD edit::OnEmGetMargins(void) const
{
	return MAKELONG(m_nTextMarginLeft, m_nTextMarginRight);
}

// バッファの行数を取得（EM_GETLINECOUNT）
int edit::OnEmGetLineCount(void) const
{
	return m_manager.get_line_count();
}

// アンドゥ(WM_UNDO, EM_UNDO)
BOOL edit::OnEmUndo(void)
{
	return FALSE;
}

// アンドゥできるか？(EM_CANUNDO)
BOOL edit::OnEmCanUndo(void) const
{
	return FALSE;
}

// リドゥ(EM_REDO)
BOOL edit::OnEmRedo(void)
{
	return FALSE;
}

// リドゥできるか？(EM_CANREDO)
BOOL edit::OnEmCanRedo(void) const
{
	return FALSE;
}

// カット(WM_CUT)
void edit::OnCut(void)
{
	// 選択範囲を切り取り、クリップボードにコピー
	Copy();
	Clear();
}

// 選択範囲をクリップボードにコピー(WM_COPY)
void edit::OnCopy(void)
{
}

// 選択範囲を削除(WM_CLEAR)
void edit::OnClear(void)
{
}

// クリップボードのデータをペースト(WM_PASTE)
void edit::OnPaste(void)
{
	if(IsReadOnly() || !::OpenClipboard(m_hWnd))
	{
		return;
	}

	// UNICODE形式でデータを取得
	HANDLE hData = ::GetClipboardData(CF_UNICODETEXT);
	if(hData != NULL)
	{
		// データを取得
		LPVOID lpData = ::GlobalLock(hData);
		InsertData(reinterpret_cast<LPCWSTR>(lpData));
		::GlobalUnlock(hData);
	}

	::CloseClipboard();
}

// 指定フォーマットをペーストできるか？(EM_CANPASTE)
BOOL edit::OnEmCanPaste(const UINT uFormat) const
{
	// uFormat が0以外のときは、uFormat を調べる
	if(uFormat != 0)
	{
		// CF_TEXT 形式か CF_UNICODETEXT 形式ならペースト可能
		return (uFormat == CF_TEXT) || (uFormat == CF_UNICODETEXT);
	}
	// uFormat が0のときは、クリップボードのデータを調べる
	else
	{
		// NT系OSでは、前半の CF_TEXT だけで判定可能。
		// 9x系OSでも通常は CF_TEXT だけでよいが、
		// CF_UNICODETEXT 形式で格納するアプリがあるかもしれないので
		// その場合は後半で判定している。
		return ::IsClipboardFormatAvailable(CF_TEXT) || ::IsClipboardFormatAvailable(CF_UNICODETEXT);
	}
}


////////////////////////////////////////////////////////////////////////////////
// メンバ変数の設定
// メンバ変数の中には、変更するときに他のオブジェクトと同期を取ったり
// 親ウインドウへ通知する必要のあるものがある。
// そのような変数を変更するときは、直接変更するのではなく変更用のメンバ関数を介する必要がある。
// その「変更用のメンバ関数」がここ。
// 少し複雑なものは次の「同期ハンドラ」で同期を取る。

// フォントサイズの変更
void edit::_SetFontSize(const int cx, const int cy)
{
	m_sizeFont.cx = cx;
	m_sizeFont.cy = cy;

	UpdateTextAreaSize();
}

// テキスト領域のサイズの変更
void edit::_SetTextAreaSize(const int cx, const int cy)
{
	m_sizeTextArea.cx = cx;
	m_sizeTextArea.cy = cy;

	SyncVScroll();
}

////////////////////////////////////////////////////////////////////////////////
// 同期ハンドラ
// 上の「メンバ変数の設定」を使用できない複雑なオブジェクトの同期を取るときは、
// 変更された後にこの同期ハンドラを呼ぶ。
// どこで変更されるかがわかるように、コメントとして呼び出し元を書いておくこと。

// 行数が変更された（SetData() / SetEmpty() / InsertLineFeed() / KeyDelete() / KeyBack()）
void edit::_SyncLineCount(void)
{
	ExpandLineNumberCols();
	SyncVScroll();
}

// 画面上端の行番号が変更された（ScrollVert()）
void edit::_SyncLineNumberTop(void)
{
	SyncVScroll();
}

_WGC_END                                // }
