// pgf.cpp �c�C���v�������e�[�V�����t�@�C��
#include "stdafx.h"
#include "pgf.h"


// PGF�C���[�W�����
IMAGERESULT pgf_import(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	return IR_INCORRECTHEADER;
}

// PGF�C���[�W���o��
IMAGERESULT pgf_export(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	return IR_WRITEERROR;
}

// �I�v�V����
BOOL pgf_option(HWND hWnd)
{
	return TRUE;
}
