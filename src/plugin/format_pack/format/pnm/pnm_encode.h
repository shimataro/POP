// pnm_encode.h �cPNM�t�@�C���̏�������
#ifndef __PNM_ENCODE__
#define __PNM_ENCODE__

#include "pnm_common.h"


// �֐��̃v���g�^�C�v�錾
void pnm_encode_header(FILE *fp, const IMAGE_INFO *info_ptr, const PNM_PIXELMODE pixelmode, const PNM_COLORMODE colormode);
void pnm_encode_image_gray8(FILE *fp, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, const PNM_PIXELMODE pixelmode, callback_t callback, long callback_data);
void pnm_encode_image_rgb8 (FILE *fp, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, const PNM_PIXELMODE pixelmode, callback_t callback, long callback_data);

#endif // __PNM_ENCODE__
