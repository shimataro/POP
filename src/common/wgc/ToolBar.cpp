// ToolBar.cpp …インプリメンテーションファイル
#include "ToolBar.h"
#include "wgfunc.h"                     // wgc::wgfLoadString()
#include <assert.h>                     // assert()


_WGC_BEGIN                              // namespace wgc {

// コンストラクタ
toolbar::toolbar(void)
{
	INITCOMMONCONTROLSEX ic = {sizeof(ic)};
	ic.dwICC = ICC_BAR_CLASSES;
	::InitCommonControlsEx(&ic);
}

// デストラクタ
toolbar::~toolbar(void)
{
}


// ステータスバーの作成
BOOL toolbar::Create(
	window *parent,
	HINSTANCE hResource,
	const UINT nResourceID,
	const int cx,
	const int cy,
	const int arCommand[],
	const int nCommandCount,
	const DWORD dwStyle        /* = WS_CHILD | WS_VISIBLE */,
	const DWORD dwCtlStyle     /* = CCS_TOP | CCS_ADJUSTABLE */,
	const DWORD dwToolBarStyle /* = TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | TBSTYLE_ALTDRAG */,
	const UINT  nID            /* = IDW_STD_TOOLBAR */)
{
	assert(parent != NULL);

	m_button_array.resize(nCommandCount);
	std::vector<TBBUTTON> tbbutton(nCommandCount);

	int nButtonCount = 0;
	for(int i = 0; i < nCommandCount; i++)
	{
		if(arCommand[i] == 0)
		{
			tbbutton[i].iBitmap   = 0;
			tbbutton[i].idCommand = 0;
			tbbutton[i].fsState   = TBSTATE_ENABLED;
			tbbutton[i].fsStyle   = TBSTYLE_SEP;
		}
		else
		{
			tbbutton[i].iBitmap   = nButtonCount++;
			tbbutton[i].idCommand = arCommand[i];
			tbbutton[i].fsState   = TBSTATE_ENABLED;
			tbbutton[i].fsStyle   = TBSTYLE_BUTTON;
		}
		tbbutton[i].dwData  = 0;
		tbbutton[i].iString = 0;

		m_button_array[i].button = tbbutton[i];

		// [block]
		// 　説明用テキスト
		{
			tstring_t text;
			wgfLoadString(hResource, arCommand[i], text);
			wgfExtractSubString(m_button_array[i].str, text, 0, _T('\n'));
		}
	}

	// コントロールのスタイルをまとめる
	const DWORD dwAllStyle = dwStyle | dwCtlStyle | dwToolBarStyle;

	_SetHookTarget(this);
	HWND hWnd = ::CreateToolbarEx(
		*parent,
		dwAllStyle & (~TBSTYLE_FLAT),
		nID,
		nButtonCount,
		hResource,
		nResourceID,
		&tbbutton[0],
		nCommandCount,
		0, 0,
		cx, cy,
		sizeof(tbbutton[0]));

	if(hWnd == NULL)
	{
		_SetHookTarget(NULL);
		return FALSE;
	}
	assert(hWnd == m_hWnd);

	// TBSTYLE_FLAT を追加
	if(dwAllStyle & TBSTYLE_FLAT)
	{
		SetStyle(dwAllStyle);
	}
	return TRUE;
}


////////////////////////////////////////////////////////////
// アトリビュート

void toolbar::GetButtonSize(SIZE &size) const
{
	const DWORD dwSize = SendMessage(TB_GETBUTTONSIZE);
	size.cx = LOWORD(dwSize);
	size.cy = HIWORD(dwSize);
}

BOOL toolbar::GetButton(const UINT iButton, TBBUTTON &tbButton) const
{
	return SendMessage(TB_GETBUTTON, iButton, reinterpret_cast<LPARAM>(&tbButton));
}

UINT toolbar::GetButtonCount(void) const
{
	return SendMessage(TB_BUTTONCOUNT);
}


////////////////////////////////////////////////////////////
// オペレーション

void toolbar::Customize(void)
{
	SendMessage(TB_CUSTOMIZE);
}

