// POP.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "POP.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "POPDoc.h"
#include "POPView.h"

#include "winutil.h"                    // GvxPathAppendToModuleDir(), GvxGetFileVersion(), GvxCHAR2TCHAR()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPOPApp

BEGIN_MESSAGE_MAP(CPOPApp, CWinApp)
	//{{AFX_MSG_MAP(CPOPApp)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_CLOSE_ALL, OnFileCloseAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE_ALL, OnUpdateFileCloseAll)
	ON_COMMAND(ID_FILE_IMPORT_CLIPBOARD, OnFileImportClipboard)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_CLIPBOARD, OnUpdateFileImportClipboard)
	ON_COMMAND(ID_FILE_IMPORT_DISPLAY, OnFileImportDisplay)
	ON_COMMAND(ID_SETTING_ENVIRONMENT, OnSettingEnvironment)
	ON_COMMAND(ID_HELP_WEBPAGE, OnHelpWebpage)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)

	// 標準のヘルプコマンド
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)

	ON_COMMAND_RANGE          (ID_SETTING_FORMAT_MIN, ID_SETTING_FORMAT_MAX, OnSettingFormat)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SETTING_FORMAT_MIN, ID_SETTING_FORMAT_MAX, OnUpdateSettingFormat)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// 定数

// 文字・文字列
#define TCHAR_DEFAULT_ALL_READ      _T("*.")

// std::vector<>の予約値
#define RESERVE_FILTER  128
#define RESERVE_FORMAT  32
#define RESERVE_PLUGIN  (RESERVE_FILTER + RESERVE_FORMAT)

#define TCHAR_WEBPAGE   _T("http://shima-t.cool.ne.jp/")


/////////////////////////////////////////////////////////////////////////////
// CPOPApp クラスの構築

CPOPApp::CPOPApp()
{
	m_nClipboardCount = 1;
	m_nDisplayCount   = 1;

	m_nFilterIndex    = 1;

	// リソースハンドル
	m_hResource = NULL;

	// std::vector<>のメモリ予約
	m_plugin_filter.reserve(RESERVE_FILTER);
	m_plugin_filter_detail.reserve(RESERVE_FILTER);

	m_plugin_format.reserve(RESERVE_FORMAT);
	m_plugin_format_detail.reserve(RESERVE_FORMAT);

	m_plugin_info.reserve(RESERVE_PLUGIN);
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CPOPApp オブジェクト

CPOPApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPOPApp クラスの初期化

BOOL CPOPApp::InitInstance()
{
	if(!CWinApp::InitInstance())
	{
		return FALSE;
	}

	InitCommonControls();
	AfxEnableControlContainer();
	if(!AfxOleInit())
	{
		return FALSE;
	}

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	// したければ以下の特定の初期化ルーチンの中から不必要なもの
	// を削除してください。

	AfxEnableControlContainer();
/*
#ifdef _AFXDLL
	Enable3dControls();    // 共有 DLL の中で MFC を使用する場合にはここを呼び出してください。
#else
	Enable3dControlsStatic();  // MFC と静的にリンクしている場合にはここを呼び出してください。
#endif
*/
	// レジストリキー登録
	SetRegistryKey(TCHAR_REG_COMPANY);

	// 設定のロード
	setting_load(m_setting_category, m_setting);
	LoadStdProfileSettings(m_setting.basic.mru_count);
	SetResource(m_setting.display.language.resource_name);

	// ドキュメントテンプレートの登録
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_POPTYPE,
		RUNTIME_CLASS(CPOPDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CPOPView));
	AddDocTemplate(pDocTemplate);

	// メイン MDI フレーム ウィンドウを作成
	CMainFrame* pMainFrame = new CMainFrame;
	if(!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// ドラッグ&ドロップを許可
	pMainFrame->DragAcceptFiles(TRUE);

	// プラグインを読み込む
	LoadPlugin();

	// すべての引数をファイルとみなして開く（オプションに対応していない）
	for(int i = 1; i < __argc; i++)
	{
		OpenDocumentFile(__targv[i]);
	}

	return TRUE;
}

int CPOPApp::ExitInstance() 
{
	// プラグインを開放
	FreePlugin();

	// 設定を保存
	setting_save(m_setting_category, m_setting);

	// リソースハンドルを開放
	if(m_hResource != NULL)
	{
		::FreeLibrary(m_hResource);
		m_hResource = NULL;
	}

	return CWinApp::ExitInstance();
}

// ヘルプファイルの呼び出し
void CPOPApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	HH_FTS_QUERY query = {sizeof(query)};

	// コマンドを取得
	UINT uCommand = HH_DISPLAY_TOPIC;
	switch(nCmd)
	{
	case HELP_CONTEXT:                      // 目次
		uCommand = HH_DISPLAY_TOC;
		break;

	case HELP_INDEX:                        // キーワード
		uCommand = HH_DISPLAY_INDEX;
		break;

	case HELP_FINDER:                       // 検索
		uCommand = HH_DISPLAY_SEARCH;
		dwData = reinterpret_cast<DWORD>(&query);
		break;
	}

	// HTMLヘルプのパス名を作成
	CString filename;
	filename.LoadString(IDS_HELPFILE);

	TCHAR szHelpPath[_MAX_PATH];
	GvxPathAppendToModuleDir(
		AfxGetInstanceHandle(),
		szHelpPath, SIZE_OF_ARRAY(szHelpPath),
		TCHAR_HELP_DIRECTORY,
		(LPCTSTR)filename,
		NULL);

	// HTMLヘルプを呼び出す
	::HtmlHelp(NULL, szHelpPath, uCommand, dwData);
}

