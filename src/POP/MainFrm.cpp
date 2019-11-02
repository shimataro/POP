// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "POP.h"

#include "MainFrm.h"
#include "ReBarState.h"

#include "winutil.h"                    // GvxFindMenu()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_TOOLBAR_FIX, OnViewToolbarFix)
	ON_COMMAND(ID_VIEW_TOPMOST, OnViewTopmost)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_FIX, OnUpdateViewToolbarFix)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOPMOST, OnUpdateViewTopmost)
	//}}AFX_MSG_MAP
	ON_COMMAND_EX_RANGE       (ID_VIEW_TOOLBAR_SELECT, ID_VIEW_TOOLBAR_WINDOW, OnBarCheck)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_TOOLBAR_SELECT, ID_VIEW_TOOLBAR_WINDOW, OnUpdateControlBarMenu)
END_MESSAGE_MAP()


// コントロールバーの設定保存用レジストリ
#define TCHAR_REG_SECTION_CONTROLBAR_PLACEMENT  _T("Control Bar Placement")
#define TCHAR_REG_SECTION_CONTROLBAR            _T("Control Bar")
#define TCHAR_REG_ENTRY_TOOLBAR_STANDARD        _T("Toolbar Standard")
#define TCHAR_REG_ENTRY_TOOLBAR_SELECT          _T("Toolbar Select")
#define TCHAR_REG_ENTRY_TOOLBAR_WINDOW          _T("Toolbar Window")

// 状態
#define TCHAR_REG_SECTION_STATUS            _T("Status")
#define TCHAR_REG_SECTION_WINDOW            _T("Window Placement")
#define TCHAR_REG_ENTRY_WINDOW_POS_X        _T("x")
#define TCHAR_REG_ENTRY_WINDOW_POS_Y        _T("y")
#define TCHAR_REG_ENTRY_WINDOW_SIZE_CX      _T("cx")
#define TCHAR_REG_ENTRY_WINDOW_SIZE_CY      _T("cy")
#define TCHAR_REG_ENTRY_WINDOW_SHOW         _T("ShowCmd")
#define TCHAR_REG_ENTRY_CURRENT_DIRECTORY   _T("Current Directory")


/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

CMainFrame::CMainFrame()
{
	// TODO: この位置にメンバの初期化処理コードを追加してください。
	m_bTopmost = FALSE;
}

CMainFrame::~CMainFrame()
{
	m_menuContextToolBar.Detach();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ツールバーのコンテキストメニューを作成
	if(TRUE)
	{
		HMENU hMenu = GvxFindMenu(
			GetMenu()->GetSafeHmenu(),
			ID_VIEW_TOOLBAR,
			NULL);

		m_menuContextToolBar.Attach(hMenu);
	}

	// コントロールバーを作成
	if(TRUE)
	{
		m_wndReBar.Create(this);
		CreateToolBar(m_wndToolBarStandard, ID_VIEW_TOOLBAR       , IDR_MAINFRAME     , IDS_CAPTION_TOOLBAR_STANDARD);
		CreateToolBar(m_wndToolBarSelect  , ID_VIEW_TOOLBAR_SELECT, IDR_TOOLBAR_SELECT, IDS_CAPTION_TOOLBAR_SELECT);
		CreateToolBar(m_wndToolBarWindow  , ID_VIEW_TOOLBAR_WINDOW, IDR_TOOLBAR_WINDOW, IDS_CAPTION_TOOLBAR_WINDOW);
		CreateStatusBar();
	}

	// ツールバーのアイコンをXP風にする
	if(TRUE)
	{
		extern CPOPApp theApp;
		SETTING setting;
		theApp.GetSetting(setting);

		if(setting.display.toolbar_XP)
		{
			m_wndToolBarStandard.ReplaceIcons(IDB_TOOLBAR_XP      , IDB_TOOLBAR_XP_DISABLED      , IDB_TOOLBAR_XP_HOT);
			m_wndToolBarWindow  .ReplaceIcons(IDB_WINDOWTOOLBAR_XP, IDB_WINDOWTOOLBAR_XP_DISABLED, IDB_WINDOWTOOLBAR_XP_HOT);
		}
	}

	// 標準で不要なボタンを削除
	if(TRUE)
	{
		const UINT nIDDelete[] =
		{
			ID_FILE_SAVE_ALL,
			ID_FILE_CLOSE,
			ID_FILE_CLOSE_ALL,
			ID_EDIT_UNDO_CLEAR,
		};
		m_wndToolBarStandard.DeleteButtons(nIDDelete, SIZE_OF_ARRAY(nIDDelete));
	}

	// 前回の状態を復元
	LoadState();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// ウインドウの状態を元に戻す
	if(TRUE)
	{
		extern CPOPApp theApp;
		SETTING setting;
		theApp.GetSetting(setting);

		if(setting.basic.check.elements.save_window_placement)
		{
			LoadWindowPlacement(cs);
		}
	}

	return TRUE;
}

