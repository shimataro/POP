// jpg.cpp
#include "../../stdafx.h"

#include "jpg.h"
#include "jpg_decode.h"
#include "jpg_data_src.h"


// JPGイメージを入力
IMAGERESULT jpg_import(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	try
	{
		// エラー処理関数の設定
		cinfo.err = jpeg_std_error(&jerr);
		jerr.error_exit = error_handler;

		// 初期化
		jpeg_create_decompress(&cinfo);         // JPEG展開ルーチンの初期化
		jpeg_handle_src(&cinfo, hFile);         // ファイルポインタを伝える
		jpeg_read_header(&cinfo, TRUE);         // ヘッダを読み込む

		// 展開パラメータの設定

		// In this example, we don't need to change any of the defaults set by
		// jpeg_read_header(), so we do nothing here.

		// 展開開始
		jpeg_start_decompress(&cinfo);

		jpg_decode_header(&cinfo, info_ptr);

		// 画像メモリを確保
		if(!image_alloc(info_ptr, data_ptr))
		{
			throw IR_NOMEMORY;
		}

		// イメージを展開
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			jpg_decode_image_gray8(&cinfo, info_ptr, data_ptr, callback, callback_data);
			break;

		case COLORMODE_RGB:
			jpg_decode_image_rgb8(&cinfo, info_ptr, data_ptr, callback, callback_data);
			break;
		}

		// 終了処理
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		return IR_SUCCESS;
	}

	catch(IMAGERESULT result)
	{
		jpeg_destroy_decompress(&cinfo);
		image_free(data_ptr);
		return result;
	}
}

// JPGイメージを出力
IMAGERESULT jpg_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName, callback_t callback, long callback_data)
{
	return IR_SUCCESS;
}

// 設定
BOOL jpg_option(HWND hWnd)
{
	return TRUE;
}
