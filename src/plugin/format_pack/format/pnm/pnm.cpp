// pnm.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "pnm.h"
#include "pnm_dialog.h"

#include "pnm_decode.h"
#include "pnm_encode.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// PNM用パラメータ
	PNMPARAM g_param =
	{
		0,                                      // 格納形式＝アスキー
	};
END_NAMESPACE()

/*
// イメージを入力
IMAGERESULT pnm_import(IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data)
{
	FILE *fp = fopen(lpszPathName, "rb");
	if(fp == NULL)
	{
		return IR_READERROR;
	}
	fsetpos(fp, pos);
	memset(data_ptr, 0, sizeof(IMAGE_DATA));

	try
	{
		PNM_PIXELMODE pixelmode;
		PNM_COLORMODE colormode;
		PNMEXTRA pnm_extra;
		pnm_decode_header(fp, info_ptr, &pixelmode, &colormode, &pnm_extra);

		if(!image_alloc(info_ptr, data_ptr))
		{
			throw IR_NOMEMORY;
		}

		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			pnm_decode_image_gray8(fp, info_ptr, data_ptr, pixelmode, &pnm_extra, callback, callback_data);
			break;

		case COLORMODE_RGB:
			pnm_decode_image_rgb8(fp, info_ptr, data_ptr, pixelmode, &pnm_extra, callback, callback_data);
			break;
		}

		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		fclose(fp);
		return result;
	}
}

// イメージを出力
IMAGERESULT pnm_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName, callback_t callback, long callback_data)
{
	FILE *fp = fopen(lpszPathName, "wb");
	if(fp == NULL)
	{
		return IR_WRITEERROR;
	}

	try
	{
		PNM_PIXELMODE pixelmode = PP_ASCII;
		PNM_COLORMODE colormode = (info_ptr->mode == COLORMODE_GRAYSCALE) ? PC_GRAYSCALE : PC_COLOR;

		// ヘッダを書き込む
		pnm_encode_header(fp, info_ptr, pixelmode, colormode);

		// 画像データを書き込む
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			pnm_encode_image_gray8(fp, info_ptr, data_ptr, pixelmode, callback, callback_data);
			break;

		case COLORMODE_RGB:
			pnm_encode_image_rgb8(fp, info_ptr, data_ptr, pixelmode, callback, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}

		fclose(fp);
		return IR_SUCCESS;
	}

	// 例外処理
	catch(IMAGERESULT result)
	{
		fclose(fp);
		return result;
	}
}

// オプション
BOOL pnm_option(HWND hWnd)
{
	pnm_dialog theDlg(g_param, hWnd);
	if(theDlg.DoModal() == IDOK)
	{
		g_param = theDlg.m_param;
		return TRUE;
	}

	return FALSE;
}
*/
