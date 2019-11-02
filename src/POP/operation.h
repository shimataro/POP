/* operation.h …画像に対する操作 */
#ifndef __OPERATION__
#define __OPERATION__

#include "callback.h"


/******************************************************************************/
/* 構造体の定義 */
typedef struct tagNEWIMAGEPARAM         /* 新規イメージのパラメータ（create_empty_image()で使用） */
{
	uint16_t width, height;                 /* 幅、高さ */
	pixel_rgb8_t rgb;                       /* 新規イメージの色 */
} NEWIMAGEPARAM;

typedef struct tagRESIZEPARAM
{
	uint16_t new_width, new_height;         /* 幅、高さ */
	INTERPOLATE_METHOD interpolate_method;  /* 補間方法 */
} RESIZEPARAM;

typedef struct tagTRIMMINGPARAM
{
	int margin_left, margin_right;
	int margin_top, margin_bottom;

	int clip_left, clip_right;
	int clip_top, clip_bottom;
} TRIMMINGPARAM;


/******************************************************************************/
/* プロトタイプ宣言 */
EXTERN_C IMAGERESULT image_create_empty(IMAGE_INFO *info, IMAGE_DATA *data, const NEWIMAGEPARAM *param);
EXTERN_C uint32_t    image_count_colors(const IMAGE_INFO *info, const IMAGE_DATA *data);
EXTERN_C IMAGERESULT image_convert_colormode(const IMAGE_INFO *in_info, const IMAGE_DATA *in_data, IMAGE_INFO *out_info, IMAGE_DATA *out_data, const int new_mode, callback_t callback_func, long callback_data);
EXTERN_C IMAGERESULT image_alpha_append (const IMAGE_INFO *in_info, const IMAGE_DATA *in_data, IMAGE_INFO *out_info, IMAGE_DATA *out_data, const BOOL append, callback_t callback_func, long callback_data);
EXTERN_C IMAGERESULT image_alpha_fill   (const IMAGE_INFO *info, const IMAGE_DATA *in_data, IMAGE_DATA *out_data, const_uint8_t alpha_val, callback_t callback_func, long callback_data);
EXTERN_C IMAGERESULT image_alpha_inverse(const IMAGE_INFO *info, const IMAGE_DATA *in_data, IMAGE_DATA *out_data, callback_t callback_func, long callback_data);
EXTERN_C IMAGERESULT image_swap_colors  (const IMAGE_INFO *info, const IMAGE_DATA *in_data, IMAGE_DATA *out_data, const int red, const int green, const int blue, callback_t callback_func, long callback_data);
EXTERN_C IMAGERESULT image_flip    (const IMAGE_INFO *in_info, const IMAGE_DATA *in_data, IMAGE_INFO *out_info, IMAGE_DATA *out_data, callback_t callback_func, long callback_data);
EXTERN_C IMAGERESULT image_mirror  (const IMAGE_INFO *in_info, const IMAGE_DATA *in_data, IMAGE_INFO *out_info, IMAGE_DATA *out_data, callback_t callback_func, long callback_data);
EXTERN_C IMAGERESULT image_turn_90 (const IMAGE_INFO *in_info, const IMAGE_DATA *in_data, IMAGE_INFO *out_info, IMAGE_DATA *out_data, callback_t callback_func, long callback_data);
EXTERN_C IMAGERESULT image_turn_180(const IMAGE_INFO *in_info, const IMAGE_DATA *in_data, IMAGE_INFO *out_info, IMAGE_DATA *out_data, callback_t callback_func, long callback_data);
EXTERN_C IMAGERESULT image_turn_270(const IMAGE_INFO *in_info, const IMAGE_DATA *in_data, IMAGE_INFO *out_info, IMAGE_DATA *out_data, callback_t callback_func, long callback_data);
EXTERN_C IMAGERESULT image_resize  (const IMAGE_INFO *in_info, const IMAGE_DATA *in_data, IMAGE_INFO *out_info, IMAGE_DATA *out_data, const RESIZEPARAM   *param, callback_t callback_func, long callback_data);
EXTERN_C IMAGERESULT image_trimming(const IMAGE_INFO *in_info, const IMAGE_DATA *in_data, IMAGE_INFO *out_info, IMAGE_DATA *out_data, const TRIMMINGPARAM *param, callback_t callback_func, long callback_data);

#endif
