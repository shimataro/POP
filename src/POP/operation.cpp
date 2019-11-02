// operation.cpp …インプリメンテーションファイル
#include "stdafx.h"
#include "operation.h"
#include "callback.h"                   // callback_t
#include "quant.h"                      // quantize()

#include <assert.h>                     // assert()
#include <memory.h>                     // memset(), memmove(), NULL

#undef max
#undef min
#include <algorithm>                    // std::fill_n(), std::copy(), std::swap()
#include <map>                          // std::map<>
#include <valarray>                     // std::valarray<>


// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	uint32_t _image_count_colors_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr);
	uint32_t _image_count_colors_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr);
	uint32_t _image_count_colors_rgb8 (const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr);

	void _image_convert_to_grayscale_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_convert_to_grayscale_rgb8 (const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);

	void _image_convert_to_index_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_convert_to_index_rgb8 (const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);

	void _image_convert_to_rgb_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_convert_to_rgb_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// アルファチャネルを追加
	void _image_alpha_append_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_alpha_append_index(                                   const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_alpha_append_rgb8 (const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// アルファチャネルを特定の値で埋める
	void _image_alpha_fill_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data);
	void _image_alpha_fill_index(                                   const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data);
	void _image_alpha_fill_rgb8 (const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data);


	// アルファ値を反転
	void _image_alpha_inverse_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_alpha_inverse_index(                                   const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_alpha_inverse_rgb8 (const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// 色の入れ替え
	void _image_swap_colors_index(                                   const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const int red, const int green, const int blue, callback_t callback_func, long callback_data);
	void _image_swap_colors_rgb8 (const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const int red, const int green, const int blue, callback_t callback_func, long callback_data);


	// フリップ（上下反転）
	void _image_flip_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_flip_index(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_flip_rgb8 (const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// ミラー（左右反転）
	void image_mirror_gray8(const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void image_mirror_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void image_mirror_rgb8 (const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// 90°回転
	void _image_turn_90_gray8(const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_90_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_90_rgb8 (const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// 180°回転
	void _image_turn_180_gray8(const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_180_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_180_rgb8 (const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// 270°回転
	void _image_turn_270_gray8(const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_270_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_270_rgb8 (const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// サイズ変更
	void _image_resize_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data);
	void _image_resize_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data);
	void _image_resize_rgb8 (const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data);


	// トリミング
	void _image_trimming_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data);
	void _image_trimming_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data);
	void _image_trimming_rgb8 (const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data);
END_NAMESPACE()


// 空白のイメージを作成
EXTERN_C
IMAGERESULT image_create_empty(IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, const NEWIMAGEPARAM *param)
{
	// IMAGE_INFO構造体の設定
	info_ptr->width      = param->width;
	info_ptr->height     = param->height;
	info_ptr->depth      = 8;
	info_ptr->mode       = COLORMODE_RGB;
	info_ptr->alpha      = FALSE;
	info_ptr->colors     = 1;
	info_ptr->density_x  = 0;
	info_ptr->density_y  = 0;
	info_ptr->gamma_file = GAMMA_DEFAULT_FILE;
	info_ptr->comment    = NULL;

	// メモリを確保
	if(!image_alloc(info_ptr, data_ptr))
	{
		return IR_NOMEMORY;
	}

	// メモリ領域をロック
	pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	uint32_t pixel_count = 0;
	auto_lock lock_obj(data_ptr, pixel_rgb8_ptr, &pixel_count);

	// 指定色で埋める
	std::fill_n(pixel_rgb8_ptr, pixel_count, param->rgb);

	return IR_SUCCESS;
}

// 色数を数える
EXTERN_C
uint32_t image_count_colors(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr)
{
	switch(info_ptr->mode)
	{
	case COLORMODE_GRAYSCALE:
		return _image_count_colors_gray8(info_ptr, data_ptr);

	case COLORMODE_INDEX:
		return _image_count_colors_index(info_ptr, data_ptr);

	case COLORMODE_RGB:
		return _image_count_colors_rgb8(info_ptr, data_ptr);

	default:
		return 0;
	}
}


////////////////////////////////////////////////////////////////////////////////
// カラーモードの変換

EXTERN_C
IMAGERESULT image_convert_colormode(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int new_mode, callback_t callback_func, long callback_data)
{
	assert(in_info_ptr->mode != new_mode);

	*out_info_ptr = *in_info_ptr;
	out_info_ptr->mode  = new_mode;
	out_info_ptr->depth = 8;

	// メモリを確保
	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// コールバック関数の補正
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	IMAGERESULT result;
	IMAGE_INFO tmp_info;
	IMAGE_DATA tmp_data = {0, 0, NULL, NULL};

	try
	{
		switch(new_mode)
		{
		case COLORMODE_GRAYSCALE:
			switch(in_info_ptr->mode)
			{
			case COLORMODE_INDEX:
				_image_convert_to_grayscale_index(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			case COLORMODE_RGB:
				_image_convert_to_grayscale_rgb8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			default:
				throw IR_UNKNOWN;
			}
			break;

		case COLORMODE_INDEX:
			switch(in_info_ptr->mode)
			{
			case COLORMODE_GRAYSCALE:
				_image_convert_to_index_gray8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			case COLORMODE_RGB:
				// 減色してから変換
				result = quantize(in_info_ptr, in_data_ptr, &tmp_info, &tmp_data, 1 << in_info_ptr->depth, QPM_FREQ, QDM_NONE, NULL, 0);
				if(result != IR_SUCCESS)
				{
					throw result;
				}
				_image_convert_to_index_rgb8(&tmp_info, &tmp_data, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			default:
				throw IR_UNKNOWN;
			}
			break;

		case COLORMODE_RGB:
			switch(in_info_ptr->mode)
			{
			case COLORMODE_GRAYSCALE:
				_image_convert_to_rgb_gray8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			case COLORMODE_INDEX:
				_image_convert_to_rgb_index(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			default:
				throw IR_UNKNOWN;
			}
		}
		out_info_ptr->colors = image_count_colors(out_info_ptr, out_data_ptr);
		image_free(&tmp_data);
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(&tmp_data);
		image_free(out_data_ptr);
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// アルファチャネルの処理

// アルファチャネルを追加/削除
EXTERN_C
IMAGERESULT image_alpha_append(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const BOOL append, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	// アルファチャネルフラグの設定
	*out_info_ptr = *in_info_ptr;
	out_info_ptr->alpha = !in_info_ptr->alpha;

	// メモリを確保
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
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_alpha_append_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_alpha_append_index(in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_alpha_append_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// アルファチャネルを特定の値で埋める
EXTERN_C
IMAGERESULT image_alpha_fill(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	assert(info_ptr->alpha);

	// メモリを確保
	if(!image_alloc(info_ptr, out_data_ptr))
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
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_alpha_fill_gray8(width, height, in_data_ptr, out_data_ptr, alpha_val, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_alpha_fill_index(in_data_ptr, out_data_ptr, alpha_val, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_alpha_fill_rgb8(width, height, in_data_ptr, out_data_ptr, alpha_val, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// アルファ値の反転
EXTERN_C
IMAGERESULT image_alpha_inverse(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	assert(info_ptr->alpha);

	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// メモリを確保
	if(!image_alloc(info_ptr, out_data_ptr))
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
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_alpha_inverse_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_alpha_inverse_index(in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_alpha_inverse_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// 色の入れ替え

EXTERN_C
IMAGERESULT image_swap_colors(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const int red, const int green, const int blue, callback_t callback_func, long callback_data)
{
	assert(info_ptr->mode != COLORMODE_GRAYSCALE);

	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// メモリを確保
	if(!image_alloc(info_ptr, out_data_ptr))
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
		switch(info_ptr->mode)
		{
		case COLORMODE_INDEX:
			_image_swap_colors_index(in_data_ptr, out_data_ptr, red, green, blue, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_swap_colors_rgb8(width, height, in_data_ptr, out_data_ptr, red, green, blue, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// 画像の回転・反転

// 上下反転（フリップ）
EXTERN_C
IMAGERESULT image_flip(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	*out_info_ptr = *in_info_ptr;

	// メモリを確保
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
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_flip_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_flip_index(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_flip_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// 左右反転（ミラー）
EXTERN_C
IMAGERESULT image_mirror(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	*out_info_ptr = *in_info_ptr;

	// メモリを確保
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
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			image_mirror_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			image_mirror_index(width, height, out_info_ptr->depth, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			image_mirror_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// 90°回転
EXTERN_C
IMAGERESULT image_turn_90(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// 幅と高さを入れ替え
	const int width  = in_info_ptr->height;
	const int height = in_info_ptr->width;

	*out_info_ptr = *in_info_ptr;
	out_info_ptr->width  = width;
	out_info_ptr->height = height;

	// メモリを確保
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
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_turn_90_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_turn_90_index(width, height, out_info_ptr->depth, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_turn_90_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// 180°回転
EXTERN_C
IMAGERESULT image_turn_180(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	*out_info_ptr = *in_info_ptr;

	// メモリを確保
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
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_turn_180_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_turn_180_index(width, height, out_info_ptr->depth, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_turn_180_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// 270°回転
EXTERN_C
IMAGERESULT image_turn_270(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// 幅と高さを入れ替え
	const int width  = in_info_ptr->height;
	const int height = in_info_ptr->width;

	*out_info_ptr = *in_info_ptr;
	out_info_ptr->width  = width;
	out_info_ptr->height = height;

	// メモリを確保
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
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_turn_270_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_turn_270_index(width, height, out_info_ptr->depth, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_turn_270_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// 画像サイズの変更
EXTERN_C
IMAGERESULT image_resize(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data)
{
	const int new_width  = param->new_width;
	const int new_height = param->new_height;

	// 画像の面積が0の場合
	if(new_width * new_height == 0)
	{
		return IR_ILLEGALPARAM;
	}

	*out_info_ptr = *in_info_ptr;
	out_info_ptr->width  = new_width;
	out_info_ptr->height = new_height;

	// メモリを確保
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
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_resize_gray8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, param, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_resize_index(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, param, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_resize_rgb8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, param, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
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


////////////////////////////////////////////////////////////////////////////////
// 画像サイズの変更
EXTERN_C
IMAGERESULT image_trimming(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const TRIMMINGPARAM *param, callback_t callback_func, long callback_data)
{
	// 新しい画像の幅、高さ
	const int new_width  =
		in_info_ptr->width +
		(param->margin_left - param->clip_left) +
		(param->margin_right - param->clip_right);
	const int new_height =
		in_info_ptr->height +
		(param->margin_top - param->clip_top) +
		(param->margin_bottom - param->clip_bottom);

	// 新しい画像の面積が0の場合
	if(new_width * new_height == 0)
	{
		return IR_ILLEGALPARAM;
	}

	*out_info_ptr = *in_info_ptr;
	out_info_ptr->width  = new_width;
	out_info_ptr->height = new_height;

	// メモリを確保
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
		// 開始位置、終了位置
		const int x_begin = param->margin_left;
		const int x_end   = new_width - param->margin_right;
		const int y_begin = param->margin_top;
		const int y_end   = new_height - param->margin_bottom;

		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_trimming_gray8(
				in_info_ptr, in_data_ptr,
				out_info_ptr, out_data_ptr,
				param->clip_left, param->clip_top,
				x_begin, x_end,
				y_begin, y_end,
				callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_trimming_index(
				in_info_ptr, in_data_ptr,
				out_info_ptr, out_data_ptr,
				param->clip_left, param->clip_top,
				x_begin, x_end,
				y_begin, y_end,
				callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_trimming_rgb8(
				in_info_ptr, in_data_ptr,
				out_info_ptr, out_data_ptr,
				param->clip_left, param->clip_top,
				x_begin, x_end,
				y_begin, y_end,
				callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
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


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// 色数を数える（グレイスケール8bit以下）
uint32_t _image_count_colors_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;
	const_uint_t depth = info_ptr->depth;

	// フラグ領域
	BOOL flags[256];
	std::fill_n(flags, SIZE_OF_ARRAY(flags), FALSE);

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_gray8_ptr);

	// すべての画素に対して調べる
	for(int xy = 0; xy < width * height; xy++)
	{
		// グレイスケール値を取得
		const_uint32_t color = pixel_gray8_ptr[xy].el.gray;

		// フラグを立てる
		flags[color] = TRUE;
	}

	// 立っているフラグの数を数える
	int color_count = 0;
	for(int i = 0; i < SIZE_OF_ARRAY(flags); i++)
	{
		color_count += flags[i];
	}
	return color_count;
}

// 色数を数える（インデックスカラー）
uint32_t _image_count_colors_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;
	const_uint_t   depth = info_ptr->depth;
	const_uint32_t hsize = CALC_HSIZE(width, depth);

	// log2(depth)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// フラグ領域
	BOOL flags[256];
	std::fill_n(flags, SIZE_OF_ARRAY(flags), FALSE);

	// メモリ領域をロック
	const_uint8_ptr_t    index_ptr = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr);

	// すべての画素に対して調べる
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// オフセットとシフト量を求める
			const_uint32_t offset = CALC_PACK_OFFSET(x, log2_depth);
			const int      shifts = CALC_PACK_SHIFTS(x, log2_depth);

			// インデックス値を取得
			const_uint8_t index = PACK_EXTRACT(index_ptr[offset], depth, shifts);

			// フラグを立てる
			flags[index] = TRUE;
		}
		index_ptr += hsize;
	}

	// 立っているフラグの数を数える
	int color_count = 0;
	for(int i = 0; i < SIZE_OF_ARRAY(flags); i++)
	{
		color_count += flags[i];
	}
	return color_count;
}

// 色数を数える（RGB8bit）
uint32_t _image_count_colors_rgb8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// フラグ領域（2MByteも使う…。ま、最近のマシンなら大丈夫か）
	std::valarray<uint8_t> flags((uint8_t)0, 0x01000000 / 8);

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

	// すべての画素に対して調べる
	uint32_t color_count = 0;
	for(int xy = 0; xy < width * height; xy++)
	{
		const_uint32_t color = pixel_rgb8_ptr[xy].value & 0x00ffffff;

		const_uint32_t offset = color >> 3;
		const_uint8_t  mask   = 0x80 >> (color & 7);
		if((flags[offset] & mask) == 0)
		{
			flags[offset] |= mask;
			color_count++;
		}
	}

	return color_count;
}


// グレイスケールに変換（インデックスカラーから）
void _image_convert_to_grayscale_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;
	const_uint_t   depth_index = in_info_ptr->depth;
	const_uint32_t hsize_index = CALC_HSIZE(width, depth_index);

	// log2(depth_index)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth_index = log2_depth_array[depth_index - 1];

	// スケールアップテーブルを作成
	std::valarray<uint8_t> scaleup_table(1 << depth_index);
	create_scaleup_table(&scaleup_table[0], depth_index);

	// メモリ領域をロック
	const_uint8_ptr_t index8_ptr = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, index8_ptr, palette8_ptr);

	pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_out(out_data_ptr, pixel_gray8_ptr);

	// グレイスケールに変換
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// 入力オフセットとシフト量を求める
			const_uint32_t offset_index = CALC_PACK_OFFSET(x, log2_depth_index);
			const int      shifts_index = CALC_PACK_SHIFTS(x, log2_depth_index);

			// インデックスからグレイスケール値を求める
			const_uint8_t index = PACK_EXTRACT(index8_ptr[offset_index], depth_index, shifts_index);
			const_uint8_t gray  = GRAYSCALE(palette8_ptr[index].r, palette8_ptr[index].g, palette8_ptr[index].b);
			const_uint8_t alpha = palette8_ptr[index].a;

			// グレイスケール値を0～255にスケールアップして格納
			pixel_gray8_ptr->el.gray  = scaleup_table[gray];
			pixel_gray8_ptr->el.alpha = scaleup_table[alpha];
			pixel_gray8_ptr++;
		}
		index8_ptr += hsize_index;

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

// グレイスケールに変換（RGB8bitから）
void _image_convert_to_grayscale_rgb8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr);

	pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_out(out_data_ptr, pixel_gray8_ptr);

	// グレイスケールに変換
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// グレイスケール値を求める
			pixel_gray8_ptr->el.gray  = GRAYSCALE(pixel_rgb8_ptr->el.r, pixel_rgb8_ptr->el.g, pixel_rgb8_ptr->el.b);
			pixel_gray8_ptr->el.alpha = pixel_rgb8_ptr->el.a;

			pixel_rgb8_ptr++;
			pixel_gray8_ptr++;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


// インデックスカラーに変換（グレイスケールから）
void _image_convert_to_index_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;
	const_uint_t   depth_index = in_info_ptr->depth;
	const_uint32_t hsize_index = CALC_HSIZE(width, depth_index);

	// log2(depth_index)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth_index = log2_depth_array[depth_index - 1];

	// スケールアップテーブルを作成
	std::valarray<uint8_t> scaleup_table(1 << depth_index);
	create_scaleup_table(&scaleup_table[0], depth_index);

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr);

	uint8_ptr_t    index8_ptr = NULL;
	palette8_ptr_t palette8_ptr = NULL;
	uint32_t       index_size = 0;
	auto_lock lock_out(out_data_ptr, index8_ptr, palette8_ptr, &index_size);

	memset(index8_ptr, 0, index_size);

	// カラーパレットを設定
	for(int i = 0; i < 1 << depth_index; i++)
	{
		palette8_ptr[i].r = palette8_ptr[i].g = palette8_ptr[i].b = scaleup_table[i];
		palette8_ptr[i].a = 255;
	}

	// インデックスカラーに変換
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// グレイスケール値を求める
			const_uint8_t gray  = pixel_gray8_ptr->el.gray;
			const_uint8_t alpha = pixel_gray8_ptr->el.alpha;

			// 出力オフセットとシフト量を求める
			const_uint32_t offset_index = CALC_PACK_OFFSET(x, log2_depth_index);
			const int      shifts_index = CALC_PACK_SHIFTS(x, log2_depth_index);

			const_uint8_t index = gray >> (8 - depth_index);
			PACK_STORE(index8_ptr[offset_index], index, shifts_index);

			pixel_gray8_ptr++;
		}
		index8_ptr += hsize_index;

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

// インデックスカラーに変換（RGB8bitから）
void _image_convert_to_index_rgb8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;
	const_uint_t   depth_index = out_info_ptr->depth;
	const_uint32_t hsize_index = CALC_HSIZE(width, depth_index);

	// log2(depth)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth_index = log2_depth_array[depth_index - 1];

	// 登録可能色数
	const_uint_t max_colors = 1 << depth_index;
	assert(in_info_ptr->colors <= max_colors);

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr);

	uint8_ptr_t    index8_ptr = NULL;
	palette8_ptr_t palette8_ptr = NULL;
	uint32_t       index_size = 0;
	auto_lock lock_out(out_data_ptr, index8_ptr, palette8_ptr, &index_size);

	// イメージをすべて0で埋める
	memset(index8_ptr, 0, index_size);

	uint_t registered_color_num = 0;        // 登録済み色数
	uint_t index                = 0;        // カラーインデックス

	// カラーマップ（インデックスから対応するCOLORREF値を求める）
	std::valarray<COLORREF> colmap(max_colors);
	colmap[0] = 0xffffffffUL;

	// 逆カラーマップ（COLORREF値から対応するインデックスを求める）（ハッシュ＋連想配列）
	typedef std::map<COLORREF, int> invmap_t;
	std::valarray<invmap_t> invmap(MAX_COLOR_HASH + 1);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// RGBの輝度を取得
			const_uint8_t r = pixel_rgb8_ptr->el.r;
			const_uint8_t g = pixel_rgb8_ptr->el.g;
			const_uint8_t b = pixel_rgb8_ptr->el.b;
			const_uint32_t rgb = pixel_rgb8_ptr->value & 0x00ffffff;
			pixel_rgb8_ptr++;

			// インデックス検索（時間短縮のため、１つ前の色と異なるときのみ検索）
			if(colmap[index] != rgb)
			{
				// 輝度より対応するカラーマップのインデックスを検索
				const int hash_val = HASH_COLOR_REF(rgb);
				invmap_t::iterator it = invmap[hash_val].find(rgb);

				// 対応するカラーマップが存在しない場合、新規登録
				if(it == invmap[hash_val].end())
				{
					// 登録可能色数を超えるとアサーションエラー
					assert(registered_color_num < max_colors);

					index = registered_color_num++;

					// カラーマップインデックスを登録
					invmap[hash_val][rgb] = index;
					colmap[index] = rgb;

					// カラーパレットを登録
					palette8_ptr[index].r = r;
					palette8_ptr[index].g = g;
					palette8_ptr[index].b = b;
					palette8_ptr[index].a = 0xff;
				}
				else
				{
					index = it->second;
				}
			}

			// 出力オフセットとシフト量を求める
			const_uint32_t offset_index = CALC_PACK_OFFSET(x, log2_depth_index);
			const int      shifts_index = CALC_PACK_SHIFTS(x, log2_depth_index);

			// インデックスをパッキングする
			PACK_STORE(index8_ptr[offset_index], index, shifts_index);
		}
		index8_ptr += hsize_index;

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


void _image_convert_to_rgb_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr);

	pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_out(out_data_ptr, pixel_rgb8_ptr);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// グレイスケール値を求める
			pixel_rgb8_ptr->el.r = pixel_rgb8_ptr->el.g = pixel_rgb8_ptr->el.b = pixel_gray8_ptr->el.gray;
			pixel_rgb8_ptr->el.a = pixel_gray8_ptr->el.alpha;

			pixel_gray8_ptr++;
			pixel_rgb8_ptr++;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_convert_to_rgb_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;
	const_uint_t   depth_index = in_info_ptr->depth;
	const_uint32_t hsize_index = CALC_HSIZE(width, depth_index);

	// log2(depth)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth_index   = log2_depth_array[depth_index - 1];

	// メモリ領域をロック
	const_uint8_ptr_t index8_ptr = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, index8_ptr, palette8_ptr);

	pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_out(out_data_ptr, pixel_rgb8_ptr);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			const_uint32_t offset_index = CALC_PACK_OFFSET(x, log2_depth_index);
			const int      shifts_index = CALC_PACK_SHIFTS(x, log2_depth_index);

			// インデックスを求める
			const int index = PACK_EXTRACT(index8_ptr[offset_index], depth_index, shifts_index);

			pixel_rgb8_ptr->el.r = palette8_ptr[index].r;
			pixel_rgb8_ptr->el.g = palette8_ptr[index].g;
			pixel_rgb8_ptr->el.b = palette8_ptr[index].b;
			pixel_rgb8_ptr->el.a = palette8_ptr[index].a;
			pixel_rgb8_ptr++;
		}
		index8_ptr += hsize_index;

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// アルファチャネルの追加
void _image_alpha_append_gray8(const int width, const int  height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// アルファ値を変更（輝度値はそのまま）
			pixel_gray8_ptr_out->value = pixel_gray8_ptr_in->value | 0xff00;

			pixel_gray8_ptr_in++;
			pixel_gray8_ptr_out++;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_append_index(const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size   = 0;
	uint32_t palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	memcpy(index8_ptr_out, index8_ptr_in, index_size);

	for(uint32_t i = 0; i < palette_size; i++)
	{
		palette8_ptr_out[i].r = palette8_ptr_in[i].r;
		palette8_ptr_out[i].g = palette8_ptr_in[i].g;
		palette8_ptr_out[i].b = palette8_ptr_in[i].b;
		palette8_ptr_out[i].a = 255;

		// コールバック
		if(callback_func(i, palette_size - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_append_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// アルファ値をFFHで埋める（輝度値はそのまま）
			pixel_rgb8_ptr_out->value = pixel_rgb8_ptr_in->value | 0xff000000;

			pixel_rgb8_ptr_in++;
			pixel_rgb8_ptr_out++;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// アルファチャネルを特定の値で埋める
void _image_alpha_fill_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// アルファ値を変更（輝度値はそのまま）
			pixel_gray8_ptr_out->el.gray  = pixel_gray8_ptr_in->el.gray;
			pixel_gray8_ptr_out->el.alpha = alpha_val;

			pixel_gray8_ptr_in++;
			pixel_gray8_ptr_out++;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_fill_index(const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size   = 0;
	uint32_t palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	memcpy(index8_ptr_out, index8_ptr_in, index_size);

	for(uint32_t i = 0; i < palette_size; i++)
	{
		palette8_ptr_out[i].r = palette8_ptr_in[i].r;
		palette8_ptr_out[i].g = palette8_ptr_in[i].g;
		palette8_ptr_out[i].b = palette8_ptr_in[i].b;
		palette8_ptr_out[i].a = alpha_val;

		// コールバック
		if(callback_func(i, palette_size - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_fill_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// アルファ値を変更（輝度値はそのまま）
			pixel_rgb8_ptr_out->el.r = pixel_rgb8_ptr_in->el.r;
			pixel_rgb8_ptr_out->el.g = pixel_rgb8_ptr_in->el.g;
			pixel_rgb8_ptr_out->el.b = pixel_rgb8_ptr_in->el.b;
			pixel_rgb8_ptr_out->el.a = alpha_val;

			pixel_rgb8_ptr_in++;
			pixel_rgb8_ptr_out++;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// アルファ値を反転
void _image_alpha_inverse_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// アルファ値を変更（輝度値はそのまま）
			pixel_gray8_ptr_out->value = pixel_gray8_ptr_in->value ^ 0xff00;

			pixel_gray8_ptr_in++;
			pixel_gray8_ptr_out++;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_inverse_index(const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size   = 0;
	uint32_t palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	memcpy(index8_ptr_out, index8_ptr_in, index_size);

	for(uint32_t i = 0; i < palette_size; i++)
	{
		palette8_ptr_out[i].r = palette8_ptr_in[i].r;
		palette8_ptr_out[i].g = palette8_ptr_in[i].g;
		palette8_ptr_out[i].b = palette8_ptr_in[i].b;
		palette8_ptr_out[i].a =~palette8_ptr_in[i].a;

		// コールバック
		if(callback_func(i, palette_size - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_inverse_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// アルファ値を反転（輝度値はそのまま）
			pixel_rgb8_ptr_out->value = pixel_rgb8_ptr_in->value ^ 0xff000000;

			pixel_rgb8_ptr_in++;
			pixel_rgb8_ptr_out++;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// 色の入れ替え
void _image_swap_colors_index(const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const int red, const int green, const int blue, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size   = 0;
	uint32_t palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	memcpy(index8_ptr_out, index8_ptr_in, index_size);

	for(uint32_t i = 0; i < palette_size; i++)
	{
		// 色の入れ替え
		const uint8_t swap_pixel[] =
		{
			palette8_ptr_in[i].r,
			palette8_ptr_in[i].g,
			palette8_ptr_in[i].b,
		};
		palette8_ptr_out[i].r = swap_pixel[red];
		palette8_ptr_out[i].g = swap_pixel[green];
		palette8_ptr_out[i].b = swap_pixel[blue];
		palette8_ptr_out[i].a = palette8_ptr_in[i].a;

		// コールバック
		if(callback_func(i, palette_size - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_swap_colors_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const int red, const int green, const int blue, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// 色の入れ替え
			const uint8_t swap_pixel[] =
			{
				pixel_rgb8_ptr_in->el.r,
				pixel_rgb8_ptr_in->el.g,
				pixel_rgb8_ptr_in->el.b,
			};
			pixel_rgb8_ptr_out->el.r = swap_pixel[red];
			pixel_rgb8_ptr_out->el.g = swap_pixel[green];
			pixel_rgb8_ptr_out->el.b = swap_pixel[blue];
			pixel_rgb8_ptr_out->el.a = pixel_rgb8_ptr_in->el.a;

			pixel_rgb8_ptr_in++;
			pixel_rgb8_ptr_out++;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// フリップ（上下反転）
void _image_flip_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const_uint32_t hsize = width * sizeof(pixel_gray8_ptr_t);

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// フリップ操作
	uint32_t offset_src = 0;
	uint32_t offset_dst = CALC_OFFSET(0, height - 1,  width);
	for(int y = 0; y < height; y++)
	{
		memcpy(&pixel_gray8_ptr_out[offset_dst], &pixel_gray8_ptr_in[offset_src], hsize);

		offset_src += width;
		offset_dst -= width;

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_flip_index(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	const_uint32_t hsize = index_size / height;

	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// フリップ操作
	uint32_t offset_src = 0;
	uint32_t offset_dst = CALC_OFFSET(0, height - 1, hsize);
	for(int y = 0; y < height; y++)
	{
		memcpy(&index8_ptr_out[offset_dst], &index8_ptr_in[offset_src], hsize);

		offset_src += hsize;
		offset_dst -= hsize;

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_flip_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const_uint32_t hsize = width * sizeof(pixel_rgb8_ptr_t);

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// フリップ操作
	uint32_t offset_src = 0;
	uint32_t offset_dst = CALC_OFFSET(0, height - 1,  width);
	for(int y = 0; y < height; y++)
	{
		memcpy(&pixel_rgb8_ptr_out[offset_dst], &pixel_rgb8_ptr_in[offset_src], hsize);

		offset_src += width;
		offset_dst -= width;

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// ミラー（左右反転）
void image_mirror_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// ミラー
	uint32_t offset = 0;
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (width - x - 1, y)
		for(int x = 0; x < width; x++)
		{
			pixel_gray8_ptr_out[offset + x] = pixel_gray8_ptr_in[offset + width - x - 1];
		}
		offset += width;

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void image_mirror_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// log2(depth)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	const_uint32_t hsize = index_size / height;

	memset(index8_ptr_out, 0, index_size);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// ミラー
	uint32_t offset = 0;
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (width - x - 1, y)
		for(int x = 0; x < width; x++)
		{
			// 入力側の座標(width - x - 1, y)のオフセット値、シフト量を求める
			const_uint32_t offset_in = CALC_PACK_OFFSET(width - x - 1, log2_depth);
			const int      shifts_in = CALC_PACK_SHIFTS(width - x - 1, log2_depth);

			// 出力側の座標(x, y)のオフセット値、シフト量を求める
			const_uint32_t offset_out = CALC_PACK_OFFSET(x, log2_depth);
			const int      shifts_out = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset_in], depth, shifts_in);
			PACK_STORE(index8_ptr_out[offset_out], index, shifts_out);
		}
		index8_ptr_in  += hsize;
		index8_ptr_out += hsize;

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void image_mirror_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// ミラー
	uint32_t offset = 0;
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (width - x - 1, y)
		for(int x = 0; x < width; x++)
		{
			pixel_rgb8_ptr_out[offset + x] = pixel_rgb8_ptr_in[offset + width - x - 1];
		}
		offset += width;

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// 90°回転
void _image_turn_90_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// 90°回転
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (y, width - x - 1)
		uint32_t offset = CALC_OFFSET(y, width - 1, height);
		for(int x = 0; x < width; x++)
		{
			*pixel_gray8_ptr_out++ = pixel_gray8_ptr_in[offset];
			offset -= height;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_90_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// log2(depth)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	const_uint32_t hsize = index_size / height;
	const_uint32_t vsize = index_size / width;

	memset(index8_ptr_out, 0, index_size);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// 90°回転
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (y, width - x - 1)
		for(int x = 0; x < width; x++)
		{
			// 入力側の座標(y, width - x - 1)のオフセット値、シフト量を求める
			const_uint32_t offset_in = CALC_PACK_OFFSET(y, log2_depth) + vsize * (width - x - 1);
			const int      shifts_in = CALC_PACK_SHIFTS(y, log2_depth);		

			// 出力側の座標(x, y)のオフセット値、シフト量を求める
			const_uint32_t offset_out = CALC_PACK_OFFSET(x, log2_depth) + hsize * y;
			const int      shifts_out = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset_in], depth, shifts_in);
			PACK_STORE(index8_ptr_out[offset_out], index, shifts_out);
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_90_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// 90°回転
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (y, width - x - 1)
		uint32_t offset = CALC_OFFSET(y, width - 1, height);
		for(int x = 0; x < width; x++)
		{
			*pixel_rgb8_ptr_out++ = pixel_rgb8_ptr_in[offset];
			offset -= height;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// 180°回転
void _image_turn_180_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	uint32_t offset_in  = CALC_OFFSET(width - 1, height - 1, width);
	uint32_t offset_out = 0;

	// 180°回転
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (width - x - 1, height - y - 1)
		for(int x = 0; x < width; x++)
		{
			pixel_gray8_ptr_out[offset_out] = pixel_gray8_ptr_in[offset_in];

			offset_in--;
			offset_out++;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_180_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// log2(depth)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	const_uint32_t hsize = index_size / height;

	memset(index8_ptr_out, 0, index_size);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// 180°回転
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (width - x - 1, height - y - 1)
		for(int x = 0; x < width; x++)
		{
			// 入力側の座標(width - x - 1, height - y - 1)のオフセット値、シフト量を求める
			const_uint32_t offset_in = CALC_PACK_OFFSET(width - x - 1, log2_depth) + hsize * (height - y - 1);
			const int      shifts_in = CALC_PACK_SHIFTS(width - x - 1, log2_depth);		

			// 出力側の座標(x, y)のオフセット値、シフト量を求める
			const_uint32_t offset_out = CALC_PACK_OFFSET(x, log2_depth) + hsize * y;
			const int      shifts_out = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset_in], depth, shifts_in);
			PACK_STORE(index8_ptr_out[offset_out], index, shifts_out);
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_180_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	uint32_t offset_in  = CALC_OFFSET(width - 1, height - 1, width);
	uint32_t offset_out = 0;

	// 180°回転
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (width - x - 1, height - y - 1)
		for(int x = 0; x < width; x++)
		{
			pixel_rgb8_ptr_out[offset_out] = pixel_rgb8_ptr_in[offset_in];

			offset_in--;
			offset_out++;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// 270°回転
void _image_turn_270_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// 270°回転
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (height - y - 1, x)
		uint32_t offset = CALC_OFFSET(height - y - 1, 0, height);
		for(int x = 0; x < width; x++)
		{
			*pixel_gray8_ptr_out++ = pixel_gray8_ptr_in[offset];
			offset += height;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_270_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// log2(depth)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	const_uint32_t hsize = index_size / height;
	const_uint32_t vsize = index_size / width;

	memset(index8_ptr_out, 0, index_size);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// 270°回転
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (height - y - 1, x)
		for(int x = 0; x < width; x++)
		{
			// 入力側の座標(height - y - 1, x)のオフセット値、シフト量を求める
			const_uint32_t offset_in = CALC_PACK_OFFSET(height - y - 1, log2_depth) + vsize * x;
			const int      shifts_in = CALC_PACK_SHIFTS(height - y - 1, log2_depth);		

			// 出力側の座標(x, y)のオフセット値、シフト量を求める
			const_uint32_t offset_out = CALC_PACK_OFFSET(x, log2_depth) + hsize * y;
			const int      shifts_out = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset_in], depth, shifts_in);
			PACK_STORE(index8_ptr_out[offset_out], index, shifts_out);
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_270_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// 270°回転
	for(int y = 0; y < height; y++)
	{
		// (x, y) ← (height - y - 1, x)
		uint32_t offset = CALC_OFFSET(height - y - 1, 0, height);
		for(int x = 0; x < width; x++)
		{
			*pixel_rgb8_ptr_out++ = pixel_rgb8_ptr_in[offset];
			offset += height;
		}

		// コールバック
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// サイズ変更
void _image_resize_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data)
{
	const int new_width  = param->new_width;
	const int new_height = param->new_height;
	const INTERPOLATE_METHOD interpolate_method = param->interpolate_method;

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	uint32_t pixel_count_in = 0;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in, &pixel_count_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// 新しいイメージのオフセット
	uint32_t offset_new = 0;

	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	try
	{
		// 
		std::valarray<uint8_t> g_plane(pixel_count_in), a_plane(pixel_count_in);
		for(uint32_t i = 0; i < pixel_count_in; i++)
		{
			g_plane[i] = pixel_gray8_ptr_in[i].el.gray;
			a_plane[i] = pixel_gray8_ptr_in[i].el.alpha;
		}

		// 拡大／縮小
		const double width_rate  = (new_width  > 1) ? (double)(width  - 1) / (new_width  - 1) : 0;
		const double height_rate = (new_height > 1) ? (double)(height - 1) / (new_height - 1) : 0;
		for(int new_y = 0; new_y < new_height; new_y++)
		{
			for(int new_x = 0; new_x < new_width; new_x++)
			{
				const double interpolate_x = new_x * width_rate;
				const double interpolate_y = new_y * height_rate;

				pixel_gray8_ptr_out[offset_new].el.gray  = interpolate(&g_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				pixel_gray8_ptr_out[offset_new].el.alpha = interpolate(&a_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				offset_new++;
			}
			// コールバック
			if(callback_func(new_y, new_height - 1, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

void _image_resize_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	const int new_width  = param->new_width;
	const int new_height = param->new_height;

	// log2(depth)を計算
	const_uint_t depth = out_info_ptr->depth;
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size_in = 0, index_size_out = 0;
	uint32_t palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in, &index_size_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size_out, &palette_size);

	const_uint32_t hsize     = index_size_in  / height;
	const_uint32_t hsize_new = index_size_out / new_height;

	memset(index8_ptr_out, 0, index_size_out);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// 拡大／縮小
	const double width_rate  = (new_width  > 1) ? (double)(width  - 1) / (new_width  - 1) : 0;
	const double height_rate = (new_height > 1) ? (double)(height - 1) / (new_height - 1) : 0;
	for(int new_y = 0; new_y < new_height; new_y++)
	{
		for(int new_x = 0; new_x < new_width; new_x++)
		{
			const_uint32_t interpolate_x = static_cast<const_uint32_t>(new_x * width_rate);
			const_uint32_t interpolate_y = static_cast<const_uint32_t>(new_y * height_rate);

			// 入力側の座標(interpolate_x, interpolate_y)のオフセット値、シフト量を求める
			const_uint32_t offset_in = CALC_PACK_OFFSET(interpolate_x, log2_depth) + hsize * interpolate_y;
			const int      shifts_in = CALC_PACK_SHIFTS(interpolate_x, log2_depth);		

			// 出力側の座標(new_x, new_y)のオフセット値、シフト量を求める
			const_uint32_t offset_out = CALC_PACK_OFFSET(new_x, log2_depth) + hsize_new * new_y;
			const int      shifts_out = CALC_PACK_SHIFTS(new_x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset_in], depth, shifts_in);
			PACK_STORE(index8_ptr_out[offset_out], index, shifts_out);
		}
		// コールバック
		if(callback_func(new_y, new_height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_resize_rgb8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data)
{
	const int new_width  = param->new_width;
	const int new_height = param->new_height;
	const INTERPOLATE_METHOD interpolate_method = param->interpolate_method;

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	uint32_t pixel_count_in = 0;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in, &pixel_count_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// 新しいイメージのオフセット
	uint32_t offset_new = 0;

	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	try
	{
		// 
		std::valarray<uint8_t> r_plane(pixel_count_in), g_plane(pixel_count_in), b_plane(pixel_count_in), a_plane(pixel_count_in);
		for(uint32_t i = 0; i < pixel_count_in; i++)
		{
			r_plane[i] = pixel_rgb8_ptr_in[i].el.r;
			g_plane[i] = pixel_rgb8_ptr_in[i].el.g;
			b_plane[i] = pixel_rgb8_ptr_in[i].el.b;
			a_plane[i] = pixel_rgb8_ptr_in[i].el.a;
		}

		// 拡大／縮小
		const double width_rate  = (new_width  > 1) ? (double)(width  - 1) / (new_width  - 1) : 0;
		const double height_rate = (new_height > 1) ? (double)(height - 1) / (new_height - 1) : 0;
		for(int new_y = 0; new_y < new_height; new_y++)
		{
			for(int new_x = 0; new_x < new_width; new_x++)
			{
				// RGBの3色分
				const double interpolate_x = new_x * width_rate;
				const double interpolate_y = new_y * height_rate;

				pixel_rgb8_ptr_out[offset_new].el.r = interpolate(&r_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				pixel_rgb8_ptr_out[offset_new].el.g = interpolate(&g_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				pixel_rgb8_ptr_out[offset_new].el.b = interpolate(&b_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				pixel_rgb8_ptr_out[offset_new].el.a = interpolate(&a_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				offset_new++;
			}
			// コールバック
			if(callback_func(new_y, new_height - 1, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}


////////////////////////////////////////////////////////////////////////////////
// トリミング
void _image_trimming_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;
	const int new_width  = out_info_ptr->width;
	const int new_height = out_info_ptr->height;
	const_pixel_gray8_t space_color = {0xffff};

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// 元画像のオフセット
	pixel_gray8_ptr_in += CALC_OFFSET(clip_left, clip_top, width);

	// 上部余白
	if(TRUE)
	{
		const int fill_count = new_width * y_begin;

		// 空白で埋める
		std::fill_n(pixel_gray8_ptr_out, fill_count, space_color);
		pixel_gray8_ptr_out += fill_count;
	}

	// イメージ部
	for(int y = y_begin; y < y_end; y++)
	{
		// 左側余白
		if(TRUE)
		{
			const int fill_count = x_begin;

			// 空白で埋める
			std::fill_n(pixel_gray8_ptr_out, fill_count, space_color);
			pixel_gray8_ptr_out += fill_count;
		}

		// イメージ部
		if(TRUE)
		{
			const int copy_count = x_end - x_begin;

			// イメージ部をコピー
			std::copy(
				&pixel_gray8_ptr_in[0],
				&pixel_gray8_ptr_in[copy_count],
				pixel_gray8_ptr_out);

			pixel_gray8_ptr_in  += width;
			pixel_gray8_ptr_out += copy_count;
		}

		// 右側余白
		if(TRUE)
		{
			const int fill_count = new_width - x_end;

			// 空白で埋める
			std::fill_n(pixel_gray8_ptr_out, fill_count, space_color);
			pixel_gray8_ptr_out += fill_count;
		}

		// コールバック
		if(callback_func(y, y_end - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}

	// 下部余白
	if(TRUE)
	{
		const int fill_count = new_width * (new_height - y_end);

		// 空白で埋める
		std::fill_n(pixel_gray8_ptr_out, fill_count, space_color);
		pixel_gray8_ptr_out += fill_count;
	}
}

void _image_trimming_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;
	const int new_width  = out_info_ptr->width;
	const int new_height = out_info_ptr->height;

	// log2(depth)を計算
	const_uint_t depth = out_info_ptr->depth;
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size_out = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size_out, &palette_size);

	const_uint32_t hsize     = CALC_HSIZE(    width,  in_info_ptr->depth);
	const_uint32_t hsize_new = CALC_HSIZE(new_width, out_info_ptr->depth);

	memset(index8_ptr_out, 0, index_size_out);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	index8_ptr_in  += clip_top * hsize;
	index8_ptr_out += y_begin  * hsize_new;

	for(int y = y_begin; y < y_end; y++)
	{
		for(int x = x_begin; x < x_end; x++)
		{
			// 元の画像のオフセット値、シフト量を求める
			const_uint32_t offset = CALC_PACK_OFFSET(clip_left + (x - x_begin), log2_depth);
			const int      shifts = CALC_PACK_SHIFTS(clip_left + (x - x_begin), log2_depth);

			// 新しい画像(x, y)のオフセット値、シフト量を求める
			const_uint32_t offset_new = CALC_PACK_OFFSET(x, log2_depth);
			const int      shifts_new = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset], depth, shifts);
			PACK_STORE(index8_ptr_out[offset_new], index, shifts_new);
		}
		index8_ptr_in  += hsize;
		index8_ptr_out += hsize_new;

		// コールバック
		if(callback_func(y, new_height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_trimming_rgb8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;
	const int new_width  = out_info_ptr->width;
	const int new_height = out_info_ptr->height;
	const_pixel_rgb8_t space_color = {0xffffffff};

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// 元画像のオフセット
	pixel_rgb8_ptr_in += CALC_OFFSET(clip_left, clip_top, width);

	// 上部余白
	if(TRUE)
	{
		const int fill_count = new_width * y_begin;

		// 空白で埋める
		std::fill_n(pixel_rgb8_ptr_out, fill_count, space_color);
		pixel_rgb8_ptr_out += fill_count;
	}

	// イメージ部
	for(int y = y_begin; y < y_end; y++)
	{
		// 左側余白
		if(TRUE)
		{
			const int fill_count = x_begin;

			// 空白で埋める
			std::fill_n(pixel_rgb8_ptr_out, fill_count, space_color);
			pixel_rgb8_ptr_out += fill_count;
		}

		// イメージ部
		if(TRUE)
		{
			const int copy_count = x_end - x_begin;

			// イメージ部をコピー
			std::copy(
				&pixel_rgb8_ptr_in[0],
				&pixel_rgb8_ptr_in[copy_count],
				pixel_rgb8_ptr_out);

			pixel_rgb8_ptr_in  += width;
			pixel_rgb8_ptr_out += copy_count;
		}

		// 右側余白
		if(TRUE)
		{
			const int fill_count = new_width - x_end;

			// 空白で埋める
			std::fill_n(pixel_rgb8_ptr_out, fill_count, space_color);
			pixel_rgb8_ptr_out += fill_count;
		}

		// コールバック
		if(callback_func(y, y_end - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}

	// 下部余白
	if(TRUE)
	{
		const int fill_count = new_width * (new_height - y_end);

		// 空白で埋める
		std::fill_n(pixel_rgb8_ptr_out, fill_count, space_color);
		pixel_rgb8_ptr_out += fill_count;
	}
}

END_NAMESPACE()
