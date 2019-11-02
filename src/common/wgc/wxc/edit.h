// edit.h …エディタコンポーネント
#ifndef __WXC_EDIT__
#define __WXC_EDIT__

#include "wxc.h"
#include "../window.h"
#include "../devicecontext.h"
#include "../../sgc/edit_manager.h"

// wxc::edit は、エディットコントロール・リッチエディットコントロールのメッセージを受理する
#include <richedit.h>


// スタイル
#define XES_LINENUMBER      0x0001      // 行番号を表示
#define XES_RULER           0x0002      // ルーラを表示
#define XES_CURSORUNDERLINE 0x0004      // カーソル行に下線を描画

// 通知メッセージ
#define XEN_READONLYCHANGED 0x0001      // 書き換え属性が変更された（EM_SETREADONLY）
#define XEN_MODIFYCHANGED   0x0002      // ダーティフラグが変更された


_WXC_BEGIN                              // namespace wxc {

class edit : public wgc::window
{
public:
	// イテレータ
	typedef sgc::edit_manager::const_iterator_t const_iterator_t;
	typedef sgc::edit_manager::wstring_t        wstring_t;

	// テキスト情報
	typedef struct _text_info_t
	{
		int nTabSize;                           // タブ幅
		int nLineSpace;                         // 行間隔（≧１）

		// 色情報
		COLORREF color;                         // 色
		COLORREF colorBack;                     // 背景色
		COLORREF colorMark;                     // 特殊文字の色
		COLORREF colorLine;                     // 罫線の色
	} text_info_t;

	// 行番号情報
	typedef struct _linenumber_info_t
	{
		int      nCols;                         // 桁数
		int      nMargin;                       // 右側余白
		COLORREF color;                         // 色
		COLORREF colorBack;                     // 背景色
	} linenumber_info_t;

	// ルーラー情報
	typedef struct _ruler_info_t
	{
		int nHeight;                            // 高さ
		int nStride;                            // 間隔
		COLORREF color;                         // 色
		COLORREF colorBack;                     // 背景色
	} ruler_info_t;


	////////////////////////////////////////////////////////////////
	// コンストラクション

	explicit edit(
		HCURSOR hCursorMargin = NULL,
		HFONT   hFont         = NULL);
	~edit(void);

	BOOL Create(
		const DWORD dwStyle, const DWORD dwExStyle,
		const DWORD dwXStyle,
		wgc::window *parent,
		const UINT nID,
		HINSTANCE hInstance = NULL,
		const int x = CW_USEDEFAULT,
		const int y = CW_USEDEFAULT,
		const int nWidth  = CW_USEDEFAULT,
		const int nHeight = CW_USEDEFAULT);


	////////////////////////////////////////////////////////////////
	// 環境設定
	void SetCursorMargin(HCURSOR hCursorMargin);


	////////////////////////////////////////////////////////////////
	// スタイル
	BOOL IsReadOnly(void) const;

	DWORD GetXStyle(void) const;
	DWORD SetXStyle(const DWORD dwNewStyle);
	BOOL IsLinenumberVisible(void) const;
	BOOL IsRulerVisible     (void) const;
	BOOL IsCursorUnderline  (void) const;

	// 操作
	BOOL GetModify(void) const;
	void SetModify(const BOOL bModified = TRUE);
	BOOL SetReadOnly(const BOOL bReadOnly = TRUE);

	void  SetMargins(const UINT nLeft, const UINT nRight);
	DWORD GetMargins(void) const;

	int  GetLineCount(void) const;

	BOOL Undo   (void);
	BOOL CanUndo(void) const;
	BOOL Redo   (void);
	BOOL CanRedo(void) const;
	void Cut     (void);
	void Copy    (void);
	void Clear   (void);
	void Paste   (void);
	BOOL CanPaste(const UINT uFormat = 0) const;

	const_iterator_t GetIteratorBegin(void) const;
	const_iterator_t GetIteratorEnd  (void) const;
	void SetData(const wchar_t *wstr);
	void SetEmpty(void);

protected:
	// エディタエンジン
	sgc::edit_manager m_manager;

	// 論理情報
	POINT m_ptCaretLogical;                 // カレットの論理座標（原点(0, 0)）
	int   m_nInternalX;                     // 内部的なX座標

