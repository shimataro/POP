// POPView.cpp : CPOPView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "POP.h"

#include "MainFrm.h"
#include "POPDoc.h"
#include "POPView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPOPView

IMPLEMENT_DYNCREATE(CPOPView, CScrollView)

BEGIN_MESSAGE_MAP(CPOPView, CScrollView)
	//{{AFX_MSG_MAP(CPOPView)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_XBUTTONDOWN, OnXButtonDown)
	ON_MESSAGE(WM_XBUTTONUP  , OnXButtonUp)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPOPView クラスの構築/消滅

CPOPView::CPOPView()
{
	//{{AFX_DATA_INIT(CPOPView)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します。
	//}}AFX_DATA_INIT
	// TODO: この場所に構築用のコードを追加してください。

	// ビューの最大サイズ＝とりあえず-1（OnInitialUpdate()で設定する）
	m_sizeViewMax.cx = -1;
	m_sizeViewMax.cy = -1;

	// マウスイベントをトラッキングしていない
	m_bTrackingMouseEvent = FALSE;

	// ボタン＝押されていない
	m_bXButton1Down = FALSE;
	m_bXButton2Down = FALSE;

	// ガンマテーブルを作成
	CreateGammaTable(GAMMA_DEFAULT_FILE, GAMMA_DEFAULT_DECODE, GAMMA_DEFAULT_DISPLAY, m_gamma_table);

	::ZeroMemory(&m_bitmapinfo, sizeof(m_bitmapinfo));
	m_buffer = NULL;
}

CPOPView::~CPOPView()
{
}

void CPOPView::DoDataExchange(CDataExchange* pDX)
{
	CScrollView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPOPView)
		// メモ: ClassWizard はこの位置に DDX と DDV の呼び出しを追加します。
	//}}AFX_DATA_MAP
}

BOOL CPOPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CScrollView::PreCreateWindow(cs);
}

// ビューが作られるときに最初に呼ばれる
void CPOPView::OnInitialUpdate()
{
	// ステータスバーへのポインタを取得
	if(TRUE)
	{
		CMainFrame *pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
		ASSERT(pMainFrame != NULL);

		CStatusBar *pStatusBar = STATIC_DOWNCAST(CStatusBar, pMainFrame->GetMessageBar());
		ASSERT(pStatusBar != NULL);

		m_pStatusBar = pStatusBar;
	}

	// ステータスバーのペインインデックスを取得
	if(TRUE)
	{
		m_nIndexColor = m_pStatusBar->CommandToIndex(ID_INDICATOR_COLOR);
		m_nIndexPoint = m_pStatusBar->CommandToIndex(ID_INDICATOR_POINT);
		ASSERT(m_nIndexColor != -1);
		ASSERT(m_nIndexPoint != -1);
	}

	// ビューの最大サイズ＝初期状態のビューサイズ
	if(TRUE)
	{
		CRect rect;
		GetClientRect(rect);
		m_sizeViewMax = rect.Size();
	}

	CScrollView::OnInitialUpdate();
}

