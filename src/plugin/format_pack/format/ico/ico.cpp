// ico.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "ico.h"


// ICO�C���[�W�����
IMAGERESULT ico_import(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	return IR_READERROR;
}

// ICO�C���[�W���o��
IMAGERESULT ico_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName, callback_t callback, long callback_data)
{
	return IR_WRITEERROR;
}

// �I�v�V����
BOOL ico_option(HWND hWnd)
{
	return TRUE;
}
