// POPDoc.cpp : CPOPDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "POP.h"

#include "POPDoc.h"
#include "POPView.h"

#include "operation.h"
#include "quant.h"

#include "QuantDlg.h"
#include "ResizeDlg.h"
#include "TrimmingDlg.h"

#include "winutil.h"                    // GvxPathAppendToModuleDir(), GvxCHAR2TCHAR()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPOPDoc

IMPLEMENT_DYNCREATE(CPOPDoc, CDocument)

BEGIN_MESSAGE_MAP(CPOPDoc, CDocument)
	//{{AFX_MSG_MAP(CPOPDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO_CLEAR, OnEditUndoClear)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO_CLEAR, OnUpdateEditUndoClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_OPERATION_ALPHA_APPEND, OnOperationAlphaAppend)
	ON_COMMAND(ID_OPERATION_ALPHA_INVERSE, OnOperationAlphaInverse)
	ON_COMMAND(ID_OPERATION_RESIZE, OnOperationResize)
	ON_COMMAND(ID_OPERATION_TRIMMING, OnOperationTrimming)
	ON_COMMAND(ID_OPERATION_QUANTIZE, OnOperationQuantize)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_ALPHA_APPEND, OnUpdateOperationAlphaAppend)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_QUANTIZE, OnUpdateOperationQuantize)
	ON_COMMAND(ID_FILTER_REAPPLY, OnFilterReapply)
	ON_COMMAND(ID_FILTER_RESUBMIT, OnFilterResubmit)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_OPERATION_COLORMODE_GRAYSCALE, ID_OPERATION_COLORMODE_RGB, OnOperationColormode)
	ON_COMMAND_RANGE(ID_OPERATION_ALPHA_0, ID_OPERATION_ALPHA_255, OnOperationAlphaFill)
	ON_COMMAND_RANGE(ID_OPERATION_TURN_FLIP, ID_OPERATION_TURN_270, OnOperationTurn)
	ON_COMMAND_RANGE(ID_OPERATION_SWAP_GRB, ID_OPERATION_SWAP_GBR, OnOperationSwap)
	ON_COMMAND_RANGE(ID_OPERATION_SELECT_RECT, ID_OPERATION_SELECT_AUTO, OnOperationSelect)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPERATION_COLORMODE_GRAYSCALE, ID_OPERATION_COLORMODE_RGB, OnUpdateOperationColormode)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPERATION_ALPHA_0, ID_OPERATION_ALPHA_INVERSE, OnUpdateOperationAlpha)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPERATION_SWAP_GRB, ID_OPERATION_SWAP_GBR, OnUpdateOperationSwap)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPERATION_SELECT_RECT, ID_OPERATION_SELECT_AUTO, OnUpdateOperationSelect)
	ON_COMMAND_RANGE(ID_FILTER_MIN, ID_FILTER_MAX, OnFilterCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILTER_REAPPLY, ID_FILTER_RESUBMIT, OnUpdateFilterRedo)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILTER_MIN, ID_FILTER_MAX, OnUpdateFilterCommand)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// クラス内static変数
int CPOPDoc::m_select_type    = 0;
int CPOPDoc::m_last_filter_no = -1;


/////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// 新規イメージ用パラメータ
	NEWIMAGEPARAM g_new_param = {128, 128, 0x00ffffff};

	// サイズ変更用パラメータ
	RESIZEPARAM g_resize_param = {0, 0, IM_LINER};

	// トリミング用パラメータ
	TRIMMINGPARAM g_trimming_param = {0, 0, TRUE, IM_LINER};
END_NAMESPACE()


/////////////////////////////////////////////////////////////////////////////
// CPOPDoc クラスの構築/消滅

CPOPDoc::CPOPDoc()
{
	m_rect.SetRectEmpty();                  // 選択領域を空にする
	m_format = 0;                           // デフォルトの形式は０番目

	// IMAGE_INFO,IMAGE_PLANEの初期設定
	memset(&m_info, 0, sizeof(m_info));
	memset(&m_data, 0, sizeof(m_data));
}

CPOPDoc::~CPOPDoc()
{
}

// 新規作成
BOOL CPOPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// 画像を作成
	if(image_create_empty(&m_info, &m_data, &g_new_param) != IR_SUCCESS)
	{
		AfxMessageBox(AFX_IDP_FAILED_MEMORY_ALLOC, MB_ICONEXCLAMATION);
		return FALSE;
	}

	// アンドゥバッファへ追加
	AddToUndoBuffer(m_info, m_data);
	return TRUE;
}

