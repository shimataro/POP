// POP.h : POP アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_POP_H__BF5410AA_332D_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_POP_H__BF5410AA_332D_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル
#include "setting.h"
#include <vector>                       // std::vector<>
#include <stack>                        // std::stack<>
#include <map>                          // std::map<>


// 「ファイルを開く」「名前をつけて保存」で使用するフィルタ
#define TCHAR_FILTER_DELIMITER      _T('|')
#define TCHAR_FILTER_EXT_DELIMITER  _T(';')

// 制御文字（文字定数）
#define TCHAR_CTRL_NIL              _T('\0')

// 言語パック/プラグイン/ヘルプファイル
#define TCHAR_LANGUAGE_DIRECTORY    _T("languages")
#define TCHAR_LANGUAGE_WILDCARD     _T("*.dll")

#define TCHAR_PLUGIN_DIRECTORY      _T("plugins")
#define TCHAR_PLUGIN_WILDCARD       _T("*.dll")

#define TCHAR_HELP_DIRECTORY        _T("help")


////////////////////////////////////////////////////////////////////////////////
// レジストリ関係

// プラグイン
#define STRING_REG_SECTION_PLUGIN    _T("Software\\SHIMATARO\\Photo Operator\\Plugin Setting")

// アプリケーション
#define TCHAR_REG_COMPANY               _T("SHIMATARO")
#define TCHAR_REG_APPLICATION           _T("Photo Operator")


// フォーマットIDの最小値、最大値
#define MAX_FORMAT_COUNT        900
#define ID_SETTING_FORMAT_MIN   (ID_SETTING_ENVIRONMENT + 1)
#define ID_SETTING_FORMAT_MAX   (ID_SETTING_FORMAT_MIN + MAX_FORMAT_COUNT - 1)

// フィルタIDの最小値、最大値
#define MAX_FILTER_COUNT        900
#define ID_FILTER_MIN   (ID_FILTER_REAPPLY + 2)	// ID_FILTER_REAPPLY, ID_FILTER_RESUBMITのぶんを除く
#define ID_FILTER_MAX   (ID_FILTER_MIN + MAX_FILTER_COUNT - 1)


/////////////////////////////////////////////////////////////////////////////
// CPOPApp:
// このクラスの動作の定義に関しては POP.cpp ファイルを参照してください。
//

class CPOPApp : public CWinApp
{
public:
	CPOPApp();

	void GetHMenuArray(HMENU hMenuArray[], const int nMenuMaxCount);
	void ReflectSetting(void);
	void GetSetting(SETTING &setting) {setting = m_setting;}

	void NewImageFromClipboard(void);
	void NewImageFromWindow(HWND hWnd, const BOOL bAltogether, const DWORD dwMilliSeconds);

	////////////////////////////////////////
	// プラグイン関連
	BOOL InstallPlugin(HMODULE hModule, HKEY hKeyPlugin);

	UINT GetPluginCount(void) const {return m_plugin_count;}
	const PLUGIN_INFO &GetPluginInfo(const UINT num) const
	{ASSERT(num < m_plugin_count); return m_plugin_info[num];}

	UINT GetPluginFilterCount(void) const {return m_plugin_filter_count;}
	UINT GetPluginFormatCount(void) const {return m_plugin_format_count;}
	UINT GetPluginFilterDetailCount(void) const {return m_plugin_filter_detail_count;}
	UINT GetPluginFormatDetailCount(void) const {return m_plugin_format_detail_count;}

	// num番目のフィルタプラグイン構造体を取得
	const FILTERPLUGIN &GetPluginFilter(const UINT num) const
	{ASSERT(num < m_plugin_filter_count); return m_plugin_filter[num];}

	const FILTERPLUGIN_DETAIL &GetPluginFilterDetail(const UINT num) const
	{ASSERT(num < m_plugin_filter_detail_count); return m_plugin_filter_detail[num];}

	// num番目のフォーマットプラグイン構造体を取得
	const FORMATPLUGIN &GetPluginFormat(const UINT num) const
	{ASSERT(num < m_plugin_format_count); return m_plugin_format[num];}

	const FORMATPLUGIN_DETAIL &GetPluginFormatDetail(const UINT num) const
	{ASSERT(num < m_plugin_format_detail_count); return m_plugin_format_detail[num];}

	// コモンダイアログで使用するフィルタを取得
	CString GetFileFilterLoad(void) const;
	CString GetFileFilterSave(void) const;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPOPApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CPOPApp)
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg void OnFileCloseAll();
	afx_msg void OnUpdateFileCloseAll(CCmdUI* pCmdUI);
	afx_msg void OnFileImportClipboard();
	afx_msg void OnUpdateFileImportClipboard(CCmdUI* pCmdUI);
	afx_msg void OnFileImportDisplay();
	afx_msg void OnSettingEnvironment();
	afx_msg void OnHelpWebpage();
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	afx_msg void OnSettingFormat(UINT nID);
	afx_msg void OnUpdateSettingFormat(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

private:
	// 設定
	int     m_setting_category;
	SETTING m_setting;

	HINSTANCE m_hResource;
	BOOL SetResource(LPCTSTR lpszResourceName);

	// 取り込み
	UINT m_nClipboardCount;
	UINT m_nDisplayCount;
	void ImportFromHandle(HBITMAP hBitmap, LPCTSTR lpszTitle);

	// CFileDialog::m_ofn::m_nFilterIndexの値（「ファイル」→「開く」で使用）
	DWORD m_nFilterIndex;

	////////////////////////////////////////
	// プラグイン関連
	UINT                     m_plugin_count;
	std::vector<PLUGIN_INFO> m_plugin_info;
	std::stack<HMODULE>      m_plugin_handle_stack;

	// フィルタプラグイン
	HMENU m_hFilterMenu;
	std::map<CString, HMENU>  m_plugin_filter_map;  // メニューマップ（ジャンルの整理に使用）
	UINT                      m_plugin_filter_count;
	std::vector<FILTERPLUGIN> m_plugin_filter;      // フィルタプラグイン関数の格納用
	UINT                      m_plugin_filter_detail_count;
	std::vector<FILTERPLUGIN_DETAIL>   m_plugin_filter_detail; // フィルタ情報の格納用

	// フォーマットプラグイン
	CString m_file_filter_load;             // 
	CString m_file_filter_load_all;         // 
	CString m_file_filter_save;             // 
	HMENU m_hFormatMenuMainframe;
	HMENU m_hFormatMenuPoptype;
	UINT                             m_plugin_format_count;           // 
	std::vector<FORMATPLUGIN>        m_plugin_format;                 // フォーマットプラグイン関数の格納用
	UINT                             m_plugin_format_detail_count;    // 
	std::vector<FORMATPLUGIN_DETAIL> m_plugin_format_detail;          // フォーマットプラグイン関数の格納用

	BOOL ReadPluginInfo(HMODULE hModule, PLUGIN_INFO &plugin_info, HKEY hKeyPlugin);
	void LoadPlugin(void);
	void FreePlugin(void);
	int  LoadPluginFormat(HMODULE hModule);
	int  LoadPluginFilter(HMODULE hModule);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POP_H__BF5410AA_332D_11D5_8205_08004605D14C__INCLUDED_)
