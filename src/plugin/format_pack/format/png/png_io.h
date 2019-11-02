/* png.h …エクスポート関数のプロトタイプ宣言 */
#ifndef __PNG__
#define __PNG__

#include <windows.h>
#include <stdio.h>


/******************************************************************************/
IMAGERESULT png_import(HANDLE hFile,       IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
IMAGERESULT png_export(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
BOOL        png_option(HWND hWnd);

#endif /* __PNG__ */
