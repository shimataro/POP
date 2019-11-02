// bmp_decode.h �cBMP�t�@�C���̓ǂݍ���
#ifndef __BMP_DECODE__
#define __BMP_DECODE__

#include "bmp_common.h"


typedef struct tagBMPDECODEDATA
{
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
} BMPDECODEDATA;


// �֐��̃v���g�^�C�v�錾
void bmp_decode_header(HANDLE hFile, IMAGE_INFO *info_ptr, BMPDECODEDATA &data);
void bmp_decode_image (HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data);

#endif // __BMP_DECODE__
