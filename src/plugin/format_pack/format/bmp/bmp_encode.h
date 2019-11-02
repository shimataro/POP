// bmp_encode.h �cBMP�t�@�C���̏�������
#ifndef __BMP_ENCODE__
#define __BMP_ENCODE__

#include "bmp_common.h"


// �֐��̃v���g�^�C�v�錾
void bmp_encode_header(HANDLE hFile, const IMAGE_INFO *info_ptr);
void bmp_encode_image (HANDLE hFile, const LONG lDistanceToMove, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);

#endif // __BMP_ENCODE__
