// pnm_decode.cpp
#include "../../stdafx.h"
#include "pnm_decode.h"
#include <stdio.h>
#include <string.h>

#undef max
#undef min
#include <valarray>                     // std::valarray<>


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// 1行読み込む（コメント、改行マークのみの行は読み飛ばす）
	int get_line(FILE *fp, char buffer[], const int buffer_max);

	// 輝度値を読み込む（読み込みに失敗したらEOFを返す）
	int get_intensity(FILE *fp, const PNM_PIXELMODE pixelmode);

	// 輝度値テーブルを作成
	void create_table(uint8_t table[], const int max_color_component);
END_NAMESPACE()


// ヘッダ部を読み込む
void pnm_decode_header(FILE *fp, IMAGE_INFO *info_ptr, PNM_PIXELMODE *pixelmode_ptr, PNM_COLORMODE *colormode_ptr, PNMEXTRA *pnm_extra_ptr)
{
	char buffer[256];

	// 署名
	fgets(buffer, sizeof(buffer), fp);
	for(int i = 0; i < SIZE_OF_ARRAY(pnm_mode); i++)
	{
		if(strncmp(buffer, pnm_mode[i].name, PNM_SIGNATURE_SIZE) == 0)
		{
			*pixelmode_ptr  = pnm_mode[i].pixelmode;
			*colormode_ptr  = pnm_mode[i].colormode;
			info_ptr->depth = pnm_mode[i].bits;
			info_ptr->mode  = pnm_mode[i].mode;
			goto SIGNATURE_END;
		}
	}
	throw IR_INCORRECTHEADER;

SIGNATURE_END:

	// 画像サイズ
	get_line(fp, buffer, sizeof(buffer));
	sscanf(buffer, "%d %d", &info_ptr->width, &info_ptr->height);

	// 最大輝度値
	get_line(fp, buffer, sizeof(buffer));
	sscanf(buffer, "%d", &pnm_extra_ptr->max_color_components);

	info_ptr->alpha      = FALSE;
	info_ptr->gamma_file = GAMMA_DEFAULT_FILE;
}

// イメージ部を読み込む
void pnm_decode_image_gray8(FILE *fp, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, const PNM_PIXELMODE pixelmode, const PNMEXTRA *pnm_extra_ptr, callback_t callback, long callback_data)
{
	const uint32_t width  = info_ptr->width;
	const uint32_t height = info_ptr->height;
	const int max_color_components = pnm_extra_ptr->max_color_components;

	// 輝度値テーブルの作成
	std::valarray<uint8_t> table(max_color_components + 1);
	create_table(&table[0], max_color_components);

	// メモリ領域をロック
	pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_gray8_ptr);

	for(uint32_t y = 0; y < height; y++)
	{
		for(uint32_t x = 0; x < width; x++)
		{
			const int gray = get_intensity(fp, pixelmode);
			if(gray == EOF || gray > max_color_components)
			{
				throw IR_IMAGEDESTROYED;
			}
			pixel_gray8_ptr->el.gray  = gray;
			pixel_gray8_ptr++;
		}

		// コールバック
		if(callback(y, height, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void pnm_decode_image_rgb8(FILE *fp, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, const PNM_PIXELMODE pixelmode, const PNMEXTRA *pnm_extra_ptr, callback_t callback, long callback_data)
{
	const uint32_t width  = info_ptr->width;
	const uint32_t height = info_ptr->height;
	const int max_color_components = pnm_extra_ptr->max_color_components;

	// 輝度値テーブルの作成
	std::valarray<uint8_t> table(max_color_components + 1);
	create_table(&table[0], max_color_components);

	// メモリ領域をロック
	pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

	for(uint32_t y = 0; y < height; y++)
	{
		for(uint32_t x = 0; x < width; x++)
		{
			const int r = get_intensity(fp, pixelmode);
			const int g = get_intensity(fp, pixelmode);
			const int b = get_intensity(fp, pixelmode);
			if(r == EOF || r > max_color_components
			|| g == EOF || g > max_color_components
			|| b == EOF || b > max_color_components)
			{
				throw IR_IMAGEDESTROYED;
			}
			// 輝度値を格納（輝度値テーブルで変換）
			pixel_rgb8_ptr->el.r = r;
			pixel_rgb8_ptr->el.g = g;
			pixel_rgb8_ptr->el.b = b;
			pixel_rgb8_ptr->el.a = 0xff;
			pixel_rgb8_ptr++;
		}

		// コールバック
		if(callback(y, height, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// 1行読み込む（コメント、改行マークのみの行は読み飛ばす）
int get_line(FILE *fp, char buffer[], const int buffer_max)
{
	int comment_count = -1;

	do
	{
		fgets(buffer, buffer_max, fp);
		comment_count++;
	}
	while(buffer[0] == ';' || buffer[0] == '\n');

	// 読み飛ばした行数を返す
	return comment_count;
}

// 輝度値を読み込む（読み込みに失敗したらEOFを返す）
int get_intensity(FILE *fp, const PNM_PIXELMODE pixelmode)
{
	int ch;
	switch(pixelmode)
	{
	case PP_ASCII:
		if(fscanf(fp, "%d", &ch) != 1)
		{
			return EOF;
		}
		return ch;

	case PP_BINARY:
		return fgetc(fp);

	default:
		__assume(0);
	}
}

// 輝度値テーブルを作成
void create_table(uint8_t table[], const int max_color_component)
{
	// table[0] = 0, table[max_color_component] = 255となるように調整
	for(int i = 0; i <= max_color_component; i++)
	{
		table[i] = i * 255 / max_color_component;
	}
}

END_NAMESPACE()