// 画像を開く
BOOL CPOPDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	try
	{
		CFile file(lpszPathName, CFile::modeRead | CFile::shareDenyWrite);
		CCallBack callback;

		// すべての画像形式で読み込んでみる
		extern CPOPApp theApp;
		const int format_count = theApp.GetPluginFormatDetailCount();
		for(int i = 0; i < format_count; i++)
		{
			const FORMATPLUGIN_DETAIL &format_detail = theApp.GetPluginFormatDetail(i);

			// 読み込み用関数が存在した場合
			if(format_detail.included_functions & IF_IMPORT)
			{
				const FORMATPLUGIN &plugin = theApp.GetPluginFormat(format_detail.plugin_no);

				// マックバイナリを考慮
				for(LONG pos = 0; pos <= 128; pos += 128)
				{
					// ファイルポインタを移動
					file.Seek(pos, CFile::begin);

					const IMAGERESULT result = plugin.import(
						format_detail.format_no,
						reinterpret_cast<HANDLE>(file.m_hFile),
						&m_info, &m_data,
						callback.GetCallBackProc(), callback.GetCallBackData());

					switch(result)
					{
					case IR_SUCCESS:
						m_format = i;
						goto SUCCESS;

					case IR_IMAGEDESTROYED:
						throw IDS_IR_SUCCESS + result;
					}
				}
			}
		}

		// 読み込みに失敗した
		throw IDS_UNABLETOIMPORT;

SUCCESS:
		m_info.colors = image_count_colors(&m_info, &m_data);

		// アンドゥバッファへ追加
		AddToUndoBuffer(m_info, m_data);

		// ドキュメントの変更＝なし
		m_undo.SetModifiedFlag(FALSE);
		UpdateDocument();
		return TRUE;
	}
	catch(int id)
	{
		ReportSaveLoadException(lpszPathName, NULL, FALSE, id);
		return FALSE;
	}
	catch(CFileException *e)
	{
		ASSERT(e != NULL);
		ReportSaveLoadException(lpszPathName, e, FALSE, 0);
		e->Delete();
		return FALSE;
	}
}

