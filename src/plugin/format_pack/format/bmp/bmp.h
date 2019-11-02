/* bmp.h …エクスポート関数のプロトタイプ宣言 */
#ifndef __BMP__
#define __BMP__

#include <windows.h>


/******************************************************************************/
/*  */
IMAGERESULT bmp_import(HANDLE hFile,       IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
IMAGERESULT bmp_export(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
BOOL        bmp_option(HWND hWnd);

#endif /* __BMP__ */