// ドキュメントが変更された（DIBセクションの更新、ウインドウのリサイズ）
void CPOPView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// 画像情報を取得
	const IMAGE_INFO &info = GetDocument()->m_info;
	const IMAGE_DATA &data = GetDocument()->m_data;

	// 現在のDIBセクションとサイズが異なっていたら、DIBセクションを作り直す
	if(m_bitmapinfo.bmiHeader.biWidth  != info.width
	|| m_bitmapinfo.bmiHeader.biHeight != info.height)
	{
		// 現在のDIBセクションを破棄
		m_dibsection.DeleteObject();

		// 作り直す
		HBITMAP hDIBSection = DIBSECTION_Create32bits(
			info.width, info.height,
			&m_bitmapinfo, reinterpret_cast<void **>(&m_buffer));

		m_dibsection.Attach(hDIBSection);
	}

	// 設定を読み出す
	SETTING setting;
	if(TRUE)
	{
		extern CPOPApp theApp;
		theApp.GetSetting(setting);
	}

	// ガンマテーブルの更新
	if(TRUE)
	{
		CreateGammaTable(
			setting.display.check.elements.activate_gamma_file     ? info.gamma_file               : GAMMA_DEFAULT_FILE,
			setting.display.check.elements.customize_gamma_decode  ? setting.display.gamma_decode  : GAMMA_DEFAULT_DECODE,
			setting.display.check.elements.customize_gamma_display ? setting.display.gamma_display : GAMMA_DEFAULT_DISPLAY,
			m_gamma_table);
	}

	// DIBセクションのデータを作成
	if(TRUE)
	{
		const int      grid_size = setting.display.background_grid_size;
		const COLORREF bgc1      = setting.display.background_color1;
		const COLORREF bgc2      = setting.display.background_color2;
		BITMAP_SetDataFromImage(
			&info, &data,
			m_gamma_table,
			(grid_size == 0) ? BITMAP_GRID_SINGLE_COLOR : grid_size + 1,
			bgc1, bgc2,
			m_buffer);
	}

	// 子フレームのタイトルバーにイメージ情報を表示
	GetParentFrame()->DelayUpdateFrameTitle();

	// ウインドウのリサイズ
	ResizeWindow();

	CScrollView::OnUpdate(pSender, lHint, pHint);
}

/////////////////////////////////////////////////////////////////////////////
// CPOPView クラスの描画

void CPOPView::OnDraw(CDC* pDC) 
{
	// pDCと互換なメモリデバイスコンテキストを作成
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);

	// DIBセクションをオブジェクトとして選択
	CBitmap *pOldBitmap = dcMemory.SelectObject(&m_dibsection);

	// クリッピング領域を取得
	CRect rectClip;
	pDC->GetClipBox(&rectClip);

	// クライアント領域の内部にブリット
	pDC->BitBlt(
		rectClip.left, rectClip.top,
		min(m_bitmapinfo.bmiHeader.biWidth , rectClip.Width()),
		min(m_bitmapinfo.bmiHeader.biHeight, rectClip.Height()),
		&dcMemory,
		rectClip.left, rectClip.top,
		SRCCOPY);

	// 選択された領域を描画
	if(TRUE)
	{
		CRect rectSelect = GetDocument()->m_rect;
		if(!rectSelect.IsRectEmpty())
		{
			rectSelect.DeflateRect(1, 1);

			// 物理座標へ変換
			CRectTracker tracker(rectSelect, CRectTracker::dottedLine | CRectTracker::resizeInside | CRectTracker::hatchInside);
			ConvertToDevicePoint(tracker.m_rect);

			tracker.Draw(pDC);
		}
	}

	// 選択オブジェクトを元に戻す
	dcMemory.SelectObject(pOldBitmap);
	pOldBitmap = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CPOPView クラスの印刷

BOOL CPOPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);                   // 最大ページを設定
	return DoPreparePrinting(pInfo);
}

void CPOPView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	CScrollView::OnPrepareDC(pDC, pInfo);
}

void CPOPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CPOPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CPOPView::OnPrint(CDC* pDC, CPrintInfo* /* pInfo */)
{
	ASSERT_VALID(pDC);

	const int width  = m_bitmapinfo.bmiHeader.biWidth;
	const int height = m_bitmapinfo.bmiHeader.biHeight;

	// ページの幅、高さ
	const int nPageWidth  = pDC->GetDeviceCaps(HORZRES);
	const int nPageHeight = pDC->GetDeviceCaps(VERTRES);

	// 印刷する幅、高さ（ページに合うように元イメージを拡大／縮小）
	int nPrintWidth, nPrintHeight;
	if(nPageWidth > nPageHeight)
	{
		nPrintWidth  = width * nPageHeight / height;
		nPrintHeight = nPageHeight;
	}
	else
	{
		nPrintWidth  = nPageWidth;
		nPrintHeight = height * nPageWidth / width;
	}

	const int nOldMap  = pDC->SetMapMode(MM_TEXT);
	const int nOldMode = pDC->SetStretchBltMode(COLORONCOLOR);

	::StretchDIBits(
		pDC->m_hDC,
		0, 0,
		nPrintWidth, nPrintHeight,
		0, 0,
		m_bitmapinfo.bmiHeader.biWidth, m_bitmapinfo.bmiHeader.biHeight,
		m_buffer,
		&m_bitmapinfo,
		DIB_RGB_COLORS,
		SRCCOPY);

	pDC->SetStretchBltMode(nOldMode);
	pDC->SetMapMode(nOldMap);
}

