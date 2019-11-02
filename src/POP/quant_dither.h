// quant_dither.h
#ifndef __QUANT_DITHER__
#define __QUANT_DITHER__


// ディザ手法
typedef enum tagQUANT_DITHER_METHOD
{
	QDM_NONE,                               // ディザなし
	QDM_BAYER,                              // Bayer法
	QDM_RANDOM,                             // ランダムディザ
	QDM_SPREAD,                             // 誤差拡散法
} QUANT_DITHER_METHOD;


// 関数のプロトタイプ宣言
void quant_dither(const IMAGE_INFO *info_ptr, const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in, pixel_rgb8_ptr_t pixel_rgb8_ptr_out, const_palette8_ptr_t palette_ptr, const int palette_size, const QUANT_DITHER_METHOD method, callback_t callback, long callback_data);

#endif // __QUANT_DITHER__
