// png_decode.h …PNGファイルの読み込み
#ifndef __PNG_DECODE__
#define __PNG_DECODE__

#include "png_common.h"


// 関数のプロトタイプ宣言
BOOL png_decode_check_signature(HANDLE hFile);
BOOL png_decode_init(png_structpp png_ptr_ptr, png_infopp png_info_ptr_ptr);
void png_decode_header(png_structp png_ptr, png_infop png_info_ptr, IMAGE_INFO *info_ptr, PNGEXTRA *png_extra_ptr);
void png_decode_image (png_structp png_ptr, png_infop png_info_ptr, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, const PNGEXTRA *png_extra_ptr, callback_t callback, long callback_data);

#endif // __PNG_DECODE__
