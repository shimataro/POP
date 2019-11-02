// gif.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "gif.h"

#include "gif_decode.h"


// GIFイメージを入力
IMAGERESULT gif_import(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	try
	{
		GIFDECODEDATA data;

		// ヘッダを読み込む
		gif_decode_header(hFile, info_ptr, data);

		// 画像格納用メモリを確保
		if(!image_alloc(info_ptr, data_ptr))
		{
			throw IR_NOMEMORY;
		}

		// 画像データを読み込む
		gif_decode_image(hFile, info_ptr, data_ptr, data, callback, callback_data);

		return IR_SUCCESS;
	}

	// 例外処理
	catch(IMAGERESULT result)
	{
		image_free(data_ptr);
		return result;
	}
}

// GIFイメージを出力
IMAGERESULT gif_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName, callback_t callback, long callback_data)
{
	return IR_WRITEERROR;
}

// オプション
BOOL gif_option(HWND hWnd)
{
	return TRUE;
}
