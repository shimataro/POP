/* PbPlugin.h **********************************************************
   PictBear Plugin Header File
   $Version 1.0��
   Copyright (C) 1999-2000 by Yasuyuki Kashiwagi. All Rights Reserved.
***********************************************************************/
#ifndef __PBPLUGIN_H
#define __PBPLUGIN_H
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union tagPixel32 {
	DWORD color;                 /* �J���[�l */
	struct {
		BYTE b;                  /* ��       */
		BYTE g;                  /* ��       */
		BYTE r;                  /* ��       */
		BYTE a;                  /* �s�����x */
	} item;
} Pixel32;

#define PBPLUGIN_VERSION  0      /* �v���O�C���̃o�[�W�����ԍ� */
#define PBPLUGIN_UNKNOWN  0x00   /* �s���ȃv���O�C��           */
#define PBPLUGIN_FILTER   0x01   /* �t�B���^�v���O�C��         */
#define PBF_MASK_CLIENT   0x00   /* �}�X�N�� PictBear ������   */
#define PBF_MASK_PLUGIN   0x01   /* �}�X�N�̓v���O�C��������   */

typedef int (WINAPI *PBF_ProgressCallback)(int, int, long);


/* PBF_QueryClientInfo �̌Ăяo���� ************************************
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
#define PBF_QUERY_VERSION    0x00 /* int     : PictBear �̃o�[�W����  */
#define PBF_QUERY_DRAWCOLOR  0x01 /* Pixel32 : �G��F                 */
#define PBF_QUERY_SUBCOLOR   0x02 /* Pixel32 : �⏕�F�i���ݎg�p�s�j */
#define PBF_QUERY_BKGNDCOLOR 0x03 /* Pixel32 : �w�i�F�ia �̒l�͕s��j */

#ifdef __cplusplus
}
#endif

#endif /* __PBPLUGIN_H */
/**********************************************************************/
