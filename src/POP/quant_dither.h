// quant_dither.h
#ifndef __QUANT_DITHER__
#define __QUANT_DITHER__


// �f�B�U��@
typedef enum tagQUANT_DITHER_METHOD
{
	QDM_NONE,                               // �f�B�U�Ȃ�
	QDM_BAYER,                              // Bayer�@
	QDM_RANDOM,                             // �����_���f�B�U
	QDM_SPREAD,                             // �덷�g�U�@
} QUANT_DITHER_METHOD;


// �֐��̃v���g�^�C�v�錾
void quant_dither(const IMAGE_INFO *info_ptr, const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in, pixel_rgb8_ptr_t pixel_rgb8_ptr_out, const_palette8_ptr_t palette_ptr, const int palette_size, const QUANT_DITHER_METHOD method, callback_t callback, long callback_data);

#endif // __QUANT_DITHER__
