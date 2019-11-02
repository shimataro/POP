// ras_encode.cpp
#include "../../stdafx.h"
#include "../iotemplate.h"
#include "ras_encode.h"

#undef max
#undef min
#include <valarray>                     // std::valarray<>


#define SET_BUFFER(ptr, data)   set_multi_bytes_bigendian(ptr, data, sizeof(data)), ptr += sizeof(data)


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	void write_RASTERFILEHEADER(HANDLE hFile, const RASTERFILEHEADER *header);
END_NAMESPACE()


void ras_encode_header(HANDLE hFile, const IMAGE_INFO *info_ptr, RASTERFILEHEADER *header)
{
	// RASTERFILEHEADERの設定
	header->rfMagic   = RAS_MAGIC;
	header->rfWidth   = info_ptr->width;
	header->rfHeight  = info_ptr->height;
	header->rfSize    = 0;
	header->rfType    = RT_STANDARD;
	switch(info_ptr->mode)
	{
	case COLORMODE_GRAYSCALE:
		header->rfBits    = info_ptr->depth;
		header->rfMapsize = 0;
		header->rfMaptype = RMT_EQUAL_RGB;
		break;

	case COLORMODE_INDEX:
		header->rfBits    = info_ptr->depth;
		header->rfMapsize = (1 << info_ptr->depth) * 3;
		header->rfMaptype = RMT_EQUAL_RGB;
		break;

	case COLORMODE_RGB:
		header->rfBits    = info_ptr->depth * 3;
		header->rfMapsize = 0;
		header->rfMaptype = RMT_NONE;
		break;
	}

	// ヘッダを書き込む
	write_RASTERFILEHEADER(hFile, header);
}

void ras_encode_image_gray8(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, RASTERFILEHEADER *header, callback_t callback, long callback_data)
{
	try
	{
		const uint32_t width  = info_ptr->width;    // イメージの幅
		const uint32_t height = info_ptr->height;   // イメージの高さ
		const uint_t   depth  = info_ptr->depth;    // イメージのビット数

		// メモリ領域をロック
		pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
		auto_lock lock_obj(data_ptr, pixel_gray8_ptr);

		////////////////////////////////////////
		// イメージを書き込む

		// 1ラインあたりのサイズ
		const uint32_t hsize     = CALC_BOUND(CALC_HSIZE(width, depth), 1);
		const uint32_t hsize_RAS = CALC_BOUND(CALC_HSIZE(width, depth), BOUND_RAS);

		// ファイルに書き出す
		std::valarray<uint8_t> buffer(hsize_RAS);
		for(uint32_t y = 0; y < height; y++)
		{
			for(uint32_t x = 0; x < width; x++)
			{
				buffer[x] = pixel_gray8_ptr->el.gray;
				pixel_gray8_ptr++;
			}
			WriteData(hFile, &buffer[0], buffer.size(), IR_WRITEERROR);

			if(callback(y, height, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}

	// 例外処理
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

void ras_encode_image_index(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, RASTERFILEHEADER *header, callback_t callback, long callback_data)
{
	try
	{
		const uint32_t width  = info_ptr->width;              // イメージの幅
		const uint32_t height = info_ptr->height;             // イメージの高さ
		const uint_t   depth  = info_ptr->depth;   // イメージのビット数

		// メモリ領域をロック
		uint8_ptr_t    index_ptr    = NULL;
		palette8_ptr_t palette8_ptr = NULL;
		auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr);

		// カラーテーブルを書き込む
		if(TRUE)
		{
			const uint32_t table_size = 1 << depth;
			std::valarray<uint8_t> palette(table_size * 3);

			// 標準カラーマップへセット
			for(uint32_t i = 0; i < table_size; i++)
			{
				palette[i + 0 * table_size] = palette8_ptr[i].r;
				palette[i + 1 * table_size] = palette8_ptr[i].g;
				palette[i + 2 * table_size] = palette8_ptr[i].b;
			}
			WriteData(hFile, &palette[0], palette.size(), IR_WRITEERROR);
		}

		// イメージを書き込む
		if(TRUE)
		{
			// 1ラインあたりのサイズ
			const uint32_t hsize     = CALC_HSIZE(width, depth);
			const uint32_t hsize_RAS = CALC_BOUND(hsize, BOUND_RAS);

			// ファイルに書き出す
			std::valarray<uint8_t> buffer(hsize_RAS);
			uint32_t offset = 0;
			for(uint32_t y = 0; y < height; y++)
			{
				memcpy(&buffer[0], &index_ptr[offset], hsize);
				WriteData(hFile, &buffer[0], buffer.size(), IR_WRITEERROR);
				offset += hsize;
			}
		}
	}

	// 例外処理
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

void ras_encode_image_rgb8(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, RASTERFILEHEADER *header, callback_t callback, long callback_data)
{
	try
	{
		const uint32_t width  = info_ptr->width;    // イメージの幅
		const uint32_t height = info_ptr->height;   // イメージの高さ
		const uint_t   depth  = info_ptr->depth;    // イメージのビット数

		// メモリ領域をロック
		pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
		auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

		// イメージを書き込む
		if(TRUE)
		{
			// 1ラインあたりのサイズ
			const uint32_t hsize     = CALC_HSIZE(width, 24);
			const uint32_t hsize_RAS = CALC_BOUND(hsize, BOUND_RAS);

			// ファイルに書き出す
			std::valarray<uint8_t> buffer(hsize_RAS);
			for(uint32_t y = 0; y < height; y++)
			{
				uint8_ptr_t ptr = &buffer[0];
				for(uint32_t x = 0; x < width; x++)
				{
					*ptr++ = pixel_rgb8_ptr->el.r;
					*ptr++ = pixel_rgb8_ptr->el.g;
					*ptr++ = pixel_rgb8_ptr->el.b;
					pixel_rgb8_ptr++;
				}
				WriteData(hFile, &buffer[0], buffer.size(), IR_WRITEERROR);

				if(callback(y, height, callback_data))
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

void write_RASTERFILEHEADER(HANDLE hFile, const RASTERFILEHEADER *header)
{
	uint8_t buffer[sizeofRASTERFILEHEADER];
	uint8_ptr_t p = buffer;

	// バッファに格納
	SET_BUFFER(p, header->rfMagic);
	SET_BUFFER(p, header->rfWidth);
	SET_BUFFER(p, header->rfHeight);
	SET_BUFFER(p, header->rfBits);
	SET_BUFFER(p, header->rfSize);
	SET_BUFFER(p, header->rfType);
	SET_BUFFER(p, header->rfMaptype);
	SET_BUFFER(p, header->rfMapsize);

	// ヘッダを書き込む
	WriteData(hFile, buffer, SIZE_OF_ARRAY(buffer), IR_WRITEERROR);
}

END_NAMESPACE()
