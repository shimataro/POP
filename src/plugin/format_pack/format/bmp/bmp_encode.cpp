// bmp_encode.cpp
#include "../../stdafx.h"
#include "../iotemplate.h"
#include "bmp_encode.h"

#include <stdio.h>
#include <io.h>                         // filelength()

#undef max
#undef min
#include <valarray>                     // std::valarray<>


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	long save_index(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
	long save_rgb8 (HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
END_NAMESPACE()


// ヘッダ部を書き込む
void bmp_encode_header(HANDLE hFile, const IMAGE_INFO *info_ptr)
{
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;

	// BITMAPFILEHEADERの設定
	fileheader.bfType      = BMP_TYPE;      // ファイルタイプ（must be "BM"）
	fileheader.bfSize      = 0;             // ファイルのサイズ（とりあえず０）
	fileheader.bfReserved1 = 0;             // 予約（must be 0）
	fileheader.bfReserved2 = 0;             // 予約（must be 0）
	fileheader.bfOffBits   = 0;             // イメージデータまでのオフセット（とりあえず０）

	// BITMAPINFOHEADERの設定
	infoheader.biSize          = sizeof(BITMAPINFOHEADER);
	infoheader.biWidth         = info_ptr->width;   // 幅
	infoheader.biHeight        = info_ptr->height;  // 高さ
	infoheader.biPlanes        = 1;             // プレーン（must be 1）
	// ビット数
	switch(info_ptr->mode)
	{
	case COLORMODE_INDEX:
		infoheader.biBitCount = info_ptr->depth;
		break;

	case COLORMODE_RGB:
		infoheader.biBitCount = 24;
		break;

	default:
		throw IR_UNKNOWN;
	}
	infoheader.biCompression   = BI_RGB;
	infoheader.biSizeImage     = 0;
	infoheader.biXPelsPerMeter = info_ptr->density_x;
	infoheader.biYPelsPerMeter = info_ptr->density_y;
	infoheader.biClrUsed       = 0;
	infoheader.biClrImportant  = 0;

	// ヘッダを書き込む
	WriteData(hFile, &fileheader, 1, IR_WRITEERROR);
	WriteData(hFile, &infoheader, 1, IR_WRITEERROR);
}

// イメージ部を書き込む
void bmp_encode_image(HANDLE hFile, const LONG lDistanceToMove, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	try
	{
		long image_offset = 0;
		switch(info_ptr->mode)
		{
		case COLORMODE_INDEX:
			image_offset = save_index(hFile, info_ptr, data_ptr, callback, callback_data);
			break;

		case COLORMODE_RGB:
			image_offset = save_rgb8(hFile, info_ptr, data_ptr, callback, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}

		// イメージファイルのサイズを取得
		::FlushFileBuffers(hFile);
		const uint32_t file_size = ::GetFileSize(hFile, NULL);

		// BITMAPFILEHEADER構造体を再び書き込む
		if(TRUE)
		{
			BITMAPFILEHEADER fileheader;
			fileheader.bfType      = BMP_TYPE;      // ファイルタイプ（must be "BM"）
			fileheader.bfSize      = file_size;     // ファイルのサイズ
			fileheader.bfReserved1 = 0;             // 予約（must be 0）
			fileheader.bfReserved2 = 0;             // 予約（must be 0）
			fileheader.bfOffBits   = image_offset;  // イメージデータまでのオフセット

			::SetFilePointer(hFile, lDistanceToMove, NULL, FILE_BEGIN);
			WriteData(hFile, &fileheader, 1, IR_WRITEERROR);
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

long save_index(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;
	const int depth  = info_ptr->depth;

	const_uint32_t hsize     = CALC_HSIZE(width, depth);
	const_uint32_t hsize_BMP = CALC_BOUND(hsize, BOUND_BMP);

	const_uint8_ptr_t    index_ptr    = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	uint32_t palette_size = 0;
	auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr, NULL, &palette_size);

	// パレットを書き込む
	if(TRUE)
	{
		std::valarray<RGBQUAD> palette(palette_size);

		// バッファに移動
		for(uint32_t i = 0; i < palette_size; i++)
		{
			palette[i].rgbRed   = palette8_ptr[i].r;
			palette[i].rgbGreen = palette8_ptr[i].g;
			palette[i].rgbBlue  = palette8_ptr[i].b;
		}
		WriteData(hFile, &palette[0], palette.size(), IR_WRITEERROR);
	}

	// イメージまでのオフセットを取得
	::FlushFileBuffers(hFile);
	const long image_offset = ::SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

	// イメージを書き込む
	if(TRUE)
	{
		std::valarray<uint8_t> buffer(hsize_BMP);
		uint32_t offset = hsize * (height - 1);
		for(int y = 0; y < height; y++)
		{
			memcpy(&buffer[0], &index_ptr[offset], hsize);
			WriteData(hFile, &buffer[0], buffer.size(), IR_WRITEERROR);
			offset -= hsize;

			// コールバック
			if(callback(y, height, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}

	return image_offset;
}

long save_rgb8(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

	// イメージまでのオフセットを取得
	::FlushFileBuffers(hFile);
	const long image_offset = ::SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

	if(TRUE)
	{
		const_uint32_t hsize_BMP = CALC_BOUND(CALC_HSIZE(width, 24), BOUND_BMP);
		std::valarray<uint8_t> buffer(hsize_BMP);

		uint32_t offset = width * (height - 1);
		for(int y = 0; y < height; y++)
		{
			uint8_ptr_t ptr = &buffer[0];
			for(int x = 0; x < width; x++)
			{
				*ptr++ = pixel_rgb8_ptr[offset + x].el.b;
				*ptr++ = pixel_rgb8_ptr[offset + x].el.g;
				*ptr++ = pixel_rgb8_ptr[offset + x].el.r;
			}
			WriteData(hFile, &buffer[0], buffer.size(), IR_WRITEERROR);
			offset -= width;

			// コールバック
			if(callback(y, height, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}

	return image_offset;
}

END_NAMESPACE()
