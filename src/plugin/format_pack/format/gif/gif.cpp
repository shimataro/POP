// gif.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "gif.h"

#include "gif_decode.h"


// GIF�C���[�W�����
IMAGERESULT gif_import(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	try
	{
		GIFDECODEDATA data;

		// �w�b�_��ǂݍ���
		gif_decode_header(hFile, info_ptr, data);

		// �摜�i�[�p���������m��
		if(!image_alloc(info_ptr, data_ptr))
		{
			throw IR_NOMEMORY;
		}

		// �摜�f�[�^��ǂݍ���
		gif_decode_image(hFile, info_ptr, data_ptr, data, callback, callback_data);

		return IR_SUCCESS;
	}

	// ��O����
	catch(IMAGERESULT result)
	{
		image_free(data_ptr);
		return result;
	}
}

// GIF�C���[�W���o��
IMAGERESULT gif_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName, callback_t callback, long callback_data)
{
	return IR_WRITEERROR;
}

// �I�v�V����
BOOL gif_option(HWND hWnd)
{
	return TRUE;
}
