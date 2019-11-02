/* jpg.h …エクスポート関数のプロトタイプ宣言 */
#ifndef __JPG__
#define __JPG__

#include <windows.h>
#include <stdio.h>


/******************************************************************************/
/*  */
IMAGERESULT jpg_import(HANDLE hFile,       IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
IMAGERESULT jpg_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName,                    callback_t callback, long callback_data);
BOOL        jpg_option(HWND hWnd);

#endif /* __JPG__ */
