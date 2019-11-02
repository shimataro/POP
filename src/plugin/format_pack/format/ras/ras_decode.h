// ras_decode.h
#ifndef __RAS_DECODE__
#define __RAS_DECODE__

#include <stdio.h>
#include "ras_common.h"


// 関数のプロトタイプ宣言
void ras_decode_header(HANDLE hFile, IMAGE_INFO *info_ptr, RASTERFILEHEADER *header);
void ras_decode_image_gray8(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, RASTERFILEHEADER *header);
void ras_decode_image_index(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, RASTERFILEHEADER *header);
void ras_decode_image_rgb8 (HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, RASTERFILEHEADER *header);

#endif // __RAS_DECODE__
