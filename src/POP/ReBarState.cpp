// ReBarState.cpp …インプリメンテーションファイル
#include "stdafx.h"
#include "ReBarState.h"
#include <afxtempl.h>


// レジストリのセクション／エントリ名
#define TCHAR_REG_SECTION_REBAR         _T("ReBar Placement#%d")
#define TCHAR_REG_ENTRY_IDMAP           _T("ID-%d")
#define TCHAR_REG_ENTRY_REBARINFO       _T("ReBar#%d")
#define TCHAR_REG_ENTRY_SUFFIX_SIZE     _T(".cx")
#define TCHAR_REG_ENTRY_SUFFIX_STYLE    _T(".fStyle")


// レジストリに保存したリバーの状態を復元
void LoadReBarState(CReBar &bar)
{
	CReBarCtrl &ctrl = bar.GetReBarCtrl();
	const int count = ctrl.GetBandCount();

	CMap<int, int &, HWND, HWND &> id_map;
	CList<HWND, HWND &> hWndList;

	// ID-HWND 対応マップを作成
	for(int i = 0; i < count; i++)
	{
		// 子ウインドウを取得
		REBARBANDINFO rbbi = {sizeof(REBARBANDINFO), RBBIM_CHILD};
		ctrl.GetBandInfo(i, &rbbi);

		// IDとウインドウハンドルを対応付ける
		HWND hWnd = rbbi.hwndChild;
		int  id   = ::GetDlgCtrlID(hWnd);
		id_map[id] = hWnd;

		hWndList.AddTail(hWnd);

		// バンドをはずす
		rbbi.hwndChild = NULL;
		ctrl.SetBandInfo(i, &rbbi);
	}

	// バンドの配置を設定
	for(int j = 0; j < count; j++)
	{
		CWinApp *pApp = AfxGetApp();
		ASSERT(pApp != NULL);

		REBARBANDINFO rbbi = {sizeof(REBARBANDINFO), 0};

		// セクションとエントリーを取得
		CString section, entry;
		section.Format(TCHAR_REG_SECTION_REBAR, bar.GetDlgCtrlID());
		entry.Format(TCHAR_REG_ENTRY_IDMAP, j);

		// j番目のウインドウIDに対応するウインドウハンドルを取得
		POSITION pos = NULL;
		int id = pApp->GetProfileInt(section, entry, -1);
		HWND hWnd = NULL;

		// IDに対応するウインドウが見つかった場合
		if(id_map.Lookup(id, hWnd))
		{
			entry.Format(TCHAR_REG_ENTRY_REBARINFO, id);

			// cxをロード
			const int cx = pApp->GetProfileInt(section, entry + TCHAR_REG_ENTRY_SUFFIX_SIZE, -1);
			if(cx != -1)
			{
				rbbi.fMask |= RBBIM_SIZE;
				rbbi.cx     = cx;
			}

			// fStyleをロード
			const int fStyle = pApp->GetProfileInt(section, entry + TCHAR_REG_ENTRY_SUFFIX_STYLE, -1);
			if(cx != -1 && fStyle != -1)
			{
				rbbi.fMask |= RBBIM_STYLE;
				rbbi.fStyle = fStyle;
			}

			pos = hWndList.Find(hWnd);
		}
		// IDに対応するウインドウが見つからなかった場合
		else
		{
			// CMap::GetStartPosition()は、どの順番で出てくるかわからないから使えない
			pos  = hWndList.GetHeadPosition();
			hWnd = hWndList.GetAt(pos);
			id   = ::GetDlgCtrlID(hWnd);
		}

		// 子ウインドウを設定
		rbbi.fMask    |= RBBIM_CHILD;
		rbbi.hwndChild = hWnd;
		ctrl.SetBandInfo(j, &rbbi);

		// ReBarに追加したIDを削除
		ASSERT(pos != NULL);
		id_map.RemoveKey(id);
		hWndList.RemoveAt(pos);
	}

	// マップとリストは空になっているはず
	ASSERT(id_map.IsEmpty());
	ASSERT(hWndList.IsEmpty());
}

// レジストリにリバーの状態を保存
void SaveReBarState(const CReBar &bar)
{
	CReBarCtrl &ctrl = bar.GetReBarCtrl();

	const int count = ctrl.GetBandCount();
	for(int i = 0; i < count; i++)
	{
		CWinApp *pApp = AfxGetApp();
		ASSERT(pApp != NULL);

		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask  = RBBIM_SIZE | RBBIM_STYLE | RBBIM_CHILD;

		ctrl.GetBandInfo(i, &rbbi);

		const int id = ::GetDlgCtrlID(rbbi.hwndChild);

		// セクションを取得
		CString section;
		section.Format(TCHAR_REG_SECTION_REBAR, bar.GetDlgCtrlID());

		// インデックスとIDの対応を登録
		if(TRUE)
		{
			CString entry;
			entry.Format(TCHAR_REG_ENTRY_IDMAP, i);
			pApp->WriteProfileInt(section, entry, id);
		}

		// 指定IDのバンド情報を登録
		if(TRUE)
		{
			CString entry;
			entry.Format(TCHAR_REG_ENTRY_REBARINFO, id);
			pApp->WriteProfileInt(section, entry + TCHAR_REG_ENTRY_SUFFIX_SIZE , rbbi.cx);
			pApp->WriteProfileInt(section, entry + TCHAR_REG_ENTRY_SUFFIX_STYLE, rbbi.fStyle);
		}
	}
}