BOOL toolbar::AddButtons(LPTBBUTTON lpButtons, const int nNumButtons)
{
	return SendMessage(TB_ADDBUTTONS, nNumButtons, reinterpret_cast<LPARAM>(lpButtons));
}

BOOL toolbar::DeleteButton(const UINT iButton)
{
	return SendMessage(TB_DELETEBUTTON, iButton);
}


////////////////////////////////////////////////////////////////////////////////
// 通知メッセージの処理
BOOL toolbar::HandleNotifyMessage(LPNMHDR lpNmHeader, BOOL &bResult)
{
	LPTBNOTIFY lpTbNotify = reinterpret_cast<LPTBNOTIFY>(lpNmHeader);
	switch(lpNmHeader->code)
	{
	case TBN_BEGINADJUST: OnNotifyBeginAdjust(lpTbNotify); break;
	case TBN_ENDADJUST:   OnNotifyEndAdjust  (lpTbNotify); break;

	case TBN_BEGINDRAG: OnNotifyBeginDrag(lpTbNotify); break;
	case TBN_ENDDRAG:   OnNotifyEndDrag  (lpTbNotify); break;

	case TBN_QUERYINSERT: bResult = OnNotifyQueryInsert(lpTbNotify); break;
	case TBN_QUERYDELETE: bResult = OnNotifyQueryInsert(lpTbNotify); break;

	case TBN_GETBUTTONINFO: bResult = OnNotifyGetButtonInfo(lpTbNotify); break;

	case TBN_TOOLBARCHANGE: OnNotifyToolbarChange(lpTbNotify); break;
	case TBN_RESET:         OnNotifyReset        (lpTbNotify); break;
	case TBN_CUSTHELP:      OnNotifyCustHelp     (lpTbNotify); break;

	default:
		return FALSE;
	}
	return bResult;
}


// 通知メッセージハンドラ
void toolbar::OnNotifyBeginAdjust(LPTBNOTIFY /* lpNotify */)
{
}

void toolbar::OnNotifyEndAdjust(LPTBNOTIFY /* lpNotify */)
{
}


BOOL toolbar::OnNotifyQueryInsert(LPTBNOTIFY /* lpNotify */)
{
	return TRUE;
}

BOOL toolbar::OnNotifyQueryDelete(LPTBNOTIFY /* lpNotify */)
{
	return TRUE;
}


void toolbar::OnNotifyBeginDrag(LPTBNOTIFY /* lpNotify */)
{
}

void toolbar::OnNotifyEndDrag(LPTBNOTIFY /* lpNotify */)
{
}


BOOL toolbar::OnNotifyGetButtonInfo(LPTBNOTIFY lpNotify)
{
	const int iItem = lpNotify->iItem;
	const int size  = m_button_array.size();
	if(!(0 <= iItem && iItem < size))
	{
		return FALSE;
	}

	const button_info_t &info = m_button_array[iItem];
	LPCTSTR lpszText = info.str.c_str();

	// ボタン情報・テキスト情報を入れる
	lpNotify->tbButton = info.button;
	lpNotify->cchText  = info.str.length();
	lpNotify->pszText  = const_cast<LPTSTR>(lpszText);

	return TRUE;
}


void toolbar::OnNotifyToolbarChange(LPTBNOTIFY /* lpNotify */)
{
}

void toolbar::OnNotifyReset(LPTBNOTIFY /* lpNotify */)
{
	// [block]
	// ボタンをすべて削除
	{
		const int nOldButtonCount = GetButtonCount();
		for(int i = 0; i < nOldButtonCount; i++)
		{
			DeleteButton(0);
		}
	}

	// [block]
	// 追加しなおす
	{
		const int nNewButtonCount = m_button_array.size();

		std::vector<TBBUTTON> tbbutton_array(nNewButtonCount);
		for(int i = 0; i < nNewButtonCount; i++)
		{
			tbbutton_array[i] = m_button_array[i].button;
		}
		AddButtons(&tbbutton_array[0], nNewButtonCount);
	}
}

void toolbar::OnNotifyCustHelp(LPTBNOTIFY /* lpNotify */)
{
}

_WGC_END                                // }
