// quant_palette.cpp
#include "stdafx.h"
#include "quant_palette.h"

#include <memory.h>                     // memset()
#include <algorithm>                    // std::partial_sort()


// ヒストグラム
typedef struct tagHISTGRAM
{
	UINT freq;                              // 出現回数
	int r, g, b;                            // 色
} HISTGRAM;


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	void _quant_palette_freq(const IMAGE_INFO *info_ptr, const_pixel_rgb8_ptr_t pixel_rgb8_ptr, palette8_ptr_t palette_ptr, const int palette_size, callback_t callback_none, long callback_data);

	// ヒストグラムの頻度比較（ソート時に使用）
	inline bool _greater_histgram(const HISTGRAM &hist1, const HISTGRAM &hist2)
	{return hist1.freq >  hist2.freq;}
END_NAMESPACE()


// 量子化パレットを作成
void quant_palette(const IMAGE_INFO *info_ptr, const_pixel_rgb8_ptr_t pixel_rgb8_ptr, palette8_ptr_t palette_ptr, const int palette_size, const QUANT_PALETTE_METHOD method, callback_t callback_none, long callback_data)
{
	switch(method)
	{
	case QPM_FREQ:
	case QPM_MEDIANCUT:
	case QPM_PADIE:
	case QPM_FIXEDPALETTE:
		_quant_palette_freq(info_ptr, pixel_rgb8_ptr, palette_ptr, palette_size, callback_none, callback_data);
		return;

	default:
		__assume(0);
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// 量子化パレットを作成（頻度法）
void _quant_palette_freq(const IMAGE_INFO *info_ptr, const_pixel_rgb8_ptr_t pixel_rgb8_ptr, palette8_ptr_t palette_ptr, const int palette_size, callback_t callback_none, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// 簡易ヒストグラムを作成
	HISTGRAM histgram[MAX_COLOR_HASH + 1];
	memset(histgram, 0, sizeof(histgram));
	for(int i = 0; i < width * height; i++)
	{
		const_uint8_t r = pixel_rgb8_ptr[i].el.r;
		const_uint8_t g = pixel_rgb8_ptr[i].el.g;
		const_uint8_t b = pixel_rgb8_ptr[i].el.b;
		const int index = HASH_COLOR_RGB(r, g, b);

		histgram[index].r += r;
		histgram[index].g += g;
		histgram[index].b += b;
		histgram[index].freq++;
	}

	// 頻度の高い順にpalette_size個のヒストグラムをソート
	std::partial_sort(
		&histgram[0],
		&histgram[palette_size],
		&histgram[MAX_COLOR_HASH + 1],
		_greater_histgram);

	// パレットへ代入
	for(int j = 0; j < palette_size; j++)
	{
		const int freq = histgram[j].freq;
		if(freq == 0)
		{
			return;
		}

		palette_ptr[j].r = histgram[j].r / freq;
		palette_ptr[j].g = histgram[j].g / freq;
		palette_ptr[j].b = histgram[j].b / freq;
	}
}

END_NAMESPACE()