// リソースファイルを設定
BOOL CPOPApp::SetResource(LPCTSTR lpszResourceName)
{
	// EXEファイルのフルパスを取得
	TCHAR szExePathName[_MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(), szExePathName, SIZE_OF_ARRAY(szExePathName));

	// 言語パックのフルパスを取得
	TCHAR szResourcePath[_MAX_PATH];
	GvxPathAppendToModuleDir(
		AfxGetInstanceHandle(),
		szResourcePath, SIZE_OF_ARRAY(szResourcePath),
		TCHAR_LANGUAGE_DIRECTORY,
		lpszResourceName,
		NULL);

	// 本体と言語パックのプロダクトバージョンを比較
	VS_FIXEDFILEINFO vffi_this, vffi_resource;
	GvxGetFileVersion(szExePathName , &vffi_this    , NULL, 0, NULL);
	GvxGetFileVersion(szResourcePath, &vffi_resource, NULL, 0, NULL);
	if(vffi_this.dwProductVersionMS != vffi_resource.dwProductVersionMS
	|| vffi_this.dwProductVersionLS != vffi_resource.dwProductVersionLS)
	{
		return FALSE;
	}

	// 言語パックをロード
	m_hResource = ::LoadLibrary(szResourcePath);
	if(szResourcePath == NULL)
	{
		return FALSE;
	}

	// 言語パックをインストール
	AfxSetResourceHandle(m_hResource);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CPOPApp メッセージ ハンドラ

// 「ファイル」→「すべて閉じる」
void CPOPApp::OnFileCloseAll() 
{
	// すべてのドキュメントテンプレートをまわる
	POSITION doctemplate_pos = GetFirstDocTemplatePosition();
	while(doctemplate_pos != NULL)
	{
		CDocTemplate *pDocTemplate = GetNextDocTemplate(doctemplate_pos);

		// すべてのドキュメントをまわる
		POSITION doc_pos = pDocTemplate->GetFirstDocPosition();
		while(doc_pos != NULL)
		{
			CDocument *pDocument = pDocTemplate->GetNextDoc(doc_pos);

			// ドキュメントを閉じる
			if(pDocument->SaveModified())
			{
				pDocument->OnCloseDocument();
			}
		}
	}
}

void CPOPApp::OnUpdateFileCloseAll(CCmdUI* pCmdUI) 
{
	// すべてのドキュメントテンプレートをまわる
	POSITION pos = GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		CDocTemplate *pDocTemplate = GetNextDocTemplate(pos);
		ASSERT(pDocTemplate != NULL);

		// ドキュメントが1つでもあれば選択可能にする
		if(pDocTemplate->GetFirstDocPosition() != NULL)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}

	// ドキュメントが1つもなければ選択不可にする
	pCmdUI->Enable(FALSE);
}

