// HyperLink.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "pop.h"
#include "HyperLink.h"

#include "winutil.h"                    // GvxTCHAR2CHAR()

#include <afxole.h>                     // AfxOleInit()


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperLink

CHyperLink::CHyperLink(const UINT nIDResource /* = 0 */, const COLORREF colorNormal /* = RGB_BLUE */, const COLORREF colorPoint /* = RGB_ORANGE */, const COLORREF colorActive /* = RGB_RED */)
{
	// テキストの色を設定
	m_colorNormal = colorNormal;          // 通常
	m_colorPoint  = colorPoint;           // ポイント時
	m_colorActive = colorActive;          // クリック時

	// リンク状態＝通常
	m_status = LS_NORMAL;

	// ブラシを作成
	m_brush.CreateStockObject(NULL_BRUSH);

	// カーソルをロード
	// Windows 98/Me/2000/XPなら ::LoadCursor(NULL, IDC_HAND) でよい
	m_hCursorFinger = AfxGetApp()->LoadCursor(IDC_FINGER);

	// ジャンプ先URLを設定
	SetURL(nIDResource);
}

CHyperLink::~CHyperLink()
{
}

BOOL CHyperLink::SetURL(const UINT nIDResource /* = 0 */)
{
	// リソースファイルからURLを取得
	if(m_url.LoadString(nIDResource))
	{
		return TRUE;
	}

	// 表示されている文字列からURLを取得
	if(GetSafeHwnd() != NULL)
	{
		GetWindowText(m_url);
		return TRUE;
	}

	// いずれも取得できなければエラー終了
	m_url.Empty();
	return FALSE;
}


// リンク先へジャンプ
void CHyperLink::JumpToURL(void)
{
	::ShellExecute(NULL, NULL, m_url, NULL, NULL, SW_SHOWNORMAL);
}

// リンク状態を設定
void CHyperLink::SetStatus(const LINKSTATE status)
{
	m_status = status;
	Invalidate();
}

// URL情報をOLEドラッグ＆ドロップ
void CHyperLink::OleDragDrop(void)
{
	COleDataSource ods;

	// テキスト情報をキャッシュ
	if(TRUE)
	{
		HGLOBAL hGlobal = CreateGlobalData(
			(LPCTSTR)m_url,
			sizeof(TCHAR) * (m_url.GetLength() + 1));

		ods.CacheGlobalData(
			sizeof(TCHAR) == sizeof(char) ? CF_TEXT : CF_UNICODETEXT,
			hGlobal);
	}

	// URLをキャッシュ
	if(TRUE)
	{
		char url[_MAX_PATH];
		GvxTCHAR2CHAR(m_url, url, SIZE_OF_ARRAY(url));

		const CLIPFORMAT cfURL = ::RegisterClipboardFormat(CFSTR_SHELLURL);
		HGLOBAL hGlobal = CreateGlobalData(url, sizeof(url));
		ods.CacheGlobalData(cfURL, hGlobal);
	}

	// OLEドラッグ＆ドロップを開始
	ods.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_LINK);
}

// グローバルメモリに指定データを格納
HGLOBAL CHyperLink::CreateGlobalData(LPCVOID pData, const int size)
{
	// メモリを確保
	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, size);
	if(hGlobal == NULL)
	{
		return NULL;
	}

	// データをコピー
	if(TRUE)
	{
		LPVOID pBuffer = ::GlobalLock(hGlobal);
		::CopyMemory(pBuffer, pData, size);
		::GlobalUnlock(hGlobal);
	}

	// メモリハンドルを返す
	return hGlobal;
}


BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
	//{{AFX_MSG_MAP(CHyperLink)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperLink メッセージ ハンドラ

// コントロールの色設定
HBRUSH CHyperLink::CtlColor(CDC* pDC, UINT /* nCtlColor */) 
{
	// 最初に呼ばれた時だけフォントの設定をする
	if(m_font.m_hObject == NULL)
	{
		// フォントの設定（下線を引く）
		LOGFONT lf;
		GetFont()->GetLogFont(&lf);
		lf.lfUnderline = TRUE;
		m_font.CreateFontIndirect(&lf);
		SetFont(&m_font);
	}

	// リンクおよび背景の色を設定
	COLORREF color;
	switch(m_status)
	{
	case LS_NORMAL:   color = m_colorNormal; break;
	case LS_HOVER:    color = m_colorPoint;  break;
	case LS_CLICKING: color = m_colorActive; break;
	case LS_DRAGGING: color = m_colorActive; break;
	default: __assume(0);
	}
	pDC->SetTextColor(color);
	pDC->SetBkMode(TRANSPARENT);

	return m_brush;
}

UINT CHyperLink::OnNcHitTest(CPoint point) 
{
	return HTCLIENT;
}

// カーソルの形状設定
BOOL CHyperLink::OnSetCursor(CWnd* /* pWnd */, UINT /* nHitTest */, UINT /* message */) 
{
	::SetCursor(m_hCursorFinger);
	return TRUE;
}

// マウスの左ボタンが押下された
void CHyperLink::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// リンク＝クリック中
	SetStatus(LS_CLICKING);
	CStatic::OnLButtonDown(nFlags, point);
}

// マウスの左ボタンが離された
void CHyperLink::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// WebPageを表示
	if(m_status == LS_CLICKING)
	{
		JumpToURL();
	}

	// リンク＝ホバー
	SetStatus(LS_HOVER);

	CStatic::OnLButtonUp(nFlags, point);
}

// マウスが移動した
void CHyperLink::OnMouseMove(UINT nFlags, CPoint point) 
{
	// リンクがクリックされている場合
	if(m_status == LS_CLICKING)
	{
		// リンク情報のOLEドラッグ＆ドロップを開始
		SetStatus(LS_DRAGGING);
		OleDragDrop();
	}

	// WM_MOUSELEAVEメッセージをキャプチャ
	if(TRUE)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize    = sizeof(tme);
		tme.dwFlags   = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		_TrackMouseEvent(&tme);
	}

	// リンク＝ホバー
	SetStatus(LS_HOVER);

	CStatic::OnMouseMove(nFlags, point);
}

// マウスがコントロール外に出た
LRESULT CHyperLink::OnMouseLeave(WPARAM /* wParam */, LPARAM /* lParam */)
{
	// ドラッグ中でなければリンクを通常色に戻す
	if(m_status != LS_DRAGGING)
	{
		SetStatus(LS_NORMAL);
	}
	return 0;
}
