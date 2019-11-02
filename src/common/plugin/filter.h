/* filter.h */
#ifndef __FILTER__
#define __FILTER__

#include <windows.h>
#include "../image.h"


/******************************************************************************/
/* モジュール名の定義 */
#define funcname_filter_count       "pop_filter_count"
#define funcname_filter_info        "pop_filter_info"
#define funcname_filter_executable  "pop_filter_executable"
#define funcname_filter_option      "pop_filter_option"
#define funcname_filter_effect      "pop_filter_effect"


/******************************************************************************/
/* 関数型の定義 */
typedef int         (WINAPI *FILTERFUNC_COUNT)     (void);
typedef DWORD       (WINAPI *FILTERFUNC_INFO)      (const int filter_no, LPSTR genre, UINT genre_max, LPSTR name, UINT name_max);
typedef BOOL        (WINAPI *FILTERFUNC_EXECUTABLE)(const int filter_no, const IMAGE_INFO *info);
typedef IMAGERESULT (WINAPI *FILTERFUNC_EFFECT)    (const int filter_no, const IMAGE_INFO *info, const IMAGE_DATA *data_in, IMAGE_DATA *data_out, callback_t callback, long callback_data);
typedef BOOL        (WINAPI *FILTERFUNC_OPTION)    (const int filter_no, const IMAGE_INFO *info, const IMAGE_DATA *data, HWND hWnd);


/******************************************************************************/
/* 構造体の定義 */

/* フィルタプラグイン構造体 */
typedef struct tagFILTERPLUGIN
{
	int count;

	FILTERFUNC_EXECUTABLE executable;
	FILTERFUNC_EFFECT effect;
	FILTERFUNC_OPTION option;
} FILTERPLUGIN;

/* 個々のフィルタ情報構造体 */
typedef struct tagFILTERPLUGIN_DETAIL
{
	/* フィルタ情報 */
	TCHAR genre[20];                        /* プラグインのジャンル */
	TCHAR name[40];                         /* プラグイン名 */

	/* 含まれている関数 */
	DWORD function_flag;

	/* どのプラグインの何番目か？ */
	int plugin_no;
	int filter_no;
} FILTERPLUGIN_DETAIL;

#endif /* __FILTER__ */
