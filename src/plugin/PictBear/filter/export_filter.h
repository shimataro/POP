/* export_filter.h …エクスポート用関数のプロトタイプ宣言 */
#ifndef __EXPORT_FILTER__
#define __EXPORT_FILTER__

#include <windows.h>


/******************************************************************************/
/* エクスポートする関数 */
EXTERN_C int         WINAPI pop_filter_get_count(void);
EXTERN_C DWORD       WINAPI pop_filter_info      (const int filter_no, LPSTR genre, UINT genre_max, LPSTR name, UINT name_max);
EXTERN_C BOOL        WINAPI pop_filter_executable(const int filter_no, const IMAGE_INFO *info_ptr);
EXTERN_C IMAGERESULT WINAPI pop_filter_effect    (const int filter_no, const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback, long callback_data);
EXTERN_C BOOL        WINAPI pop_filter_option    (const int filter_no, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __EXPORT_FILTER__ */