// 「ヘルプ」→「作者のホームページを表示」
void CPOPApp::OnHelpWebpage() 
{
	::ShellExecute(
		NULL, NULL,
		TCHAR_WEBPAGE,
		NULL, NULL,
		SW_SHOWNORMAL);
}

// 「ヘルプ」→「Photo Operatorのバージョン情報」
#include "AboutDlg.h"
void CPOPApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// メニューを最大nMenuMaxCount個取得
void CPOPApp::GetHMenuArray(HMENU hMenuArray[], const int nMenuMaxCount)
{
	ASSERT(nMenuMaxCount > 0);

	// メインフレームウインドウを取得
	CFrameWnd *pFrameWnd = STATIC_DOWNCAST(CFrameWnd, GetMainWnd());
	ASSERT(pFrameWnd != NULL);

	// メインフレームウインドウのメニューを取得
	hMenuArray[0] = pFrameWnd->m_hMenuDefault;

	// ドキュメントテンプレートを取得
	POSITION pos = GetFirstDocTemplatePosition();
	for(int i = 1; i < nMenuMaxCount && pos != NULL; i++)
	{
		CMultiDocTemplate *pMultiDocTemplate = STATIC_DOWNCAST(CMultiDocTemplate, GetNextDocTemplate(pos));
		ASSERT(pMultiDocTemplate != NULL);

		hMenuArray[i] = pMultiDocTemplate->m_hMenuShared;
	}
}

