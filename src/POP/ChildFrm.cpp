// ChildFrm.cpp : CChildFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "POP.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame クラスの構築/消滅

CChildFrame::CChildFrame()
{
	// TODO: メンバ初期化コードをこの位置に追加してください。
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs の設定を行って、Window クラスまたは
	//       スタイルを変更してください。

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame クラスの診断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame クラスのメッセージハンドラ

// フレームのキャプションを設定
#include "POPDoc.h"
void CChildFrame::OnUpdateFrameTitle(BOOL bTitle)
{
	CMDIChildWnd::OnUpdateFrameTitle(bTitle);

	// アクティブなドキュメントを取得
	CPOPDoc *pPOPDoc = NULL;
	if(TRUE)
	{
		// ビューを取得
		CView *pView = GetActiveView();
		ASSERT(pView != NULL);

		pPOPDoc = STATIC_DOWNCAST(CPOPDoc, pView->GetDocument());
		ASSERT(pPOPDoc != NULL);
	}

	// 画像情報を取得
	const IMAGE_INFO &info = pPOPDoc->m_info;
	const UINT title_id = pPOPDoc->IsModified() ? IDS_FORMAT_TITLE_MODIFIED : IDS_FORMAT_TITLE;

	CString colormode;
	colormode.LoadString(IDS_FORMAT_TITLE_COLORMODE_GRAYSCALE + info.mode);

	// キャプション文字列を作成
	CString caption;
	caption.Format(
		title_id,
		pPOPDoc->GetTitle(),
		info.width, info.height,
		info.depth, colormode,
		info.colors);

	// タイトルバーに情報を表示
	SetWindowText(caption);
}
