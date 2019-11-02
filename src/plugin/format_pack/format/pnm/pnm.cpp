// pnm.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "pnm.h"
#include "pnm_dialog.h"

#include "pnm_decode.h"
#include "pnm_encode.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// PNM�p�p�����[�^
	PNMPARAM g_param =
	{
		0,                                      // �i�[�`�����A�X�L�[
	};
END_NAMESPACE()

/*
// �C���[�W�����
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

// �C���[�W���o��
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

		// �w�b�_����������
		pnm_encode_header(fp, info_ptr, pixelmode, colormode);

		// �摜�f�[�^����������
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

	// ��O����
	catch(IMAGERESULT result)
	{
		fclose(fp);
		return result;
	}
}

// �I�v�V����
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
