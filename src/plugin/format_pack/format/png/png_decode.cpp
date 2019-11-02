// png_decode.cpp
#include "../../stdafx.h"
#include "../iotemplate.h"              // ReadData()
#include "png_decode.h"

#undef min
#undef max
#include <valarray>                     // std::valarray<>


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	void get_gamma(png_structp png_ptr, png_infop png_info_ptr, double *gamma_ptr);
	void get_image_gray8(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
	void get_image_index(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
	void get_image_rgb8 (png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
END_NAMESPACE()


// PNGポインタを初期化
BOOL png_decode_init(png_structpp png_ptr_ptr, png_infopp png_info_ptr_ptr)
{
	*png_ptr_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(*png_ptr_ptr == NULL)
	{
		return FALSE;
	}
	*png_info_ptr_ptr = png_create_info_struct(*png_ptr_ptr);
	if(*png_info_ptr_ptr == NULL)
	{
		png_destroy_read_struct(png_ptr_ptr, png_info_ptr_ptr, (png_infopp)NULL);
		return FALSE;
	}
	return TRUE;
}

// ヘッダを読み込む
void png_decode_header(png_structp png_ptr, png_infop png_info_ptr, IMAGE_INFO *info_ptr, PNGEXTRA *png_extra_ptr)
{
	// 画像情報を取得
	png_uint_32 width, height;
	png_get_IHDR(
		png_ptr, png_info_ptr,
		&width, &height,
		&png_extra_ptr->bit_depth, &png_extra_ptr->color_type,
		&png_extra_ptr->interlace_type,
		NULL, NULL);

	// ガンマを取得
	get_gamma(png_ptr, png_info_ptr, &info_ptr->gamma_file);

	// アルファチャネルを含んでいるか
	info_ptr->alpha = (png_extra_ptr->color_type & PNG_COLOR_MASK_ALPHA) ? TRUE : FALSE;

	// 画像情報の設定
	info_ptr->width     = static_cast<uint16_t>(width);
	info_ptr->height    = static_cast<uint16_t>(height);
	info_ptr->depth     = png_ptr->bit_depth;
	info_ptr->density_x = 0;
	info_ptr->density_y = 0;
	info_ptr->comment   = NULL;

	switch(png_ptr->color_type)
	{
	case PNG_COLOR_TYPE_RGB:                // フルカラー
		info_ptr->mode  = COLORMODE_RGB;
		info_ptr->alpha = FALSE;
		break;

	case PNG_COLOR_TYPE_RGB_ALPHA:          // フルカラー＋α
		info_ptr->mode  = COLORMODE_RGB;
		info_ptr->alpha = TRUE;
		break;

	case PNG_COLOR_TYPE_PALETTE:            // インデックスカラー
		info_ptr->mode  = COLORMODE_INDEX;
		info_ptr->alpha = FALSE;
		break;

	case PNG_COLOR_TYPE_GRAY:               // グレイスケール
		info_ptr->mode  = COLORMODE_GRAYSCALE;
		info_ptr->alpha = FALSE;
		break;

	case PNG_COLOR_TYPE_GRAY_ALPHA:         // グレイスケール＋α
		info_ptr->mode  = COLORMODE_GRAYSCALE;
		info_ptr->alpha = TRUE;
		break;

	default:
		throw IR_INCORRECTHEADER;
	}
}

// 画像データを取得
void png_decode_image(png_structp png_ptr, png_infop png_info_ptr, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, const PNGEXTRA *png_extra_ptr, callback_t callback, long callback_data)
{
	try
	{
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			switch(info_ptr->depth)
			{
			case 16:
				throw IR_INCORRECTHEADER;
				break;

			default:
				get_image_gray8(png_ptr, png_info_ptr, info_ptr, data_ptr, callback, callback_data);
				break;
			}
			break;

		case COLORMODE_INDEX:
			get_image_index(png_ptr, png_info_ptr, info_ptr, data_ptr, callback, callback_data);
			break;

		case COLORMODE_RGB:
			switch(info_ptr->depth)
			{
			case 16:
				throw IR_INCORRECTHEADER;
				break;

			case 8:
				get_image_rgb8(png_ptr, png_info_ptr, info_ptr, data_ptr, callback, callback_data);
				break;

			default:
				throw IR_INCORRECTHEADER;
			}
			break;
		}
	}

	// 例外処理
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// ガンマを取得
void get_gamma(png_structp png_ptr, png_infop png_info_ptr, double *gamma_ptr)
{
	if(!png_get_gAMA(png_ptr, png_info_ptr, gamma_ptr))
	{
		// ガンマが読み込めなかったらデフォルトに設定
		*gamma_ptr = GAMMA_DEFAULT_FILE;
	}
}


////////////////////////////////////////////////////////////////////////////////
// イメージを展開
void get_image_gray8(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	const_uint32_t width  = info_ptr->width;
	const_uint32_t height = info_ptr->height;
	const_uint_t   depth  = info_ptr->depth;
	const_uint32_t hsize  = CALC_HSIZE(width, 16);

	// log2(depth)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	std::valarray<uint8_t> buffer(hsize);

	// メモリ領域をロック
	pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_gray8_ptr);

	// スケールアップテーブルを作成
	std::valarray<uint8_t> scaleup_table(1 << depth);
	create_scaleup_table(&scaleup_table[0], depth);

	// 画像の読み出し
	const int loops = png_set_interlace_handling(png_ptr);
	const int denom = loops * height;
	int num = 0;
	for(int i = 0; i < loops; i++)
	{
		for(uint32_t y = 0; y < height; y++)
		{
			int j = 0;
			png_read_row(png_ptr, &buffer[0], NULL);
			for(uint32_t x = 0; x < width; x++)
			{
				// 出力オフセットとシフト量を求める
				const_uint32_t offset_index = CALC_PACK_OFFSET(j, log2_depth);
				const int      shifts_index = CALC_PACK_SHIFTS(j, log2_depth);
				j++;

				const_uint8_t gray = PACK_EXTRACT(buffer[offset_index], depth, shifts_index);

				pixel_gray8_ptr->el.gray = scaleup_table[gray];
				if(info_ptr->alpha)
				{
					// 出力オフセットとシフト量を求める
					const_uint32_t offset_index = CALC_PACK_OFFSET(j, log2_depth);
					const int      shifts_index = CALC_PACK_SHIFTS(j, log2_depth);
					j++;

					const_uint8_t alpha = PACK_EXTRACT(buffer[offset_index], depth, shifts_index);

					pixel_gray8_ptr->el.alpha = scaleup_table[alpha];
				}
				pixel_gray8_ptr++;
			}

			// コールバック
			if(callback(num++, denom, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}
}

void get_image_index(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	const_uint32_t width  = info_ptr->width;
	const_uint32_t height = info_ptr->height;
	const_uint_t   depth  = info_ptr->depth;
	const_uint32_t hsize  = CALC_HSIZE(width, depth);

	// メモリ領域をロック
	uint8_ptr_t    index_ptr    = NULL;
	palette8_ptr_t palette8_ptr = NULL;
	auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr);

	// PNGファイルからパレットを取得
	if(TRUE)
	{
		int palette_size;
		png_colorp png_palette;
		png_get_PLTE(png_ptr, png_info_ptr, &png_palette, &palette_size);

		// パレットを格納
		for(int i = 0; i < palette_size; i++)
		{
			palette8_ptr[i].r = png_palette[i].red;
			palette8_ptr[i].g = png_palette[i].green;
			palette8_ptr[i].b = png_palette[i].blue;
			palette8_ptr[i].a = 255;
		}
	}

	// 画像の読み出し
	if(TRUE)
	{
		const int loops = png_set_interlace_handling(png_ptr);
		const int denom = loops * height;
		int num = 0;
		for(int i = 0; i < loops; i++)
		{
			uint8_ptr_t ptr = index_ptr;
			for(uint32_t y = 0; y < height; y++)
			{
				png_read_row(png_ptr, ptr, NULL);
				ptr += hsize;

				// コールバック
				if(callback(num++, denom, callback_data))
				{
					throw IR_CANCELED;
				}
			}
		}
	}
}

void get_image_rgb8(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	const uint32_t width  = info_ptr->width;
	const uint32_t height = info_ptr->height;
	const uint32_t hsize  = CALC_HSIZE(width, 32);

	// メモリ領域をロック
	pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

	// 画像の読み出し
	if(TRUE)
	{
		const int loops = png_set_interlace_handling(png_ptr);
		const int denom = loops * height;

		std::valarray<uint8_t> buffer(hsize);
		int num = 0;

		for(int i = 0; i < loops; i++)
		{
			uint32_t offset = 0;
			for(uint32_t y = 0; y < height; y++)
			{
				uint8_ptr_t ptr = &buffer[0];
				png_read_row(png_ptr, ptr, NULL);
				for(uint32_t x = 0; x < width; x++)
				{
					pixel_rgb8_ptr[offset].el.r = *ptr++;
					pixel_rgb8_ptr[offset].el.g = *ptr++;
					pixel_rgb8_ptr[offset].el.b = *ptr++;
					pixel_rgb8_ptr[offset].el.a = info_ptr->alpha ? *ptr++ : 0xff;
					offset++;
				}

				// コールバック
				if(callback(num++, denom, callback_data))
				{
					throw IR_CANCELED;
				}
			}
		}
	}
}

END_NAMESPACE()
