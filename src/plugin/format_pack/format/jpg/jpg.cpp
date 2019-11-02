// jpg.cpp
#include "../../stdafx.h"

#include "jpg.h"
#include "jpg_decode.h"
#include "jpg_data_src.h"


// JPG�C���[�W�����
IMAGERESULT jpg_import(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	try
	{
		// �G���[�����֐��̐ݒ�
		cinfo.err = jpeg_std_error(&jerr);
		jerr.error_exit = error_handler;

		// ������
		jpeg_create_decompress(&cinfo);         // JPEG�W�J���[�`���̏�����
		jpeg_handle_src(&cinfo, hFile);         // �t�@�C���|�C���^��`����
		jpeg_read_header(&cinfo, TRUE);         // �w�b�_��ǂݍ���

		// �W�J�p�����[�^�̐ݒ�

		// In this example, we don't need to change any of the defaults set by
		// jpeg_read_header(), so we do nothing here.

		// �W�J�J�n
		jpeg_start_decompress(&cinfo);

		jpg_decode_header(&cinfo, info_ptr);

		// �摜���������m��
		if(!image_alloc(info_ptr, data_ptr))
		{
			throw IR_NOMEMORY;
		}

		// �C���[�W��W�J
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			jpg_decode_image_gray8(&cinfo, info_ptr, data_ptr, callback, callback_data);
			break;

		case COLORMODE_RGB:
			jpg_decode_image_rgb8(&cinfo, info_ptr, data_ptr, callback, callback_data);
			break;
		}

		// �I������
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

// JPG�C���[�W���o��
IMAGERESULT jpg_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName, callback_t callback, long callback_data)
{
	return IR_SUCCESS;
}

// �ݒ�
BOOL jpg_option(HWND hWnd)
{
	return TRUE;
}
