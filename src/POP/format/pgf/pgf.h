/* pgf.h …エクスポート関数のプロトタイプ宣言 */
#ifndef __PGF__
#define __PGF__

#include <windows.h>


/******************************************************************************/
/*  */
IMAGERESULT pgf_import(HANDLE hFile,       IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
IMAGERESULT pgf_export(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
BOOL        pgf_option(HWND hWnd);

#endif /* __PGF__ */
