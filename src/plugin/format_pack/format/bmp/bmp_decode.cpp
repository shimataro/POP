// bmp_decode.cpp
#include "../../stdafx.h"
#include "../iotemplate.h"              // ReadData()
#include "bmp_decode.h"

#undef max
#undef min
#include <valarray>                     // std::valarray<>


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// デコード関数
	void bmp_decode_image_index(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data);
	void bmp_decode_image_rgb5 (HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data);
	void bmp_decode_image_rgb8 (HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data);

//	void UnRGB  (const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, const BOOL bottomup, const int bits_per_pixel, callback_t callback, long callback_data);
	void UnIndex(const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data);
	void UnRLE8 (const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data);
	void UnRLE4 (const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data);

	// 式が成り立たなければ例外をスロー
	inline void throw_if_not(BOOL expression)
	{if(!expression) {throw IR_INCORRECTHEADER;}}
END_NAMESPACE()


// ヘッダ部を読み込む
void bmp_decode_header(HANDLE hFile, IMAGE_INFO *info_ptr, BMPDECODEDATA &data)
{
	BITMAPFILEHEADER &fileheader = data.fileheader;
	BITMAPINFOHEADER &infoheader = data.infoheader;

	// ファイルヘッダ、情報ヘッダを読む
	ReadData(hFile, &fileheader, 1, IR_INCORRECTHEADER);
	ReadData(hFile, &infoheader, 1, IR_INCORRECTHEADER);

	// ヘッダのチェック
	throw_if_not(fileheader.bfType == BMP_TYPE);               // 先頭が"BM"でなければエラー
	throw_if_not(fileheader.bfOffBits <= fileheader.bfSize);  // イメージデータ位置よりサイズの方が大きかったらエラー
	throw_if_not(fileheader.bfReserved1 == 0);                 // 予約が0でなければエラー
	throw_if_not(fileheader.bfReserved2 == 0);                 // 〃
	throw_if_not(infoheader.biSize == sizeof(BITMAPINFOHEADER));  // 構造体のサイズが違っていたらエラー
	throw_if_not(infoheader.biWidth > 0);                      // ビットマップの幅が０以下ならエラー
	throw_if_not(infoheader.biHeight != 0);                    // ビットマップの高さが０ならエラー（負の場合はトップダウン型ビットマップ）
	throw_if_not(!(infoheader.biCompression == BI_RLE8 && infoheader.biBitCount != 8)); // 圧縮形式が8bit連長圧縮で、ビット数が8でなければエラー
	throw_if_not(infoheader.biPlanes == 1);                    // biPlanesが1でなければエラー

	// IMAGE_INFO構造体の設定
	info_ptr->width  = static_cast<uint16_t>(infoheader.biWidth);
	info_ptr->height = static_cast<uint16_t>(abs(infoheader.biHeight));
	if(infoheader.biBitCount > 8)
	{
		info_ptr->depth = 8;
		info_ptr->mode  = COLORMODE_RGB;
	}
	else
	{
		info_ptr->depth = infoheader.biBitCount;
		info_ptr->mode  = COLORMODE_INDEX;
	}
	info_ptr->alpha      = FALSE;
	info_ptr->colors     = infoheader.biClrUsed;
	info_ptr->density_x  = static_cast<uint16_t>(infoheader.biXPelsPerMeter);
	info_ptr->density_y  = static_cast<uint16_t>(infoheader.biYPelsPerMeter);
	info_ptr->gamma_file = GAMMA_DEFAULT_FILE;
	info_ptr->comment    = NULL;
}