/////////////////////////////////////////////////////////////////////////////
// CPOPView クラスの診断

#ifdef _DEBUG
void CPOPView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPOPView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CPOPDoc* CPOPView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPOPDoc)));
	return (CPOPDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// 補助関数

// スクロールビューのサイズを変更
void CPOPView::ResizeWindow(void)
{
	const SIZE sizeNew = {m_bitmapinfo.bmiHeader.biWidth, m_bitmapinfo.bmiHeader.biHeight};

	SetScrollSizes(MM_TEXT, sizeNew);

	// 子フレームが最大化されていなければフレームのサイズ変更
	if(!GetParentFrame()->IsZoomed())
	{
		BOOL bShrinkOnly = FALSE;
		if(sizeNew.cx > m_sizeViewMax.cx)
		{
			bShrinkOnly = TRUE;
		}
		if(sizeNew.cy > m_sizeViewMax.cy)
		{
			bShrinkOnly = TRUE;
		}
		ResizeParentToFit(bShrinkOnly);
	}
}

// ステータスバーにマウスポインタ位置の色成分を表示
void CPOPView::UpdateStatusPaneColor(const POINT &point)
{
	const CRect rect(0, 0, m_bitmapinfo.bmiHeader.biWidth, m_bitmapinfo.bmiHeader.biHeight);
	CString status_color;

	if(rect.PtInRect(point))
	{
		// 点の色成分を表示
		const IMAGE_INFO &info = GetDocument()->m_info;
		const IMAGE_DATA &data = GetDocument()->m_data;
		uint_t r = 0, g = 0, b = 0;

		switch(info.mode)
		{
		case COLORMODE_GRAYSCALE:
			if(TRUE)
			{
				const_uint32_t offset = CALC_OFFSET(point.x, point.y, info.width);

				// メモリ領域をロック
				const_pixel_gray8_ptr_t pixel_gray8_ptr;
				auto_lock lock_obj(&data, pixel_gray8_ptr);

				r = g = b = pixel_gray8_ptr[offset].el.gray;
			}
			break;

		case COLORMODE_INDEX:
			if(TRUE)
			{
				const_uint_t   depth  = info.depth;
				const_uint32_t hsize  = CALC_HSIZE(info.width, depth);

				// log2(depth)を計算
				const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
				const_uint_t log2_depth = log2_depth_array[depth - 1];

				// メモリ領域をロック
				const_uint8_ptr_t    index_ptr = NULL;
				const_palette8_ptr_t palette8_ptr = NULL;
				auto_lock lock_obj(&data, index_ptr, palette8_ptr);

				// オフセットとシフト量を求める
				const_uint32_t offset = CALC_PACK_OFFSET(point.x, log2_depth);
				const int      shifts = CALC_PACK_SHIFTS(point.x, log2_depth);

				// インデックス値を取得
				const_uint8_t index = PACK_EXTRACT(index_ptr[offset + hsize * point.y], depth, shifts);

				r = palette8_ptr[index].r;
				g = palette8_ptr[index].g;
				b = palette8_ptr[index].b;
			}
			break;

		case COLORMODE_RGB:
			if(TRUE)
			{
				const_uint32_t offset = CALC_OFFSET(point.x, point.y, info.width);

				// メモリ領域をロック
				const_pixel_rgb8_ptr_t pixel_rgb8_ptr;
				auto_lock lock_obj(&data, pixel_rgb8_ptr);

				r = pixel_rgb8_ptr[offset].el.r;
				g = pixel_rgb8_ptr[offset].el.g;
				b = pixel_rgb8_ptr[offset].el.b;
			}
			break;
		}

		// 色情報を文字列化
		status_color.Format(IDS_INDICATOR_COLOR, r, g, b);
	}
	else
	{
		// 色情報を初期化
		status_color.LoadString(ID_INDICATOR_COLOR);
	}

	m_pStatusBar->SetPaneText(m_nIndexColor, status_color);
}

// ステータスバーにマウスポインタの座標を表示
void CPOPView::UpdateStatusPanePoint(const POINT &point)
{
	const CRect rect(0, 0, m_bitmapinfo.bmiHeader.biWidth, m_bitmapinfo.bmiHeader.biHeight);
	CString status;

	if(rect.PtInRect(point))
	{
		// マウス座標を文字列化
		status.Format(IDS_INDICATOR_POINT, point.x, point.y);
	}
	else
	{
		// マウス座標を初期化
		status.LoadString(ID_INDICATOR_POINT);
	}

	m_pStatusBar->SetPaneText(m_nIndexPoint, status);
}

// 点を論理座標へ変換
void CPOPView::ConvertToLogicalPoint(POINT &point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
}

// 矩形を論理座標へ変換
void CPOPView::ConvertToLogicalPoint(RECT &rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&rect);
}

