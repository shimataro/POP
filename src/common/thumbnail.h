// thumbnail.h …サムネイル作成
#ifndef __THUMBNAIL__
#define __THUMBNAIL__

#include "image.h"


EXTERN_C IMAGERESULT create_thumbnail(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const_uint16_t new_width, const_uint16_t new_height);

#endif // __THUMBNAIL__
