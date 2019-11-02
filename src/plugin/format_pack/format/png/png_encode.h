// png_encode.h …PNGファイルの書き込み
#ifndef __PNG_ENCODE__
#define __PNG_ENCODE__

#include "png_common.h"
#include "png_dialog.h"


// 関数のプロトタイプ宣言
BOOL png_encode_init(png_structpp png_ptr_ptr, png_infopp png_info_ptr_ptr);
void png_encode_header(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, const PNGPARAM *param);
void png_encode_image_gray8(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
void png_encode_image_index(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
void png_encode_image_rgb8 (png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);

#endif // __PNG_ENCODE__