	// 物理情報
	HCURSOR m_hCursorMargin;                // 余白部のカーソル
	HFONT   m_hFont;                        // フォント
	SIZE    m_sizeFont;                     // フォントの大きさ
	int    *m_nCharWidth;                   // 文字の幅

	SIZE m_sizeTextArea;                    // テキスト領域のサイズ（カレット単位）

	BOOL  m_bCaretVisible;                  // カレットが可視か

	POINT m_ptCaretPhysical;                // カレットの物理座標（画面上の位置 原点(0, 0)）
	POINT m_ptCaretPhysicalPrev;            // 前回チェックしたときのカレットの物理座標

	int m_nColumnLeft;                      // 一番左の桁番号
	int m_nColumnLeftPrev;                  // 前回チェックしたときの一番左の桁番号

	int m_nLineNumberTopPrev;               // 前回チェックしたときの画面の上端に表示されている行番号
	int m_nLineCountPrev;                   // 前回チェックしたときの行数

	text_info_t       m_infoText;
	linenumber_info_t m_infoLineNumber;
	ruler_info_t      m_infoRuler;

	DWORD m_dwXStyle;
	int m_nTextMarginLeft;                  // 左側余白（行番号非表示時）
	int m_nTextMarginRight;                 // 右側余白


	////////////////////////////////////////////////////////////
	// コンポーネントの初期化
	BOOL InitEditComponent(void);


	////////////////////////////////////////////////////////////
	// 論理/物理ルーチン

	// 座標変換
	BOOL PhysicalToLogical(void);
	BOOL LogicalToPhysical(const BOOL bUpdateInternalX = TRUE);

	// 文字長計算
	int CalcCodeLength(const wchar_t code) const;
	int CalcTabLength (const int cols) const;
	int CalcPhysicalLength(const wstring_t &wstr) const;
	int CalcPhysicalLength(LPCWSTR wstr, const int length) const;
	int CalcLogicalLength (LPCWSTR wstr, const int length) const;
	int CalcNearestPos    (LPCWSTR wstr, const int pos) const;

	// スクロール
	void ScrollHorz(const int columns, const BOOL bPhysical, const BOOL bLogical);
	void ScrollVert(const int lines  , const BOOL bPhysical, const BOOL bLogical);

	////////////////////////////////////////////////////////////
	// 論理ルーチン
	void ExpandLineNumberCols(void);


	////////////////////////////////////////////////////////////
	// 物理ルーチン

	// ヒットテスト
	typedef enum _client_hittest_t
	{
		CHT_TEXT,                               // テキスト領域
		CHT_MARGIN,                             // 余白領域
		CHT_SELECTED,                           // 選択領域
	} client_hittest_t;
	client_hittest_t ClientHitTest(const POINT &pos_display) const;

	void UpdateTextAreaSize(void);          // テキスト領域のサイズを更新

	void SyncHScroll(void);                 // 水平スクロールバーを同期
	void SyncVScroll(void);                 // 垂直スクロールバーを同期

	// 変更を反映
	void UpdateClient(const int linenumber_modify);
	BOOL EnsureVisible(const BOOL bByScroll = TRUE);
	void AdjustPosition(void);              // カレット位置、現在行を修正

	// 領域計算ルーチン
	int  CalcMarginLeft(const BOOL bAddTextMargin = TRUE) const;
	int  CalcMarginTop (void) const;
	void CalcRectLineNumber(RECT &rRect) const;
	void CalcRectRuler     (RECT &rRect) const;
	void CalcRectText      (RECT &rRect, const BOOL bIncludeMargin = TRUE) const;

	// カレットルーチン
	void DisplayCaret(const BOOL bShow = TRUE);
	void CalcDisplayPos(const POINT &pos_caret  , POINT &pos_display) const;
	void CalcCaretPos  (const POINT &pos_display, POINT &pos_caret  ) const;
	void MoveCaret(void);

	// キーボードルーチン
	void KeyPageUp  (void);
	void KeyPageDown(void);
	void KeyHome    (void);
	void KeyEnd     (void);

	void KeyLeft (void);
	void KeyRight(void);
	void KeyUp   (void);
	void KeyDown (void);

