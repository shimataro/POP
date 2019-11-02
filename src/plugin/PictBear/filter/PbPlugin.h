/* PbPlugin.h **********************************************************
   PictBear Plugin Header File
   $Version 1.0β
   Copyright (C) 1999-2000 by Yasuyuki Kashiwagi. All Rights Reserved.
***********************************************************************/
#ifndef __PBPLUGIN_H
#define __PBPLUGIN_H
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union tagPixel32 {
	DWORD color;                 /* カラー値 */
	struct {
		BYTE b;                  /* 青       */
		BYTE g;                  /* 緑       */
		BYTE r;                  /* 赤       */
		BYTE a;                  /* 不透明度 */
	} item;
} Pixel32;

#define PBPLUGIN_VERSION  0      /* プラグインのバージョン番号 */
#define PBPLUGIN_UNKNOWN  0x00   /* 不明なプラグイン           */
#define PBPLUGIN_FILTER   0x01   /* フィルタプラグイン         */
#define PBF_MASK_CLIENT   0x00   /* マスクは PictBear が処理   */
#define PBF_MASK_PLUGIN   0x01   /* マスクはプラグインが処理   */

typedef int (WINAPI *PBF_ProgressCallback)(int, int, long);


/* PBF_QueryClientInfo の呼び出し方 ************************************
	const char* lpszProc = "PBF_QueryClientInfo";
	char        szClientEXE[MAX_PATH];
	HINSTANCE   hModule;

	GetModuleFileName(NULL, szClientEXE, MAX_PATH);

	hModule = LoadLibrary(szClientEXE);
	if (hModule != NULL) {
		PBF_QueryClientInfoProc proc = GetProcAddress(hModule, lpszProc);
		if (proc != NULL) {
			int     nVersion;
			Pixel32 DrawColor, SubColor, BkgndColor;
			proc(PBF_QUERY_VERSION, &nVersion);
			proc(PBF_QUERY_DRAWCOLOR , &DrawColor);
			proc(PBF_QUERY_SUBCOLOR  , &SubColor);
			proc(PBF_QUERY_BKGNDCOLOR, &BkgndColor);
		}
		FreeLibrary(hModule);
	}
***********************************************************************/
typedef BOOL (WINAPI* PBF_QueryClientInfoProc)(DWORD, void*);
#define PBF_QUERY_VERSION    0x00 /* int     : PictBear のバージョン  */
#define PBF_QUERY_DRAWCOLOR  0x01 /* Pixel32 : 絵画色                 */
#define PBF_QUERY_SUBCOLOR   0x02 /* Pixel32 : 補助色（現在使用不可） */
#define PBF_QUERY_BKGNDCOLOR 0x03 /* Pixel32 : 背景色（a の値は不定） */

#ifdef __cplusplus
}
#endif

#endif /* __PBPLUGIN_H */
/**********************************************************************/
