// bmp_decode.cpp
#include "../../stdafx.h"
#include "../iotemplate.h"              // ReadData()
#include "bmp_decode.h"

#undef max
#undef min
#include <valarray>                     // std::valarray<>


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// �f�R�[�h�֐�
	void bmp_decode_image_index(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data);
	void bmp_decode_image_rgb5 (HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data);
	void bmp_decode_image_rgb8 (HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data);

//	void UnRGB  (const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, const BOOL bottomup, const int bits_per_pixel, callback_t callback, long callback_data);
	void UnIndex(const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data);
	void UnRLE8 (const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data);
	void UnRLE4 (const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data);

	// �������藧���Ȃ���Η�O���X���[
	inline void throw_if_not(BOOL expression)
	{if(!expression) {throw IR_INCORRECTHEADER;}}
END_NAMESPACE()


// �w�b�_����ǂݍ���
void bmp_decode_header(HANDLE hFile, IMAGE_INFO *info_ptr, BMPDECODEDATA &data)
{
	BITMAPFILEHEADER &fileheader = data.fileheader;
	BITMAPINFOHEADER &infoheader = data.infoheader;

	// �t�@�C���w�b�_�A���w�b�_��ǂ�
	ReadData(hFile, &fileheader, 1, IR_INCORRECTHEADER);
	ReadData(hFile, &infoheader, 1, IR_INCORRECTHEADER);

	// �w�b�_�̃`�F�b�N
	throw_if_not(fileheader.bfType == BMP_TYPE);               // �擪��"BM"�łȂ���΃G���[
	throw_if_not(fileheader.bfOffBits <= fileheader.bfSize);  // �C���[�W�f�[�^�ʒu���T�C�Y�̕����傫��������G���[
	throw_if_not(fileheader.bfReserved1 == 0);                 // �\��0�łȂ���΃G���[
	throw_if_not(fileheader.bfReserved2 == 0);                 // �V
	throw_if_not(infoheader.biSize == sizeof(BITMAPINFOHEADER));  // �\���̂̃T�C�Y������Ă�����G���[
	throw_if_not(infoheader.biWidth > 0);                      // �r�b�g�}�b�v�̕����O�ȉ��Ȃ�G���[
	throw_if_not(infoheader.biHeight != 0);                    // �r�b�g�}�b�v�̍������O�Ȃ�G���[�i���̏ꍇ�̓g�b�v�_�E���^�r�b�g�}�b�v�j
	throw_if_not(!(infoheader.biCompression == BI_RLE8 && infoheader.biBitCount != 8)); // ���k�`����8bit�A�����k�ŁA�r�b�g����8�łȂ���΃G���[
	throw_if_not(infoheader.biPlanes == 1);                    // biPlanes��1�łȂ���΃G���[

	// IMAGE_INFO�\���̂̐ݒ�
	info_ptr->width  = static_cast<uint16_t>(infoheader.biWidth);
	info_ptr->height = static_cast<uint16_t>(abs(infoheader.biHeight));
	if(infoheader.biBitCount > 8)
	{
		info_ptr->depth = 8;
		info_ptr->mode  = COLORMODE_RGB;
	}
	else
	{
		info_ptr->depth = infoheader.biBitCount;
		info_ptr->mode  = COLORMODE_INDEX;
	}
	info_ptr->alpha      = FALSE;
	info_ptr->colors     = infoheader.biClrUsed;
	info_ptr->density_x  = static_cast<uint16_t>(infoheader.biXPelsPerMeter);
	info_ptr->density_y  = static_cast<uint16_t>(infoheader.biYPelsPerMeter);
	info_ptr->gamma_file = GAMMA_DEFAULT_FILE;
	info_ptr->comment    = NULL;
}

