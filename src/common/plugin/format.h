/* format.h */
#ifndef __FORMAT__
#define __FORMAT__

#include <windows.h>
#include "../image.h"


/******************************************************************************/
/* Impremented Function */
#define IF_IMPORT   1
#define IF_EXPORT   2
#define IF_OPTION   4

/* モジュール名の定義 */
#define funcname_format_count           "pop_format_count"
#define funcname_format_info            "pop_format_info"
#define funcname_format_support_mode    "pop_format_support_mode"
#define funcname_format_import          "pop_format_import"
#define funcname_format_export          "pop_format_export"
#define funcname_format_option          "pop_format_option"


/******************************************************************************/
/* 関数型の定義 */
typedef int         (WINAPI *FORMATFUNC_COUNT) (void);
typedef DWORD       (WINAPI *FORMATFUNC_INFO  )(const int format_no, LPSTR name, UINT name_max, LPSTR ext, UINT ext_max, LPSTR filter, UINT filter_max, LPSTR expression, UINT expression_max);
typedef uint_t      (WINAPI *FORMATFUNC_BITS  )(const int format_no);
typedef IMAGERESULT (WINAPI *FORMATFUNC_IMPORT)(const int format_no, HANDLE hFile,       IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
typedef IMAGERESULT (WINAPI *FORMATFUNC_EXPORT)(const int format_no, HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
typedef BOOL        (WINAPI *FORMATFUNC_OPTION)(const int format_no, HWND hWnd);


/******************************************************************************/
/* 構造体の定義 */

/* フォーマットプラグイン構造体 */
typedef struct tagFORMATPLUGIN
{
	int count;

	/* 関数ポインタ */
	FORMATFUNC_IMPORT import;
	FORMATFUNC_EXPORT export;
	FORMATFUNC_OPTION option;
} FORMATPLUGIN;

/* 個々のフォーマット情報構造体 */
typedef struct tagFORMATPLUGIN_DETAIL
{
	/* コモンダイアログのフィルタ情報 */
	TCHAR name[10];
	TCHAR ext[10];
	TCHAR filter[50];
	TCHAR expression[50];

	/* サポートしているカラーモード */
	uint_t support_mode;

	/* 含まれている関数 */
	DWORD included_functions;

	/* どのプラグインの何番目か？ */
	int plugin_no;
	int format_no;
} FORMATPLUGIN_DETAIL;

#endif /* __FORMAT__ */
