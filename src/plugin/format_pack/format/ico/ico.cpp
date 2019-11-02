// ico.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "ico.h"


// ICOイメージを入力
IMAGERESULT ico_import(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	return IR_READERROR;
}

// ICOイメージを出力
IMAGERESULT ico_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName, callback_t callback, long callback_data)
{
	return IR_WRITEERROR;
}

// オプション
BOOL ico_option(HWND hWnd)
{
	return TRUE;
}
