// ToolTipDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "ToolTipDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipDialog

CToolTipDialog::CToolTipDialog()
	: CDialog()
{
}

CToolTipDialog::CToolTipDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd /*= NULL*/)
	: CDialog(lpszTemplateName, pParentWnd)
{
}

CToolTipDialog::CToolTipDialog(UINT nIDTemplate, CWnd* pParentWnd /*=NULL*/)
	: CDialog(nIDTemplate, pParentWnd)
{
}

CToolTipDialog::~CToolTipDialog()
{
}


/////////////////////////////////////////////////////////////////////////////
// CToolTipDialog メッセージ ハンドラ

// ダイアログの初期化（ツールチップコントロールの初期化）
BOOL CToolTipDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// ツールチップコントロールの初期化
	m_ctrlToolTip.Create(this, TTS_ALWAYSTIP);  // ツールチップコントロールを作成
	m_ctrlToolTip.SetMaxTipWidth(INT_MAX);      // ツールチップの最大幅を設定（これがないと改行ができない）

	return TRUE;                          // コントロールにフォーカスを設定しない
}

// ツールチップコントロールの作成
void CToolTipDialog::SetToolTips(const UINT arIDs[], const UINT nCount)
{
	TOOLINFO ti;
	ti.cbSize   = sizeof(TOOLINFO);
	ti.uFlags   = TTF_IDISHWND | TTF_SUBCLASS;
	ti.hwnd     = m_hWnd;
	ti.hinst    = AfxGetResourceHandle();

	for(UINT i=0; i<nCount; i++)
	{
		// コントロールのウインドウ、ツールチップ文字列のIDを取得
		HWND hWnd    = GetDlgItem(arIDs[i])->GetSafeHwnd();
		UINT nTextID = arIDs[i];

		// ダイアログが有効ならツールチップを追加
		if(hWnd != NULL)
		{
			ti.uId      = reinterpret_cast<UINT>(hWnd);
			ti.lpszText = MAKEINTRESOURCE(nTextID);
			m_ctrlToolTip.SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti);
		}
	}
}