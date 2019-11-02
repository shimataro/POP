// ToolBarEx.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "POP.h"
#include "ToolBarEx.h"

#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx

BEGIN_MESSAGE_MAP(CToolBarEx, CToolBar)
	//{{AFX_MSG_MAP(CToolBarEx)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(TBN_QUERYINSERT, OnNotifyQueryInsert)
	ON_NOTIFY_REFLECT(TBN_QUERYDELETE, OnNotifyQueryDelete)
	ON_NOTIFY_REFLECT(TBN_BEGINADJUST, OnNotifyBeginAdjust)
	ON_NOTIFY_REFLECT(TBN_ENDADJUST, OnNotifyEndAdjust)
	ON_NOTIFY_REFLECT(TBN_RESET, OnNotifyReset)
	ON_NOTIFY_REFLECT(TBN_TOOLBARCHANGE, OnNotifyToolBarChange)
	ON_NOTIFY_REFLECT(TBN_GETBUTTONINFO, OnNotifyGetButtonInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPOPView クラスの構築/消滅

CToolBarEx::CToolBarEx()
{
	m_nItemCount = -1;
	m_nButtonCount = -1;

	m_pContextMenu = NULL;
	m_pWndOwner    = NULL;
	m_bAutoDestroy = FALSE;
}

CToolBarEx::~CToolBarEx()
{
	SetContextMenu(NULL, NULL);
}

// ツールバーをロード
BOOL CToolBarEx::LoadToolBar(LPCTSTR lpszResourceName)
{
	if(!CToolBar::LoadToolBar(lpszResourceName))
	{
		return FALSE;
	}

	// 初期状態
	if(TRUE)
	{
		CToolBarCtrl &ctrl = GetToolBarCtrl();

		// アイテムの総数をカウント
		m_nItemCount = ctrl.GetButtonCount();
		m_toolbarinfos.SetSize(m_nItemCount);

		// 各ボタンの情報とテキストを取得
		const int item_count = m_nItemCount;
		for(int i = 0; i < item_count; i++)
		{
			// TBBUTTON構造体の取得
			TBBUTTON tbButton;
			ctrl.GetButton(i, &tbButton);
			m_toolbarinfos[i].tbButton = tbButton;

			// テキスト＝'\n'の前の部分
			if(TRUE)
			{
				CString string;
				string.LoadString(tbButton.idCommand);

				AfxExtractSubString(
					m_toolbarinfos[i].strText,
					string,
					0);
			}
		}
	}

	// セパレータを除いたボタンの個数を取得
	if(TRUE)
	{
		m_nButtonCount = 0;

		const int item_count = m_nItemCount;
		for(int i = 0; i < item_count; i++)
		{
			// セパレータでないアイテムの数をカウント
			if(GetItemID(i) != ID_SEPARATOR)
			{
				m_nButtonCount++;
			}
		}
	}

	return TRUE;
}

// コンテキストメニューの設定
void CToolBarEx::SetContextMenu(CMenu *pContextMenu, CWnd *pWndOwner, const BOOL bAutoDestroy /* = FALSE */)
{
	if(m_pContextMenu != NULL && m_bAutoDestroy)
	{
		m_pContextMenu->DestroyMenu();
		m_pContextMenu = NULL;
	}

	m_pContextMenu = pContextMenu;
	m_pWndOwner    = pWndOwner;
	m_bAutoDestroy = bAutoDestroy;
}

// ツールバーのアイコンを置き換え
void CToolBarEx::ReplaceIcons(const UINT nIDNormal, const UINT nIDDisabled, const UINT nIDHot)
{
	// ツールバーのイメージリストを作成
	if(TRUE)
	{
		CreateToolbarImageList(nIDNormal  , m_ilToolbar);
		CreateToolbarImageList(nIDDisabled, m_ilToolbarDisabled);
		CreateToolbarImageList(nIDHot     , m_ilToolbarHot);
	}

	// ツールバーにイメージリストを割り当てる
	if(TRUE)
	{
		CToolBarCtrl &ctrl = GetToolBarCtrl();
		ctrl.SetImageList        (&m_ilToolbar);
		ctrl.SetDisabledImageList(&m_ilToolbarDisabled);
		ctrl.SetHotImageList     (&m_ilToolbarHot);
	}
}

// 指定ボタンの削除
BOOL CToolBarEx::DeleteButtons(const UINT nIDDelete[], const int count)
{
	CToolBarCtrl &ctrl = GetToolBarCtrl();

	for(int i = 0; i < count; i++)
	{
		const int index = CommandToIndex(nIDDelete[i]);
		if(index == -1)
		{
			return FALSE;
		}

		ctrl.DeleteButton(index);
	}

	return TRUE;
}

// ツールバー用のイメージリストを作成
BOOL CToolBarEx::CreateToolbarImageList(const UINT nIDBitmap, CImageList &il)
{
	// ビットマップをロード
	CBitmap bm;
	bm.LoadBitmap(nIDBitmap);

	// イメージリストの情報を取得
	IMAGELISTINFO info;
	GetImageListInfo(bm, info);

	// イメージリストを作成
	ASSERT(m_nButtonCount > 0);
	il.Create(info.width / m_nButtonCount, info.height, info.flags | ILC_MASK, m_nButtonCount, 0);
	il.Add(&bm, info.mask);

	return TRUE;
}

// イメージリストの情報を取得
BOOL CToolBarEx::GetImageListInfo(CBitmap &rBitmap, IMAGELISTINFO &info)
{
	// ビットマップ情報を取得
	BITMAP bmInfo;
	rBitmap.GetBitmap(&bmInfo);

	// 幅と高さを格納
	info.width  = bmInfo.bmWidth;
	info.height = bmInfo.bmHeight;

	// イメージリストの作成時に使用するフラグを設定
	switch(bmInfo.bmBitsPixel)
	{
	case 4 : info.flags = ILC_COLOR4 ; break;
	case 8 : info.flags = ILC_COLOR8 ; break;
	case 16: info.flags = ILC_COLOR16; break;
	case 24: info.flags = ILC_COLOR24; break;
	case 32: info.flags = ILC_COLOR32; break;
	default: return FALSE;
	}

	// ピクセルデータを格納するバッファ
	CArray<RGBQUAD, RGBQUAD &> buffer;
	buffer.SetSize(info.width);

	// BITMAPINFO構造体に値を設定
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize        = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth       = bmInfo.bmWidth;
	bmi.bmiHeader.biHeight      =-bmInfo.bmHeight;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 8 * sizeof(buffer[0]);
	bmi.bmiHeader.biCompression = 0;

	// 32bitに変換したピクセルデータを取得
	if(TRUE)
	{
		CDC *pDC = GetDC();
		::GetDIBits(pDC->m_hDC, rBitmap, 0, 1, &buffer[0], &bmi, DIB_RGB_COLORS);
		ReleaseDC(pDC);
	}

	// 左上の色をマスクとする
	info.mask = RGB(buffer[0].rgbRed, buffer[0].rgbGreen, buffer[0].rgbBlue);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CToolBarEx メッセージ ハンドラ

// コンテキストメニューの表示
void CToolBarEx::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(m_pContextMenu == NULL)
	{
		return;
	}

	m_pContextMenu->TrackPopupMenu(
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		point.x, point.y,
		m_pWndOwner);
}


////////////////////////////////////////
// ツールバーカスタマイズ用の通知メッセージ処理

// ツールボタンが挿入可能か？
void CToolBarEx::OnNotifyQueryInsert(LPNMHDR /* lpNotifyStruct */, LRESULT *result)
{
	ASSERT(m_nItemCount != -1);

	*result = TRUE;
}

// ツールボタンが削除可能か？
void CToolBarEx::OnNotifyQueryDelete(LPNMHDR /* lpNotifyStruct */, LRESULT *result)
{
	ASSERT(m_nItemCount != -1);

	*result = TRUE;
}

// カスタマイズ開始
void CToolBarEx::OnNotifyBeginAdjust(LPNMHDR /* lpNotifyStruct */, LRESULT * /*result */)
{
	ASSERT(m_nItemCount != -1);
}

// カスタマイズ終了
void CToolBarEx::OnNotifyEndAdjust(LPNMHDR /* lpNotifyStruct */, LRESULT * /* result */)
{
	ASSERT(m_nItemCount != -1);
}

// ツールバーのリセット（LoadToolBar()を呼び出したときの状態へ戻す）
void CToolBarEx::OnNotifyReset(LPNMHDR /* lpNotifyStruct */, LRESULT * /*result */)
{
	ASSERT(m_nItemCount != -1);

	// ボタンをすべて削除
	if(TRUE)
	{
		CToolBarCtrl &ctrl = GetToolBarCtrl();

		const int nOldButtonCount = ctrl.GetButtonCount();
		for(int i = 0; i < nOldButtonCount; i++)
		{
			ctrl.DeleteButton(0);
		}
	}

	// 追加しなおす
	if(TRUE)
	{
		CToolBarCtrl &ctrl = GetToolBarCtrl();

		const int nNewButtonCount = m_nItemCount;
		CArray<TBBUTTON, TBBUTTON &> array_tbbutton;
		array_tbbutton.SetSize(nNewButtonCount);

		for(int i = 0; i < nNewButtonCount; i++)
		{
			array_tbbutton[i] = m_toolbarinfos[i].tbButton;
		}
		ctrl.AddButtons(nNewButtonCount, &array_tbbutton[0]);
	}

	// 変更完了
	OnNotifyToolBarChange(NULL, NULL);
}

// ツールバー変更完了
void CToolBarEx::OnNotifyToolBarChange(LPNMHDR /* lpNotifyStruct */, LRESULT * /* result */)
{
	ASSERT(m_nItemCount != -1);

	GetParentFrame()->RecalcLayout();
}

// ツールバーのボタン情報取得
void CToolBarEx::OnNotifyGetButtonInfo(LPNMHDR lpNotifyStruct, LRESULT *result)
{
	ASSERT(m_nItemCount != -1);

	LPTBNOTIFY tbStruct = reinterpret_cast<LPTBNOTIFY>(lpNotifyStruct);

	const int iItem = tbStruct->iItem;
	if(0 <= iItem && iItem < m_nItemCount)
	{
		TOOLBARINFO &info = m_toolbarinfos[iItem];

		// テキスト情報を取得
		const int cchText = info.strText.GetLength();
		LPCTSTR   pszText = info.strText;

		// ボタン情報・テキスト情報を入れる
		tbStruct->tbButton = info.tbButton;
		tbStruct->cchText  = cchText;
		tbStruct->pszText  = const_cast<LPTSTR>(pszText);

		*result = TRUE;
	}
	else
	{
		*result = FALSE;
	}
}
