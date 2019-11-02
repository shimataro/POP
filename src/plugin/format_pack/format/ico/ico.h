/* ico.h …エクスポート関数のプロトタイプ宣言 */
#ifndef __ICO__
#define __ICO__

#include <windows.h>
#include <stdio.h>


/******************************************************************************/
/*  */
IMAGERESULT ico_import(HANDLE hFile,       IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
IMAGERESULT ico_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName,                    callback_t callback, long callback_data);
BOOL        ico_option(HWND hWnd);

#endif /* __ICO__ */
