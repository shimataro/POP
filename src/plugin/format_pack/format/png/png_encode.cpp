// png_encode.cpp
#include "../../stdafx.h"
#include "png_encode.h"

#include <time.h>                       // time()

#undef max
#undef min
#include <valarray>                     // std::valarray<>


// テキストデータのキーワード
#define KEYWORD_TITLE         "Title"           // キャプション
#define KEYWORD_AUTHOR        "Author"          // 作者の名前
#define KEYWORD_DESCRIPTION   "Description"     // 画像の説明
#define KEYWORD_COPYRIGHT     "Copyright"       // 版権情報
#define KEYWORD_CREATION_TIME "Creation Time"   // 作成された時間
#define KEYWORD_SOFTWARE      "Software"        // 画像作成に使用したソフトウェア
#define KEYWORD_DISCLAIMER    "Disclaimer"      // 権利の放棄
#define KEYWORD_WARNING       "Warning"         // 内容の性質の警告
#define KEYWORD_SOURCE        "Source"          // 画像作成に使用された機器
#define KEYWORD_COMMENT       "Comment"         // コメント


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	void set_filter_type      (png_structp png_ptr, UINT filter_type_index);
	void set_compression_level(png_structp png_ptr, uint_t compression_level);
	void set_image_info       (png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, const uint_t interlace_type);
	void set_text             (png_structp png_ptr, png_infop png_info_ptr, char *comment, bool compress);
	void set_time             (png_structp png_ptr, png_infop png_info_ptr);
	void set_gamma            (png_structp png_ptr, png_infop png_info_ptr, const double gamma);
END_NAMESPACE()


BOOL png_encode_init(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr)
{
	*png_ptr_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(*png_ptr_ptr == NULL)
	{
		return FALSE;
	}
	*info_ptr_ptr = png_create_info_struct(*png_ptr_ptr);
	if(*info_ptr_ptr == NULL)
	{
		png_destroy_write_struct(png_ptr_ptr, info_ptr_ptr);
		return FALSE;
	}
	return TRUE;
}

void png_encode_header(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, const PNGPARAM *param)
{
	// フィルタタイプの設定
	set_filter_type(png_ptr, param->filter_type);

	// 圧縮レベルの設定
	set_compression_level(png_ptr, param->compression_level);

	// 画像情報の設定
	set_image_info(png_ptr, png_info_ptr, info_ptr, param->interlace_type);

	// 更新時刻の設定
	if(param->check.elements.save_time)
	{
		set_time(png_ptr, png_info_ptr);
	}

	// ガンマの設定
	if(param->check.elements.save_gamma)
	{
		set_gamma(png_ptr, png_info_ptr, info_ptr->gamma_file);
	}

	// コメントの設定
	if(info_ptr->comment != NULL)
	{
		const bool compress_text = (param->check.elements.compress_text != FALSE);
		set_text(png_ptr, png_info_ptr, info_ptr->comment, compress_text);
	}
}

void png_encode_image_gray8(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	const_uint32_t width  = info_ptr->width;
	const_uint32_t height = info_ptr->height;
	const uint32_t hsize  = CALC_HSIZE(width, 16);
	const BOOL     alpha  = info_ptr->alpha;

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_gray8_ptr);

	try
	{

		// 画像データを書き込む
		// 前処理
		png_write_info(png_ptr, png_info_ptr);

		std::valarray<uint8_t> buffer(hsize);

		// 画像を保存
		const int loop_count = png_set_interlace_handling(png_ptr);
		const int denom      = loop_count * height;
		int num = 0;
		for(int i = 0; i < loop_count; i++)
		{
			uint32_t offset = 0;
			for(uint32_t y = 0; y < height; y++)
			{
				uint8_ptr_t ptr = &buffer[0];
				for(uint32_t x = 0; x < width; x++)
				{
					*ptr++ = pixel_gray8_ptr[offset].el.gray;
					if(alpha)
					{
						*ptr++ = pixel_gray8_ptr[offset].el.alpha;
					}
					offset++;
				}
				png_write_row(png_ptr, &buffer[0]);

				if(callback(num++, denom, callback_data))
				{
					throw IR_CANCELED;
				}
			}
		}
	}

	// 例外処理
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

void png_encode_image_index(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	const_uint32_t width  = info_ptr->width;
	const_uint32_t height = info_ptr->height;
	const_uint_t   depth  = info_ptr->depth;
	const uint32_t hsize  = CALC_HSIZE(width, depth);

	// メモリ領域をロック
	uint8_ptr_t          index_ptr    = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr);

	try
	{
		////////////////////////////////////////
		// パレットを書き込む
		const int palette_size = 1 << depth;
		std::valarray<png_color> png_palette(palette_size);
		for(int i = 0; i < palette_size; i++)
		{
			png_palette[i].red   = palette8_ptr[i].r;
			png_palette[i].green = palette8_ptr[i].g;
			png_palette[i].blue  = palette8_ptr[i].b;
		}
		png_set_PLTE(png_ptr, png_info_ptr, &png_palette[0], palette_size);


		////////////////////////////////////////
		// 画像データを書き込む
		// 前処理
		png_write_info(png_ptr, png_info_ptr);

		// 画像を保存
		const int loop_count = png_set_interlace_handling(png_ptr);
		const int denom      = loop_count * height;
		int num = 0;
		for(int j = 0; j < loop_count; j++)
		{
			uint32_t offset = 0;
			for(uint32_t y = 0; y < height; y++)
			{
				png_write_row(png_ptr, &index_ptr[offset]);
				offset += hsize;
				if(callback(num++, denom, callback_data))
				{
					throw IR_CANCELED;
				}
			}
		}
	}

	// 例外処理
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

