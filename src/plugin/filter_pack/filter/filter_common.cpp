// filter_common.cpp …インプリメンテーションファイル
#include "../stdafx.h"
#include "filter_common.h"

#undef max
#undef min
#include <valarray>                     // std::valarray<>


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// インクリメンタルコピー
	template<typename T>
	int copy_elements(T *const dst, const T *const src, const int count)
	{
		memcpy(dst, src, count * sizeof(T));
		return count;
	}

	// デフォルトコールバック関数
	int CALLBACK callback_none(const int /* num */, const int /* denom */, const long /* data */)
	{return 0;}

	void transform_color_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, COLORPROC colorproc, void_ptr_t param, callback_t callback, long callback_data);
	void transform_color_rgb8 (const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, COLORPROC colorproc, void_ptr_t param, callback_t callback, long callback_data);

	void transform_geometric_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC geometricproc, void_ptr_t param, const INTERPOLATE_METHOD interpolate_method, callback_t callback, long callback_data);
	void transform_geometric_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC geometricproc, void_ptr_t param                                         , callback_t callback, long callback_data);
	void transform_geometric_rgb8 (const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC geometricproc, void_ptr_t param, const INTERPOLATE_METHOD interpolate_method, callback_t callback, long callback_data);

	void transform_geometric_integer_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC_INT geometricproc, void_ptr_t param, callback_t callback, long callback_data);
	void transform_geometric_integer_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC_INT geometricproc, void_ptr_t param, callback_t callback, long callback_data);
	void transform_geometric_integer_rgb8 (const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC_INT geometricproc, void_ptr_t param, callback_t callback, long callback_data);

	void custom_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, CUSTOMPROC customproc, const int pixelcount_x, const int pixelcount_y, void_ptr_t param, callback_t callback, long callback_data);
	void custom_rgb8 (const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, CUSTOMPROC customproc, const int pixelcount_x, const int pixelcount_y, void_ptr_t param, callback_t callback, long callback_data);
END_NAMESPACE()


