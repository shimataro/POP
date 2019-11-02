// ImportWindowDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "pop.h"
#include "ImportWindowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportWindowDlg ダイアログ


CImportWindowDlg::CImportWindowDlg(CWnd* pParent /*=NULL*/)
	: CToolTipDialog(CImportWindowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportWindowDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_hWndCapture = NULL;
	m_bCaptured = FALSE;
	m_hCursorDefault = ::GetCursor();
	m_hCursorDrag    = ::LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_CROSSHAIR));
}


void CImportWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CToolTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportWindowDlg)
	DDX_Control(pDX, IDC_IMPORT_WINDOW_STATIC_CROSSHAIR, m_staticCrosshair);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportWindowDlg, CToolTipDialog)
	//{{AFX_MSG_MAP(CImportWindowDlg)
	ON_BN_CLICKED(IDC_IMPORT_WINDOW_STATIC_CROSSHAIR, OnStaticCrosshair)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////
// 補助関数

// フレームを反転
BOOL CImportWindowDlg::InvertFrame(HWND hWnd)
{
	const int width = 4;

	if(!::IsWindow(hWnd))
	{
		return FALSE;
	}

	// ウインドウの大きさを取得
	RECT rect;
	::GetWindowRect(hWnd, &rect);

	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.left = 0;
	rect.top = 0;

	// クリッピング領域を求める
	HDC hDC = ::GetWindowDC(hWnd);
	::ExcludeClipRect(	
		hDC,
		rect.left   + width,
		rect.top    + width,
		rect.right  - width,
		rect.bottom - width);

	// 求めたクリッピング領域を反転
	::InvertRect(hDC, &rect);

	::ReleaseDC(hWnd, hDC);
	return TRUE;
}

void CImportWindowDlg::Restore()
{
	// まだm_hWndCaptureのフレームが反転しているので元に戻す
	InvertFrame(m_hWndCapture);

	// マウスキャプチャを解除
	ReleaseCapture();
	m_bCaptured = FALSE;

	// マークを表示
	m_staticCrosshair.ShowWindow(SW_SHOW);

	// カーソルを元に戻す
	::SetCursor(m_hCursorDefault);
}

/////////////////////////////////////////////////////////////////////////////
// CImportWindowDlg メッセージ ハンドラ

BOOL CImportWindowDlg::OnInitDialog() 
{
	CToolTipDialog::OnInitDialog();

	// ツールチップを設定
	if(TRUE)
	{
		const UINT arIDs[] =
		{
			IDC_IMPORT_WINDOW_STATIC_CROSSHAIR,
		};
		SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));
	}

	// マークにカーソルを設定
	m_staticCrosshair.SetCursor(m_hCursorDrag);

	return TRUE;                            // コントロールにフォーカスを設定しない
}

// マークがクリックされた
void CImportWindowDlg::OnStaticCrosshair() 
{
	// ドラッグ用カーソルに変更
	::SetCursor(m_hCursorDrag);

	// マークを消す（マークがドラッグされているように見える）
	m_staticCrosshair.ShowWindow(SW_HIDE);

	// キャプチャ開始
	SetCapture();
	m_bCaptured = TRUE;

	// この時点ではまだウインドウを選択しない
	m_hWndCapture = NULL;
}

// マウスを動かした
void CImportWindowDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bCaptured)
	{
		// スクリーン座標でのマウス位置を取得
		POINT ptScreen;
		::GetCursorPos(&ptScreen);

		HWND hWnd = ::WindowFromPoint(ptScreen);
		if(hWnd != NULL && hWnd != m_hWndCapture)
		{
			// マウスのあるウインドウフレームを反転
			InvertFrame(m_hWndCapture);
			InvertFrame(hWnd);
			m_hWndCapture = hWnd;
		}
	}

	CToolTipDialog::OnMouseMove(nFlags, point);
}

// 左ボタンを離した
void CImportWindowDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bCaptured)
	{
		Restore();

		if(m_hWndCapture != NULL)
		{
			// ウインドウハンドルからイメージを新規作成
			extern CPOPApp theApp;
			theApp.NewImageFromWindow(m_hWndCapture, m_bAltogether, m_dwMilliSeconds);
		}
		DestroyWindow();
	}
}

// 右ボタンを離した
void CImportWindowDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bCaptured)
	{
		// キャンセル処理
		Restore();
		m_hWndCapture = NULL;
	}

	CToolTipDialog::OnRButtonDown(nFlags, point);
}

// キャンセル
void CImportWindowDlg::OnCancel() 
{
	DestroyWindow();
}

void CImportWindowDlg::PostNcDestroy() 
{
	CToolTipDialog::PostNcDestroy();
	delete this;
}
