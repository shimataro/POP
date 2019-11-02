// HyperLinkCtrl.cpp …インプリメンテーションファイル
#include "HyperLinkCtrl.h"
#include <shlobj.h>                     // CFSTR_SHELLURL
#include <assert.h>                     // assert()

#include "ole_drop_source.h"            // wgc::ole_drop_source
#include "../devicecontext.h"           // wgc::devicecontext
#include "../wgfunc.h"                  // wgc::wgfMultiByteToWideChar()


#define CF_SHELLURL     (::RegisterClipboardFormat(CFSTR_SHELLURL))

_WXC_BEGIN                              // namespace wxc {


////////////////////////////////////////////////////////////
// コンストラクション

hyperlink_ctrl::hyperlink_ctrl(
	HCURSOR hCursor,
	LPCTSTR lpszURL /* = NULL */,
	const COLORREF colorNormal /* = RGB_BLUE */,
	const COLORREF colorPoint  /* = RGB_ORANGE */,
	const COLORREF colorActive /* = RGB_RED */)
{
	m_hCursor = hCursor;
	m_hFont   = NULL;

	// テキストの色を設定
	m_colorNormal = colorNormal;          // 通常
	m_colorPoint  = colorPoint;           // ポイント時
	m_colorActive = colorActive;          // クリック時

	// リンク状態＝通常
	m_status = LS_NORMAL;

	// フォーカス矩形の描画＝なし
	m_bDrawFocusRect = FALSE;

	// ジャンプ先URLを設定
	SetURL(lpszURL);
}

hyperlink_ctrl::~hyperlink_ctrl()
{
	if(m_hFont != NULL)
	{
		::DeleteObject(m_hFont);
		m_hFont = NULL;
	}
}

BOOL hyperlink_ctrl::SetURL(LPCTSTR lpszURL /* = NULL */)
{
	// リソースファイルからURLを取得
	if(lpszURL != NULL)
	{
		m_url = lpszURL;
		return TRUE;
	}

	// 表示されている文字列からURLを取得
	if(m_hWnd != NULL)
	{
		GetWindowText(m_url);
		return TRUE;
	}

	// いずれも取得できなければエラー終了
	m_url.erase();
	return FALSE;
}

