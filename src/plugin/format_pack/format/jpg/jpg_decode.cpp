// jpg_decode.cpp
#include "../../stdafx.h"
#include "jpg_decode.h"

#undef max
#undef min
#include <valarray>                     // std::valarray<>


// �w�b�_��ǂݍ���
void jpg_decode_header(jpeg_decompress_struct *cinfo, IMAGE_INFO *info_ptr)
{
	// IMAGE_INFO�\���̂̐ݒ�
	info_ptr->width      = cinfo->output_width;
	info_ptr->height     = cinfo->output_height;
	info_ptr->depth      = 8;
	info_ptr->mode       = (cinfo->output_components == 1) ? COLORMODE_GRAYSCALE : COLORMODE_RGB;
	info_ptr->alpha      = FALSE;
	info_ptr->gamma_file = GAMMA_DEFAULT_FILE;  // cinfo->output_gamma;
	info_ptr->density_x  = 0;
	info_ptr->density_y  = 0;
	info_ptr->comment    = NULL;
}

// �C���[�W�������
void jpg_decode_image_gray8(jpeg_decompress_struct *cinfo, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	try
	{
		const uint32_t width  = cinfo->output_width;
		const uint32_t height = cinfo->output_height;
		const uint32_t hsize  = CALC_HSIZE(width, 8);

		// �������̈�����b�N
		pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
		auto_lock lock_obj(data_ptr, pixel_gray8_ptr);

		// ���ׂẴ��C���ɂ��ăX�L��������
		std::valarray<JSAMPLE> image(hsize);
		while(cinfo->output_scanline < height)
		{
			JSAMPROW scanlines = &image[0];
			jpeg_read_scanlines(cinfo, &scanlines, 1);

			for(uint32_t x = 0; x < width; x++)
			{
				pixel_gray8_ptr->el.gray  = *scanlines++;
				pixel_gray8_ptr++;
			}

			// �R�[���o�b�N
			if(callback(cinfo->output_scanline, cinfo->output_height, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}

	// std::bad_alloc�̏���
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

// �C���[�W�������
void jpg_decode_image_rgb8(jpeg_decompress_struct *cinfo, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	try
	{
		const uint32_t width  = cinfo->output_width;
		const uint32_t height = cinfo->output_height;
		const uint32_t hsize  = CALC_HSIZE(width, 24);

		// �������̈�����b�N
		pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
		auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

		// ���ׂẴ��C���ɂ��ăX�L��������
		std::valarray<JSAMPLE> image(hsize);
		while(cinfo->output_scanline < height)
		{
			JSAMPROW scanlines = &image[0];
			jpeg_read_scanlines(cinfo, &scanlines, 1);

			for(uint32_t x = 0; x < width; x++)
			{
				pixel_rgb8_ptr->el.r = *scanlines++;
				pixel_rgb8_ptr->el.g = *scanlines++;
				pixel_rgb8_ptr->el.b = *scanlines++;
				pixel_rgb8_ptr->el.a = 0xff;
				pixel_rgb8_ptr++;
			}

			// �R�[���o�b�N
			if(callback(cinfo->output_scanline, cinfo->output_height, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}

	// std::bad_alloc�̏���
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}
