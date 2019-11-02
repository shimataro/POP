// png_io.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "../iotemplate.h"              // ReadData()
#include "png_io.h"
#include "png_dialog.h"

#include "png_decode.h"
#include "png_encode.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// PNG�p�p�����[�^
	PNGPARAM g_param =
	{
		0x00000000,                             // �e�L�X�g�f�[�^�̈��k���Ȃ��A�K���}�l�̕ۑ����Ȃ��A�X�V�����̕ۑ����Ȃ�
		0,                                      // �C���^�[���[�X���Ȃ�
		0,                                      // �t�B���^���Ȃ�
		6,                                      // ���k���x�����U
	};

	void PNGAPI read_fn (png_structp png_ptr, png_bytep buffer, png_size_t size);
	void PNGAPI write_fn(png_structp png_ptr, png_bytep buffer, png_size_t size);
	void PNGAPI flush_fn(png_structp png_ptr);
END_NAMESPACE()


// PNG�C���[�W�����
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
		// �G���[����
		if(setjmp(png_ptr->jmpbuf))
		{
			throw IR_INCORRECTHEADER;
		}

		// ���͎�i��`����
		png_set_read_fn(png_ptr, hFile, read_fn);
		png_read_info(png_ptr, png_info_ptr);

		// �w�b�_��ǂݍ���
		PNGEXTRA png_extra;
		png_decode_header(png_ptr, png_info_ptr, info_ptr, &png_extra);

		// �摜�i�[�p���������m��
		if(!image_alloc(info_ptr, data_ptr))
		{
			throw IR_NOMEMORY;
		}

		// �摜�f�[�^���擾
		png_decode_image(png_ptr, png_info_ptr, info_ptr, data_ptr, &png_extra, callback, callback_data);

		// �I������
		png_read_end(png_ptr, png_info_ptr);
		png_destroy_read_struct(&png_ptr, &png_info_ptr, (png_infopp)NULL);

		return IR_SUCCESS;
	}

	// ��O����
	catch(IMAGERESULT result)
	{
		png_destroy_read_struct(&png_ptr, &png_info_ptr, (png_infopp)NULL);
		image_free(data_ptr);
		return result;
	}
}

// PNG�C���[�W���o��
IMAGERESULT png_export(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	png_structp png_ptr  = NULL;
	png_infop   png_info_ptr = NULL;

	// �摜�ۑ��p�\���̂��m��
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

		// �o�͎�i��`����
		png_set_write_fn(png_ptr, hFile, write_fn, flush_fn);

		// �w�b�_����������
		png_encode_header(png_ptr, png_info_ptr, info_ptr, &g_param);

		// �摜�f�[�^����������
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

		// �I������
		png_write_end(png_ptr, png_info_ptr);
		png_destroy_write_struct(&png_ptr, &png_info_ptr);

		return IR_SUCCESS;
	}

	// ��O����
	catch(IMAGERESULT result)
	{
		png_destroy_write_struct(&png_ptr, &png_info_ptr);
		return result;
	}
}

// �I�v�V����
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

// �ǂݍ��݊֐�
void PNGAPI read_fn(png_structp png_ptr, png_bytep buffer, png_size_t size)
{
	HANDLE hFile = reinterpret_cast<HANDLE>(png_get_io_ptr(png_ptr));
	ReadData(hFile, buffer, size, IR_IMAGEDESTROYED);
}

// �������݊֐�
void PNGAPI write_fn(png_structp png_ptr, png_bytep buffer, png_size_t size)
{
	HANDLE hFile = reinterpret_cast<HANDLE>(png_get_io_ptr(png_ptr));
	WriteData(hFile, buffer, size, IR_WRITEERROR);
}

// �t���b�V���֐�
void PNGAPI flush_fn(png_structp png_ptr)
{
	HANDLE hFile = reinterpret_cast<HANDLE>(png_get_io_ptr(png_ptr));
	::FlushFileBuffers(hFile);
}

END_NAMESPACE()
