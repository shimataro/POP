// png_common.h �c
#ifndef __PNG_COMMON__
#define __PNG_COMMON__

extern "C"
{
#include "libpng\png.h"
}


////////////////////////////////////////////////////////////////////////////////
// PNG�`���Ǝ��̃f�[�^
typedef struct tagPNGEXTRA
{
	int bit_depth;                          // �摜�̐[�x�i1 / 2 / 4 / 8 / 16�j
	int color_type;                         // �J���[�^�C�v�iPNG_COLOR_TYPE_GRAY / PNG_COLOR_TYPE_PALETTE / PNG_COLOR_TYPE_RGB / PNG_COLOR_TYPE_RGB_ALPHA / PNG_COLOR_TYPE_GRAY_ALPHA�j
	int interlace_type;                     // �C���^�[���[�X�^�C�v�iPNG_INTERLACE_NONE / PNG_INTERLACE_ADAM7�j
} PNGEXTRA;


#endif // __PPM_COMMON__