// 濃度変換
IMAGERESULT _pop_filter_transform_color(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, COLORPROC colorproc, void_ptr_t param, callback_t callback, long callback_data)
{
	// コールバック関数の補正
	if(callback == NULL)
	{
		callback = callback_none;
	}

	try
	{
		switch(info_ptr->mode)
		{
		case COLORMODE_INDEX:
			transform_color_index(info_ptr, in_data_ptr, out_data_ptr, colorproc, param, callback, callback_data);
			break;

		case COLORMODE_RGB:
			transform_color_rgb8(info_ptr, in_data_ptr, out_data_ptr, colorproc, param, callback, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		return result;
	}
}

// 幾何学的変換(実数座標）
IMAGERESULT _pop_filter_transform_geometric(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC geometricproc, void_ptr_t param, const INTERPOLATE_METHOD interpolate_method, callback_t callback, long callback_data)
{
	// コールバック関数の補正
	if(callback == NULL)
	{
		callback = callback_none;
	}

	try
	{
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			transform_geometric_gray8(info_ptr, in_data_ptr, out_data_ptr, geometricproc, param, interpolate_method, callback, callback_data);
			break;

		case COLORMODE_INDEX:
			transform_geometric_index(info_ptr, in_data_ptr, out_data_ptr, geometricproc, param, callback, callback_data);
			break;

		case COLORMODE_RGB:
			transform_geometric_rgb8(info_ptr, in_data_ptr, out_data_ptr, geometricproc, param, interpolate_method, callback, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		return result;
	}
}

// 幾何学的変換（整数座標）
IMAGERESULT _pop_filter_transform_geometric_integer(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC_INT geometricproc, void_ptr_t param, callback_t callback, long callback_data)
{
	// コールバック関数の補正
	if(callback == NULL)
	{
		callback = callback_none;
	}

	try
	{
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			transform_geometric_integer_gray8(info_ptr, in_data_ptr, out_data_ptr, geometricproc, param, callback, callback_data);
			break;

		case COLORMODE_INDEX:
			transform_geometric_integer_index(info_ptr, in_data_ptr, out_data_ptr, geometricproc, param, callback, callback_data);
			break;

		case COLORMODE_RGB:
			transform_geometric_integer_rgb8(info_ptr, in_data_ptr, out_data_ptr, geometricproc, param, callback, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		return result;
	}
}


IMAGERESULT _pop_filter_custom(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, CUSTOMPROC customproc, const int pixelcount_x, const int pixelcount_y, void_ptr_t param, callback_t callback, long callback_data)
{
	// コールバック関数の補正
	if(callback == NULL)
	{
		callback = callback_none;
	}

	try
	{
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			custom_gray8(info_ptr, in_data_ptr, out_data_ptr, customproc, pixelcount_x, pixelcount_y, param, callback, callback_data);
			break;

		case COLORMODE_RGB:
			custom_rgb8(info_ptr, in_data_ptr, out_data_ptr, customproc, pixelcount_x, pixelcount_y, param, callback, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

void transform_color_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, COLORPROC colorproc, void_ptr_t param, callback_t callback, long callback_data)
{
	// メモリ領域をロック
	const_uint8_ptr_t    index_ptr_in    = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index_ptr_out    = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	memcpy(index_ptr_out, index_ptr_in, index_size);

	for(uint32_t i = 0; i < palette_size; i++)
	{
		// 輝度値を変換
		COLOR color = {palette8_ptr_in->r, palette8_ptr_in->g, palette8_ptr_in->b};
		colorproc(&color, param);

		// 新しいパレットへ格納
		palette8_ptr_out->r = color.red;
		palette8_ptr_out->g = color.green;
		palette8_ptr_out->b = color.blue;
		palette8_ptr_out->a = palette8_ptr_in->a;

		palette8_ptr_in++;
		palette8_ptr_out++;

		// コールバック
		if(callback(i, palette_size - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void transform_color_rgb8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, COLORPROC colorproc, void_ptr_t param, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// 輝度値を変換
			COLOR color = {pixel_rgb8_ptr_in->el.r, pixel_rgb8_ptr_in->el.g, pixel_rgb8_ptr_in->el.b};
			colorproc(&color, param);

			// 新しいイメージへ格納
			pixel_rgb8_ptr_out->el.r = color.red;
			pixel_rgb8_ptr_out->el.g = color.green;
			pixel_rgb8_ptr_out->el.b = color.blue;
			pixel_rgb8_ptr_out->el.a = pixel_rgb8_ptr_in->el.a;

			pixel_rgb8_ptr_in++;
			pixel_rgb8_ptr_out++;
		}

		// コールバック
		if(callback(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


void transform_geometric_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC geometricproc, void_ptr_t param, const INTERPOLATE_METHOD interpolate_method, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;     // イメージの幅
	const int height = info_ptr->height;    // イメージの高さ

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	uint32_t pixel_count = 0;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in, &pixel_count), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	try
	{
		std::valarray<uint8_t> g_plane(pixel_count), a_plane(pixel_count);
		for(uint32_t i = 0; i < pixel_count; i++)
		{
			g_plane[i] = pixel_gray8_ptr_in[i].el.gray;
			a_plane[i] = pixel_gray8_ptr_in[i].el.alpha;
		}

		uint32_t offset = 0;
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				// 座標変換
				POINT_2D point = {x, y};
				geometricproc(&point, param);
				point.x = saturate<double>(point.x, 0, width  - 1);
				point.y = saturate<double>(point.y, 0, height - 1);

				// 補間
				pixel_gray8_ptr_out[offset].el.gray  = interpolate(&g_plane[0], point.x, point.y, width, height, interpolate_method);
				pixel_gray8_ptr_out[offset].el.alpha = interpolate(&a_plane[0], point.x, point.y, width, height, interpolate_method);
				offset++;
			}

			// コールバック
			if(callback(y, height - 1, callback_data))
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

void transform_geometric_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC geometricproc, void_ptr_t param, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;     // イメージの幅
	const int height = info_ptr->height;    // イメージの高さ
	const_uint_t   depth = info_ptr->depth;
	const_uint32_t hsize = CALC_HSIZE(width, depth);

	// log2(depth)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// メモリ領域をロック
	const_uint8_ptr_t    index_ptr_in    = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index_ptr_out    = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	// パレットをコピー
	memset(index_ptr_out, 0, index_size);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// 座標変換
			POINT_2D point = {x, y};
			geometricproc(&point, param);

			const int new_x = saturate<int>(static_cast<int>(point.x), 0, width  - 1);
			const int new_y = saturate<int>(static_cast<int>(point.y), 0, height - 1);

			// 入力側の座標のオフセット値、シフト量を求める
			const_uint32_t offset_in = CALC_PACK_OFFSET(new_x, log2_depth);
			const int      shifts_in = CALC_PACK_SHIFTS(new_x, log2_depth);

			// 出力側の座標のオフセット値、シフト量を求める
			const_uint32_t offset_out = CALC_PACK_OFFSET(x, log2_depth);
			const int      shifts_out = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index_ptr_in[offset_in + new_y * hsize], depth, shifts_in);
			PACK_STORE(index_ptr_out[offset_out], index, shifts_out);
		}
		index_ptr_out += hsize;

		// コールバック
		if(callback(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void transform_geometric_rgb8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC geometricproc, void_ptr_t param, const INTERPOLATE_METHOD interpolate_method, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;     // イメージの幅
	const int height = info_ptr->height;    // イメージの高さ

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	uint32_t pixel_count = 0;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in, &pixel_count), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	try
	{
		std::valarray<uint8_t> r_plane(pixel_count), g_plane(pixel_count), b_plane(pixel_count), a_plane(pixel_count);
		for(uint32_t i = 0; i < pixel_count; i++)
		{
			r_plane[i] = pixel_rgb8_ptr_in[i].el.r;
			g_plane[i] = pixel_rgb8_ptr_in[i].el.g;
			b_plane[i] = pixel_rgb8_ptr_in[i].el.b;
			a_plane[i] = pixel_rgb8_ptr_in[i].el.a;
		}

		uint32_t offset = 0;
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				// 座標変換
				POINT_2D point = {x, y};
				geometricproc(&point, param);
				point.x = saturate<double>(point.x, 0, width  - 1);
				point.y = saturate<double>(point.y, 0, height - 1);

				// 補間
				pixel_rgb8_ptr_out[offset].el.r = interpolate(&r_plane[0], point.x, point.y, width, height, interpolate_method);
				pixel_rgb8_ptr_out[offset].el.g = interpolate(&g_plane[0], point.x, point.y, width, height, interpolate_method);
				pixel_rgb8_ptr_out[offset].el.b = interpolate(&b_plane[0], point.x, point.y, width, height, interpolate_method);
				pixel_rgb8_ptr_out[offset].el.a = interpolate(&a_plane[0], point.x, point.y, width, height, interpolate_method);
				offset++;
			}

			// コールバック
			if(callback(y, height - 1, callback_data))
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


void transform_geometric_integer_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC_INT geometricproc, void_ptr_t param, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;     // イメージの幅
	const int height = info_ptr->height;    // イメージの高さ

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// 座標変換
			POINT_2D_INTEGER point = {x, y};
			geometricproc(&point, param);
			point.x = saturate<int>(point.x, 0, width  - 1);
			point.y = saturate<int>(point.y, 0, height - 1);

			// 逆変換
			const_uint32_t offset = CALC_OFFSET(point.x, point.y, width);
			*pixel_gray8_ptr_out++ = pixel_gray8_ptr_in[offset];
		}

		// コールバック
		if(callback(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void transform_geometric_integer_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC_INT geometricproc, void_ptr_t param, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;     // イメージの幅
	const int height = info_ptr->height;    // イメージの高さ
	const_uint_t   depth = info_ptr->depth;
	const_uint32_t hsize = CALC_HSIZE(width, depth);

	// log2(depth)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// メモリ領域をロック
	const_uint8_ptr_t    index_ptr_in    = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index_ptr_out    = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	// パレットをコピー
	memset(index_ptr_out, 0, index_size);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// 座標変換
			POINT_2D_INTEGER point = {x, y};
			geometricproc(&point, param);
			point.x = saturate<int>(point.x, 0, width  - 1);
			point.y = saturate<int>(point.y, 0, height - 1);

			// 入力側の座標のオフセット値、シフト量を求める
			const_uint32_t offset_in = CALC_PACK_OFFSET(point.x, log2_depth);
			const int      shifts_in = CALC_PACK_SHIFTS(point.x, log2_depth);

			// 出力側の座標のオフセット値、シフト量を求める
			const_uint32_t offset_out = CALC_PACK_OFFSET(x, log2_depth);
			const int      shifts_out = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index_ptr_in[offset_in + point.y * hsize], depth, shifts_in);
			PACK_STORE(index_ptr_out[offset_out], index, shifts_out);
		}
		index_ptr_out += hsize;

		// コールバック
		if(callback(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void transform_geometric_integer_rgb8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC_INT geometricproc, void_ptr_t param, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;     // イメージの幅
	const int height = info_ptr->height;    // イメージの高さ

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// 座標変換
			POINT_2D_INTEGER point = {x, y};
			geometricproc(&point, param);
			point.x = saturate<int>(point.x, 0, width  - 1);
			point.y = saturate<int>(point.y, 0, height - 1);

			// 逆変換
			const_uint32_t offset = CALC_OFFSET(point.x, point.y, width);
			*pixel_rgb8_ptr_out++ = pixel_rgb8_ptr_in[offset];
		}

		// コールバック
		if(callback(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


void custom_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, CUSTOMPROC customproc, const int pixelcount_x, const int pixelcount_y, void_ptr_t param, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	const_uint_t size = (2 * pixelcount_x + 1) * (2 * pixelcount_y + 1);
	std::valarray<uint8_t> pixel_array_gray(size), pixel_array_alpha(size);

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	int offset = 0;

	// 上端の処理
	offset += copy_elements(&pixel_gray8_ptr_out[offset], &pixel_gray8_ptr_in[offset], width * pixelcount_y);

	for(int y = pixelcount_y; y < height - pixelcount_y; y++)
	{
		// 左端の処理
		offset += copy_elements(&pixel_gray8_ptr_out[offset], &pixel_gray8_ptr_in[offset], pixelcount_x);

		for(int x = pixelcount_x; x < width - pixelcount_x; x++)
		{
			// 配列を作成
			int index = offset + CALC_OFFSET(-pixelcount_x, -pixelcount_y, width);
			assert(0 <= index && index < width * height);

			int index_array = 0;
			for(int j = 0; j < 2 * pixelcount_y + 1; j++)
			{
				for(int i = 0; i < 2 * pixelcount_x + 1; i++)
				{
					pixel_array_gray [index_array] = pixel_gray8_ptr_in[index + i].el.gray;
					pixel_array_alpha[index_array] = pixel_gray8_ptr_in[index + i].el.alpha;
					index_array++;
				}
				index += width;
			}

			// カスタム処理
			pixel_gray8_ptr_out[offset].el.gray  = customproc(&pixel_array_gray [0], pixelcount_x, pixelcount_y, param);
			pixel_gray8_ptr_out[offset].el.alpha = customproc(&pixel_array_alpha[0], pixelcount_x, pixelcount_y, param);
			offset++;
		}

		// 右端の処理
		offset += copy_elements(&pixel_gray8_ptr_out[offset], &pixel_gray8_ptr_in[offset], pixelcount_x);

		// コールバック
		if(callback(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}

	// 下端の処理
	offset += copy_elements(&pixel_gray8_ptr_out[offset], &pixel_gray8_ptr_in[offset], width * pixelcount_y);
}

void custom_rgb8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, CUSTOMPROC customproc, const int pixelcount_x, const int pixelcount_y, void_ptr_t param, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	const_uint_t size = (2 * pixelcount_x + 1) * (2 * pixelcount_y + 1);
	std::valarray<uint8_t> pixel_array_r(size), pixel_array_g(size), pixel_array_b(size), pixel_array_a(size);

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	int offset = 0;

	// 上端の処理
	offset += copy_elements(&pixel_rgb8_ptr_out[offset], &pixel_rgb8_ptr_in[offset], width * pixelcount_y);

	for(int y = pixelcount_y; y < height - pixelcount_y; y++)
	{
		// 左端の処理
		offset += copy_elements(&pixel_rgb8_ptr_out[offset], &pixel_rgb8_ptr_in[offset], pixelcount_x);

		for(int x = pixelcount_x; x < width - pixelcount_x; x++)
		{
			// 配列を作成
			int index = offset + CALC_OFFSET(-pixelcount_x, -pixelcount_y, width);
			assert(0 <= index && index < width * height);

			int index_array = 0;
			for(int j = 0; j < 2 * pixelcount_y + 1; j++)
			{
				for(int i = 0; i < 2 * pixelcount_x + 1; i++)
				{
					pixel_array_r[index_array] = pixel_rgb8_ptr_in[index + i].el.r;
					pixel_array_g[index_array] = pixel_rgb8_ptr_in[index + i].el.g;
					pixel_array_b[index_array] = pixel_rgb8_ptr_in[index + i].el.b;
					pixel_array_a[index_array] = pixel_rgb8_ptr_in[index + i].el.a;
					index_array++;
				}
				index += width;
			}

			// カスタム処理
			pixel_rgb8_ptr_out[offset].el.r = customproc(&pixel_array_r[0], pixelcount_x, pixelcount_y, param);
			pixel_rgb8_ptr_out[offset].el.g = customproc(&pixel_array_g[0], pixelcount_x, pixelcount_y, param);
			pixel_rgb8_ptr_out[offset].el.b = customproc(&pixel_array_b[0], pixelcount_x, pixelcount_y, param);
			pixel_rgb8_ptr_out[offset].el.a = customproc(&pixel_array_a[0], pixelcount_x, pixelcount_y, param);
			offset++;
		}

		// 右端の処理
		offset += copy_elements(&pixel_rgb8_ptr_out[offset], &pixel_rgb8_ptr_in[offset], pixelcount_x);

		// コールバック
		if(callback(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}

	// 下端の処理
	offset += copy_elements(&pixel_rgb8_ptr_out[offset], &pixel_rgb8_ptr_in[offset], width * pixelcount_y);
}

END_NAMESPACE()


////////////////////////////////////////////////////////////////////////////////
// おまけ
#if 0

// カスタムフィルタ
IMAGERESULT __pop_filter_custom(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const int pixelcount_x, const int pixelcount_y, const int coef[], const int div, callback_t callback, long callback_data)
{
	// コールバック関数の補正
	if(callback == NULL)
	{
		callback = callback_none;
	}

	try
	{
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			{
				const int width  = info_ptr->width;
				const int height = info_ptr->height;
				const int size   = in_data_ptr->image_size;

				// メモリ領域をロック
				const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
				pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
				auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

				// int型のvalarrayを宣言
				std::valarray<int> src_array(size);
				{
					for(int i = 0; i < src_array.size(); i++)
					{
						src_array[i] = ((uint8_ptr_t)pixel_gray8_ptr_in)[i];
					}
				}
				int index = 0;

				// カスタム処理
				std::valarray<int> dst_array(0, size);
				for(int j = -pixelcount_y; j <= pixelcount_y; j++)
				{
					for(int i = -pixelcount_x; i <= pixelcount_x; i++)
					{
						const int offset = (i + j * width) * 2;
						dst_array += coef[index++] * src_array.shift(offset);
					}

					// コールバック
					if(callback(index, (2 * pixelcount_y + 1) * (2 * pixelcount_x + 1), callback_data))
					{
						throw IR_CANCELED;
					}
				}
				dst_array /= div;

				dst_array[dst_array <   0] = 0;
				dst_array[dst_array > 255] = 255;

				{
					// dstに変換
					for(int i = 0; i < dst_array.size(); i++)
					{
						((uint8_ptr_t)pixel_gray8_ptr_out)[i] = dst_array[i];
					}
				}
			}
			break;
/*
		case COLORMODE_RGB:
			custom_rgb8(info_ptr, in_data_ptr, out_data_ptr, customproc, pixelcount_x, pixelcount_y, param, callback, callback_data);
			break;
*/
		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		return result;
	}
}

#endif