// イメージ部を読み込む
void bmp_decode_image(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data)
{
	switch(data.infoheader.biBitCount)
	{
	case 24:
		bmp_decode_image_rgb8(hFile, info_ptr, data_ptr, data, callback, callback_data);
		break;

	case 16:
		bmp_decode_image_rgb5(hFile, info_ptr, data_ptr, data, callback, callback_data);
		break;

	case 8:
	case 4:
	case 1:
		bmp_decode_image_index(hFile, info_ptr, data_ptr, data, callback, callback_data);
		break;

	default:
		throw IR_INCORRECTHEADER;
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

void bmp_decode_image_rgb8(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data)
{
	try
	{
		const_uint32_t width  = info_ptr->width;
		const_uint32_t height = info_ptr->height;
		const_uint_t   depth  = info_ptr->depth;
		const_uint32_t hsize     = CALC_HSIZE(width, 24);
		const_uint32_t hsize_BMP = CALC_BOUND(hsize, BOUND_BMP);

		// メモリ領域をロック
		pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
		auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);


		////////////////////////////////////////
		// イメージ部を読み込む
		std::valarray<uint8_t> buffer(hsize_BMP);

		// イメージデータの先頭へ移動
		::SetFilePointer(hFile, data.fileheader.bfOffBits, NULL, FILE_BEGIN);

		uint32_t offset = width * (height - 1);
		for(uint32_t y = 0; y < height; y++)
		{
			// 画像を下から順に読み込む（トップダウン型なら上から順）
			ReadData(hFile, &buffer[0], buffer.size(), IR_IMAGEDESTROYED);

			uint8_ptr_t ptr = &buffer[0];
			for(uint32_t x = 0; x < width; x++)
			{
				pixel_rgb8_ptr[offset + x].el.b = *ptr++;
				pixel_rgb8_ptr[offset + x].el.g = *ptr++;
				pixel_rgb8_ptr[offset + x].el.r = *ptr++;
				pixel_rgb8_ptr[offset + x].el.a = 0xff;
			}
			offset -= width;

			if(callback(y, height - 1, callback_data))
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

void bmp_decode_image_rgb5(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data)
{
	try
	{
		const_uint32_t width  = info_ptr->width;
		const_uint32_t height = info_ptr->height;
		const_uint_t   depth  = info_ptr->depth;
		const_uint32_t hsize  = CALC_BOUND(width, BOUND_BMP);

		// メモリ領域をロック
		pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
		auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);


		////////////////////////////////////////
		// イメージ部を読み込む
		std::valarray<uint16_t> buffer(hsize);

		// イメージデータの先頭へ移動
		::SetFilePointer(hFile, data.fileheader.bfOffBits, NULL, FILE_BEGIN);

		uint32_t offset = width * (height - 1);
		for(uint32_t y = 0; y < height; y++)
		{
			// 画像を下から順に読み込む（トップダウン型なら上から順）
			ReadData(hFile, &buffer[0], buffer.size(), IR_IMAGEDESTROYED);
			for(uint32_t x = 0; x < width; x++)
			{
				const bitmap_rgb5_t bitmap_rgb5 = {buffer[x]};
				pixel_rgb8_ptr[offset + x].el.b = SCALEUP(bitmap_rgb5.el.b, 5, 8);
				pixel_rgb8_ptr[offset + x].el.g = SCALEUP(bitmap_rgb5.el.g, 5, 8);
				pixel_rgb8_ptr[offset + x].el.r = SCALEUP(bitmap_rgb5.el.r, 5, 8);
				pixel_rgb8_ptr[offset + x].el.a = 0xff;
			}
			offset -= width;

			if(callback(y, height - 1, callback_data))
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

void bmp_decode_image_index(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data)
{
	try
	{
		const uint32_t width  = info_ptr->width;
		const uint32_t height = info_ptr->height;
		const uint_t   depth  = info_ptr->depth;

		// メモリ領域をロック
		uint8_ptr_t    index_ptr    = NULL;
		palette8_ptr_t palette8_ptr = NULL;
		auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr);

		////////////////////////////////////////
		// カラーテーブルを読み込む
		if(TRUE)
		{
			const uint_t palette_size = 1 << depth;
			std::valarray<RGBQUAD> palette_buffer(palette_size);
			ReadData(hFile, &palette_buffer[0], palette_buffer.size(), IR_IMAGEDESTROYED);

			// paletteに移動
			for(uint_t i = 0; i < palette_size; i++)
			{
				palette8_ptr[i].r = palette_buffer[i].rgbRed;
				palette8_ptr[i].g = palette_buffer[i].rgbGreen;
				palette8_ptr[i].b = palette_buffer[i].rgbBlue;
				palette8_ptr[i].a = 0xff;
			}
		}

		////////////////////////////////////////
		// イメージ部を読み込む

		// イメージデータの先頭へ移動
		::SetFilePointer(hFile, data.fileheader.bfOffBits, NULL, FILE_BEGIN);

		UnIndex(info_ptr, data_ptr, hFile, callback, callback_data);
	}

	// 例外処理
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

// インデックス格納形式を解く
void UnIndex(const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data)
{
	const_uint32_t width  = info_ptr->width;
	const_uint32_t height = info_ptr->height;
	const_uint_t   depth  = info_ptr->depth;

	const_uint32_t hsize     = CALC_HSIZE(width, depth);
	const_uint32_t hsize_BMP = CALC_BOUND(hsize, BOUND_BMP);

	std::valarray<uint8_t> buffer(hsize_BMP);

	uint8_ptr_t          index_ptr    = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr);

	uint32_t offset = hsize * (height - 1);
	for(uint32_t y = 0; y < height; y++)
	{
		// 画像を下から順に読み込む（トップダウン型なら上から順）
		ReadData(hFile, &buffer[0], buffer.size(), IR_IMAGEDESTROYED);
		memcpy(&index_ptr[offset], &buffer[0], hsize);
		offset -= hsize;

		if(callback(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}
/*
// 8bit連長圧縮を解く
void UnRLE8(const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data)
{
	const uint32_t width  = info_ptr->width;
	const uint32_t height = info_ptr->height;

	uint32_t nX = 0;                        // x座標
	uint32_t nY = 0;                        // y座標
	uint32_t nOffset = (height - 1) * width;// オフセット

	while(true)
	{
		// ランの長さ
		const_uint8_t nRun = fgetc(fp);

		// ランの長さが１以上
		if(nRun != 0x00)
		{
			const_uint8_t nData = fgetc(fp);               // データ

			memset(&image[nOffset], nData, nRun);  // nDataがnRunバイト連続
			nOffset += nRun;
			nX      += nRun;
		}
		// ランの長さが０（コード化モード）
		else
		{
			const_uint8_t nCode = fgetc(fp);

			switch(nCode)
			{
			case 0x00:                              // 行の終端
				// 改行してオフセットを更新
				nX = 0;
				nY++;
				nOffset = (height - nY - 1) * width;
				break;

			case 0x01:                              // イメージの終端
				return;                                 // 展開を終了

			case 0x02:                              // イメージの移動
				{
					const_uint8_t nPosX = fgetc(fp);
					const_uint8_t nPosY = fgetc(fp);

					// 現在位置から(nPosX, nPosY)離れた位置からイメージを再開
					nX += nPosX;
					nY += nPosY;
					nOffset = (height - nY - 1) * width + nX;
				}
				break;

			default:                                // コード化絶対モード
				{
					UINT nLength = CALC_BOUND(nCode, 2);    // 読み込む長さ（ワード境界）
					uint8_t lpBuffer[256];                   // バッファ

					// イメージを格納
					if(fread(lpBuffer, sizeof(uint8_t), nLength, fp) < nLength)
					{
						throw IR_IMAGEDESTROYED;
					}
					memcpy(&image[nOffset], lpBuffer, nCode);

					// オフセット＆座標を更新
					nOffset += nCode;
					nX      += nCode;
				}
			}
		}
	}
}

// 4bit連長圧縮を解く
void UnRLE4(const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data)
{
}
*/

END_NAMESPACE()