// �C���[�W����ǂݍ���
void bmp_decode_image(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data)
{
	switch(data.infoheader.biBitCount)
	{
	case 24:
		bmp_decode_image_rgb8(hFile, info_ptr, data_ptr, data, callback, callback_data);
		break;

	case 16:
		bmp_decode_image_rgb5(hFile, info_ptr, data_ptr, data, callback, callback_data);
		break;

	case 8:
	case 4:
	case 1:
		bmp_decode_image_index(hFile, info_ptr, data_ptr, data, callback, callback_data);
		break;

	default:
		throw IR_INCORRECTHEADER;
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

void bmp_decode_image_rgb8(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data)
{
	try
	{
		const_uint32_t width  = info_ptr->width;
		const_uint32_t height = info_ptr->height;
		const_uint_t   depth  = info_ptr->depth;
		const_uint32_t hsize     = CALC_HSIZE(width, 24);
		const_uint32_t hsize_BMP = CALC_BOUND(hsize, BOUND_BMP);

		// �������̈�����b�N
		pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
		auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);


		////////////////////////////////////////
		// �C���[�W����ǂݍ���
		std::valarray<uint8_t> buffer(hsize_BMP);

		// �C���[�W�f�[�^�̐擪�ֈړ�
		::SetFilePointer(hFile, data.fileheader.bfOffBits, NULL, FILE_BEGIN);

		uint32_t offset = width * (height - 1);
		for(uint32_t y = 0; y < height; y++)
		{
			// �摜�������珇�ɓǂݍ��ށi�g�b�v�_�E���^�Ȃ�ォ�珇�j
			ReadData(hFile, &buffer[0], buffer.size(), IR_IMAGEDESTROYED);

			uint8_ptr_t ptr = &buffer[0];
			for(uint32_t x = 0; x < width; x++)
			{
				pixel_rgb8_ptr[offset + x].el.b = *ptr++;
				pixel_rgb8_ptr[offset + x].el.g = *ptr++;
				pixel_rgb8_ptr[offset + x].el.r = *ptr++;
				pixel_rgb8_ptr[offset + x].el.a = 0xff;
			}
			offset -= width;

			if(callback(y, height - 1, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}

	// ��O����
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

void bmp_decode_image_rgb5(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data)
{
	try
	{
		const_uint32_t width  = info_ptr->width;
		const_uint32_t height = info_ptr->height;
		const_uint_t   depth  = info_ptr->depth;
		const_uint32_t hsize  = CALC_BOUND(width, BOUND_BMP);

		// �������̈�����b�N
		pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
		auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);


		////////////////////////////////////////
		// �C���[�W����ǂݍ���
		std::valarray<uint16_t> buffer(hsize);

		// �C���[�W�f�[�^�̐擪�ֈړ�
		::SetFilePointer(hFile, data.fileheader.bfOffBits, NULL, FILE_BEGIN);

		uint32_t offset = width * (height - 1);
		for(uint32_t y = 0; y < height; y++)
		{
			// �摜�������珇�ɓǂݍ��ށi�g�b�v�_�E���^�Ȃ�ォ�珇�j
			ReadData(hFile, &buffer[0], buffer.size(), IR_IMAGEDESTROYED);
			for(uint32_t x = 0; x < width; x++)
			{
				const bitmap_rgb5_t bitmap_rgb5 = {buffer[x]};
				pixel_rgb8_ptr[offset + x].el.b = SCALEUP(bitmap_rgb5.el.b, 5, 8);
				pixel_rgb8_ptr[offset + x].el.g = SCALEUP(bitmap_rgb5.el.g, 5, 8);
				pixel_rgb8_ptr[offset + x].el.r = SCALEUP(bitmap_rgb5.el.r, 5, 8);
				pixel_rgb8_ptr[offset + x].el.a = 0xff;
			}
			offset -= width;

			if(callback(y, height - 1, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}

	// ��O����
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

void bmp_decode_image_index(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, BMPDECODEDATA &data, callback_t callback, long callback_data)
{
	try
	{
		const uint32_t width  = info_ptr->width;
		const uint32_t height = info_ptr->height;
		const uint_t   depth  = info_ptr->depth;

		// �������̈�����b�N
		uint8_ptr_t    index_ptr    = NULL;
		palette8_ptr_t palette8_ptr = NULL;
		auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr);

		////////////////////////////////////////
		// �J���[�e�[�u����ǂݍ���
		if(TRUE)
		{
			const uint_t palette_size = 1 << depth;
			std::valarray<RGBQUAD> palette_buffer(palette_size);
			ReadData(hFile, &palette_buffer[0], palette_buffer.size(), IR_IMAGEDESTROYED);

			// palette�Ɉړ�
			for(uint_t i = 0; i < palette_size; i++)
			{
				palette8_ptr[i].r = palette_buffer[i].rgbRed;
				palette8_ptr[i].g = palette_buffer[i].rgbGreen;
				palette8_ptr[i].b = palette_buffer[i].rgbBlue;
				palette8_ptr[i].a = 0xff;
			}
		}

		////////////////////////////////////////
		// �C���[�W����ǂݍ���

		// �C���[�W�f�[�^�̐擪�ֈړ�
		::SetFilePointer(hFile, data.fileheader.bfOffBits, NULL, FILE_BEGIN);

		UnIndex(info_ptr, data_ptr, hFile, callback, callback_data);
	}

	// ��O����
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

// �C���f�b�N�X�i�[�`��������
void UnIndex(const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data)
{
	const_uint32_t width  = info_ptr->width;
	const_uint32_t height = info_ptr->height;
	const_uint_t   depth  = info_ptr->depth;

	const_uint32_t hsize     = CALC_HSIZE(width, depth);
	const_uint32_t hsize_BMP = CALC_BOUND(hsize, BOUND_BMP);

	std::valarray<uint8_t> buffer(hsize_BMP);

	uint8_ptr_t          index_ptr    = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr);

	uint32_t offset = hsize * (height - 1);
	for(uint32_t y = 0; y < height; y++)
	{
		// �摜�������珇�ɓǂݍ��ށi�g�b�v�_�E���^�Ȃ�ォ�珇�j
		ReadData(hFile, &buffer[0], buffer.size(), IR_IMAGEDESTROYED);
		memcpy(&index_ptr[offset], &buffer[0], hsize);
		offset -= hsize;

		if(callback(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}
/*
// 8bit�A�����k������
void UnRLE8(const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data)
{
	const uint32_t width  = info_ptr->width;
	const uint32_t height = info_ptr->height;

	uint32_t nX = 0;                        // x���W
	uint32_t nY = 0;                        // y���W
	uint32_t nOffset = (height - 1) * width;// �I�t�Z�b�g

	while(true)
	{
		// �����̒���
		const_uint8_t nRun = fgetc(fp);

		// �����̒������P�ȏ�
		if(nRun != 0x00)
		{
			const_uint8_t nData = fgetc(fp);               // �f�[�^

			memset(&image[nOffset], nData, nRun);  // nData��nRun�o�C�g�A��
			nOffset += nRun;
			nX      += nRun;
		}
		// �����̒������O�i�R�[�h�����[�h�j
		else
		{
			const_uint8_t nCode = fgetc(fp);

			switch(nCode)
			{
			case 0x00:                              // �s�̏I�[
				// ���s���ăI�t�Z�b�g���X�V
				nX = 0;
				nY++;
				nOffset = (height - nY - 1) * width;
				break;

			case 0x01:                              // �C���[�W�̏I�[
				return;                                 // �W�J���I��

			case 0x02:                              // �C���[�W�̈ړ�
				{
					const_uint8_t nPosX = fgetc(fp);
					const_uint8_t nPosY = fgetc(fp);

					// ���݈ʒu����(nPosX, nPosY)���ꂽ�ʒu����C���[�W���ĊJ
					nX += nPosX;
					nY += nPosY;
					nOffset = (height - nY - 1) * width + nX;
				}
				break;

			default:                                // �R�[�h����΃��[�h
				{
					UINT nLength = CALC_BOUND(nCode, 2);    // �ǂݍ��ޒ����i���[�h���E�j
					uint8_t lpBuffer[256];                   // �o�b�t�@

					// �C���[�W���i�[
					if(fread(lpBuffer, sizeof(uint8_t), nLength, fp) < nLength)
					{
						throw IR_IMAGEDESTROYED;
					}
					memcpy(&image[nOffset], lpBuffer, nCode);

					// �I�t�Z�b�g�����W���X�V
					nOffset += nCode;
					nX      += nCode;
				}
			}
		}
	}
}

// 4bit�A�����k������
void UnRLE4(const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data)
{
}
*/

END_NAMESPACE()
