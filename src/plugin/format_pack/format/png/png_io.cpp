// png_io.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "../iotemplate.h"              // ReadData()
#include "png_io.h"
#include "png_dialog.h"

#include "png_decode.h"
#include "png_encode.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// PNG用パラメータ
	PNGPARAM g_param =
	{
		0x00000000,                             // テキストデータの圧縮＝なし、ガンマ値の保存＝なし、更新時刻の保存＝なし
		0,                                      // インターレース＝なし
		0,                                      // フィルタ＝なし
		6,                                      // 圧縮レベル＝６
	};

	void PNGAPI read_fn (png_structp png_ptr, png_bytep buffer, png_size_t size);
	void PNGAPI write_fn(png_structp png_ptr, png_bytep buffer, png_size_t size);
	void PNGAPI flush_fn(png_structp png_ptr);
END_NAMESPACE()


// PNGイメージを入力
IMAGERESULT png_import(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	png_structp png_ptr      = NULL;
	png_infop   png_info_ptr = NULL;

	if(!png_decode_init(&png_ptr, &png_info_ptr))
	{
		return IR_NOMEMORY;
	}

	try
	{
		// エラー処理
		if(setjmp(png_ptr->jmpbuf))
		{
			throw IR_INCORRECTHEADER;
		}

		// 入力手段を伝える
		png_set_read_fn(png_ptr, hFile, read_fn);
		png_read_info(png_ptr, png_info_ptr);

		// ヘッダを読み込む
		PNGEXTRA png_extra;
		png_decode_header(png_ptr, png_info_ptr, info_ptr, &png_extra);

		// 画像格納用メモリを確保
		if(!image_alloc(info_ptr, data_ptr))
		{
			throw IR_NOMEMORY;
		}

		// 画像データを取得
		png_decode_image(png_ptr, png_info_ptr, info_ptr, data_ptr, &png_extra, callback, callback_data);

		// 終了処理
		png_read_end(png_ptr, png_info_ptr);
		png_destroy_read_struct(&png_ptr, &png_info_ptr, (png_infopp)NULL);

		return IR_SUCCESS;
	}

	// 例外処理
	catch(IMAGERESULT result)
	{
		png_destroy_read_struct(&png_ptr, &png_info_ptr, (png_infopp)NULL);
		image_free(data_ptr);
		return result;
	}
}

// PNGイメージを出力
IMAGERESULT png_export(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	png_structp png_ptr  = NULL;
	png_infop   png_info_ptr = NULL;

	// 画像保存用構造体を確保
	if(!png_encode_init(&png_ptr, &png_info_ptr))
	{
		return IR_NOMEMORY;
	}

	try
	{
		if(setjmp(png_ptr->jmpbuf))
		{
			throw IR_WRITEERROR;
		}

		// 出力手段を伝える
		png_set_write_fn(png_ptr, hFile, write_fn, flush_fn);

		// ヘッダを書き込む
		png_encode_header(png_ptr, png_info_ptr, info_ptr, &g_param);

		// 画像データを書き込む
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			png_encode_image_gray8(png_ptr, png_info_ptr, info_ptr, data_ptr, callback, callback_data);
			break;

		case COLORMODE_INDEX:
			png_encode_image_index(png_ptr, png_info_ptr, info_ptr, data_ptr, callback, callback_data);
			break;

		case COLORMODE_RGB:
			png_encode_image_rgb8(png_ptr, png_info_ptr, info_ptr, data_ptr, callback, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}

		// 終了処理
		png_write_end(png_ptr, png_info_ptr);
		png_destroy_write_struct(&png_ptr, &png_info_ptr);

		return IR_SUCCESS;
	}

	// 例外処理
	catch(IMAGERESULT result)
	{
		png_destroy_write_struct(&png_ptr, &png_info_ptr);
		return result;
	}
}

// オプション
BOOL png_option(HWND hWnd)
{
	png_dialog theDlg(g_param, hWnd);
	if(theDlg.DoModal() == IDOK)
	{
		g_param = theDlg.m_param;
		return TRUE;
	}

	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// 読み込み関数
void PNGAPI read_fn(png_structp png_ptr, png_bytep buffer, png_size_t size)
{
	HANDLE hFile = reinterpret_cast<HANDLE>(png_get_io_ptr(png_ptr));
	ReadData(hFile, buffer, size, IR_IMAGEDESTROYED);
}

// 書き込み関数
void PNGAPI write_fn(png_structp png_ptr, png_bytep buffer, png_size_t size)
{
	HANDLE hFile = reinterpret_cast<HANDLE>(png_get_io_ptr(png_ptr));
	WriteData(hFile, buffer, size, IR_WRITEERROR);
}

// フラッシュ関数
void PNGAPI flush_fn(png_structp png_ptr)
{
	HANDLE hFile = reinterpret_cast<HANDLE>(png_get_io_ptr(png_ptr));
	::FlushFileBuffers(hFile);
}

END_NAMESPACE()