// 画像を保存
BOOL CPOPDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	extern CPOPApp theApp;
	
	try
	{
		const FORMATPLUGIN_DETAIL &format_detail = theApp.GetPluginFormatDetail(m_format);
		ASSERT(format_detail.included_functions & IF_EXPORT);

		// ファイルを開く
		CFile file(lpszPathName, CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive);

		// 現在の形式に最適なカラーモードを求める
		const int new_mode = GetOptimumColormode(format_detail.support_mode);
		if(new_mode == -1)
		{
			throw IDS_NOEXPORTABLEFORMAT;
		}

		// カラーモードを変更
		if(new_mode != m_info.mode)
		{
			IMAGE_INFO new_info;
			IMAGE_DATA new_data;
			image_convert_colormode(&m_info, &m_data, &new_info, &new_data, new_mode, NULL, 0);

			// アンドゥバッファへ追加
			AddToUndoBuffer(new_info, new_data);
		}

		// コメントをANSI文字に変換
		char comment_ansi[256];
		if(TRUE)
		{
			SETTING setting;
			theApp.GetSetting(setting);
			if(setting.basic.check.elements.implant_comment && !setting.basic.comment.IsEmpty())
			{
				GvxTCHAR2CHAR(setting.basic.comment, comment_ansi, SIZE_OF_ARRAY(comment_ansi));
				m_info.comment = comment_ansi;
			}
			else
			{
				m_info.comment = NULL;
			}
		}

		// イメージを書き出す
		if(TRUE)
		{
			const FORMATPLUGIN &plugin = theApp.GetPluginFormat(format_detail.plugin_no);

			CCallBack callback;
			const IMAGERESULT result = plugin.export(
				format_detail.format_no,
				reinterpret_cast<HANDLE>(file.m_hFile),
				&m_info, &m_data,
				callback.GetCallBackProc(), callback.GetCallBackData());

			if(result != IR_SUCCESS)
			{
				throw IDS_IR_SUCCESS + result;
			}
		}

		// ドキュメントの変更＝なし
		m_undo.SetModifiedFlag(FALSE);
		UpdateDocument();

		return TRUE;
	}
	catch(int id)
	{
		ReportSaveLoadException(lpszPathName, NULL, TRUE, id);
		return FALSE;
	}
	catch(CFileException *e)
	{
		ASSERT(e != NULL);
		ReportSaveLoadException(lpszPathName, e, TRUE, 0);
		e->Delete();
		return FALSE;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CPOPDoc クラスの診断

#ifdef _DEBUG
void CPOPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPOPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPOPDoc コマンド

// 「ファイル」→「新規作成」
#include "FileNewDlg.h"
void CPOPApp::OnFileNew() 
{
	CFileNewDlg dlg(g_new_param);

	// 新規イメージのサイズを問い合わせる
	if(dlg.DoModal() == IDOK)
	{
		CWinApp::OnFileNew();
	}
}

// 「ファイル」→「開く」
void CPOPApp::OnFileOpen() 
{
	// ファイル名を格納するバッファ
	TCHAR buffer[1 << 10];
	buffer[0] = TCHAR_CTRL_NIL;

	// フィルタのデリミタを'|'から'\0'に変換
	CString filter = GetFileFilterLoad();
	filter.Replace(TCHAR_FILTER_DELIMITER, TCHAR_CTRL_NIL);

	// OPENFILENAME構造体の設定
	OPENFILENAME ofn = {sizeof(OPENFILENAME)};
	ofn.hwndOwner    = AfxGetMainWnd()->m_hWnd;
	ofn.lpstrFilter  = filter;
	ofn.nFilterIndex = m_nFilterIndex;
	ofn.lpstrFile    = buffer;
	ofn.nMaxFile     = SIZE_OF_ARRAY(buffer);
	ofn.Flags        = OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	// ダイアログを表示
	if(::GetOpenFileName(&ofn))
	{
		// ファイル名のオフセットを取得
		int pos = ofn.nFileOffset;

		while(buffer[pos] != TCHAR_CTRL_NIL)
		{
			// 指定されたファイルを開く
			LPCTSTR lpszFileName = &buffer[pos];
			OpenDocumentFile(lpszFileName);

			// 次のファイル名の位置
			pos += _tcslen(lpszFileName) + 1;
		}
		m_nFilterIndex = ofn.nFilterIndex;
	}
}

// 「ファイル」→「保存」
void CPOPDoc::OnFileSave() 
{
	extern CPOPApp theApp;

	// ファイル名の存在をチェック
	const CString &pathname = GetPathName();
	if(pathname.IsEmpty())
	{
		// 存在しなければ名前をつけて保存
		OnFileSaveAs();
		return;
	}

	// 保存用関数の存在をチェック
	const FORMATPLUGIN_DETAIL &format_detail = theApp.GetPluginFormatDetail(m_format);
	if((format_detail.included_functions & IF_EXPORT) != IF_EXPORT)
	{
		// 存在しなければ名前をつけて保存
		ReportSaveLoadException(pathname, NULL, TRUE, IDS_UNABLETOEXPORT);
		OnFileSaveAs();
		return;
	}
	OnSaveDocument(pathname);
	return;
}

// 「ファイル」→「名前をつけて保存」
void CPOPDoc::OnFileSaveAs() 
{
	extern CPOPApp theApp;

	// フィルタインデックスを求める
	DWORD index = 0;
	for(int i = 0; i <= m_format; i++)
	{
		const FORMATPLUGIN_DETAIL &format_detail = theApp.GetPluginFormatDetail(i);
		if(format_detail.included_functions & IF_EXPORT)
		{
			index++;
		}
	}

	// ファイル名を格納するバッファ
	TCHAR buffer[1 << 10];
	buffer[0] = TCHAR_CTRL_NIL;

	// フィルタのデリミタを'|'から'\0'に変換
	CString filter = theApp.GetFileFilterSave();
	filter.Replace(TCHAR_FILTER_DELIMITER, TCHAR_CTRL_NIL);

	// OPENFILENAME構造体の設定
	OPENFILENAME ofn = {sizeof(OPENFILENAME)};
	ofn.hwndOwner    = AfxGetMainWnd()->m_hWnd;
	ofn.lpstrFilter  = filter;
	ofn.nFilterIndex = index;
	ofn.lpstrFile    = buffer;
	ofn.nMaxFile     = SIZE_OF_ARRAY(buffer);
	ofn.Flags        = OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_OVERWRITEPROMPT | OFN_EXPLORER;

	// ダイアログを表示
	if(!::GetSaveFileName(&ofn))
	{
		return;
	}

	// 対応するフォーマットを求める
	DWORD save_plugin_count = 0;

	const int plugin_count = theApp.GetPluginFormatDetailCount();
	for(int i = 0; i < plugin_count; i++)
	{
		const FORMATPLUGIN_DETAIL &format_detail = theApp.GetPluginFormatDetail(i);
		if((format_detail.included_functions & IF_EXPORT) != IF_EXPORT)
		{
			continue;
		}

		save_plugin_count++;
		if(save_plugin_count != ofn.nFilterIndex)
		{
			continue;
		}

		// 対応フォーマットが見つかった
		m_format = i;
		::PathRenameExtension(buffer, format_detail.ext);
		break;
	}

	ASSERT(save_plugin_count == ofn.nFilterIndex);

	// ドキュメントを保存
	if(OnSaveDocument(buffer))
	{
		SetPathName(buffer);                    // ファイル名を変更
		SetModifiedFlag(FALSE);                 // ダーティフラグをクリア
	}
}

// 「ファイル」→「すべて保存」
void CPOPDoc::OnFileSaveAll() 
{
	// ドキュメントテンプレートを取得
	CDocTemplate *pDocTemplate = GetDocTemplate();

	// すべてのドキュメントに対して繰り返す
	POSITION pos = pDocTemplate->GetFirstDocPosition();
	while(pos != NULL)
	{
		// ドキュメントクラスへのポインタを取得
		CPOPDoc *pPOPDoc = STATIC_DOWNCAST(CPOPDoc, pDocTemplate->GetNextDoc(pos));
		ASSERT(pPOPDoc != NULL);

		// ドキュメントを保存
		pPOPDoc->OnFileSave();
	}
}

void CPOPDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// ダーティーフラグが立っていたら選択可
	pCmdUI->Enable(IsModified());
}

