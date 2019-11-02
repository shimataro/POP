// ras_encode.h …RASファイルの書き込み
#ifndef __RAS_ENCODE__
#define __RAS_ENCODE__

#include "ras_common.h"


// 関数のプロトタイプ宣言
void ras_encode_header(HANDLE hFile, const IMAGE_INFO *info_ptr, RASTERFILEHEADER *header);
void ras_encode_image_gray8(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, RASTERFILEHEADER *header, callback_t callback, long callback_data);
void ras_encode_image_index(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, RASTERFILEHEADER *header, callback_t callback, long callback_data);
void ras_encode_image_rgb8 (HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, RASTERFILEHEADER *header, callback_t callback, long callback_data);

#endif // __RAS_ENCODE__
