/* gif.h …エクスポート関数のプロトタイプ宣言 */
#ifndef __GIF__
#define __GIF__

#include <windows.h>
#include <stdio.h>


/******************************************************************************/
/*  */
IMAGERESULT gif_import(HANDLE hFile,       IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
IMAGERESULT gif_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName,                    callback_t callback, long callback_data);
BOOL        gif_option(HWND hWnd);

#endif /* __BMP__ */