// 「ファイル」→「取り込み」→「クリップボードから」
void CPOPApp::OnFileImportClipboard() 
{
	NewImageFromClipboard();
}

void CPOPApp::OnUpdateFileImportClipboard(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_BITMAP));
}

// 「ファイル」→「取り込み」→「画面から」
#include "FileImportDisplayDlg.h"
#include "ImportWindowDlg.h"
void CPOPApp::OnFileImportDisplay() 
{
	////////////////////////////////////////////////////////////
	// 取り込む領域（デスクトップ／ウインドウ全体／ウインドウの内側）を決定する
	CFileImportDisplayDlg dlgImport;
	dlgImport.m_nArea   = 1;
	dlgImport.m_nTiming = 0;
	if(dlgImport.DoModal() != IDOK)
	{
		return;
	}

	////////////////////////////////////////////////////////////
	// 取り込むウインドウを決定する

	// デスクトップ全体
	if(dlgImport.m_nArea == 0)
	{
		HWND hWndCapture = ::GetDesktopWindow();
		BOOL bAltogether = TRUE;
		NewImageFromWindow(hWndCapture, bAltogether, dlgImport.m_nTiming * 1000);
		return;
	}
	// ウインドウ全体or内側
	else
	{
		// 取り込むウインドウを選択
		CImportWindowDlg *pDlgSelect = new CImportWindowDlg;
		pDlgSelect->m_bAltogether = (dlgImport.m_nArea == 2) ? FALSE : TRUE;
		pDlgSelect->m_dwMilliSeconds = dlgImport.m_nTiming * 1000;

		// モードレスダイアログを作成
		pDlgSelect->Create(IDD_IMPORT_WINDOW);
		pDlgSelect->ShowWindow(SW_SHOW);
		return;
	}
}


////////////////////////////////////////
// 補助関数

// ドキュメントがクローズされる前に呼び出される
BOOL CPOPDoc::SaveModified() 
{
	// ドキュメントが変更されていなければクローズ可
	if(!IsModified())
	{
		return TRUE;
	}

	// プロンプトを表示（○○への変更を保存しますか?）
	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, GetTitle());
	switch(AfxMessageBox(prompt, MB_YESNOCANCEL))
	{
	case IDYES:                             // 「はい」
		OnFileSaveAs();                         // 名前を付けて保存
		return !IsModified();

	case IDNO:                              // 「いいえ」
		return TRUE;                            // クローズ可

	case IDCANCEL:                          // キャンセル
		return FALSE;                           // クローズ不可
	}

	return TRUE;
}

// アンドゥバッファへアンドゥ情報を追加
bool CPOPDoc::AddToUndoBuffer(const IMAGE_INFO &info, const IMAGE_DATA &data)
{
	// アンドゥ情報を追加
	if(TRUE)
	{
		UNDOINFO undoinfo;
		undoinfo.info = info;
		undoinfo.data = data;

		if(!m_undo.AddUndoInfo(undoinfo))
		{
			// 画像データを開放
			if(!binary_equal(m_data, undoinfo.data))
			{
				image_free(&undoinfo.data);
			}
			return false;
		}

		// 追加に成功したら、画像情報を更新
		m_info = undoinfo.info;
		m_data = undoinfo.data;
	}

	// ドキュメントを更新
	UpdateDocument();

	return true;
}

// ドキュメントが変更された
void CPOPDoc::UpdateDocument()
{
	// ダーティーフラグの設定
	SetModifiedFlag(m_undo.IsModified());

	// ビューを更新し、ウインドウをリサイズ
	UpdateAllViews(NULL);
}

