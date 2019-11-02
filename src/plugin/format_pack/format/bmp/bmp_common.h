// bmp_common.h …
#ifndef __BMP_COMMON__
#define __BMP_COMMON__

#include <windows.h>


// 定数の定義
#define BMP_TYPE  0x4d42                // BITMAPFILEHEADER::bfTypeの識別文字（"BM"）
#define BOUND_BMP 4                     // ビットマップファイルのラインバウンダリ


// bit ビットの data を、 max_bit にスケールアップ
#define SCALEUP(data, bit, max_bit)     (((data) << ((max_bit) - (bit))) | (data) >> (2 * (bit) - (max_bit)))


// RGB各5ビットの画像形式
typedef union
{
	uint16_t value;
	struct elements
	{
		unsigned b : 5;                         // 青:5bit
		unsigned g : 5;                         // 緑:5bit
		unsigned r : 5;                         // 赤:5bit
		unsigned x : 1;                         // ダミー:1bit
	} el;
} bitmap_rgb5_t;

#endif // __BMP_COMMON__