// 設定を反映
void CPOPApp::ReflectSetting()
{
	// すべてのドキュメントテンプレートをまわる
	POSITION doctemplate_pos = GetFirstDocTemplatePosition();
	while(doctemplate_pos != NULL)
	{
		CDocTemplate *pDocTemplate = GetNextDocTemplate(doctemplate_pos);

		// すべてのドキュメントをまわる
		POSITION doc_pos = pDocTemplate->GetFirstDocPosition();
		while(doc_pos != NULL)
		{
			// ドキュメントを更新
			CDocument *pDocument = pDocTemplate->GetNextDoc(doc_pos);
			pDocument->UpdateAllViews(NULL);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// 画像の取り込み

// クリップボードから新規画像を作成
void CPOPApp::NewImageFromClipboard(void)
{
	// クリップボードを開く
	const BOOL clip_status = ::OpenClipboard(NULL);
	if(!clip_status)
	{
		return;
	}

	// クリップボードからビットマップを取得
	HBITMAP hBitmap = reinterpret_cast<HBITMAP>(::GetClipboardData(CF_BITMAP));
	ASSERT(hBitmap != NULL);

	// タイトルを作成
	CString title;
	title.Format(IDS_DOCTITLE_CLIPBOARD, m_nClipboardCount++);

	// イメージを取り込む
	ImportFromHandle(hBitmap, title);

	// 後処理
	::CloseClipboard();
}

// ウインドウから新規画像を作成
void CPOPApp::NewImageFromWindow(HWND hWnd, const BOOL bAltogether, const DWORD dwMilliSeconds)
{
	// ウエイト
	::Sleep(dwMilliSeconds);

	// 取り込むウインドウをアクティブにする
	if(!::IsWindow(hWnd))
	{
		return;
	}
	::SetForegroundWindow(hWnd);
	::UpdateWindow(hWnd);

	// ウインドウハンドルからビットマップを取得
	if(TRUE)
	{
		CBitmap bitmap;
		bitmap.Attach(GetWindowBitmap(hWnd, bAltogether));
		if((HBITMAP)bitmap == NULL)
		{
			return;
		}

		// タイトルを作成
		CString title;
		title.Format(IDS_DOCTITLE_DISPLAY, m_nDisplayCount++);

		// イメージを取り込む
		ImportFromHandle(bitmap, title);
	}
}


////////////////////////////////////////////////////////////////////////////////
// プラグイン関連

// hModuleで与えられたプラグインをインストール
BOOL CPOPApp::InstallPlugin(HMODULE hModule, HKEY hKeyPlugin)
{
	PLUGIN_INFO plugin;
	if(!ReadPluginInfo(hModule, plugin, hKeyPlugin))
	{
		// プラグインハンドルを開放
		::FreeLibrary(hModule);
		return FALSE;
	}

	// インストール
	const int filter_count = LoadPluginFilter(hModule);
	const int format_count = LoadPluginFormat(hModule);

	// プラグイン関数が含まれていなかった場合
	if(filter_count == -1
	&& format_count == -1)
	{
		// プラグインハンドルを開放
		::FreeLibrary(hModule);
		return FALSE;
	}

	// プラグインハンドルをスタックに積む
	m_plugin_handle_stack.push(hModule);

	m_plugin_info.push_back(plugin);
	m_plugin_count++;

	return TRUE;
}

// 「開く」メニューで使用するファイルフィルタを取得
CString CPOPApp::GetFileFilterLoad(void) const
{
	CString filter;
	filter.Format(
		IDS_FORMAT_OPEN,                        // %sすべての画像ﾌｧｲﾙ%c%s%cすべてのﾌｧｲﾙ(*.*)%c*.*%c%c
		m_file_filter_load,
		TCHAR_FILTER_DELIMITER,
		m_file_filter_load_all,
		TCHAR_FILTER_DELIMITER, TCHAR_FILTER_DELIMITER, TCHAR_FILTER_DELIMITER, TCHAR_FILTER_DELIMITER);

	return filter;
}

// 「保存」メニューで使用するファイルフィルタを取得
CString CPOPApp::GetFileFilterSave(void) const
{
	return m_file_filter_save;
}


////////////////////////////////////////////////////////////////////////////////
// private関数

// ビットマップハンドルから画像を取り込む
void CPOPApp::ImportFromHandle(HBITMAP hBitmap, LPCTSTR lpszTitle)
{
	// CPOPDoc のドキュメントテンプレートを取得
	POSITION doctemplate_pos   = GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = GetNextDocTemplate(doctemplate_pos);

	// 新規ドキュメントを作成
	CDocument *pDocument = pDocTemplate->CreateNewDocument();
	CPOPDoc *pPOPDoc = STATIC_DOWNCAST(CPOPDoc, pDocument);
	ASSERT(pPOPDoc != NULL);

	// 新規作成に成功した場合
	if(pPOPDoc->PasteAsNew(hBitmap))
	{
		// タイトルを設定
		pPOPDoc->SetTitle(lpszTitle);

		// 新規フレームを作成
		CFrameWnd *pFrameWnd = pDocTemplate->CreateNewFrame(pPOPDoc, NULL);
		pDocTemplate->InitialUpdateFrame(pFrameWnd, pPOPDoc);
	}
	// 失敗した場合
	else
	{
		// ドキュメントテンプレートリストから削除
		pDocTemplate->RemoveDocument(pPOPDoc);

		// 作成したドキュメントを破棄
		pPOPDoc->DeleteContents();
		if(pPOPDoc->m_bAutoDelete)
		{
			delete pPOPDoc;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// プラグイン

// hModuleで与えられたプラグインの情報を取得
BOOL CPOPApp::ReadPluginInfo(HMODULE hModule, PLUGIN_INFO &plugin, HKEY hKeyPlugin)
{
	GETPRODUCT  get_product  = reinterpret_cast<GETPRODUCT >(::GetProcAddress(hModule, funcname_get_product));
	SETREGISTRY set_registry = reinterpret_cast<SETREGISTRY>(::GetProcAddress(hModule, funcname_set_registry));
	SETUP       setup        = reinterpret_cast<SETUP      >(::GetProcAddress(hModule, funcname_setup));
	if(get_product == NULL || set_registry == NULL)
	{
		return FALSE;
	}

	// プロダクト情報を取得
	const UINT author_max      = SIZE_OF_ARRAY(plugin.author);
	const UINT description_max = SIZE_OF_ARRAY(plugin.description);
	char author[author_max];
	char description[description_max];
	const DWORD version = get_product(
		author     , author_max,
		description, description_max);

	// TCHAR文字列に変換
	GvxCHAR2TCHAR(author     , plugin.author     , author_max);
	GvxCHAR2TCHAR(description, plugin.description, description_max);

	// バージョン情報を格納
	plugin.version = version;

	// ファイル名を格納
	TCHAR path[_MAX_PATH], fname[_MAX_FNAME], ext[_MAX_EXT];
	::GetModuleFileName(hModule, path, SIZE_OF_ARRAY(path));
	_tsplitpath(path, NULL, NULL, fname, ext);
	_tmakepath(plugin.filename, NULL, NULL, fname, ext);

	// レジストリの設定
	set_registry(hKeyPlugin);

	// セットアップ関数を取得
	plugin.setup = reinterpret_cast<SETUP>(setup);

	return TRUE;
}

// プラグインを読み込む
void CPOPApp::LoadPlugin()
{
	HMENU hMenuArray[2];
	GetHMenuArray(hMenuArray, SIZE_OF_ARRAY(hMenuArray));

	// 変数の初期化
	if(TRUE)
	{
		m_plugin_count = 0;
		m_plugin_format_count = 0;
		m_plugin_filter_count = 0;
		m_plugin_format_detail_count = 0;
		m_plugin_filter_detail_count = 0;
	}

	// フォーマットプラグインメニューを取得
	m_hFormatMenuMainframe = GvxFindMenu(hMenuArray[0], ID_SETTING_ENVIRONMENT, NULL);
	m_hFormatMenuPoptype   = GvxFindMenu(hMenuArray[1], ID_SETTING_ENVIRONMENT, NULL);
	ASSERT(m_hFormatMenuMainframe != NULL);
	ASSERT(m_hFormatMenuPoptype != NULL);

	// フィルタプラグイン用メニューを取得
	m_hFilterMenu = GvxFindMenu(hMenuArray[1], ID_FILTER_REAPPLY, NULL);
	ASSERT(m_hFilterMenu != NULL);

	// 組み込みフォーマットをロード
	if(TRUE)
	{
		const int format_count = LoadPluginFormat(AfxGetInstanceHandle());
		ASSERT(format_count != -1);
	}

	// プラグインをロード
	if(TRUE)
	{
		// プラグインの設定保存用レジストリ
		HKEY hKeyPlugin = NULL;
		::RegCreateKey(HKEY_CURRENT_USER, STRING_REG_SECTION_PLUGIN, &hKeyPlugin);

		// プラグインを検索
		TCHAR szFindPath[_MAX_PATH];
		GvxPathAppendToModuleDir(
			AfxGetInstanceHandle(),
			szFindPath, SIZE_OF_ARRAY(szFindPath),
			TCHAR_PLUGIN_DIRECTORY,
			TCHAR_PLUGIN_WILDCARD,
			NULL);

		CFileFind find;
		BOOL bFound = find.FindFile(szFindPath);

		while(bFound)
		{
			bFound = find.FindNextFile();

			// 見つかったプラグインをロード
			HMODULE hModule = ::LoadLibrary(find.GetFilePath());
			if(hModule != NULL)
			{
				InstallPlugin(hModule, hKeyPlugin);
			}
		}

		// レジストリキーを閉じる
		::RegCloseKey(hKeyPlugin);
	}

	// 「開く」ダイアログボックスのフィルタインデックス
	m_nFilterIndex = m_plugin_format_detail_count + 1;
}

// プラグインを開放
void CPOPApp::FreePlugin()
{
	// プラグインハンドルを解放
	while(!m_plugin_handle_stack.empty())
	{
		// スタックの先頭からプラグインハンドルを取り出して開放
		HMODULE hModule = m_plugin_handle_stack.top();
		::FreeLibrary(hModule);

		// スタックを1つ減らす
		m_plugin_handle_stack.pop();
	}
	m_plugin_filter.clear();        m_plugin_filter_count = 0;
	m_plugin_filter_detail.clear(); m_plugin_filter_detail_count = 0;
	m_plugin_format.clear();        m_plugin_format_count = 0;
	m_plugin_format_detail.clear(); m_plugin_format_detail_count = 0;
	m_plugin_info.clear();          m_plugin_count = 0;
}

// フィルタプラグインを読み込む
int CPOPApp::LoadPluginFilter(HMODULE hModule)
{
	// プラグイン内の関数を取得
	FILTERFUNC_COUNT      get_count  = reinterpret_cast<FILTERFUNC_COUNT     >(::GetProcAddress(hModule, funcname_filter_count));
	FILTERFUNC_INFO       get_info   = reinterpret_cast<FILTERFUNC_INFO      >(::GetProcAddress(hModule, funcname_filter_info));
	FILTERFUNC_EXECUTABLE executable = reinterpret_cast<FILTERFUNC_EXECUTABLE>(::GetProcAddress(hModule, funcname_filter_executable));
	FILTERFUNC_EFFECT     effect     = reinterpret_cast<FILTERFUNC_EFFECT    >(::GetProcAddress(hModule, funcname_filter_effect));
	FILTERFUNC_OPTION     option     = reinterpret_cast<FILTERFUNC_OPTION    >(::GetProcAddress(hModule, funcname_filter_option));

	// プラグインに含まれている関数を確認
	if(get_count == NULL || get_info == NULL || executable == NULL || effect == NULL || option == NULL)
	{
		return -1;
	}

	// 含まれているフィルタの数だけ繰り返す
	const int filter_count_available = MAX_FILTER_COUNT - m_plugin_filter_count;
	const int filter_count           = min(get_count(), filter_count_available);
	for(int i = 0; i < filter_count; i++)
	{
		FILTERPLUGIN_DETAIL filter_detail;
		filter_detail.plugin_no = m_plugin_filter_count;    // どのプラグインの
		filter_detail.filter_no = i;                        // 何番目か

		// プラグインの情報を取得
		char genre[20];                         // プラグインのジャンル
		char name[40];                          // プラグイン名
		filter_detail.function_flag = get_info(
			i,
			genre, SIZE_OF_ARRAY(genre),
			name , SIZE_OF_ARRAY(name));

		// TCHAR文字列に変換
		GvxCHAR2TCHAR(genre, filter_detail.genre, SIZE_OF_ARRAY(filter_detail.genre));
		GvxCHAR2TCHAR(name , filter_detail.name , SIZE_OF_ARRAY(filter_detail.name));

		// ジャンル名のサブメニューがなかったら新しく作り、フィルタメニューの最後に追加
		HMENU &hSubMenu = m_plugin_filter_map[filter_detail.genre];
		if(hSubMenu == NULL)
		{
			hSubMenu = ::CreatePopupMenu();
			ASSERT(hSubMenu != NULL);
			::AppendMenu(
				m_hFilterMenu,
				MF_POPUP | MF_STRING,
				reinterpret_cast<UINT>(hSubMenu),
				filter_detail.genre);
		}
		// サブメニューの最後にフィルタ名のメニューを追加
		::AppendMenu(
			hSubMenu,
			MF_STRING,
			ID_FILTER_MIN + m_plugin_filter_detail_count,
			filter_detail.name);

		// フィルタリストに追加
		m_plugin_filter_detail.push_back(filter_detail);
		m_plugin_filter_detail_count++;
	}

	if(filter_count > 0)
	{
		FILTERPLUGIN plugin;
		plugin.count      = filter_count;
		plugin.executable = executable;
		plugin.effect     = effect;
		plugin.option     = option;

		// プラグインリストに追加
		m_plugin_filter.push_back(plugin);
		m_plugin_filter_count++;
	}

	return filter_count;
}

// フォーマットプラグインを読み込む
int CPOPApp::LoadPluginFormat(HMODULE hModule)
{
	// プラグイン内の関数を取得
	FORMATFUNC_COUNT  get_count     = reinterpret_cast<FORMATFUNC_COUNT >(::GetProcAddress(hModule, funcname_format_count));
	FORMATFUNC_INFO   get_info      = reinterpret_cast<FORMATFUNC_INFO  >(::GetProcAddress(hModule, funcname_format_info));
	FORMATFUNC_BITS   get_bits      = reinterpret_cast<FORMATFUNC_BITS  >(::GetProcAddress(hModule, funcname_format_support_mode));
	FORMATFUNC_IMPORT format_import = reinterpret_cast<FORMATFUNC_IMPORT>(::GetProcAddress(hModule, funcname_format_import));
	FORMATFUNC_EXPORT format_export = reinterpret_cast<FORMATFUNC_EXPORT>(::GetProcAddress(hModule, funcname_format_export));
	FORMATFUNC_OPTION format_option = reinterpret_cast<FORMATFUNC_OPTION>(::GetProcAddress(hModule, funcname_format_option));

	// プラグインに含まれている関数を確認
	if(get_count == NULL || get_info == NULL || get_bits == NULL || format_import == NULL || format_export == NULL || format_option == NULL)
	{
		return -1;
	}

	// 含まれているフォーマットの数だけ繰り返す
	const int format_count_available = MAX_FORMAT_COUNT - m_plugin_format_count;
	const int format_count           = min(get_count(), format_count_available);
	for(int i = 0; i < format_count; i++)
	{
		FORMATPLUGIN_DETAIL detail;
		detail.plugin_no = m_plugin_format_count;     // どのプラグインの
		detail.format_no = i;                         // 何番目か

		// プラグインの情報を取得
		char name[10];
		char ext[10];
		char filter[50];
		char expression[50];
		detail.included_functions = get_info(
			i,
			name      , SIZE_OF_ARRAY(name),
			ext       , SIZE_OF_ARRAY(ext),
			filter    , SIZE_OF_ARRAY(filter),
			expression, SIZE_OF_ARRAY(expression));

		// TCHAR文字列に変換
		GvxCHAR2TCHAR(name      , detail.name      , SIZE_OF_ARRAY(detail.name));
		GvxCHAR2TCHAR(ext       , detail.ext       , SIZE_OF_ARRAY(detail.ext));
		GvxCHAR2TCHAR(filter    , detail.filter    , SIZE_OF_ARRAY(detail.filter));
		GvxCHAR2TCHAR(expression, detail.expression, SIZE_OF_ARRAY(detail.expression));

		detail.support_mode = get_bits(i);

		// メニューの最後にフォーマットメニューの文字列を追加
		const UINT menu_id = ID_SETTING_FORMAT_MIN + m_plugin_format_detail_count;
		CString setting_string;
		setting_string.Format(IDS_FORMAT_SETTING, detail.name);
		::AppendMenu(m_hFormatMenuMainframe, MF_STRING, menu_id, setting_string);
		::AppendMenu(m_hFormatMenuPoptype  , MF_STRING, menu_id, setting_string);

		// 拡張子を連結
		if(detail.included_functions & IF_IMPORT)
		{
			m_file_filter_load = m_file_filter_load
				+ detail.expression
				+ TCHAR_FILTER_DELIMITER
				+ detail.filter
				+ TCHAR_FILTER_DELIMITER;

			m_file_filter_load_all = m_file_filter_load_all
				+ detail.filter
				+ TCHAR_FILTER_EXT_DELIMITER;
		}
		if(detail.included_functions & IF_EXPORT)
		{
			m_file_filter_save = m_file_filter_save
				+ detail.expression
				+ TCHAR_FILTER_DELIMITER
				+ detail.filter
				+ TCHAR_FILTER_DELIMITER;
		}

		// フォーマットリストに追加
		m_plugin_format_detail.push_back(detail);
		m_plugin_format_detail_count++;
	}

	if(format_count > 0)
	{
		FORMATPLUGIN plugin;
		plugin.count  = format_count;
		plugin.import = format_import;
		plugin.export = format_export;
		plugin.option = format_option;

		// プラグインリストに追加
		m_plugin_format.push_back(plugin);
		m_plugin_format_count++;
	}

	// 含まれているプラグインの数を返す
	return format_count;
}