// サイズを論理座標へ変換
void CPOPView::ConvertToLogicalPoint(SIZE &size)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&size);
}

// 点を物理座標へ変換
void CPOPView::ConvertToDevicePoint(POINT &point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.LPtoDP(&point);
}

// 矩形を物理座標へ変換
void CPOPView::ConvertToDevicePoint(RECT &rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.LPtoDP(&rect);
}

// サイズを物理座標へ変換
void CPOPView::ConvertToDevicePoint(SIZE &size)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.LPtoDP(&size);
}


/////////////////////////////////////////////////////////////////////////////
// CPOPView クラスのメッセージ ハンドラ

// ビューのサイズが変更された（スクロールシャフトクリック時のスクロール量を変更）
void CPOPView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	SetScrollSizes(
		MM_TEXT,
		CSize(m_bitmapinfo.bmiHeader.biWidth, m_bitmapinfo.bmiHeader.biHeight),
		CSize(cx / 2, cy / 2));
}

// 背景を塗りつぶす
BOOL CPOPView::OnEraseBkgnd(CDC* pDC) 
{
	CBrush br;                              // ブラシを作成
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(128, 128, 128));
	FillOutsideRect(pDC, &br);              // クライアント領域の外側を塗りつぶす

	return TRUE;
}

// カーソルの形状設定
BOOL CPOPView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CRectTracker tracker;
	tracker.m_rect = GetDocument()->m_rect;
	ConvertToDevicePoint(tracker.m_rect);

	// トラッカに転送
	if(!tracker.m_rect.IsRectEmpty() && tracker.SetCursor(this, nHitTest))
	{
		return TRUE;
	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

// マウスカーソルが動いた（ステータスバーに情報を表示）
void CPOPView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// マウス座標をスクロール位置も考慮した論理座標に変換
	POINT ptLogical = point;
	ConvertToLogicalPoint(ptLogical);

	// ステータスバーの色・座標情報を更新
	UpdateStatusPaneColor(ptLogical);
	UpdateStatusPanePoint(ptLogical);

	if(!m_bTrackingMouseEvent)
	{
		m_bTrackingMouseEvent = TRUE;

		// WM_MOUSELEAVEメッセージをキャプチャ
		TRACKMOUSEEVENT tme;
		tme.cbSize    = sizeof(tme);
		tme.dwFlags   = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		_TrackMouseEvent(&tme);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

// マウスがビュー外に出た
LRESULT CPOPView::OnMouseLeave(WPARAM /* wParam */, LPARAM /* lParam */)
{
	// ステータスバーの色・座標表示を初期化
	UpdateStatusPaneColor(CPoint(-1, -1));
	UpdateStatusPanePoint(CPoint(-1, -1));

	m_bTrackingMouseEvent = FALSE;
	return 0;
}

// 5ボタンマウスのサイドボタン押下
LRESULT CPOPView::OnXButtonDown(WPARAM wParam, LPARAM lParam)
{
	const UINT nFlags = LOWORD(wParam);
	const CPoint point(LOWORD(lParam), HIWORD(lParam));

	switch(HIWORD(wParam))
	{
	case XBUTTON1:                          // サイドボタン大
		OnXButton1Down(nFlags, point);
		break;

	case XBUTTON2:                          // サイドボタン小
		OnXButton2Down(nFlags, point);
		break;
	}

	return 0;
}

// 5ボタンマウスのサイドボタン開放
LRESULT CPOPView::OnXButtonUp(WPARAM wParam, LPARAM lParam)
{
	const UINT nFlags = LOWORD(wParam);
	const CPoint point(LOWORD(lParam), HIWORD(lParam));

	switch(HIWORD(wParam))
	{
	case XBUTTON1:                          // サイドボタン大
		OnXButton1Up(nFlags, point);
		break;

	case XBUTTON2:                          // サイドボタン小
		OnXButton2Up(nFlags, point);
		break;
	}

	return 0;
}

void CPOPView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect &rect = GetDocument()->m_rect;

	CRectTracker tracker;
	tracker.m_rect = rect;
	ConvertToDevicePoint(tracker.m_rect);

	// トラッカ内にカーソルがない場合
	if(tracker.HitTest(point) == -1)
	{
		// ラバーバンド描画
		tracker.TrackRubberBand(this, point, TRUE);
	}
	else
	{
		// トラッキング
		tracker.Track(this, point, TRUE);
	}

	// 選択領域を論理座標へ変換
	rect = tracker.m_rect;
	rect.NormalizeRect();
	ConvertToLogicalPoint(rect);

	Invalidate(FALSE);

	CScrollView::OnLButtonDown(nFlags, point);
}

// 5ボタンマウスのサイドボタン（大）が押下された
void CPOPView::OnXButton1Down(UINT /* nFlags */, CPoint /* point */)
{
	m_bXButton1Down = TRUE;
	SetCapture();
}

// 5ボタンマウスのサイドボタン（大）が開放された
void CPOPView::OnXButton1Up(UINT /* nFlags */, CPoint point)
{
	if(m_bXButton1Down)
	{
		m_bXButton1Down = FALSE;
		ReleaseCapture();

		// クライアント領域の矩形を取得
		CRect rect;
		GetClientRect(rect);

		// マウスがクライアント領域に入っていたらアンドゥ
		if(rect.PtInRect(point))
		{
			// アンドゥ
			AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKELONG(ID_EDIT_UNDO, 1));
		}
	}
}

// 5ボタンマウスのサイドボタン（小）が押下された
void CPOPView::OnXButton2Down(UINT /* nFlags */, CPoint /* point */)
{
	m_bXButton2Down = TRUE;
	SetCapture();
}

// 5ボタンマウスのサイドボタン（小）が開放された
void CPOPView::OnXButton2Up(UINT /* nFlags */, CPoint point)
{
	if(m_bXButton2Down)
	{
		m_bXButton2Down = FALSE;
		ReleaseCapture();

		// クライアント領域の矩形を取得
		CRect rect;
		GetClientRect(rect);

		// マウスがクライアント領域に入っていたらリドゥ
		if(rect.PtInRect(point))
		{
			// リドゥ
			AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKELONG(ID_EDIT_REDO, 1));
		}
	}
}
