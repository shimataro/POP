// png_common.h …
#ifndef __PNG_COMMON__
#define __PNG_COMMON__

extern "C"
{
#include "libpng\png.h"
}


////////////////////////////////////////////////////////////////////////////////
// PNG形式独自のデータ
typedef struct tagPNGEXTRA
{
	int bit_depth;                          // 画像の深度（1 / 2 / 4 / 8 / 16）
	int color_type;                         // カラータイプ（PNG_COLOR_TYPE_GRAY / PNG_COLOR_TYPE_PALETTE / PNG_COLOR_TYPE_RGB / PNG_COLOR_TYPE_RGB_ALPHA / PNG_COLOR_TYPE_GRAY_ALPHA）
	int interlace_type;                     // インターレースタイプ（PNG_INTERLACE_NONE / PNG_INTERLACE_ADAM7）
} PNGEXTRA;


#endif // __PPM_COMMON__
