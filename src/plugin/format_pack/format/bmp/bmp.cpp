// bmp.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "bmp.h"

#include "bmp_decode.h"
#include "bmp_encode.h"


// BMPイメージを入力
IMAGERESULT bmp_import(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	try
	{
		BMPDECODEDATA data;

		// ヘッダを読み込む
		bmp_decode_header(hFile, info_ptr, data);

		// 画像格納用メモリを確保
		if(!image_alloc(info_ptr, data_ptr))
		{
			throw IR_NOMEMORY;
		}

		// 画像データを読み込む
		bmp_decode_image(hFile, info_ptr, data_ptr, data, callback, callback_data);

		return IR_SUCCESS;
	}

	// 例外処理
	catch(IMAGERESULT result)
	{
		image_free(data_ptr);
		return result;
	}
}

// BMPイメージを出力
IMAGERESULT bmp_export(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	try
	{
		const LONG lDistanceToMove = ::SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

		bmp_encode_header(hFile, info_ptr);
		bmp_encode_image (hFile, lDistanceToMove, info_ptr, data_ptr, callback, callback_data);

		return IR_SUCCESS;
	}

	// 例外処理
	catch(IMAGERESULT result)
	{
		return result;
	}
}

// オプション
BOOL bmp_option(HWND hWnd)
{
	return TRUE;
}