// サポートしているカラーモードのなかで最適なものを求める
int CPOPDoc::GetOptimumColormode(uint_t support_mode)
{
	const int support_mask[] = {SUPPORT_GRAYSCALE, SUPPORT_INDEX, SUPPORT_RGB};

	// 精度の高いほうを検索
	if(TRUE)
	{
		for(int new_mode = m_info.mode; new_mode <= COLORMODE_RGB; new_mode++)
		{
			if(support_mode & support_mask[new_mode])
			{
				return new_mode;
			}
		}
	}

	// 精度の低いほうを検索
	if(TRUE)
	{
		for(int new_mode = m_info.mode; new_mode >= COLORMODE_GRAYSCALE; new_mode--)
		{
			if(support_mode & support_mask[new_mode])
			{
				return new_mode;
			}
		}
	}

	return -1;
}

// クリップボード内のビットマップを新規ドキュメントとする
BOOL CPOPDoc::PasteAsNew(HBITMAP hBitmap)
{
	IMAGE_INFO new_info;
	IMAGE_DATA new_data;
	if(HBITMAP_GetImage(hBitmap, &new_info, &new_data) != IR_SUCCESS)
	{
		return FALSE;
	}

	new_info.colors = image_count_colors(&new_info, &new_data);

	// アンドゥバッファへ追加
	AddToUndoBuffer(new_info, new_data);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// アンドゥ／リドゥ

// アンドゥ
void CPOPDoc::OnEditUndo() 
{
	if(TRUE)
	{
		UNDOINFO info;
		m_undo.Undo(info);

		m_info = info.info;
		m_data = info.data;
	}

	UpdateDocument();
}

// リドゥ
void CPOPDoc::OnEditRedo() 
{
	if(TRUE)
	{
		UNDOINFO info;
		m_undo.Redo(info);

		m_info = info.info;
		m_data = info.data;
	}

	UpdateDocument();
}

// アンドゥメモリのクリア
void CPOPDoc::OnEditUndoClear() 
{
	const int return_id = AfxMessageBox(IDS_UNDOCLEAR, MB_OKCANCEL | MB_ICONQUESTION);
	if(return_id == IDOK)
	{
		m_undo.ClearMemory();
	}
}

// カット
void CPOPDoc::OnEditCut() 
{
	// カット＝コピー＋クリア
	OnEditCopy();
	OnEditClear();
}

// コピー
void CPOPDoc::OnEditCopy() 
{
	// 現在の画像からビットマップハンドルを作成
	HBITMAP hBitmap = HBITMAP_CreateFromImage(&m_info, &m_data);
	if(hBitmap == NULL)
	{
		return;
	}

	// とりあえず画像全部をクリップボードへコピー
	::OpenClipboard(NULL);
	::EmptyClipboard();
	::SetClipboardData(CF_BITMAP, hBitmap);
	::CloseClipboard();
}

// ペースト
void CPOPDoc::OnEditPaste() 
{
}

// クリア
void CPOPDoc::OnEditClear() 
{
}

// すべて選択
void CPOPDoc::OnEditSelectAll() 
{
	// 選択領域を更新
	m_rect.SetRect(0, 0, m_info.width, m_info.height);
	UpdateDocument();
}

// アンドゥのUPDATE_COMMAND_UIメッセージハンドラ
void CPOPDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_undo.IsAbleToUndo());
}

// リドゥのUPDATE_COMMAND_UIメッセージハンドラ
void CPOPDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_undo.IsAbleToRedo());
}

// アンドゥメモリのクリアのUPDATE_COMMAND_UIメッセージハンドラ
void CPOPDoc::OnUpdateEditUndoClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_undo.IsAbleToUndo() || m_undo.IsAbleToRedo());
}

// カットのUPDATE_COMMAND_UIメッセージハンドラ
void CPOPDoc::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_rect.IsRectEmpty());
}

// コピーのUPDATE_COMMAND_UIメッセージハンドラ
void CPOPDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_rect.IsRectEmpty());
}

// ペーストのUPDATE_COMMAND_UIメッセージハンドラ
void CPOPDoc::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_BITMAP));
}

// クリアのUPDATE_COMMAND_UIメッセージハンドラ
void CPOPDoc::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_rect.IsRectEmpty());
}

/////////////////////////////////////////////////////////////////////////////
// 操作

