// callback.cpp …インプリメンテーションファイル
#include "stdafx.h"
#include "POP.h"
#include "callback.h"


#define TCHAR_SPACE   _T(" ")


/////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// ステータスバーへのポインタを取得
	CStatusBar *GetStatusBar(void)
	{
		CFrameWnd  *pFrameWnd  = STATIC_DOWNCAST(CFrameWnd , AfxGetMainWnd());
		CStatusBar *pStatusBar = STATIC_DOWNCAST(CStatusBar, pFrameWnd->GetMessageBar());

		return pStatusBar;
	}
END_NAMESPACE()


////////////////////////////////////////////////////////////////////////////////
// コールバック初期化
long callback_init(const int id)
{
	// マウスカーソルを砂時計にする
	AfxGetApp()->DoWaitCursor(1);

	// ステータスバーへのポインタを取得
	CStatusBar *pStatusBar = GetStatusBar();

	// 処理中のキャプションを表示
	CString strCaption;
	strCaption.LoadString(id);
	pStatusBar->SetWindowText(strCaption);

	// キャプションの幅を取得
	int width = 0;
	if(TRUE)
	{
		CClientDC dc(pStatusBar);
		CFont *pOldFont = dc.SelectObject(pStatusBar->GetFont());

		const CSize caption_size = dc.GetTextExtent(strCaption + TCHAR_SPACE);
		width = caption_size.cx;

		dc.SelectObject(pOldFont);
	}

	// ステータスバーのクライアント領域サイズを取得
	RECT rect;
	pStatusBar->GetClientRect(&rect);

	// プログレスバーを作成
	HWND hProgress = CreateWindow(
		PROGRESS_CLASS, NULL,                   // lpClassName, lpWindowName
		WS_CHILD | WS_VISIBLE,                  // dwStyle
		width         , rect.bottom * 1 / 8,    // x, y
		rect.right / 4, rect.bottom * 3 / 4,    // nWidth, nHeight
		pStatusBar->m_hWnd,                     // hWndParent
		NULL,                                   // hMenu
		AfxGetInstanceHandle(),                 // hInstance
		NULL);                                  // lpParam

	// プログレスバーのハンドルを戻り値とする
	return reinterpret_cast<long>(hProgress);
}

// コールバック（終了ならTRUEを返す）
int CALLBACK callback_default(const int num, const int denom, const long data)
{
	// プログレスバーのハンドルを取得
	HWND hProgress = reinterpret_cast<HWND>(data);
	ASSERT(hProgress != NULL);

	// キュー内のメッセージを処理
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		switch(msg.message)
		{
		case WM_PAINT:
			// WM_PAINTメッセージならばディスパッチ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			break;

		case WM_KEYDOWN:
			// ESCキーが押されたら終了
			if(msg.wParam == VK_ESCAPE)
			{
				return TRUE;
			}
		}
	}

	// プログレスバーを伸ばす
	SendMessage(hProgress, PBM_SETRANGE, 0  , MAKELPARAM(0, denom));
	SendMessage(hProgress, PBM_SETPOS  , num, 0);

	return FALSE;
}

// 何もしないコールバック関数
int CALLBACK callback_none(const int /* num */, const int /* denom */, const long /* data */)
{
	return 0;
}

// コールバック終了（プログレスバーの破棄＆キャプションの復元）
void callback_term(long data)
{
	// プログレスバーの破棄
	if(TRUE)
	{
		// プログレスバーのハンドルを取得
		HWND hProgress = reinterpret_cast<HWND>(data);
		ASSERT(hProgress != NULL);

		DestroyWindow(hProgress);
	}

	// キャプションを元に戻す
	if(TRUE)
	{
		CString strCaption;
		strCaption.LoadString(AFX_IDS_IDLEMESSAGE);

		CStatusBar *pStatusBar = GetStatusBar();
		pStatusBar->SetWindowText(strCaption);
	}

	// マウスカーソルを元に戻す
	AfxGetApp()->DoWaitCursor(-1);
}