void png_encode_image_rgb8(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	const_uint32_t width  = info_ptr->width;
	const_uint32_t height = info_ptr->height;
	const uint32_t hsize  = CALC_HSIZE(width, 32);
	const BOOL     alpha  = info_ptr->alpha;

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

	try
	{

		// 画像データを書き込む
		// 前処理
		png_write_info(png_ptr, png_info_ptr);

		std::valarray<uint8_t> buffer(hsize);

		// 画像を保存
		const int loop_count = png_set_interlace_handling(png_ptr);
		const int denom      = loop_count * height;
		int num = 0;
		for(int i = 0; i < loop_count; i++)
		{
			uint32_t offset = 0;
			for(uint32_t y = 0; y < height; y++)
			{
				uint8_ptr_t ptr = &buffer[0];
				for(uint32_t x = 0; x < width; x++)
				{
					*ptr++ = pixel_rgb8_ptr[offset].el.r;
					*ptr++ = pixel_rgb8_ptr[offset].el.g;
					*ptr++ = pixel_rgb8_ptr[offset].el.b;
					if(alpha)
					{
						*ptr++ = pixel_rgb8_ptr[offset].el.a;
					}
					offset++;
				}
				png_write_row(png_ptr, &buffer[0]);

				if(callback(num++, denom, callback_data))
				{
					throw IR_CANCELED;
				}
			}
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

// フィルタタイプの設定
void set_filter_type(png_structp png_ptr, UINT filter_type_index)
{
	const UINT filter_type[] =
	{
		PNG_NO_FILTERS,
		PNG_FILTER_NONE,
		PNG_FILTER_SUB,
		PNG_FILTER_UP,
		PNG_FILTER_AVG,
		PNG_FILTER_PAETH,
	};

	png_set_filter(
		png_ptr,
		PNG_FILTER_TYPE_BASE,
		filter_type[filter_type_index]);
}

// 圧縮レベルの設定
void set_compression_level(png_structp png_ptr, uint_t compression_level)
{
	png_set_compression_level(png_ptr, compression_level);
}

// 画像情報の設定
void set_image_info(png_structp png_ptr, png_infop png_info_ptr, const IMAGE_INFO *info_ptr, const uint_t interlace_type)
{
	const png_uint_32 width  = info_ptr->width;
	const png_uint_32 height = info_ptr->height;
	const BOOL alpha = info_ptr->alpha;

	int bit_depth, color_type;
	switch(info_ptr->mode)
	{
	case COLORMODE_RGB:
		bit_depth  = 8;
		color_type = alpha ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB;
		break;

	case COLORMODE_INDEX:
		bit_depth  = info_ptr->depth;
		color_type = PNG_COLOR_TYPE_PALETTE;
		break;

	case COLORMODE_GRAYSCALE:
		bit_depth  = 8;
		color_type = alpha ? PNG_COLOR_TYPE_GRAY_ALPHA : PNG_COLOR_TYPE_GRAY;
		break;

	default:
		__assume(0);
	}

	png_set_IHDR(
		png_ptr, png_info_ptr,
		width, height,
		bit_depth, color_type,
		PNG_INTERLACE_NONE + interlace_type,
		PNG_COMPRESSION_TYPE_BASE,
		PNG_FILTER_TYPE_BASE);
}

// コメントの設定
void set_text(png_structp png_ptr, png_infop png_info_ptr, char *comment, bool compress)
{
	png_text text_data;
	text_data.compression = compress ? PNG_TEXT_COMPRESSION_zTXt : PNG_TEXT_COMPRESSION_NONE;   // テキストデータを圧縮するか？
	text_data.key         = KEYWORD_COMMENT;                                                    // キーワード
	text_data.text        = comment;                                                            // テキストデータ
	text_data.text_length = strlen(comment);                                                    // テキストの長さ

	png_set_text(png_ptr, png_info_ptr, &text_data, 1);
}

// 更新時刻の設定
void set_time(png_structp png_ptr, png_infop png_info_ptr)
{
	time_t gmt;                             // システム時刻（秒単位）
	png_time mod_time;                      // システム時刻（png_set_tIME()関数用）

	// 万国標準時を取得
	time(&gmt);
	png_convert_from_time_t(&mod_time, gmt);

	png_set_tIME(png_ptr, png_info_ptr, &mod_time);
}

// ガンマの設定
void set_gamma(png_structp png_ptr, png_infop png_info_ptr, const double gamma)
{
	png_set_gAMA(png_ptr, png_info_ptr, gamma);
}

END_NAMESPACE()
