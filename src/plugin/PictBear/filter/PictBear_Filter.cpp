// PictBear_Filter.cpp …インプリメンテーションファイル
#include "../stdafx.h"
#include "PictBear_Filter.h"

#include <vector>


////////////////////////////////////////////////////////////////////////////////
// プラグイン関数名
#define PBF_FilterCount_name    "PBF_FilterCount"
#define PBF_GetName_name        "PBF_GetName"
#define PBF_Execute_name        "PBF_Execute"
#define PBF_Setup_name          "PBF_Setup"


////////////////////////////////////////////////////////////////////////////////
// プラグイン関数型
typedef int  (WINAPI *PBF_FilterCount_t)(void);
typedef void (WINAPI *PBF_GetName_t    )(int no, LPSTR kind, int kind_len, LPSTR name, int name_len);
typedef void (WINAPI *PBF_Execute_t    )(int no, HWND hwnd, const Pixel32* src, Pixel32* dst, BYTE* mask, RECT* area, DWORD* maskflag, int width, int height, FARPROC proc, long data);
typedef BOOL (WINAPI *PBF_Setup_t      )(int no, HWND hwnd, const Pixel32* src, int width, int height);

typedef void (WINAPI *PBF_GetName_sub_t)(LPSTR kind, int kind_len, LPSTR name, int name_len);
typedef void (WINAPI *PBF_Execute_sub_t)(HWND hwnd, const Pixel32* src, Pixel32* dst, BYTE* mask, RECT* area, DWORD* maskflag, int width, int height, FARPROC proc, long data);
typedef BOOL (WINAPI *PBF_Setup_sub_t  )(HWND hwnd, const Pixel32* src, int width, int height);


////////////////////////////////////////////////////////////////////////////////
// 
typedef struct tagPictBear_PLUGIN_FILTER_FUNC_t
{
	PBF_FilterCount_t PBF_FilterCount;
	PBF_GetName_t     PBF_GetName;
	PBF_Execute_t     PBF_Execute;
	PBF_Setup_t       PBF_Setup;
} PictBear_PLUGIN_FILTER_FUNC_t;

// 
typedef struct tagPictBear_PLUGIN_FILTER_FUNC_NO_t
{
	int plugin_no;
	int filter_no;
} PictBear_PLUGIN_FILTER_FUNC_NO_t;


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	int g_filter_count = 0;
	std::vector<PictBear_PLUGIN_FILTER_FUNC_t>    g_PictBear_plugin_filter_func;
	std::vector<PictBear_PLUGIN_FILTER_FUNC_NO_t> g_PictBear_plugin_filter_func_no;
END_NAMESPACE()


////////////////////////////////////////////////////////////////////////////////
// 初期化
void init_PictBear_filter(void)
{
	g_PictBear_plugin_filter_func.reserve(64);
	g_PictBear_plugin_filter_func_no.reserve(128);
}

////////////////////////////////////////////////////////////////////////////////
// PictBearプラグインのインストール

int get_PictBear_plugin_filter_func(HMODULE hModule, const int filter_plugin_no)
{
	PictBear_PLUGIN_FILTER_FUNC_t PictBear_plugin_filter_func;
	PictBear_plugin_filter_func.PBF_FilterCount = reinterpret_cast<PBF_FilterCount_t>(::GetProcAddress(hModule, PBF_FilterCount_name));
	PictBear_plugin_filter_func.PBF_GetName     = reinterpret_cast<PBF_GetName_t    >(::GetProcAddress(hModule, PBF_GetName_name));
	PictBear_plugin_filter_func.PBF_Execute     = reinterpret_cast<PBF_Execute_t    >(::GetProcAddress(hModule, PBF_Execute_name));
	PictBear_plugin_filter_func.PBF_Setup       = reinterpret_cast<PBF_Setup_t      >(::GetProcAddress(hModule, PBF_Setup_name));

	// プラグイン内に含まれている関数をチェック
	if(PictBear_plugin_filter_func.PBF_FilterCount == NULL
	|| PictBear_plugin_filter_func.PBF_GetName     == NULL
	|| PictBear_plugin_filter_func.PBF_Execute     == NULL
	|| PictBear_plugin_filter_func.PBF_Setup       == NULL)
	{
		return 0;
	}

	// 
	const int filter_count = PictBear_plugin_filter_func.PBF_FilterCount();
	for(int i = 0; i < filter_count; i++)
	{
		PictBear_PLUGIN_FILTER_FUNC_NO_t PictBear_plugin_filter_func_no;
		PictBear_plugin_filter_func_no.plugin_no = filter_plugin_no;
		PictBear_plugin_filter_func_no.filter_no = i;
		g_PictBear_plugin_filter_func_no.push_back(PictBear_plugin_filter_func_no);
	}

	// プラグインを登録
	g_PictBear_plugin_filter_func.push_back(PictBear_plugin_filter_func);

	// プラグインの数を更新
	g_filter_count += filter_count;
	return filter_count;
}

// プラグインの数を返す
int get_filter_count(void)
{
	return g_filter_count;
}


////////////////////////////////////////////////////////////////////////////////
// PictBearプラグイン内の関数をコール

// PBF_GetNameをコール
void call_PBF_GetName_sub(int filter_no, LPSTR kind, int kind_len, LPSTR name, int name_len)
{
	PictBear_PLUGIN_FILTER_FUNC_NO_t PictBear_plugin_filter_func_no = g_PictBear_plugin_filter_func_no[filter_no];

	PBF_GetName_t PBF_GetName = g_PictBear_plugin_filter_func[PictBear_plugin_filter_func_no.plugin_no].PBF_GetName;
	PBF_GetName(PictBear_plugin_filter_func_no.filter_no, kind, kind_len, name, name_len);
}

// PBF_Executeをコール
void call_PBF_Execute_sub(int filter_no, HWND hwnd, const Pixel32* src, Pixel32* dst, BYTE* mask, RECT* area, DWORD* maskflag, int width, int height, FARPROC proc, long data)
{
	PictBear_PLUGIN_FILTER_FUNC_NO_t PictBear_plugin_filter_func_no = g_PictBear_plugin_filter_func_no[filter_no];

	PBF_Execute_t PBF_Execute = g_PictBear_plugin_filter_func[PictBear_plugin_filter_func_no.plugin_no].PBF_Execute;
	PBF_Execute(PictBear_plugin_filter_func_no.filter_no, hwnd, src, dst, mask, area, maskflag, width, height, proc, data);
}

// PBF_Setupをコール
BOOL call_PBF_Setup_sub(int filter_no, HWND hwnd, const Pixel32* src, int width, int height)
{
	PictBear_PLUGIN_FILTER_FUNC_NO_t PictBear_plugin_filter_func_no = g_PictBear_plugin_filter_func_no[filter_no];

	PBF_Setup_t PBF_Setup = g_PictBear_plugin_filter_func[PictBear_plugin_filter_func_no.plugin_no].PBF_Setup;
	return PBF_Setup(PictBear_plugin_filter_func_no.filter_no, hwnd, src, width, height);
}