	int  KeyReturn(void);
	int  KeyBack(void);
	int  KeyDelete(void);
	void KeyString(const wstring_t &wstr);

	void InsertData(const wchar_t *wstr);
	void InsertString(const wstring_t &wstr);
	void InsertLineFeed(void);

	// 描画ルーチン
	void PaintClient(
		wgc::devicecontext &dc,
		const int  linenumber_redraw = 1,
		const BOOL bLinenumber = TRUE,
		const BOOL bRuler      = TRUE) const;

	void PaintText    (wgc::devicecontext &dc, const int linenumber_redraw) const;
	void PaintTextBack(wgc::devicecontext &dc) const;
	void PaintLineNumber    (wgc::devicecontext &dc) const;
	void PaintLineNumberBack(wgc::devicecontext &dc) const;
	void PaintRuler    (wgc::devicecontext &dc) const;
	void PaintRulerBack(wgc::devicecontext &dc) const;
	void PaintUnderline(wgc::devicecontext &dc, const POINT &pos, const BOOL bDraw = TRUE) const;

	int  DrawString        (wgc::devicecontext &dc, const POINT &pos, const wstring_t &wstr, const COLORREF colorText, const COLORREF colorBack) const;
	void PaintString       (wgc::devicecontext &dc, const POINT &pos, LPCWSTR wstr, const int length) const;
	void PaintMarkLineFeed (wgc::devicecontext &dc, const POINT &pos) const;
	void PaintMarkEOF      (wgc::devicecontext &dc, const POINT &pos) const;
	int  PaintMarkTab      (wgc::devicecontext &dc, const POINT &pos_base, const int x) const;
	int  PaintMarkWideSpace(wgc::devicecontext &dc, const POINT &pos_base, const int x) const;


	////////////////////////////////////////////////////////////
	// メッセージ関連
	LRESULT SendNotifyCommandToParent(const WORD wNotify);

	LRESULT CALLBACK WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	UINT OnGetDlgCode(LPMSG lpMsg);

	void OnSize(const UINT nType, const int cx, const int cy);

	void OnSetFocus (HWND hWndLoseFocus);
	void OnKillFocus(HWND hWndGetFocus);

	void  OnSetFont(HFONT hFont, const BOOL bRedraw);
	HFONT OnGetFont(void);

	void OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);
	void OnVScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);

	BOOL OnSetCursor(HWND hWnd, const UINT nHitTest, const UINT message);

	void OnKeyDown(const UINT nChar, const UINT nRepeatCount, const UINT nFlags);
	void OnChar   (const UINT nChar, const UINT nRepeatCount, const UINT nFlags);

	void OnImeComposition(const WORD chDBCS, const UINT nFlags);

	BOOL OnMouseWheel(const UINT nFlags, const short zDelta, const POINT &point);
	void OnLButtonDown(const UINT nFlags, const POINT &point);
	void OnRButtonDown(const UINT nFlags, const POINT &point);
	void OnMButtonDown(const UINT nFlags, const POINT &point);

	void OnPaint(void);
	BOOL OnEraseBkgnd(HDC hDC);

	// 操作
	BOOL OnEmGetModify(void) const;
	void OnEmSetModify(const BOOL bModified);
	BOOL OnEmSetReadOnly(const BOOL bReadOnly);

	void  OnEmSetMargins(const WORD fwMargin, const WORD wLeft, const WORD wRight);
	DWORD OnEmGetMargins(void) const;

	int  OnEmGetLineCount(void) const;

	BOOL OnEmUndo   (void);
	BOOL OnEmCanRedo(void) const;
	BOOL OnEmRedo   (void);
	BOOL OnEmCanUndo(void) const;
	void OnCut       (void);
	void OnCopy      (void);
	void OnClear     (void);
	void OnPaste     (void);
	BOOL OnEmCanPaste(const UINT uFormat) const;

private:
	// メンバ変数の設定
	void _SetFontSize    (const int cx, const int cy);
	void _SetTextAreaSize(const int cx, const int cy);

	// 同期ハンドラ
	void _SyncLineCount    (void);
	void _SyncLineNumberTop(void);
};

_WXC_END                                // }

#endif // __WXC_EDIT__
