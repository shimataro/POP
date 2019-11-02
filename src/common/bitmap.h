/* bitmap.h …ビットマップを扱う関数の定義 */
#ifndef __BITMAP__
#define __BITMAP__

#include <windows.h>
#include "image.h"


/******************************************************************************/
/* 定数の定義 */

/* ビットマップのバウンダリ（BMPと同じ） */
#define BOUND_BITMAP  4

/* BITMAP_SetDataFromImage()のグリッドサイズ（単色） */
#define BITMAP_GRID_SINGLE_COLOR  16


/******************************************************************************/
/* 関数のプロトタイプ宣言 */

/* DIBセクション関数 */
EXTERN_C HBITMAP DIBSECTION_Create32bits(const int width, const int height, BITMAPINFO *bitmapinfo_ptr, void **buffer_ptr);

/* ビットマップ関数 */
EXTERN_C void BITMAP_SetDataFromImage(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, const_uint8_t gamma_table[256], const int grid_size, const COLORREF bgc1, const COLORREF bgc2, RGBQUAD *argb);

/* ビットマップハンドル関数 */
EXTERN_C HBITMAP     HBITMAP_CreateFromImage(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr);
EXTERN_C IMAGERESULT HBITMAP_GetImage(HBITMAP hBitmap, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr);
EXTERN_C BOOL        HBITMAP_GetSize(HBITMAP hBitmap, LPSIZE lpSize);
EXTERN_C BOOL        HBITMAP_GetData(HBITMAP hBitmap, const SIZE *lpSize, LPVOID  lpBuffer);

EXTERN_C HBITMAP GetWindowBitmap(HWND hWnd, BOOL bAltogether);

EXTERN_C void CreateGammaTable(const double gamma_file, const double gamma_decode, const double gamma_display, uint8_t gamma_table[256]);
EXTERN_C void create_scaleup_table(uint8_t table[], const_uint_t depth);


#endif  /* __BITMAP__ */
