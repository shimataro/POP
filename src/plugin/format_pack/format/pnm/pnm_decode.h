// pnm_decode.h
#ifndef __PNM_DECODE__
#define __PNM_DECODE__

#include <stdio.h>
#include "pnm_common.h"


// 関数のプロトタイプ宣言
void pnm_decode_header(FILE *fp, IMAGE_INFO *info_ptr, PNM_PIXELMODE *pixelmode_ptr, PNM_COLORMODE *colormode_ptr, PNMEXTRA *pnm_extra_ptr);
void pnm_decode_image_gray8(FILE *fp, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, const PNM_PIXELMODE pixelmode, const PNMEXTRA *pnm_extra_ptr, callback_t callback, long callback_data);
void pnm_decode_image_rgb8 (FILE *fp, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, const PNM_PIXELMODE pixelmode, const PNMEXTRA *pnm_extra_ptr, callback_t callback, long callback_data);

#endif // __PNM_DECODE__
