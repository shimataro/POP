// quant_dither.cpp
#include "stdafx.h"
#include "quant_dither.h"

#undef max
#undef min
#include <map>                          // std::map
#include <valarray>


typedef struct _pixel_rgb_t
{
	int b;                                  /* 青 */
	int g;                                  /* 緑 */
	int r;                                  /* 赤 */
	int a;                                  /* 不透明度 */
} pixel_rgb_t;

////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	void _quant_dither_none  (const IMAGE_INFO *info_ptr, const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in, pixel_rgb8_ptr_t pixel_rgb8_ptr_out, const_palette8_ptr_t palette_ptr, const int palette_size, callback_t callback_func, long callback_data);
	void _quant_dither_spread(const IMAGE_INFO *info_ptr, const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in, pixel_rgb8_ptr_t pixel_rgb8_ptr_out, const_palette8_ptr_t palette_ptr, const int palette_size, callback_t callback_func, long callback_data);
	int _get_nearest_index(const int r, const int g, const int b, const palette8_t palette[], const size_t size);
END_NAMESPACE()


// ディザ
void quant_dither(const IMAGE_INFO *info_ptr, const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in, pixel_rgb8_ptr_t pixel_rgb8_ptr_out, const_palette8_ptr_t palette_ptr, const int palette_size, const QUANT_DITHER_METHOD method, callback_t callback_func, long callback_data)
{
	switch(method)
	{
	case QDM_NONE:                          // ディザなし
		_quant_dither_none  (info_ptr, pixel_rgb8_ptr_in, pixel_rgb8_ptr_out, palette_ptr, palette_size, callback_func, callback_data);
		return;

	case QDM_BAYER:                         // Bayer法
		_quant_dither_spread(info_ptr, pixel_rgb8_ptr_in, pixel_rgb8_ptr_out, palette_ptr, palette_size, callback_func, callback_data);
		return;

	case QDM_RANDOM:                        // ランダムディザ
		_quant_dither_spread(info_ptr, pixel_rgb8_ptr_in, pixel_rgb8_ptr_out, palette_ptr, palette_size, callback_func, callback_data);
		return;

	case QDM_SPREAD:                        // 誤差拡散法
		_quant_dither_spread(info_ptr, pixel_rgb8_ptr_in, pixel_rgb8_ptr_out, palette_ptr, palette_size, callback_func, callback_data);
		return;

	default:
		__assume(0);
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// ディザリングなし
void _quant_dither_none(const IMAGE_INFO *info_ptr, const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in, pixel_rgb8_ptr_t pixel_rgb8_ptr_out, const_palette8_ptr_t palette_ptr, const int palette_size, callback_t callback_func, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// ハッシュバケツ
	typedef std::map<uint32_t, int> pixel_map_t;
	pixel_map_t hash_map[MAX_COLOR_HASH + 1];

	// パレットの中から近い色を探す
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			pixel_rgb8_t pixel = *pixel_rgb8_ptr_in;
			pixel.el.a = 0;

			int index = -1;

			// ハッシュ＋マップで対応するパレットインデックスを検索
			const int hash_index = HASH_COLOR_RGB(pixel.el.r, pixel.el.g, pixel.el.b);
			pixel_map_t::iterator p = hash_map[hash_index].find(pixel.value);
			if(p != hash_map[hash_index].end())
			{
				index = p->second;
			}
			else
			{
				index = _get_nearest_index(
					pixel.el.r, pixel.el.g, pixel.el.b,
					palette_ptr, palette_size);

				hash_map[hash_index][pixel.value] = index;
			}

			// ピクセルの色を決定
			pixel_rgb8_ptr_out->el.r = palette_ptr[index].r;
			pixel_rgb8_ptr_out->el.g = palette_ptr[index].g;
			pixel_rgb8_ptr_out->el.b = palette_ptr[index].b;
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

// 誤差拡散法
void _quant_dither_spread(const IMAGE_INFO *info_ptr, const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in, pixel_rgb8_ptr_t pixel_rgb8_ptr_out, const_palette8_ptr_t palette_ptr, const int palette_size, callback_t callback_func, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	int offset = 0;
	std::valarray<pixel_rgb_t> buffer(width * height);
	pixel_rgb_t *pixel_ptr = &buffer[0];

	std::copy(
		reinterpret_cast<const_uint8_ptr_t>(pixel_rgb8_ptr_in),
		reinterpret_cast<const_uint8_ptr_t>(&pixel_rgb8_ptr_in[width * height]),
		reinterpret_cast<int *>(pixel_ptr));

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// パレットの中から近い色を探す
			const int index = _get_nearest_index(
				pixel_ptr[0].r, pixel_ptr[0].g, pixel_ptr[0].b,
				palette_ptr, palette_size);

			// ピクセルの色を決定
			pixel_rgb8_ptr_out->el.r = palette_ptr[index].r;
			pixel_rgb8_ptr_out->el.g = palette_ptr[index].g;
			pixel_rgb8_ptr_out->el.b = palette_ptr[index].b;
			pixel_rgb8_ptr_out->el.a = pixel_rgb8_ptr_in->el.a;

			// 誤差を求める
			const int error_r = pixel_rgb8_ptr_in->el.r - pixel_rgb8_ptr_out->el.r;
			const int error_g = pixel_rgb8_ptr_in->el.g - pixel_rgb8_ptr_out->el.g;
			const int error_b = pixel_rgb8_ptr_in->el.b - pixel_rgb8_ptr_out->el.b;

			// 右に3/8割り振る
			if(x < width - 1)
			{
				pixel_ptr[1].r += error_b * 3 / 8;
				pixel_ptr[1].g += error_g * 3 / 8;
				pixel_ptr[1].b += error_r * 3 / 8;
			}

			// 下に3/8割り振る
			if(y < height - 1)
			{
				pixel_ptr[width].r += error_b * 3 / 8;
				pixel_ptr[width].g += error_g * 3 / 8;
				pixel_ptr[width].b += error_r * 3 / 8;
			}

			// 右下に3/8割り振る
			if(x < width - 1 && y < height - 1)
			{
				pixel_ptr[width + 1].r += error_b * 2 / 8;
				pixel_ptr[width + 1].g += error_g * 2 / 8;
				pixel_ptr[width + 1].b += error_r * 2 / 8;
			}

			pixel_ptr++;
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

// 色空間内で距離が最小のインデックスを取得
int _get_nearest_index(const int r, const int g, const int b, const palette8_t palette[], const size_t size)
{
	int min_index = -1;                     // 距離が最小のインデックス
	int min_distance = INT_MAX;             // 最小距離

	// min_distanceおよびmin_indexを求める
	for(size_t i = 0; i < size; i++)
	{
		const int pr = palette[i].r;
		const int pg = palette[i].g;
		const int pb = palette[i].b;

		// ピクセルとカラーテーブル要素の距離計算
		const int distance =
			(r - pr) * (r - pr) +
			(g - pg) * (g - pg) +
			(b - pb) * (b - pb);

		// 最小距離を更新
		if(distance < min_distance)
		{
			min_distance = distance;
			min_index = i;
		}
	}
	return min_index;
}

END_NAMESPACE()
