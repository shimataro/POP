// quant_palette.h
#ifndef __QUANT_PALETTE__
#define __QUANT_PALETTE__


// �ʎq���p���b�g�쐬��@
typedef enum tagQUANT_PALETTE_METHOD
{
	QPM_FREQ,
	QPM_MEDIANCUT,
	QPM_PADIE,
	QPM_FIXEDPALETTE,
} QUANT_PALETTE_METHOD;


// �֐��̃v���g�^�C�v�錾
void quant_palette(const IMAGE_INFO *info_ptr, const_pixel_rgb8_ptr_t pixel_rgb8_ptr, palette8_ptr_t palette_ptr, const int palette_size, const QUANT_PALETTE_METHOD method, callback_t callback, long callback_data);

#endif // __QUANT_PALETTE__