// 「操作」→「カラーモード」
void CPOPDoc::OnOperationColormode(UINT nID)
{
	const int new_mode = COLORMODE_GRAYSCALE + (nID - ID_OPERATION_COLORMODE_GRAYSCALE);
	if(m_info.mode == new_mode)
	{
		return;
	}

	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	// 処理
	CCallBack callback;
	const IMAGERESULT result = image_convert_colormode(
		&m_info, &m_data,
		&new_info, &new_data,
		new_mode,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// アンドゥバッファへ追加
	AddToUndoBuffer(new_info, new_data);
}

// 「操作」→「アルファ」→「アルファを追加」
void CPOPDoc::OnOperationAlphaAppend() 
{
	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	// 処理
	CCallBack callback;
	const IMAGERESULT result = image_alpha_append(
		&m_info, &m_data,
		&new_info, &new_data,
		!m_info.alpha,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// アンドゥバッファへ追加
	AddToUndoBuffer(new_info, new_data);
}

// 「操作」→「アルファ」→「透明化／不透明化」
void CPOPDoc::OnOperationAlphaFill(UINT nID)
{
	const_uint8_t alpha_val = (nID == ID_OPERATION_ALPHA_0) ? 0 : 255;
	IMAGE_DATA new_data;

	// 処理
	CCallBack callback;
	const IMAGERESULT result = image_alpha_fill(
		&m_info, &m_data, &new_data,
		alpha_val,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// アンドゥバッファへ追加
	AddToUndoBuffer(m_info, new_data);
}

// 「操作」→「アルファ」→「アルファ値を反転」
void CPOPDoc::OnOperationAlphaInverse() 
{
	IMAGE_DATA new_data;

	// 処理
	CCallBack callback;
	const IMAGERESULT result = image_alpha_inverse(
		&m_info, &m_data, &new_data,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// アンドゥバッファへ追加
	AddToUndoBuffer(m_info, new_data);
}

// 「操作」→「回転/反転」
void CPOPDoc::OnOperationTurn(UINT nID)
{
	typedef IMAGERESULT (*TURNFUNC)(const IMAGE_INFO *in_info, const IMAGE_DATA *in_data, IMAGE_INFO *out_info, IMAGE_DATA *out_data, callback_t callback_default, long data);
	TURNFUNC func[] =
	{
		image_flip,
		image_mirror,
		image_turn_90,
		image_turn_180,
		image_turn_270,
	};

	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	// 処理
	CCallBack callback;
	IMAGERESULT result = func[nID - ID_OPERATION_TURN_FLIP](
		&m_info, &m_data,
		&new_info, &new_data,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// アンドゥバッファへ追加
	AddToUndoBuffer(new_info, new_data);
}

// 「操作」→「色の入れ替え」
void CPOPDoc::OnOperationSwap(UINT nID)
{
	typedef struct tagSWAP_COLOR_INFO
	{
		int red;
		int green;
		int blue;
	} SWAP_COLOR_INFO;
	const SWAP_COLOR_INFO swap_color_info_array[] =
	{
		{1, 0, 2},
		{2, 1, 0},
		{0, 2, 1},
		{2, 0, 1},
		{1, 2, 0},
	};

	const SWAP_COLOR_INFO &swap_color_info = swap_color_info_array[nID - ID_OPERATION_SWAP_GRB];
	IMAGE_DATA new_data;

	// 処理
	CCallBack callback;
	IMAGERESULT result = image_swap_colors(
		&m_info, &m_data, &new_data,
		swap_color_info.red, swap_color_info.green, swap_color_info.blue,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// 処理後の画像の色数を数える
	IMAGE_INFO new_info = m_info;
	new_info.colors = image_count_colors(&m_info, &new_data);

	// アンドゥバッファへ追加
	AddToUndoBuffer(new_info, new_data);
}

// 「操作」→「選択」
void CPOPDoc::OnOperationSelect(UINT nID)
{
	m_select_type = nID - ID_OPERATION_SELECT_RECT;
}

// 「操作」→「サイズ変更」
void CPOPDoc::OnOperationResize() 
{
	CResizeDlg dlg;
	dlg.nWidth            = m_info.width;
	dlg.nHeight           = m_info.height;
	dlg.m_bIsAbleToSmooth =(m_info.mode==COLORMODE_GRAYSCALE || m_info.mode==COLORMODE_RGB);

	// OKボタンが押された場合
	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	// パラメータを取り出す
	g_resize_param = dlg.m_param;

	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	// 処理
	CCallBack callback;
	const IMAGERESULT result = image_resize(
		&m_info, &m_data,
		&new_info, &new_data,
		&g_resize_param,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// アンドゥバッファへ追加
	AddToUndoBuffer(new_info, new_data);
}

// 「操作」→「トリミング」
void CPOPDoc::OnOperationTrimming() 
{
	CTrimmingDlg dlg;
	dlg.m_nWidth  = m_info.width;
	dlg.m_nHeight = m_info.height;

	// OKボタンが押された場合
	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	// パラメータを取り出す
	g_trimming_param = dlg.m_param;

	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	// 処理
	CCallBack callback;
	const IMAGERESULT result = image_trimming(
		&m_info, &m_data,
		&new_info, &new_data,
		&g_trimming_param,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// アンドゥバッファへ追加
	AddToUndoBuffer(new_info, new_data);
}

// 「操作」→「減色」
void CPOPDoc::OnOperationQuantize() 
{
	CQuantDlg dlg;
	dlg.m_radioDither = 0;
	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	const QUANT_PALETTE_METHOD palette_method = static_cast<QUANT_PALETTE_METHOD>(dlg.m_radioMethod);
	const QUANT_DITHER_METHOD  dither_method  = static_cast<QUANT_DITHER_METHOD >(dlg.m_radioDither);

	// 処理
	CCallBack callback;
	const IMAGERESULT result = quantize(
		&m_info, &m_data,
		&new_info, &new_data,
		dlg.m_nQuantColors, palette_method, dither_method,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// アンドゥバッファへ追加
	AddToUndoBuffer(new_info, new_data);
}

void CPOPDoc::OnUpdateOperationColormode(CCmdUI* pCmdUI) 
{
	// ラジオボタンチェックマークをつける
	pCmdUI->SetRadio(m_info.mode == COLORMODE_GRAYSCALE + (pCmdUI->m_nID - ID_OPERATION_COLORMODE_GRAYSCALE));
}

void CPOPDoc::OnUpdateOperationAlphaAppend(CCmdUI* pCmdUI) 
{
	// アルファチャネルが存在していたらチェックマークをつける
	pCmdUI->SetCheck(m_info.alpha);
}

void CPOPDoc::OnUpdateOperationAlpha(CCmdUI* pCmdUI)
{
	// 画像にアルファチャネルが追加されていたら選択可
	pCmdUI->Enable(m_info.alpha);
}

void CPOPDoc::OnUpdateOperationSwap(CCmdUI* pCmdUI)
{
	// グレイスケール画像は入れ替え不可
	pCmdUI->Enable(m_info.mode != COLORMODE_GRAYSCALE);
}

// 
void CPOPDoc::OnUpdateOperationSelect(CCmdUI *pCmdUI)
{
	const UINT id = m_select_type + ID_OPERATION_SELECT_RECT;
	pCmdUI->SetRadio(pCmdUI->m_nID == id);
}

void CPOPDoc::OnUpdateOperationQuantize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_info.mode == COLORMODE_RGB);
}


/////////////////////////////////////////////////////////////////////////////
// フィルタ操作

BOOL CPOPDoc::DoFilter(const UINT filter_no, const BOOL bSetup /* = TRUE */)
{
	// 該当するプラグイン関数を取り出す
	extern CPOPApp theApp;
	const FILTERPLUGIN_DETAIL &filter_detail = theApp.GetPluginFilterDetail(filter_no);
	const FILTERPLUGIN &plugin = theApp.GetPluginFilter(filter_detail.plugin_no);

	// オプション関数を実行して偽が返ってきたら、フィルタ処理を中断
	if(bSetup && !plugin.option(filter_detail.filter_no, &m_info, &m_data, AfxGetMainWnd()->m_hWnd))
	{
		return FALSE;
	}

	// 新しいイメージのメモリを確保
	IMAGE_DATA new_data;
	if(!image_alloc(&m_info, &new_data))
	{
		AfxMessageBox(AFX_IDP_FAILED_MEMORY_ALLOC, MB_ICONEXCLAMATION);
		return FALSE;
	}

	// フィルタ処理
	CCallBack callback;
	const IMAGERESULT result = plugin.effect(
		filter_detail.filter_no,
		&m_info, &m_data, &new_data,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		// メモリ開放
		image_free(&new_data);
		return FALSE;
	}

	// 「フィルタの再試行」用のフィルタ番号を保存
	m_last_filter_no = filter_no;

	// 処理後の画像の色数を数える
	IMAGE_INFO new_info = m_info;
	new_info.colors = image_count_colors(&m_info, &new_data);

	// アンドゥバッファへ追加
	AddToUndoBuffer(new_info, new_data);
	return TRUE;
}

// 「フィルタの再適用」（オプションなしで実行）
void CPOPDoc::OnFilterReapply() 
{
	DoFilter(m_last_filter_no, FALSE);
}

// 「フィルタの再実行」（オプションつきで実行）
void CPOPDoc::OnFilterResubmit() 
{
	DoFilter(m_last_filter_no);
}

void CPOPDoc::OnUpdateFilterRedo(CCmdUI* pCmdUI) 
{
	if(m_last_filter_no < 0)
	{
		// 直前のフィルタが登録されていなければ選択不可
		pCmdUI->Enable(FALSE);
	}
	else
	{
		// 該当するプラグイン関数を取り出す
		extern CPOPApp theApp;
		const FILTERPLUGIN_DETAIL &filter_detail = theApp.GetPluginFilterDetail(m_last_filter_no);
		const FILTERPLUGIN &plugin = theApp.GetPluginFilter(filter_detail.plugin_no);

		// 直前のフィルタが実行可能だったら選択可
		pCmdUI->Enable(plugin.executable(filter_detail.filter_no, &m_info));
	}
}

// フィルタ
void CPOPDoc::OnFilterCommand(UINT nID)
{
	DoFilter(nID - ID_FILTER_MIN);
}

void CPOPDoc::OnUpdateFilterCommand(CCmdUI* pCmdUI)
{
	// 該当するプラグイン関数を取り出す
	extern CPOPApp theApp;
	const FILTERPLUGIN_DETAIL &filter_detail = theApp.GetPluginFilterDetail(pCmdUI->m_nID - ID_FILTER_MIN);
	const FILTERPLUGIN &plugin = theApp.GetPluginFilter(filter_detail.plugin_no);

	pCmdUI->Enable(plugin.executable(filter_detail.filter_no, &m_info));
}


/////////////////////////////////////////////////////////////////////////////
// 環境設定

// 「設定」→「環境設定」
#include "SettingDlg.h"
void CPOPApp::OnSettingEnvironment() 
{
	CSettingDlg theDlg;
	theDlg.m_setting_category = m_setting_category;

	// 基本カテゴリの設定
	theDlg.m_bBasicImplantComment      = m_setting.basic.check.elements.implant_comment;
	theDlg.m_strBasicComment           = m_setting.basic.comment;
	theDlg.m_bBasicSaveWindowPlacement = m_setting.basic.check.elements.save_window_placement;
	theDlg.m_nBasicMRUCount            = m_setting.basic.mru_count;

	// 表示カテゴリの設定
	theDlg.m_bDisplayActivateGammaFile     = m_setting.display.check.elements.activate_gamma_file;
	theDlg.m_bDisplayCustomizeGammaDecode  = m_setting.display.check.elements.customize_gamma_decode;
	theDlg.m_dDisplayGammaDecode           = m_setting.display.gamma_decode;
	theDlg.m_bDisplayCustomizeGammaDisplay = m_setting.display.check.elements.customize_gamma_display;
	theDlg.m_dDisplayGammaDisplay          = m_setting.display.gamma_display;
	theDlg.m_nDisplayBackgroundGridSize    = m_setting.display.background_grid_size;
	theDlg.m_color_background1             = m_setting.display.background_color1;
	theDlg.m_color_background2             = m_setting.display.background_color2;
	theDlg.m_bDisplayToolbarXP             = m_setting.display.toolbar_XP;

	// 表示→言語の設定
	theDlg.m_resource_name = m_setting.display.language.resource_name;

	if(theDlg.DoModal() == IDOK)
	{
		m_setting_category = theDlg.m_setting_category;

		// 基本カテゴリの設定を保存
		m_setting.basic.check.elements.implant_comment       = theDlg.m_bBasicImplantComment;
		m_setting.basic.comment                              = theDlg.m_strBasicComment;
		m_setting.basic.check.elements.save_window_placement = theDlg.m_bBasicSaveWindowPlacement;
		m_setting.basic.mru_count                            = theDlg.m_nBasicMRUCount;

		// 表示カテゴリの設定を保存
		m_setting.display.check.elements.activate_gamma_file     = theDlg.m_bDisplayActivateGammaFile;
		m_setting.display.check.elements.customize_gamma_decode  = theDlg.m_bDisplayCustomizeGammaDecode;
		m_setting.display.gamma_decode                           = theDlg.m_dDisplayGammaDecode;
		m_setting.display.check.elements.customize_gamma_display = theDlg.m_bDisplayCustomizeGammaDisplay;
		m_setting.display.gamma_display                          = theDlg.m_dDisplayGammaDisplay;
		m_setting.display.background_grid_size                   = theDlg.m_nDisplayBackgroundGridSize;
		m_setting.display.background_color1                      = theDlg.m_color_background1;
		m_setting.display.background_color2                      = theDlg.m_color_background2;
		m_setting.display.toolbar_XP                             = theDlg.m_bDisplayToolbarXP;

		// 表示→言語の設定を保存
		m_setting.display.language.resource_name = theDlg.m_resource_name;

		// 設定を反映
		ReflectSetting();
	}
}

// 「設定」→「フォーマットごとの設定」
void CPOPApp::OnSettingFormat(UINT nID)
{
	const FORMATPLUGIN_DETAIL &format_detail = GetPluginFormatDetail(nID - ID_SETTING_FORMAT_MIN);
	const FORMATPLUGIN &plugin = GetPluginFormat(format_detail.plugin_no);
	ASSERT(format_detail.included_functions & IF_OPTION);

	plugin.option(format_detail.format_no, AfxGetMainWnd()->m_hWnd);
}

void CPOPApp::OnUpdateSettingFormat(CCmdUI* pCmdUI)
{
	const FORMATPLUGIN_DETAIL &format_detail = GetPluginFormatDetail(pCmdUI->m_nID - ID_SETTING_FORMAT_MIN);
	pCmdUI->Enable(format_detail.included_functions & IF_OPTION);
}
