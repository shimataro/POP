// jpg_decode.h …JPGファイルの読み込み
#ifndef __JPG_DECODE__
#define __JPG_DECODE__

#include <stdio.h>
#include "jpg_common.h"


// 関数のプロトタイプ宣言
void jpg_decode_header(jpeg_decompress_struct *cinfo, IMAGE_INFO *info_ptr);
void jpg_decode_image_gray8(jpeg_decompress_struct *cinfo, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
void jpg_decode_image_rgb8 (jpeg_decompress_struct *cinfo, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data);

#endif // __JPG_DECODE__
