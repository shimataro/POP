// gif_decode.h …GIFファイルの読み込み
#ifndef __GIF_DECODE__
#define __GIF_DECODE__

#include "gif_common.h"


typedef struct tagGIFDECODEDATA
{
	GIFHEADER_GLOBAL header;
	palette8_t       palette[256];
} GIFDECODEDATA;


// 関数のプロトタイプ宣言
void gif_decode_header(HANDLE hFile, IMAGE_INFO *info_ptr, GIFDECODEDATA &data);
void gif_decode_image (HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, GIFDECODEDATA &data, callback_t callback, long callback_data);

#endif // __GIF_DECODE__
