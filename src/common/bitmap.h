/* bitmap.h �c�r�b�g�}�b�v�������֐��̒�` */
#ifndef __BITMAP__
#define __BITMAP__

#include <windows.h>
#include "image.h"


/******************************************************************************/
/* �萔�̒�` */

/* �r�b�g�}�b�v�̃o�E���_���iBMP�Ɠ����j */
#define BOUND_BITMAP  4

/* BITMAP_SetDataFromImage()�̃O���b�h�T�C�Y�i�P�F�j */
#define BITMAP_GRID_SINGLE_COLOR  16


/******************************************************************************/
/* �֐��̃v���g�^�C�v�錾 */

/* DIB�Z�N�V�����֐� */
EXTERN_C HBITMAP DIBSECTION_Create32bits(const int width, const int height, BITMAPINFO *bitmapinfo_ptr, void **buffer_ptr);

/* �r�b�g�}�b�v�֐� */
EXTERN_C void BITMAP_SetDataFromImage(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, const_uint8_t gamma_table[256], const int grid_size, const COLORREF bgc1, const COLORREF bgc2, RGBQUAD *argb);

/* �r�b�g�}�b�v�n���h���֐� */
EXTERN_C HBITMAP     HBITMAP_CreateFromImage(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr);
EXTERN_C IMAGERESULT HBITMAP_GetImage(HBITMAP hBitmap, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr);
EXTERN_C BOOL        HBITMAP_GetSize(HBITMAP hBitmap, LPSIZE lpSize);
EXTERN_C BOOL        HBITMAP_GetData(HBITMAP hBitmap, const SIZE *lpSize, LPVOID  lpBuffer);

EXTERN_C HBITMAP GetWindowBitmap(HWND hWnd, BOOL bAltogether);

EXTERN_C void CreateGammaTable(const double gamma_file, const double gamma_decode, const double gamma_display, uint8_t gamma_table[256]);
EXTERN_C void create_scaleup_table(uint8_t table[], const_uint_t depth);


#endif  /* __BITMAP__ */
