// PictBear.cpp …インプリメンテーションファイル
#include "../stdafx.h"
#include "PictBear.h"
#include "PictBear_filter.h"

#include <stack>                        // std::stack<>
#include <vector>                       // std::vector<>

#include <shlwapi.h>                    // ::PathCombine()

#pragma comment(lib, "shlwapi.lib")


////////////////////////////////////////////////////////////////////////////////
// プラグイン関数名
#define PB_GetPluginInfo_name   "PB_GetPluginInfo"


////////////////////////////////////////////////////////////////////////////////
// プラグイン関数型
typedef WORD (WINAPI *PB_GetPluginInfo_t)(LPSTR buf, int buflen);


////////////////////////////////////////////////////////////////////////////////
// 
typedef struct tagPictBear_PLUGIN_FUNC_t
{
	PB_GetPluginInfo_t PB_GetPluginInfo;
} PictBear_PLUGIN_FUNC_t;


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	std::stack<HMODULE> g_stack_module;
	std::vector<PictBear_PLUGIN_FUNC_t> g_PictBear_plugin_func;

	BOOL get_PictBear_plugin_func(HMODULE hModule, PictBear_PLUGIN_FUNC_t &PictBear_plugin_func);
END_NAMESPACE()


////////////////////////////////////////
// 
BOOL load_PictBear_plugin(LPCTSTR lpDirectory)
{
	TCHAR szFindPath[_MAX_PATH];
	::PathCombine(szFindPath, lpDirectory, _T("*.ppi"));

	// プラグインを検索
	WIN32_FIND_DATA fd;
	HANDLE find_handle = ::FindFirstFile(szFindPath, &fd);
	if(find_handle == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	int filter_plugin_no = 0;
	do
	{
		// 見つかったプラグインをロード
		TCHAR szPluginPath[_MAX_PATH];
		::PathCombine(szPluginPath, lpDirectory, fd.cFileName);

		HMODULE hModule = ::LoadLibrary(szPluginPath);

		if(install_PictBear_plugin(hModule, filter_plugin_no))
		{
			filter_plugin_no++;
		}
	}
	while(::FindNextFile(find_handle, &fd));
	::FindClose(find_handle);

	return TRUE;
}

////////////////////////////////////////
// モジュールハンドルを開放
void free_PictBear_plugin(void)
{
	while(!g_stack_module.empty())
	{
		// スタックの先頭からプラグインハンドルを取り出して開放
		HMODULE hModule = g_stack_module.top();
		::FreeLibrary(hModule);

		// スタックを1つ減らす
		g_stack_module.pop();
	}
}

////////////////////////////////////////
// PictBearプラグインのインストール
BOOL install_PictBear_plugin(HMODULE hModule, const int filter_plugin_no)
{
	// プラグイン関数を取得
	PictBear_PLUGIN_FUNC_t PictBear_plugin_func;
	if(!get_PictBear_plugin_func(hModule, PictBear_plugin_func))
	{
		// プラグインハンドルを開放
		::FreeLibrary(hModule);
		return FALSE;
	}

	// フィルタ関数を取得
	const int filter_count = get_PictBear_plugin_filter_func(hModule, filter_plugin_no);
	if(filter_count == 0)
	{
		// プラグインハンドルを開放
		::FreeLibrary(hModule);
		return FALSE;
	}

	// プラグインを登録
	g_stack_module.push(hModule);
	g_PictBear_plugin_func.push_back(PictBear_plugin_func);
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

BOOL get_PictBear_plugin_func(HMODULE hModule, PictBear_PLUGIN_FUNC_t &PictBear_plugin_func)
{
	PictBear_plugin_func.PB_GetPluginInfo = reinterpret_cast<PB_GetPluginInfo_t>(::GetProcAddress(hModule, PB_GetPluginInfo_name));
	if(PictBear_plugin_func.PB_GetPluginInfo == NULL)
	{
		return FALSE;
	}
	return TRUE;
}

END_NAMESPACE()
