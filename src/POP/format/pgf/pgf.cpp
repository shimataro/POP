// pgf.cpp …インプリメンテーションファイル
#include "stdafx.h"
#include "pgf.h"


// PGFイメージを入力
IMAGERESULT pgf_import(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	return IR_INCORRECTHEADER;
}

// PGFイメージを出力
IMAGERESULT pgf_export(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	return IR_WRITEERROR;
}

// オプション
BOOL pgf_option(HWND hWnd)
{
	return TRUE;
}