BOOL CMainFrame::DestroyWindow() 
{
	// ウインドウの状態を保存
	SaveWindowPlacement();

	return CMDIFrameWnd::DestroyWindow();
}

void CMainFrame::OnClose() 
{
	// 状態を保存
	SaveState();

	CMDIFrameWnd::OnClose();
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame 追加メンバ関数

// ツールバーを作成
BOOL CMainFrame::CreateToolBar(CToolBarEx &toolbar, const UINT nID, const UINT nIDResource, const UINT nIDCaption /* = 0 */, const DWORD dwReBarStyle /* = RBBS_GRIPPERALWAYS | RBBS_BREAK | RBBS_CHILDEDGE */)
{
	// ツールバーを作成
	toolbar.CreateEx(
		this,
		TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_TOOLTIPS | TBSTYLE_ALTDRAG | CCS_ADJUSTABLE,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_FLYBY,
		CRect(0, 0, 0, 0),
		nID);
	toolbar.LoadToolBar(nIDResource);

	// キャプションを設定
	if(nIDCaption != 0)
	{
		CString caption;
		caption.LoadString(nIDCaption);
		toolbar.SetWindowText(caption);
	}

	// コンテキストメニューを設定
	if(TRUE)
	{
		HMENU hMenu = m_menuContextToolBar.m_hMenu;
		toolbar.SetContextMenu(
			(hMenu == NULL) ? NULL : &m_menuContextToolBar,
			this);
	}

	// リバーバンドに追加
	m_wndReBar.AddBar(&toolbar, NULL, NULL, dwReBarStyle);

	return TRUE;
}

// ステータスバーを作成
BOOL CMainFrame::CreateStatusBar()
{
	const UINT indicators[] =
	{
		ID_SEPARATOR,           // ステータス ライン インジケータ
		ID_INDICATOR_COLOR,
		ID_INDICATOR_POINT,
/*
		ID_INDICATOR_KANA,
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
*/
	};

	m_wndStatusBar.Create(this);
	m_wndStatusBar.SetIndicators(indicators, SIZE_OF_ARRAY(indicators));

	return TRUE;
}

void CMainFrame::LoadWindowPlacement(CREATESTRUCT &cs)
{
	extern CPOPApp theApp;

	// ウインドウの位置を元に戻す
	cs.x = theApp.GetProfileInt(TCHAR_REG_SECTION_WINDOW, TCHAR_REG_ENTRY_WINDOW_POS_X, cs.x);
	cs.y = theApp.GetProfileInt(TCHAR_REG_SECTION_WINDOW, TCHAR_REG_ENTRY_WINDOW_POS_Y, cs.y);

	// ウインドウのサイズを元に戻す
	cs.cx = theApp.GetProfileInt(TCHAR_REG_SECTION_WINDOW, TCHAR_REG_ENTRY_WINDOW_SIZE_CX, cs.cx);
	cs.cy = theApp.GetProfileInt(TCHAR_REG_SECTION_WINDOW, TCHAR_REG_ENTRY_WINDOW_SIZE_CY, cs.cy);

	// ウインドウの表示状態を元に戻す
	theApp.m_nCmdShow = theApp.GetProfileInt(TCHAR_REG_SECTION_WINDOW, TCHAR_REG_ENTRY_WINDOW_SHOW, theApp.m_nCmdShow);
	if(theApp.m_nCmdShow != SW_NORMAL
	&& theApp.m_nCmdShow != SW_MAXIMIZE
	&& theApp.m_nCmdShow != SW_SHOWMAXIMIZED)
	{
		theApp.m_nCmdShow = SW_NORMAL;
	}
}

void CMainFrame::SaveWindowPlacement(void)
{
	extern CPOPApp theApp;

	// ウインドウの状態を取得
	WINDOWPLACEMENT wndpl = {sizeof(WINDOWPLACEMENT)};
	GetWindowPlacement(&wndpl);

	// レジストリに保存
	const CRect rect = wndpl.rcNormalPosition;
	theApp.WriteProfileInt(TCHAR_REG_SECTION_WINDOW, TCHAR_REG_ENTRY_WINDOW_POS_X, rect.left);
	theApp.WriteProfileInt(TCHAR_REG_SECTION_WINDOW, TCHAR_REG_ENTRY_WINDOW_POS_Y, rect.top);
	theApp.WriteProfileInt(TCHAR_REG_SECTION_WINDOW, TCHAR_REG_ENTRY_WINDOW_SIZE_CX, rect.Width());
	theApp.WriteProfileInt(TCHAR_REG_SECTION_WINDOW, TCHAR_REG_ENTRY_WINDOW_SIZE_CY, rect.Height());
	theApp.WriteProfileInt(TCHAR_REG_SECTION_WINDOW, TCHAR_REG_ENTRY_WINDOW_SHOW, wndpl.showCmd);
}

void CMainFrame::LoadState()
{
	LoadControlBar();
	LoadCurrentDirectory();
}

void CMainFrame::SaveState()
{
	SaveCurrentDirectory();
	SaveControlBar();
}

// レジストリに保存したコントロールバーの状態を復元
void CMainFrame::LoadControlBar()
{
	extern CPOPApp theApp;

	// リバーの状態を復元
	LoadReBarState(m_wndReBar);

	// ツールバーの状態を復元
	LoadBarState(TCHAR_REG_SECTION_CONTROLBAR_PLACEMENT);

	// ツールバーコントロールの状態を復元
	if(TRUE)
	{
		HKEY hKey = theApp.GetAppRegistryKey();
		m_wndToolBarStandard.GetToolBarCtrl().RestoreState(hKey, TCHAR_REG_SECTION_CONTROLBAR, TCHAR_REG_ENTRY_TOOLBAR_STANDARD);
		m_wndToolBarSelect  .GetToolBarCtrl().RestoreState(hKey, TCHAR_REG_SECTION_CONTROLBAR, TCHAR_REG_ENTRY_TOOLBAR_SELECT);
		m_wndToolBarWindow  .GetToolBarCtrl().RestoreState(hKey, TCHAR_REG_SECTION_CONTROLBAR, TCHAR_REG_ENTRY_TOOLBAR_WINDOW);
		::RegCloseKey(hKey);
	}
}

// レジストリにコントロールバーの状態を保存
void CMainFrame::SaveControlBar()
{
	extern CPOPApp theApp;

	// ツールバーコントロールの状態を保存
	if(TRUE)
	{
		HKEY hKey = theApp.GetAppRegistryKey();
		m_wndToolBarStandard.GetToolBarCtrl().SaveState(hKey, TCHAR_REG_SECTION_CONTROLBAR, TCHAR_REG_ENTRY_TOOLBAR_STANDARD);
		m_wndToolBarSelect  .GetToolBarCtrl().SaveState(hKey, TCHAR_REG_SECTION_CONTROLBAR, TCHAR_REG_ENTRY_TOOLBAR_SELECT);
		m_wndToolBarWindow  .GetToolBarCtrl().SaveState(hKey, TCHAR_REG_SECTION_CONTROLBAR, TCHAR_REG_ENTRY_TOOLBAR_WINDOW);
		::RegCloseKey(hKey);
	}

	// ツールバーの状態を保存
	SaveBarState(TCHAR_REG_SECTION_CONTROLBAR_PLACEMENT);

	// リバーの状態を保存
	SaveReBarState(m_wndReBar);
}

// レジストリに保存した前回のカレントディレクトリを復元
void CMainFrame::LoadCurrentDirectory()
{
	// レジストリからパス名を取得
	CString path = AfxGetApp()->GetProfileString(TCHAR_REG_SECTION_STATUS, TCHAR_REG_ENTRY_CURRENT_DIRECTORY);
	if(path.IsEmpty())
	{
		// 取得に失敗したら"マイ ドキュメント"のパス名を取得
		TCHAR buffer[_MAX_PATH];
		::SHGetSpecialFolderPath(NULL, buffer, CSIDL_PERSONAL, FALSE);
		path = buffer;
	}

	SetCurrentDirectory(path);
}

// レジストリにカレントディレクトリを保存
void CMainFrame::SaveCurrentDirectory()
{
	TCHAR path[_MAX_PATH];
	GetCurrentDirectory(SIZE_OF_ARRAY(path), path);

	AfxGetApp()->WriteProfileString(TCHAR_REG_SECTION_STATUS, TCHAR_REG_ENTRY_CURRENT_DIRECTORY, path);
}

// ツールバーをカスタマイズ
void CMainFrame::CustomizeToolBar(const UINT nIDResource)
{
	CToolBarEx *pToolBarEx = NULL;

	switch(nIDResource)
	{
	case IDR_MAINFRAME:
		pToolBarEx = &m_wndToolBarStandard;
		break;

	case IDR_TOOLBAR_SELECT:
		pToolBarEx = &m_wndToolBarSelect;
		break;

	case IDR_TOOLBAR_WINDOW:
		pToolBarEx = &m_wndToolBarWindow;
		break;

	default:
		__assume(0);
	}

	// カスタマイズ
	ASSERT(pToolBarEx != NULL);
	pToolBarEx->GetToolBarCtrl().Customize();
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

// 表示→ツールバーを固定
void CMainFrame::OnViewToolbarFix() 
{
	CReBarCtrl &ctrl = m_wndReBar.GetReBarCtrl();

	const int count = ctrl.GetBandCount();
	for(int i = 0; i < count; i++)
	{
		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask  = RBBIM_STYLE;
		ctrl.GetBandInfo(i, &rbbi);

		// グリッパの状態を反転
		rbbi.fStyle ^= (RBBS_GRIPPERALWAYS | RBBS_NOGRIPPER);
		ctrl.SetBandInfo(i, &rbbi);
	}
}

// 表示→常に手前に表示
void CMainFrame::OnViewTopmost() 
{
	// フラグを反転
	m_bTopmost ^= 1;

	// トップモスト／非トップモスト表示
	::SetWindowPos(
		m_hWnd,
		m_bTopmost ? HWND_TOPMOST : HWND_NOTOPMOST,
		0, 0, 0, 0,
		SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
}

void CMainFrame::OnUpdateViewToolbarFix(CCmdUI* pCmdUI) 
{
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask  = RBBIM_STYLE;
	m_wndReBar.GetReBarCtrl().GetBandInfo(0, &rbbi);

	pCmdUI->SetCheck(rbbi.fStyle & RBBS_NOGRIPPER);
}

void CMainFrame::OnUpdateViewTopmost(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bTopmost);
}
