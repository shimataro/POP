/* ras.h …エクスポート関数のプロトタイプ宣言 */
#ifndef __RAS__
#define __RAS__

#include <windows.h>
#include <stdio.h>


/******************************************************************************/
/* */
IMAGERESULT ras_import(HANDLE hFile,       IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
IMAGERESULT ras_export(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
BOOL        ras_option(HWND hWnd);

#endif /* __RAS__ */
