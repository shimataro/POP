// bmp_decode.h …BMPファイルの読み込み
#ifndef __BMP_DECODE__
#define __BMP_DECODE__

#include "bmp_common.h"


typedef struct tagBMPDECODEDATA
{
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
} BMPDECODEDATA;


// 関数のプロトタイプ宣言
void bmp_decode_header(HANDLE hFile, IMAGE_INFO *info_ptr, BMPDECODEDATA &data);
void bmp_decode_image (HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data);

#endif // __BMP_DECODE__
