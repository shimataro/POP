// ras.cpp
#include "../../stdafx.h"
#include "ras.h"

#include "ras_decode.h"
#include "ras_encode.h"


// RASイメージを入力
IMAGERESULT ras_import(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	try
	{
		RASTERFILEHEADER header;
		ras_decode_header(hFile, info_ptr, &header);

		if(!image_alloc(info_ptr, data_ptr))
		{
			throw IR_NOMEMORY;
		}

		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			ras_decode_image_gray8(hFile, info_ptr, data_ptr, &header);
			break;

		case COLORMODE_INDEX:
			ras_decode_image_index(hFile, info_ptr, data_ptr, &header);
			break;

		case COLORMODE_RGB:
			ras_decode_image_rgb8(hFile, info_ptr, data_ptr, &header);
			break;

		default:
			throw IR_UNKNOWN;
		}

		return IR_SUCCESS;
	}

	// 例外処理
	catch(IMAGERESULT result)
	{
		image_free(data_ptr);
		return result;
	}
}

// RASイメージを出力
IMAGERESULT ras_export(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	try
	{
		RASTERFILEHEADER header;
		ras_encode_header(hFile, info_ptr, &header);

		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			ras_encode_image_gray8(hFile, info_ptr, data_ptr, &header, callback, callback_data);
			break;

		case COLORMODE_INDEX:
			ras_encode_image_index(hFile, info_ptr, data_ptr, &header, callback, callback_data);
			break;

		case COLORMODE_RGB:
			ras_encode_image_rgb8(hFile, info_ptr, data_ptr, &header, callback, callback_data);
			break;
		}
		return IR_SUCCESS;
	}

	// 例外処理
	catch(IMAGERESULT result)
	{
		return result;
	}
}

// オプション
BOOL ras_option(HWND hWnd)
{
	return TRUE;
}