void hyperlink_ctrl::SetFontStyle(const BOOL bUnderline /* = TRUE */, const BOOL bItalic /* = FALSE */)
{
	if(m_hFont != NULL)
	{
		::DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	// 指定されたスタイルのフォントを作成
	LOGFONT lf;
	::GetObject(GetFont(), sizeof(lf), &lf);
	lf.lfUnderline = bUnderline;
	lf.lfItalic    = bItalic;

	m_hFont = ::CreateFontIndirect(&lf);
}


// リンク先へジャンプ
void hyperlink_ctrl::JumpToURL(void) const
{
	::ShellExecute(NULL, NULL, m_url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

// リンク状態を設定
void hyperlink_ctrl::SetStatus(const LINKSTATE status)
{
	m_status = status;
	InvalidateRect();
}

// リンク色を取得
COLORREF hyperlink_ctrl::GetLinkColor(void) const
{
	COLORREF color;
	switch(m_status)
	{
	case LS_NORMAL:   color = m_colorNormal; break;
	case LS_HOVER:    color = m_colorPoint;  break;
	case LS_CLICKING: color = m_colorActive; break;
	case LS_DRAGGING: color = m_colorActive; break;
	default: __assume(0);
	}
	return color;
}

// URL情報をOLEドラッグ＆ドロップ
DWORD hyperlink_ctrl::OleDragDrop(void) const
{
	ole_drop_source ods;

	// [block]
	// テキスト情報をキャッシュ
	{
		HGLOBAL hGlobal = CreateGlobalData(
			m_url.c_str(),
			sizeof(TCHAR) * (m_url.length() + 1));

		ods.CacheGlobalData(
			(sizeof(TCHAR) == sizeof(char)) ? CF_TEXT : CF_UNICODETEXT,
			hGlobal);
	}

	// [block]
	// URLをキャッシュ
	{
		wgc::string_t url;
#ifndef UNICODE
		url = m_url;
#else // UNICODE
		wgc::wgfWideCharToMultiByte(m_url, url, CP_ACP, 0);
#endif // UNICODE

		HGLOBAL hGlobal = CreateGlobalData(
			url.c_str(),
			url.length() + 1);
		ods.CacheGlobalData(CF_SHELLURL, hGlobal);
	}

	// OLEドラッグ＆ドロップを開始
	return ods.DoDragDrop(DROPEFFECT_COPY);
}

// グローバルメモリに指定データを格納
HGLOBAL hyperlink_ctrl::CreateGlobalData(LPCVOID pData, const int size)
{
	// メモリを確保
	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, size);
	if(hGlobal == NULL)
	{
		return NULL;
	}

	// [block]
	// データをコピー
	{
		LPVOID pBuffer = ::GlobalLock(hGlobal);
		::CopyMemory(pBuffer, pData, size);
		::GlobalUnlock(hGlobal);
	}

	// メモリハンドルを返す
	return hGlobal;
}


/////////////////////////////////////////////////////////////////////////////
// hyperlink_ctrl メッセージ ハンドラ

// カーソルの形状設定
BOOL hyperlink_ctrl::OnSetCursor(HWND /* hWnd */, const UINT /* nHitTest */, const UINT /* message */) 
{
	::SetCursor(m_hCursor);
	return TRUE;
}

// マウスがコントロール外に出た
void hyperlink_ctrl::OnMouseLeave(void)
{
	// ドラッグ中でなければリンクを通常色に戻す
	if(m_status != LS_DRAGGING)
	{
		SetStatus(LS_NORMAL);
	}

	static_ctrl::OnMouseLeave();
}


void hyperlink_ctrl::OnSetFocus(HWND hWndLoseFocus)
{
	m_bDrawFocusRect = TRUE;
	InvalidateRect();

	static_ctrl::OnSetFocus(hWndLoseFocus);
}

void hyperlink_ctrl::OnKillFocus(HWND hWndLoseFocus)
{
	m_bDrawFocusRect = TRUE;
	InvalidateRect();

	static_ctrl::OnKillFocus(hWndLoseFocus);
}


// ヒットテスト
UINT hyperlink_ctrl::OnNcHitTest(const POINT & /* point */)
{
	// ここで常にクライアント領域内を返すことにより、
	// スタティックコントロールに SS_NOTIFY スタイルを追加する必要がなくなる
	// （SS_NOTIFY を設定していないスタティックコントロールは常に HTTRANSPARENT を返す）
	return HTCLIENT;
}


// マウスが移動した
void hyperlink_ctrl::OnMouseMove(const UINT nFlags, const POINT &point) 
{
	// リンクがクリックされている場合
	if(m_status == LS_CLICKING)
	{
		// リンク情報のOLEドラッグ＆ドロップを開始
		SetStatus(LS_DRAGGING);
		OleDragDrop();
	}

	if(m_status != LS_HOVER)
	{
		// リンク＝ホバー
		SetStatus(LS_HOVER);

		// WM_MOUSELEAVEメッセージをキャプチャ
		TRACKMOUSEEVENT tme;
		tme.cbSize    = sizeof(tme);
		tme.dwFlags   = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		_TrackMouseEvent(&tme);
	}

	static_ctrl::OnMouseMove(nFlags, point);
}

// マウスの左ボタンが押下された
void hyperlink_ctrl::OnLButtonDown(const UINT nFlags, const POINT &point) 
{
	// リンク＝クリック中
	SetStatus(LS_CLICKING);
	SetFocus();

	static_ctrl::OnLButtonDown(nFlags, point);
}

// マウスの左ボタンが離された
void hyperlink_ctrl::OnLButtonUp(const UINT nFlags, const POINT &point) 
{
	// WebPageを表示
	if(m_status == LS_CLICKING)
	{
		JumpToURL();
	}

	// リンク＝ホバー
	SetStatus(LS_HOVER);

	static_ctrl::OnLButtonUp(nFlags, point);
}

// 描画
void hyperlink_ctrl::OnPaint(void)
{
	wgc::devicecontext_paint dc(m_hWnd);

	// フォーカス矩形を描画/消去
	// DrawFocusRect() はXOR演算を使用しているので、2度呼び出せば消える
	if(m_bDrawFocusRect)
	{
		RECT rect;
		GetClientRect(rect);
		dc.DrawFocusRect(rect);

		m_bDrawFocusRect = FALSE;
	}

	HFONT   hFont      = (m_hFont != NULL) ? m_hFont : GetFont();
	HGDIOBJ hOldObject = dc.SelectObject(hFont);
	{
		// 表示テキストを取得
		wgc::tstring_t tstr;
		GetWindowText(tstr);

		// テキストを描画
		dc.SetTextColor(GetLinkColor());
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(0, 0, tstr);
	}
	dc.SelectObject(hOldObject);
}

_WXC_END                                // }
