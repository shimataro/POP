/* export_format.h …エクスポート関数のプロトタイプ宣言 */
#ifndef __EXPORT_FORMAT__
#define __EXPORT_FORMAT__

#include <windows.h>
#include <stdio.h>


/******************************************************************************/
/* エクスポートする関数 */
EXTERN_C int         WINAPI pop_format_count       (void);
EXTERN_C DWORD       WINAPI pop_format_info        (const int format_no, LPSTR name, UINT name_max, LPSTR ext, UINT ext_max, LPSTR filter, UINT filter_max, LPSTR expression, UINT expression_max);
EXTERN_C uint_t      WINAPI pop_format_support_mode(const int format_no);
EXTERN_C IMAGERESULT WINAPI pop_format_import      (const int format_no, HANDLE hFile,       IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
EXTERN_C IMAGERESULT WINAPI pop_format_export      (const int format_no, HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
EXTERN_C BOOL        WINAPI pop_format_option      (const int format_no, HWND hWnd);

#endif /* __EXPORT_FORMAT__ */
