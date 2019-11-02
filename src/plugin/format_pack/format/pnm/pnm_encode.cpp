// pnm_encode.cpp
#include "../../stdafx.h"
#include "pnm_encode.h"
#include <stdio.h>
#include <time.h>                       // time()


// 
void pnm_encode_header(FILE *fp, const IMAGE_INFO *info_ptr, const PNM_PIXELMODE pixelmode, const PNM_COLORMODE colormode)
{
	// signature
	for(int i = 0; i < SIZE_OF_ARRAY(pnm_mode); i++)
	{
		if(pnm_mode[i].pixelmode == pixelmode
		&& pnm_mode[i].colormode == colormode)
		{
			fprintf(fp, "%s\n", pnm_mode[i].name);
			break;
		}
	}

	// コメント
	if(info_ptr->comment != NULL)
	{
		fprintf(fp, "# %s\n", info_ptr->comment);
	}

	// 画像サイズ
	fprintf(fp, "%d %d\n", info_ptr->width, info_ptr->height);

	// 最大輝度値
	fprintf(fp, "255\n");
}

void pnm_encode_image_gray8(FILE *fp, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, const PNM_PIXELMODE pixelmode, callback_t callback, long callback_data)
{
	const_uint32_t width  = info_ptr->width;
	const_uint32_t height = info_ptr->height;

	// メモリ領域をロック
	pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_gray8_ptr);

	for(uint32_t y = 0; y < height; y++)
	{
		for(uint32_t x = 0; x < width; x++)
		{
			const_uint8_t gray = pixel_gray8_ptr->el.gray;
			pixel_gray8_ptr++;

			// アスキーモードの場合
			switch(pixelmode)
			{
			case PP_ASCII:
				fprintf(fp, "%d ", gray);
				break;

			// バイナリモードの場合
			case PP_BINARY:
				fputc(gray, fp);
				break;

			default:
				__assume(0);
			}
		}

		// コールバック
		if(callback(y, height, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void pnm_encode_image_rgb8(FILE *fp, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, const PNM_PIXELMODE pixelmode, callback_t callback, long callback_data)
{
	const_uint32_t width  = info_ptr->width;
	const_uint32_t height = info_ptr->height;

	// メモリ領域をロック
	pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

	for(uint32_t y = 0; y < height; y++)
	{
		for(uint32_t x = 0; x < width; x++)
		{
			const int r = pixel_rgb8_ptr->el.r;;
			const int g = pixel_rgb8_ptr->el.g;
			const int b = pixel_rgb8_ptr->el.b;
			pixel_rgb8_ptr++;

			// アスキーモードの場合
			switch(pixelmode)
			{
			case PP_ASCII:
				fprintf(fp, "%d %d %d ", r, g, b);
				break;

			// バイナリモードの場合
			case PP_BINARY:
				fputc(r, fp);
				fputc(g, fp);
				fputc(b, fp);
				break;

			default:
				__assume(0);
			}
		}

		// コールバック
		if(callback(y, height, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}
