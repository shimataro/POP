/* plugin.h */
#ifndef __PLUGIN__
#define __PLUGIN__

#include <windows.h>


/******************************************************************************/
// 関数名の定義
#define funcname_get_product    "pop_get_product"
#define funcname_set_registry   "pop_set_registry"
#define funcname_setup          "pop_setup"


/******************************************************************************/
/* 関数型の定義 */
typedef DWORD (WINAPI *GETPRODUCT)(LPSTR author, UINT author_max, LPSTR description, UINT description_max);
typedef void  (WINAPI *SETREGISTRY)(HKEY key_root);
typedef BOOL  (WINAPI *SETUP)(HWND hWnd);


/******************************************************************************/
/* 構造体の定義 */

/* プラグイン情報構造体 */
typedef struct tagPLUGIN_INFO
{
	DWORD version;                          /* プラグインのバージョン（上位バイトからメジャー番号・マイナー番号・ビルド番号・リビジョン） */
	TCHAR filename[_MAX_FNAME];             /* ファイル名 */
	TCHAR author[64];                       /* 作者 */
	TCHAR description[256];                 /* プラグインについての記述 */

	SETUP setup;                            /* プラグインのセットアップ関数 */
} PLUGIN_INFO;

#endif /* __FILTER__ */
