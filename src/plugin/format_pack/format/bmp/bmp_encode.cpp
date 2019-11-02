// bmp_encode.cpp
#include "../../stdafx.h"
#include "../iotemplate.h"
#include "bmp_encode.h"

#include <stdio.h>
#include <io.h>                         // filelength()

#undef max
#undef min
#include <valarray>                     // std::valarray<>


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	long save_index(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
	long save_rgb8 (HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
END_NAMESPACE()


// �w�b�_������������
void bmp_encode_header(HANDLE hFile, const IMAGE_INFO *info_ptr)
{
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;

	// BITMAPFILEHEADER�̐ݒ�
	fileheader.bfType      = BMP_TYPE;      // �t�@�C���^�C�v�imust be "BM"�j
	fileheader.bfSize      = 0;             // �t�@�C���̃T�C�Y�i�Ƃ肠�����O�j
	fileheader.bfReserved1 = 0;             // �\��imust be 0�j
	fileheader.bfReserved2 = 0;             // �\��imust be 0�j
	fileheader.bfOffBits   = 0;             // �C���[�W�f�[�^�܂ł̃I�t�Z�b�g�i�Ƃ肠�����O�j

	// BITMAPINFOHEADER�̐ݒ�
	infoheader.biSize          = sizeof(BITMAPINFOHEADER);
	infoheader.biWidth         = info_ptr->width;   // ��
	infoheader.biHeight        = info_ptr->height;  // ����
	infoheader.biPlanes        = 1;             // �v���[���imust be 1�j
	// �r�b�g��
	switch(info_ptr->mode)
	{
	case COLORMODE_INDEX:
		infoheader.biBitCount = info_ptr->depth;
		break;

	case COLORMODE_RGB:
		infoheader.biBitCount = 24;
		break;

	default:
		throw IR_UNKNOWN;
	}
	infoheader.biCompression   = BI_RGB;
	infoheader.biSizeImage     = 0;
	infoheader.biXPelsPerMeter = info_ptr->density_x;
	infoheader.biYPelsPerMeter = info_ptr->density_y;
	infoheader.biClrUsed       = 0;
	infoheader.biClrImportant  = 0;

	// �w�b�_����������
	WriteData(hFile, &fileheader, 1, IR_WRITEERROR);
	WriteData(hFile, &infoheader, 1, IR_WRITEERROR);
}

// �C���[�W������������
void bmp_encode_image(HANDLE hFile, const LONG lDistanceToMove, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	try
	{
		long image_offset = 0;
		switch(info_ptr->mode)
		{
		case COLORMODE_INDEX:
			image_offset = save_index(hFile, info_ptr, data_ptr, callback, callback_data);
			break;

		case COLORMODE_RGB:
			image_offset = save_rgb8(hFile, info_ptr, data_ptr, callback, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}

		// �C���[�W�t�@�C���̃T�C�Y���擾
		::FlushFileBuffers(hFile);
		const uint32_t file_size = ::GetFileSize(hFile, NULL);

		// BITMAPFILEHEADER�\���̂��Ăя�������
		if(TRUE)
		{
			BITMAPFILEHEADER fileheader;
			fileheader.bfType      = BMP_TYPE;      // �t�@�C���^�C�v�imust be "BM"�j
			fileheader.bfSize      = file_size;     // �t�@�C���̃T�C�Y
			fileheader.bfReserved1 = 0;             // �\��imust be 0�j
			fileheader.bfReserved2 = 0;             // �\��imust be 0�j
			fileheader.bfOffBits   = image_offset;  // �C���[�W�f�[�^�܂ł̃I�t�Z�b�g

			::SetFilePointer(hFile, lDistanceToMove, NULL, FILE_BEGIN);
			WriteData(hFile, &fileheader, 1, IR_WRITEERROR);
		}
	}

	// ��O����
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

long save_index(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;
	const int depth  = info_ptr->depth;

	const_uint32_t hsize     = CALC_HSIZE(width, depth);
	const_uint32_t hsize_BMP = CALC_BOUND(hsize, BOUND_BMP);

	const_uint8_ptr_t    index_ptr    = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	uint32_t palette_size = 0;
	auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr, NULL, &palette_size);

	// �p���b�g����������
	if(TRUE)
	{
		std::valarray<RGBQUAD> palette(palette_size);

		// �o�b�t�@�Ɉړ�
		for(uint32_t i = 0; i < palette_size; i++)
		{
			palette[i].rgbRed   = palette8_ptr[i].r;
			palette[i].rgbGreen = palette8_ptr[i].g;
			palette[i].rgbBlue  = palette8_ptr[i].b;
		}
		WriteData(hFile, &palette[0], palette.size(), IR_WRITEERROR);
	}

	// �C���[�W�܂ł̃I�t�Z�b�g���擾
	::FlushFileBuffers(hFile);
	const long image_offset = ::SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

	// �C���[�W����������
	if(TRUE)
	{
		std::valarray<uint8_t> buffer(hsize_BMP);
		uint32_t offset = hsize * (height - 1);
		for(int y = 0; y < height; y++)
		{
			memcpy(&buffer[0], &index_ptr[offset], hsize);
			WriteData(hFile, &buffer[0], buffer.size(), IR_WRITEERROR);
			offset -= hsize;

			// �R�[���o�b�N
			if(callback(y, height, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}

	return image_offset;
}

long save_rgb8(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

	// �C���[�W�܂ł̃I�t�Z�b�g���擾
	::FlushFileBuffers(hFile);
	const long image_offset = ::SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

	if(TRUE)
	{
		const_uint32_t hsize_BMP = CALC_BOUND(CALC_HSIZE(width, 24), BOUND_BMP);
		std::valarray<uint8_t> buffer(hsize_BMP);

		uint32_t offset = width * (height - 1);
		for(int y = 0; y < height; y++)
		{
			uint8_ptr_t ptr = &buffer[0];
			for(int x = 0; x < width; x++)
			{
				*ptr++ = pixel_rgb8_ptr[offset + x].el.b;
				*ptr++ = pixel_rgb8_ptr[offset + x].el.g;
				*ptr++ = pixel_rgb8_ptr[offset + x].el.r;
			}
			WriteData(hFile, &buffer[0], buffer.size(), IR_WRITEERROR);
			offset -= width;

			// �R�[���o�b�N
			if(callback(y, height, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}

	return image_offset;
}

END_NAMESPACE()
