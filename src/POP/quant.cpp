// quant.cpp
#include "stdafx.h"
#include "quant.h"

#include "quant_palette.h"              // quant_palette()
#include "quant_dither.h"               // quant_dither()
#include "operation.h"                  // count_colors()

#undef max
#undef min
#include <algorithm>                    // std::copy()
#include <valarray>                     // std::valarray<>


// 指定色に減色
EXTERN_C
IMAGERESULT quantize(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const uint_t new_colors, const QUANT_PALETTE_METHOD palette_method, const QUANT_DITHER_METHOD dither_method, callback_t callback_func, long callback_data)
{
	assert(new_colors <= MAX_COLOR_HASH + 1);
	assert(in_info_ptr->mode == COLORMODE_RGB);

	const int width  = in_info_ptr->width;  // イメージの幅
	const int height = in_info_ptr->height; // イメージの高さ

	*out_info_ptr = *in_info_ptr;

	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// コールバック関数の補正
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		// メモリ領域をロック
		const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
		pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
		uint32_t               pixel_count = 0;
		auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out, &pixel_count);

		// すでに色数が指定数より少ない場合
		if(in_info_ptr->colors <= new_colors)
		{
			callback_func(0, 1, callback_data);
			std::copy(&pixel_rgb8_ptr_in[0], &pixel_rgb8_ptr_in[pixel_count], &pixel_rgb8_ptr_out[0]);
			callback_func(1, 1, callback_data);

			return IR_SUCCESS;
		}

		// パレット作成＆ディザリング
		if(TRUE)
		{
			std::valarray<palette8_t> palette(new_colors);

			quant_palette(
				in_info_ptr, pixel_rgb8_ptr_in,
				&palette[0], palette.size(),
				palette_method,
				callback_func, callback_data);

			quant_dither(
				in_info_ptr, pixel_rgb8_ptr_in, pixel_rgb8_ptr_out,
				&palette[0], palette.size(),
				dither_method,
				callback_func, callback_data);
		}

		out_info_ptr->colors = image_count_colors(out_info_ptr, out_data_ptr);
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}
