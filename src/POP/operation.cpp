// operation.cpp �c�C���v�������e�[�V�����t�@�C��
#include "stdafx.h"
#include "operation.h"
#include "callback.h"                   // callback_t
#include "quant.h"                      // quantize()

#include <assert.h>                     // assert()
#include <memory.h>                     // memset(), memmove(), NULL

#undef max
#undef min
#include <algorithm>                    // std::fill_n(), std::copy(), std::swap()
#include <map>                          // std::map<>
#include <valarray>                     // std::valarray<>


// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	uint32_t _image_count_colors_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr);
	uint32_t _image_count_colors_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr);
	uint32_t _image_count_colors_rgb8 (const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr);

	void _image_convert_to_grayscale_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_convert_to_grayscale_rgb8 (const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);

	void _image_convert_to_index_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_convert_to_index_rgb8 (const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);

	void _image_convert_to_rgb_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_convert_to_rgb_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// �A���t�@�`���l����ǉ�
	void _image_alpha_append_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_alpha_append_index(                                   const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_alpha_append_rgb8 (const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// �A���t�@�`���l�������̒l�Ŗ��߂�
	void _image_alpha_fill_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data);
	void _image_alpha_fill_index(                                   const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data);
	void _image_alpha_fill_rgb8 (const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data);


	// �A���t�@�l�𔽓]
	void _image_alpha_inverse_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_alpha_inverse_index(                                   const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_alpha_inverse_rgb8 (const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// �F�̓���ւ�
	void _image_swap_colors_index(                                   const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const int red, const int green, const int blue, callback_t callback_func, long callback_data);
	void _image_swap_colors_rgb8 (const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const int red, const int green, const int blue, callback_t callback_func, long callback_data);


	// �t���b�v�i�㉺���]�j
	void _image_flip_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_flip_index(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_flip_rgb8 (const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// �~���[�i���E���]�j
	void image_mirror_gray8(const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void image_mirror_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void image_mirror_rgb8 (const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// 90����]
	void _image_turn_90_gray8(const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_90_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_90_rgb8 (const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// 180����]
	void _image_turn_180_gray8(const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_180_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_180_rgb8 (const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// 270����]
	void _image_turn_270_gray8(const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_270_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);
	void _image_turn_270_rgb8 (const int width, const int height,                     const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data);


	// �T�C�Y�ύX
	void _image_resize_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data);
	void _image_resize_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data);
	void _image_resize_rgb8 (const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data);


	// �g���~���O
	void _image_trimming_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data);
	void _image_trimming_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data);
	void _image_trimming_rgb8 (const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data);
END_NAMESPACE()


// �󔒂̃C���[�W���쐬
EXTERN_C
IMAGERESULT image_create_empty(IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, const NEWIMAGEPARAM *param)
{
	// IMAGE_INFO�\���̂̐ݒ�
	info_ptr->width      = param->width;
	info_ptr->height     = param->height;
	info_ptr->depth      = 8;
	info_ptr->mode       = COLORMODE_RGB;
	info_ptr->alpha      = FALSE;
	info_ptr->colors     = 1;
	info_ptr->density_x  = 0;
	info_ptr->density_y  = 0;
	info_ptr->gamma_file = GAMMA_DEFAULT_FILE;
	info_ptr->comment    = NULL;

	// ���������m��
	if(!image_alloc(info_ptr, data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �������̈�����b�N
	pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	uint32_t pixel_count = 0;
	auto_lock lock_obj(data_ptr, pixel_rgb8_ptr, &pixel_count);

	// �w��F�Ŗ��߂�
	std::fill_n(pixel_rgb8_ptr, pixel_count, param->rgb);

	return IR_SUCCESS;
}

// �F���𐔂���
EXTERN_C
uint32_t image_count_colors(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr)
{
	switch(info_ptr->mode)
	{
	case COLORMODE_GRAYSCALE:
		return _image_count_colors_gray8(info_ptr, data_ptr);

	case COLORMODE_INDEX:
		return _image_count_colors_index(info_ptr, data_ptr);

	case COLORMODE_RGB:
		return _image_count_colors_rgb8(info_ptr, data_ptr);

	default:
		return 0;
	}
}


////////////////////////////////////////////////////////////////////////////////
// �J���[���[�h�̕ϊ�

EXTERN_C
IMAGERESULT image_convert_colormode(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int new_mode, callback_t callback_func, long callback_data)
{
	assert(in_info_ptr->mode != new_mode);

	*out_info_ptr = *in_info_ptr;
	out_info_ptr->mode  = new_mode;
	out_info_ptr->depth = 8;

	// ���������m��
	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	IMAGERESULT result;
	IMAGE_INFO tmp_info;
	IMAGE_DATA tmp_data = {0, 0, NULL, NULL};

	try
	{
		switch(new_mode)
		{
		case COLORMODE_GRAYSCALE:
			switch(in_info_ptr->mode)
			{
			case COLORMODE_INDEX:
				_image_convert_to_grayscale_index(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			case COLORMODE_RGB:
				_image_convert_to_grayscale_rgb8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			default:
				throw IR_UNKNOWN;
			}
			break;

		case COLORMODE_INDEX:
			switch(in_info_ptr->mode)
			{
			case COLORMODE_GRAYSCALE:
				_image_convert_to_index_gray8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			case COLORMODE_RGB:
				// ���F���Ă���ϊ�
				result = quantize(in_info_ptr, in_data_ptr, &tmp_info, &tmp_data, 1 << in_info_ptr->depth, QPM_FREQ, QDM_NONE, NULL, 0);
				if(result != IR_SUCCESS)
				{
					throw result;
				}
				_image_convert_to_index_rgb8(&tmp_info, &tmp_data, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			default:
				throw IR_UNKNOWN;
			}
			break;

		case COLORMODE_RGB:
			switch(in_info_ptr->mode)
			{
			case COLORMODE_GRAYSCALE:
				_image_convert_to_rgb_gray8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			case COLORMODE_INDEX:
				_image_convert_to_rgb_index(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, callback_func, callback_data);
				break;

			default:
				throw IR_UNKNOWN;
			}
		}
		out_info_ptr->colors = image_count_colors(out_info_ptr, out_data_ptr);
		image_free(&tmp_data);
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(&tmp_data);
		image_free(out_data_ptr);
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// �A���t�@�`���l���̏���

// �A���t�@�`���l����ǉ�/�폜
EXTERN_C
IMAGERESULT image_alpha_append(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const BOOL append, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	// �A���t�@�`���l���t���O�̐ݒ�
	*out_info_ptr = *in_info_ptr;
	out_info_ptr->alpha = !in_info_ptr->alpha;

	// ���������m��
	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_alpha_append_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_alpha_append_index(in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_alpha_append_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// �A���t�@�`���l�������̒l�Ŗ��߂�
EXTERN_C
IMAGERESULT image_alpha_fill(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	assert(info_ptr->alpha);

	// ���������m��
	if(!image_alloc(info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_alpha_fill_gray8(width, height, in_data_ptr, out_data_ptr, alpha_val, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_alpha_fill_index(in_data_ptr, out_data_ptr, alpha_val, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_alpha_fill_rgb8(width, height, in_data_ptr, out_data_ptr, alpha_val, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// �A���t�@�l�̔��]
EXTERN_C
IMAGERESULT image_alpha_inverse(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	assert(info_ptr->alpha);

	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// ���������m��
	if(!image_alloc(info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		switch(info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_alpha_inverse_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_alpha_inverse_index(in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_alpha_inverse_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// �F�̓���ւ�

EXTERN_C
IMAGERESULT image_swap_colors(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const int red, const int green, const int blue, callback_t callback_func, long callback_data)
{
	assert(info_ptr->mode != COLORMODE_GRAYSCALE);

	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// ���������m��
	if(!image_alloc(info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		switch(info_ptr->mode)
		{
		case COLORMODE_INDEX:
			_image_swap_colors_index(in_data_ptr, out_data_ptr, red, green, blue, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_swap_colors_rgb8(width, height, in_data_ptr, out_data_ptr, red, green, blue, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// �摜�̉�]�E���]

// �㉺���]�i�t���b�v�j
EXTERN_C
IMAGERESULT image_flip(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	*out_info_ptr = *in_info_ptr;

	// ���������m��
	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_flip_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_flip_index(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_flip_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// ���E���]�i�~���[�j
EXTERN_C
IMAGERESULT image_mirror(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	*out_info_ptr = *in_info_ptr;

	// ���������m��
	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			image_mirror_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			image_mirror_index(width, height, out_info_ptr->depth, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			image_mirror_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// 90����]
EXTERN_C
IMAGERESULT image_turn_90(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// ���ƍ��������ւ�
	const int width  = in_info_ptr->height;
	const int height = in_info_ptr->width;

	*out_info_ptr = *in_info_ptr;
	out_info_ptr->width  = width;
	out_info_ptr->height = height;

	// ���������m��
	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_turn_90_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_turn_90_index(width, height, out_info_ptr->depth, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_turn_90_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// 180����]
EXTERN_C
IMAGERESULT image_turn_180(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	*out_info_ptr = *in_info_ptr;

	// ���������m��
	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_turn_180_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_turn_180_index(width, height, out_info_ptr->depth, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_turn_180_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}

// 270����]
EXTERN_C
IMAGERESULT image_turn_270(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// ���ƍ��������ւ�
	const int width  = in_info_ptr->height;
	const int height = in_info_ptr->width;

	*out_info_ptr = *in_info_ptr;
	out_info_ptr->width  = width;
	out_info_ptr->height = height;

	// ���������m��
	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_turn_270_gray8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_turn_270_index(width, height, out_info_ptr->depth, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_turn_270_rgb8(width, height, in_data_ptr, out_data_ptr, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// �摜�T�C�Y�̕ύX
EXTERN_C
IMAGERESULT image_resize(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data)
{
	const int new_width  = param->new_width;
	const int new_height = param->new_height;

	// �摜�̖ʐς�0�̏ꍇ
	if(new_width * new_height == 0)
	{
		return IR_ILLEGALPARAM;
	}

	*out_info_ptr = *in_info_ptr;
	out_info_ptr->width  = new_width;
	out_info_ptr->height = new_height;

	// ���������m��
	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_resize_gray8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, param, callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_resize_index(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, param, callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_resize_rgb8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr, param, callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		out_info_ptr->colors = image_count_colors(out_info_ptr, out_data_ptr);
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// �摜�T�C�Y�̕ύX
EXTERN_C
IMAGERESULT image_trimming(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const TRIMMINGPARAM *param, callback_t callback_func, long callback_data)
{
	// �V�����摜�̕��A����
	const int new_width  =
		in_info_ptr->width +
		(param->margin_left - param->clip_left) +
		(param->margin_right - param->clip_right);
	const int new_height =
		in_info_ptr->height +
		(param->margin_top - param->clip_top) +
		(param->margin_bottom - param->clip_bottom);

	// �V�����摜�̖ʐς�0�̏ꍇ
	if(new_width * new_height == 0)
	{
		return IR_ILLEGALPARAM;
	}

	*out_info_ptr = *in_info_ptr;
	out_info_ptr->width  = new_width;
	out_info_ptr->height = new_height;

	// ���������m��
	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback_func == NULL)
	{
		callback_func = callback_none;
	}

	try
	{
		// �J�n�ʒu�A�I���ʒu
		const int x_begin = param->margin_left;
		const int x_end   = new_width - param->margin_right;
		const int y_begin = param->margin_top;
		const int y_end   = new_height - param->margin_bottom;

		switch(out_info_ptr->mode)
		{
		case COLORMODE_GRAYSCALE:
			_image_trimming_gray8(
				in_info_ptr, in_data_ptr,
				out_info_ptr, out_data_ptr,
				param->clip_left, param->clip_top,
				x_begin, x_end,
				y_begin, y_end,
				callback_func, callback_data);
			break;

		case COLORMODE_INDEX:
			_image_trimming_index(
				in_info_ptr, in_data_ptr,
				out_info_ptr, out_data_ptr,
				param->clip_left, param->clip_top,
				x_begin, x_end,
				y_begin, y_end,
				callback_func, callback_data);
			break;

		case COLORMODE_RGB:
			_image_trimming_rgb8(
				in_info_ptr, in_data_ptr,
				out_info_ptr, out_data_ptr,
				param->clip_left, param->clip_top,
				x_begin, x_end,
				y_begin, y_end,
				callback_func, callback_data);
			break;

		default:
			throw IR_UNKNOWN;
		}
		out_info_ptr->colors = image_count_colors(out_info_ptr, out_data_ptr);
		return IR_SUCCESS;
	}
	catch(IMAGERESULT result)
	{
		image_free(out_data_ptr);
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// �F���𐔂���i�O���C�X�P�[��8bit�ȉ��j
uint32_t _image_count_colors_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;
	const_uint_t depth = info_ptr->depth;

	// �t���O�̈�
	BOOL flags[256];
	std::fill_n(flags, SIZE_OF_ARRAY(flags), FALSE);

	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_gray8_ptr);

	// ���ׂẲ�f�ɑ΂��Ē��ׂ�
	for(int xy = 0; xy < width * height; xy++)
	{
		// �O���C�X�P�[���l���擾
		const_uint32_t color = pixel_gray8_ptr[xy].el.gray;

		// �t���O�𗧂Ă�
		flags[color] = TRUE;
	}

	// �����Ă���t���O�̐��𐔂���
	int color_count = 0;
	for(int i = 0; i < SIZE_OF_ARRAY(flags); i++)
	{
		color_count += flags[i];
	}
	return color_count;
}

// �F���𐔂���i�C���f�b�N�X�J���[�j
uint32_t _image_count_colors_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;
	const_uint_t   depth = info_ptr->depth;
	const_uint32_t hsize = CALC_HSIZE(width, depth);

	// log2(depth)���v�Z
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// �t���O�̈�
	BOOL flags[256];
	std::fill_n(flags, SIZE_OF_ARRAY(flags), FALSE);

	// �������̈�����b�N
	const_uint8_ptr_t    index_ptr = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr);

	// ���ׂẲ�f�ɑ΂��Ē��ׂ�
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// �I�t�Z�b�g�ƃV�t�g�ʂ����߂�
			const_uint32_t offset = CALC_PACK_OFFSET(x, log2_depth);
			const int      shifts = CALC_PACK_SHIFTS(x, log2_depth);

			// �C���f�b�N�X�l���擾
			const_uint8_t index = PACK_EXTRACT(index_ptr[offset], depth, shifts);

			// �t���O�𗧂Ă�
			flags[index] = TRUE;
		}
		index_ptr += hsize;
	}

	// �����Ă���t���O�̐��𐔂���
	int color_count = 0;
	for(int i = 0; i < SIZE_OF_ARRAY(flags); i++)
	{
		color_count += flags[i];
	}
	return color_count;
}

// �F���𐔂���iRGB8bit�j
uint32_t _image_count_colors_rgb8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// �t���O�̈�i2MByte���g���c�B�܁A�ŋ߂̃}�V���Ȃ���v���j
	std::valarray<uint8_t> flags((uint8_t)0, 0x01000000 / 8);

	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

	// ���ׂẲ�f�ɑ΂��Ē��ׂ�
	uint32_t color_count = 0;
	for(int xy = 0; xy < width * height; xy++)
	{
		const_uint32_t color = pixel_rgb8_ptr[xy].value & 0x00ffffff;

		const_uint32_t offset = color >> 3;
		const_uint8_t  mask   = 0x80 >> (color & 7);
		if((flags[offset] & mask) == 0)
		{
			flags[offset] |= mask;
			color_count++;
		}
	}

	return color_count;
}


// �O���C�X�P�[���ɕϊ��i�C���f�b�N�X�J���[����j
void _image_convert_to_grayscale_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;
	const_uint_t   depth_index = in_info_ptr->depth;
	const_uint32_t hsize_index = CALC_HSIZE(width, depth_index);

	// log2(depth_index)���v�Z
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth_index = log2_depth_array[depth_index - 1];

	// �X�P�[���A�b�v�e�[�u�����쐬
	std::valarray<uint8_t> scaleup_table(1 << depth_index);
	create_scaleup_table(&scaleup_table[0], depth_index);

	// �������̈�����b�N
	const_uint8_ptr_t index8_ptr = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, index8_ptr, palette8_ptr);

	pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_out(out_data_ptr, pixel_gray8_ptr);

	// �O���C�X�P�[���ɕϊ�
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// ���̓I�t�Z�b�g�ƃV�t�g�ʂ����߂�
			const_uint32_t offset_index = CALC_PACK_OFFSET(x, log2_depth_index);
			const int      shifts_index = CALC_PACK_SHIFTS(x, log2_depth_index);

			// �C���f�b�N�X����O���C�X�P�[���l�����߂�
			const_uint8_t index = PACK_EXTRACT(index8_ptr[offset_index], depth_index, shifts_index);
			const_uint8_t gray  = GRAYSCALE(palette8_ptr[index].r, palette8_ptr[index].g, palette8_ptr[index].b);
			const_uint8_t alpha = palette8_ptr[index].a;

			// �O���C�X�P�[���l��0�`255�ɃX�P�[���A�b�v���Ċi�[
			pixel_gray8_ptr->el.gray  = scaleup_table[gray];
			pixel_gray8_ptr->el.alpha = scaleup_table[alpha];
			pixel_gray8_ptr++;
		}
		index8_ptr += hsize_index;

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

// �O���C�X�P�[���ɕϊ��iRGB8bit����j
void _image_convert_to_grayscale_rgb8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;

	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr);

	pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_out(out_data_ptr, pixel_gray8_ptr);

	// �O���C�X�P�[���ɕϊ�
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// �O���C�X�P�[���l�����߂�
			pixel_gray8_ptr->el.gray  = GRAYSCALE(pixel_rgb8_ptr->el.r, pixel_rgb8_ptr->el.g, pixel_rgb8_ptr->el.b);
			pixel_gray8_ptr->el.alpha = pixel_rgb8_ptr->el.a;

			pixel_rgb8_ptr++;
			pixel_gray8_ptr++;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


// �C���f�b�N�X�J���[�ɕϊ��i�O���C�X�P�[������j
void _image_convert_to_index_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;
	const_uint_t   depth_index = in_info_ptr->depth;
	const_uint32_t hsize_index = CALC_HSIZE(width, depth_index);

	// log2(depth_index)���v�Z
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth_index = log2_depth_array[depth_index - 1];

	// �X�P�[���A�b�v�e�[�u�����쐬
	std::valarray<uint8_t> scaleup_table(1 << depth_index);
	create_scaleup_table(&scaleup_table[0], depth_index);

	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr);

	uint8_ptr_t    index8_ptr = NULL;
	palette8_ptr_t palette8_ptr = NULL;
	uint32_t       index_size = 0;
	auto_lock lock_out(out_data_ptr, index8_ptr, palette8_ptr, &index_size);

	memset(index8_ptr, 0, index_size);

	// �J���[�p���b�g��ݒ�
	for(int i = 0; i < 1 << depth_index; i++)
	{
		palette8_ptr[i].r = palette8_ptr[i].g = palette8_ptr[i].b = scaleup_table[i];
		palette8_ptr[i].a = 255;
	}

	// �C���f�b�N�X�J���[�ɕϊ�
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// �O���C�X�P�[���l�����߂�
			const_uint8_t gray  = pixel_gray8_ptr->el.gray;
			const_uint8_t alpha = pixel_gray8_ptr->el.alpha;

			// �o�̓I�t�Z�b�g�ƃV�t�g�ʂ����߂�
			const_uint32_t offset_index = CALC_PACK_OFFSET(x, log2_depth_index);
			const int      shifts_index = CALC_PACK_SHIFTS(x, log2_depth_index);

			const_uint8_t index = gray >> (8 - depth_index);
			PACK_STORE(index8_ptr[offset_index], index, shifts_index);

			pixel_gray8_ptr++;
		}
		index8_ptr += hsize_index;

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

// �C���f�b�N�X�J���[�ɕϊ��iRGB8bit����j
void _image_convert_to_index_rgb8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;
	const_uint_t   depth_index = out_info_ptr->depth;
	const_uint32_t hsize_index = CALC_HSIZE(width, depth_index);

	// log2(depth)���v�Z
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth_index = log2_depth_array[depth_index - 1];

	// �o�^�\�F��
	const_uint_t max_colors = 1 << depth_index;
	assert(in_info_ptr->colors <= max_colors);

	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr);

	uint8_ptr_t    index8_ptr = NULL;
	palette8_ptr_t palette8_ptr = NULL;
	uint32_t       index_size = 0;
	auto_lock lock_out(out_data_ptr, index8_ptr, palette8_ptr, &index_size);

	// �C���[�W�����ׂ�0�Ŗ��߂�
	memset(index8_ptr, 0, index_size);

	uint_t registered_color_num = 0;        // �o�^�ςݐF��
	uint_t index                = 0;        // �J���[�C���f�b�N�X

	// �J���[�}�b�v�i�C���f�b�N�X����Ή�����COLORREF�l�����߂�j
	std::valarray<COLORREF> colmap(max_colors);
	colmap[0] = 0xffffffffUL;

	// �t�J���[�}�b�v�iCOLORREF�l����Ή�����C���f�b�N�X�����߂�j�i�n�b�V���{�A�z�z��j
	typedef std::map<COLORREF, int> invmap_t;
	std::valarray<invmap_t> invmap(MAX_COLOR_HASH + 1);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// RGB�̋P�x���擾
			const_uint8_t r = pixel_rgb8_ptr->el.r;
			const_uint8_t g = pixel_rgb8_ptr->el.g;
			const_uint8_t b = pixel_rgb8_ptr->el.b;
			const_uint32_t rgb = pixel_rgb8_ptr->value & 0x00ffffff;
			pixel_rgb8_ptr++;

			// �C���f�b�N�X�����i���ԒZ�k�̂��߁A�P�O�̐F�ƈقȂ�Ƃ��̂݌����j
			if(colmap[index] != rgb)
			{
				// �P�x���Ή�����J���[�}�b�v�̃C���f�b�N�X������
				const int hash_val = HASH_COLOR_REF(rgb);
				invmap_t::iterator it = invmap[hash_val].find(rgb);

				// �Ή�����J���[�}�b�v�����݂��Ȃ��ꍇ�A�V�K�o�^
				if(it == invmap[hash_val].end())
				{
					// �o�^�\�F���𒴂���ƃA�T�[�V�����G���[
					assert(registered_color_num < max_colors);

					index = registered_color_num++;

					// �J���[�}�b�v�C���f�b�N�X��o�^
					invmap[hash_val][rgb] = index;
					colmap[index] = rgb;

					// �J���[�p���b�g��o�^
					palette8_ptr[index].r = r;
					palette8_ptr[index].g = g;
					palette8_ptr[index].b = b;
					palette8_ptr[index].a = 0xff;
				}
				else
				{
					index = it->second;
				}
			}

			// �o�̓I�t�Z�b�g�ƃV�t�g�ʂ����߂�
			const_uint32_t offset_index = CALC_PACK_OFFSET(x, log2_depth_index);
			const int      shifts_index = CALC_PACK_SHIFTS(x, log2_depth_index);

			// �C���f�b�N�X���p�b�L���O����
			PACK_STORE(index8_ptr[offset_index], index, shifts_index);
		}
		index8_ptr += hsize_index;

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


void _image_convert_to_rgb_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;

	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr);

	pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_out(out_data_ptr, pixel_rgb8_ptr);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// �O���C�X�P�[���l�����߂�
			pixel_rgb8_ptr->el.r = pixel_rgb8_ptr->el.g = pixel_rgb8_ptr->el.b = pixel_gray8_ptr->el.gray;
			pixel_rgb8_ptr->el.a = pixel_gray8_ptr->el.alpha;

			pixel_gray8_ptr++;
			pixel_rgb8_ptr++;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_convert_to_rgb_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const int width  = out_info_ptr->width;
	const int height = out_info_ptr->height;
	const_uint_t   depth_index = in_info_ptr->depth;
	const_uint32_t hsize_index = CALC_HSIZE(width, depth_index);

	// log2(depth)���v�Z
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth_index   = log2_depth_array[depth_index - 1];

	// �������̈�����b�N
	const_uint8_ptr_t index8_ptr = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	auto_lock lock_in(in_data_ptr, index8_ptr, palette8_ptr);

	pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_out(out_data_ptr, pixel_rgb8_ptr);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			const_uint32_t offset_index = CALC_PACK_OFFSET(x, log2_depth_index);
			const int      shifts_index = CALC_PACK_SHIFTS(x, log2_depth_index);

			// �C���f�b�N�X�����߂�
			const int index = PACK_EXTRACT(index8_ptr[offset_index], depth_index, shifts_index);

			pixel_rgb8_ptr->el.r = palette8_ptr[index].r;
			pixel_rgb8_ptr->el.g = palette8_ptr[index].g;
			pixel_rgb8_ptr->el.b = palette8_ptr[index].b;
			pixel_rgb8_ptr->el.a = palette8_ptr[index].a;
			pixel_rgb8_ptr++;
		}
		index8_ptr += hsize_index;

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// �A���t�@�`���l���̒ǉ�
void _image_alpha_append_gray8(const int width, const int  height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// �A���t�@�l��ύX�i�P�x�l�͂��̂܂܁j
			pixel_gray8_ptr_out->value = pixel_gray8_ptr_in->value | 0xff00;

			pixel_gray8_ptr_in++;
			pixel_gray8_ptr_out++;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_append_index(const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size   = 0;
	uint32_t palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	memcpy(index8_ptr_out, index8_ptr_in, index_size);

	for(uint32_t i = 0; i < palette_size; i++)
	{
		palette8_ptr_out[i].r = palette8_ptr_in[i].r;
		palette8_ptr_out[i].g = palette8_ptr_in[i].g;
		palette8_ptr_out[i].b = palette8_ptr_in[i].b;
		palette8_ptr_out[i].a = 255;

		// �R�[���o�b�N
		if(callback_func(i, palette_size - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_append_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// �A���t�@�l��FFH�Ŗ��߂�i�P�x�l�͂��̂܂܁j
			pixel_rgb8_ptr_out->value = pixel_rgb8_ptr_in->value | 0xff000000;

			pixel_rgb8_ptr_in++;
			pixel_rgb8_ptr_out++;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// �A���t�@�`���l�������̒l�Ŗ��߂�
void _image_alpha_fill_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// �A���t�@�l��ύX�i�P�x�l�͂��̂܂܁j
			pixel_gray8_ptr_out->el.gray  = pixel_gray8_ptr_in->el.gray;
			pixel_gray8_ptr_out->el.alpha = alpha_val;

			pixel_gray8_ptr_in++;
			pixel_gray8_ptr_out++;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_fill_index(const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size   = 0;
	uint32_t palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	memcpy(index8_ptr_out, index8_ptr_in, index_size);

	for(uint32_t i = 0; i < palette_size; i++)
	{
		palette8_ptr_out[i].r = palette8_ptr_in[i].r;
		palette8_ptr_out[i].g = palette8_ptr_in[i].g;
		palette8_ptr_out[i].b = palette8_ptr_in[i].b;
		palette8_ptr_out[i].a = alpha_val;

		// �R�[���o�b�N
		if(callback_func(i, palette_size - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_fill_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const_uint8_t alpha_val, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// �A���t�@�l��ύX�i�P�x�l�͂��̂܂܁j
			pixel_rgb8_ptr_out->el.r = pixel_rgb8_ptr_in->el.r;
			pixel_rgb8_ptr_out->el.g = pixel_rgb8_ptr_in->el.g;
			pixel_rgb8_ptr_out->el.b = pixel_rgb8_ptr_in->el.b;
			pixel_rgb8_ptr_out->el.a = alpha_val;

			pixel_rgb8_ptr_in++;
			pixel_rgb8_ptr_out++;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// �A���t�@�l�𔽓]
void _image_alpha_inverse_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// �A���t�@�l��ύX�i�P�x�l�͂��̂܂܁j
			pixel_gray8_ptr_out->value = pixel_gray8_ptr_in->value ^ 0xff00;

			pixel_gray8_ptr_in++;
			pixel_gray8_ptr_out++;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_inverse_index(const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size   = 0;
	uint32_t palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	memcpy(index8_ptr_out, index8_ptr_in, index_size);

	for(uint32_t i = 0; i < palette_size; i++)
	{
		palette8_ptr_out[i].r = palette8_ptr_in[i].r;
		palette8_ptr_out[i].g = palette8_ptr_in[i].g;
		palette8_ptr_out[i].b = palette8_ptr_in[i].b;
		palette8_ptr_out[i].a =~palette8_ptr_in[i].a;

		// �R�[���o�b�N
		if(callback_func(i, palette_size - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_alpha_inverse_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// �A���t�@�l�𔽓]�i�P�x�l�͂��̂܂܁j
			pixel_rgb8_ptr_out->value = pixel_rgb8_ptr_in->value ^ 0xff000000;

			pixel_rgb8_ptr_in++;
			pixel_rgb8_ptr_out++;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// �F�̓���ւ�
void _image_swap_colors_index(const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const int red, const int green, const int blue, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size   = 0;
	uint32_t palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	memcpy(index8_ptr_out, index8_ptr_in, index_size);

	for(uint32_t i = 0; i < palette_size; i++)
	{
		// �F�̓���ւ�
		const uint8_t swap_pixel[] =
		{
			palette8_ptr_in[i].r,
			palette8_ptr_in[i].g,
			palette8_ptr_in[i].b,
		};
		palette8_ptr_out[i].r = swap_pixel[red];
		palette8_ptr_out[i].g = swap_pixel[green];
		palette8_ptr_out[i].b = swap_pixel[blue];
		palette8_ptr_out[i].a = palette8_ptr_in[i].a;

		// �R�[���o�b�N
		if(callback_func(i, palette_size - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_swap_colors_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, const int red, const int green, const int blue, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// �F�̓���ւ�
			const uint8_t swap_pixel[] =
			{
				pixel_rgb8_ptr_in->el.r,
				pixel_rgb8_ptr_in->el.g,
				pixel_rgb8_ptr_in->el.b,
			};
			pixel_rgb8_ptr_out->el.r = swap_pixel[red];
			pixel_rgb8_ptr_out->el.g = swap_pixel[green];
			pixel_rgb8_ptr_out->el.b = swap_pixel[blue];
			pixel_rgb8_ptr_out->el.a = pixel_rgb8_ptr_in->el.a;

			pixel_rgb8_ptr_in++;
			pixel_rgb8_ptr_out++;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// �t���b�v�i�㉺���]�j
void _image_flip_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const_uint32_t hsize = width * sizeof(pixel_gray8_ptr_t);

	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// �t���b�v����
	uint32_t offset_src = 0;
	uint32_t offset_dst = CALC_OFFSET(0, height - 1,  width);
	for(int y = 0; y < height; y++)
	{
		memcpy(&pixel_gray8_ptr_out[offset_dst], &pixel_gray8_ptr_in[offset_src], hsize);

		offset_src += width;
		offset_dst -= width;

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_flip_index(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	const_uint32_t hsize = index_size / height;

	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// �t���b�v����
	uint32_t offset_src = 0;
	uint32_t offset_dst = CALC_OFFSET(0, height - 1, hsize);
	for(int y = 0; y < height; y++)
	{
		memcpy(&index8_ptr_out[offset_dst], &index8_ptr_in[offset_src], hsize);

		offset_src += hsize;
		offset_dst -= hsize;

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_flip_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	const_uint32_t hsize = width * sizeof(pixel_rgb8_ptr_t);

	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// �t���b�v����
	uint32_t offset_src = 0;
	uint32_t offset_dst = CALC_OFFSET(0, height - 1,  width);
	for(int y = 0; y < height; y++)
	{
		memcpy(&pixel_rgb8_ptr_out[offset_dst], &pixel_rgb8_ptr_in[offset_src], hsize);

		offset_src += width;
		offset_dst -= width;

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// �~���[�i���E���]�j
void image_mirror_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// �~���[
	uint32_t offset = 0;
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (width - x - 1, y)
		for(int x = 0; x < width; x++)
		{
			pixel_gray8_ptr_out[offset + x] = pixel_gray8_ptr_in[offset + width - x - 1];
		}
		offset += width;

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void image_mirror_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// log2(depth)���v�Z
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// �������̈�����b�N
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	const_uint32_t hsize = index_size / height;

	memset(index8_ptr_out, 0, index_size);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// �~���[
	uint32_t offset = 0;
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (width - x - 1, y)
		for(int x = 0; x < width; x++)
		{
			// ���͑��̍��W(width - x - 1, y)�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset_in = CALC_PACK_OFFSET(width - x - 1, log2_depth);
			const int      shifts_in = CALC_PACK_SHIFTS(width - x - 1, log2_depth);

			// �o�͑��̍��W(x, y)�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset_out = CALC_PACK_OFFSET(x, log2_depth);
			const int      shifts_out = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset_in], depth, shifts_in);
			PACK_STORE(index8_ptr_out[offset_out], index, shifts_out);
		}
		index8_ptr_in  += hsize;
		index8_ptr_out += hsize;

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void image_mirror_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// �~���[
	uint32_t offset = 0;
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (width - x - 1, y)
		for(int x = 0; x < width; x++)
		{
			pixel_rgb8_ptr_out[offset + x] = pixel_rgb8_ptr_in[offset + width - x - 1];
		}
		offset += width;

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// 90����]
void _image_turn_90_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// 90����]
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (y, width - x - 1)
		uint32_t offset = CALC_OFFSET(y, width - 1, height);
		for(int x = 0; x < width; x++)
		{
			*pixel_gray8_ptr_out++ = pixel_gray8_ptr_in[offset];
			offset -= height;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_90_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// log2(depth)���v�Z
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// �������̈�����b�N
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	const_uint32_t hsize = index_size / height;
	const_uint32_t vsize = index_size / width;

	memset(index8_ptr_out, 0, index_size);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// 90����]
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (y, width - x - 1)
		for(int x = 0; x < width; x++)
		{
			// ���͑��̍��W(y, width - x - 1)�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset_in = CALC_PACK_OFFSET(y, log2_depth) + vsize * (width - x - 1);
			const int      shifts_in = CALC_PACK_SHIFTS(y, log2_depth);		

			// �o�͑��̍��W(x, y)�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset_out = CALC_PACK_OFFSET(x, log2_depth) + hsize * y;
			const int      shifts_out = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset_in], depth, shifts_in);
			PACK_STORE(index8_ptr_out[offset_out], index, shifts_out);
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_90_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// 90����]
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (y, width - x - 1)
		uint32_t offset = CALC_OFFSET(y, width - 1, height);
		for(int x = 0; x < width; x++)
		{
			*pixel_rgb8_ptr_out++ = pixel_rgb8_ptr_in[offset];
			offset -= height;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// 180����]
void _image_turn_180_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	uint32_t offset_in  = CALC_OFFSET(width - 1, height - 1, width);
	uint32_t offset_out = 0;

	// 180����]
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (width - x - 1, height - y - 1)
		for(int x = 0; x < width; x++)
		{
			pixel_gray8_ptr_out[offset_out] = pixel_gray8_ptr_in[offset_in];

			offset_in--;
			offset_out++;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_180_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// log2(depth)���v�Z
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// �������̈�����b�N
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	const_uint32_t hsize = index_size / height;

	memset(index8_ptr_out, 0, index_size);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// 180����]
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (width - x - 1, height - y - 1)
		for(int x = 0; x < width; x++)
		{
			// ���͑��̍��W(width - x - 1, height - y - 1)�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset_in = CALC_PACK_OFFSET(width - x - 1, log2_depth) + hsize * (height - y - 1);
			const int      shifts_in = CALC_PACK_SHIFTS(width - x - 1, log2_depth);		

			// �o�͑��̍��W(x, y)�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset_out = CALC_PACK_OFFSET(x, log2_depth) + hsize * y;
			const int      shifts_out = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset_in], depth, shifts_in);
			PACK_STORE(index8_ptr_out[offset_out], index, shifts_out);
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_180_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	uint32_t offset_in  = CALC_OFFSET(width - 1, height - 1, width);
	uint32_t offset_out = 0;

	// 180����]
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (width - x - 1, height - y - 1)
		for(int x = 0; x < width; x++)
		{
			pixel_rgb8_ptr_out[offset_out] = pixel_rgb8_ptr_in[offset_in];

			offset_in--;
			offset_out++;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// 270����]
void _image_turn_270_gray8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// 270����]
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (height - y - 1, x)
		uint32_t offset = CALC_OFFSET(height - y - 1, 0, height);
		for(int x = 0; x < width; x++)
		{
			*pixel_gray8_ptr_out++ = pixel_gray8_ptr_in[offset];
			offset += height;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_270_index(const int width, const int height, const_uint_t depth, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// log2(depth)���v�Z
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// �������̈�����b�N
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size, &palette_size);

	const_uint32_t hsize = index_size / height;
	const_uint32_t vsize = index_size / width;

	memset(index8_ptr_out, 0, index_size);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// 270����]
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (height - y - 1, x)
		for(int x = 0; x < width; x++)
		{
			// ���͑��̍��W(height - y - 1, x)�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset_in = CALC_PACK_OFFSET(height - y - 1, log2_depth) + vsize * x;
			const int      shifts_in = CALC_PACK_SHIFTS(height - y - 1, log2_depth);		

			// �o�͑��̍��W(x, y)�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset_out = CALC_PACK_OFFSET(x, log2_depth) + hsize * y;
			const int      shifts_out = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset_in], depth, shifts_in);
			PACK_STORE(index8_ptr_out[offset_out], index, shifts_out);
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_turn_270_rgb8(const int width, const int height, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback_func, long callback_data)
{
	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// 270����]
	for(int y = 0; y < height; y++)
	{
		// (x, y) �� (height - y - 1, x)
		uint32_t offset = CALC_OFFSET(height - y - 1, 0, height);
		for(int x = 0; x < width; x++)
		{
			*pixel_rgb8_ptr_out++ = pixel_rgb8_ptr_in[offset];
			offset += height;
		}

		// �R�[���o�b�N
		if(callback_func(y, height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// �T�C�Y�ύX
void _image_resize_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data)
{
	const int new_width  = param->new_width;
	const int new_height = param->new_height;
	const INTERPOLATE_METHOD interpolate_method = param->interpolate_method;

	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	uint32_t pixel_count_in = 0;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in, &pixel_count_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// �V�����C���[�W�̃I�t�Z�b�g
	uint32_t offset_new = 0;

	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	try
	{
		// 
		std::valarray<uint8_t> g_plane(pixel_count_in), a_plane(pixel_count_in);
		for(uint32_t i = 0; i < pixel_count_in; i++)
		{
			g_plane[i] = pixel_gray8_ptr_in[i].el.gray;
			a_plane[i] = pixel_gray8_ptr_in[i].el.alpha;
		}

		// �g��^�k��
		const double width_rate  = (new_width  > 1) ? (double)(width  - 1) / (new_width  - 1) : 0;
		const double height_rate = (new_height > 1) ? (double)(height - 1) / (new_height - 1) : 0;
		for(int new_y = 0; new_y < new_height; new_y++)
		{
			for(int new_x = 0; new_x < new_width; new_x++)
			{
				const double interpolate_x = new_x * width_rate;
				const double interpolate_y = new_y * height_rate;

				pixel_gray8_ptr_out[offset_new].el.gray  = interpolate(&g_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				pixel_gray8_ptr_out[offset_new].el.alpha = interpolate(&a_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				offset_new++;
			}
			// �R�[���o�b�N
			if(callback_func(new_y, new_height - 1, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

void _image_resize_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	const int new_width  = param->new_width;
	const int new_height = param->new_height;

	// log2(depth)���v�Z
	const_uint_t depth = out_info_ptr->depth;
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// �������̈�����b�N
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size_in = 0, index_size_out = 0;
	uint32_t palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in, &index_size_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size_out, &palette_size);

	const_uint32_t hsize     = index_size_in  / height;
	const_uint32_t hsize_new = index_size_out / new_height;

	memset(index8_ptr_out, 0, index_size_out);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	// �g��^�k��
	const double width_rate  = (new_width  > 1) ? (double)(width  - 1) / (new_width  - 1) : 0;
	const double height_rate = (new_height > 1) ? (double)(height - 1) / (new_height - 1) : 0;
	for(int new_y = 0; new_y < new_height; new_y++)
	{
		for(int new_x = 0; new_x < new_width; new_x++)
		{
			const_uint32_t interpolate_x = static_cast<const_uint32_t>(new_x * width_rate);
			const_uint32_t interpolate_y = static_cast<const_uint32_t>(new_y * height_rate);

			// ���͑��̍��W(interpolate_x, interpolate_y)�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset_in = CALC_PACK_OFFSET(interpolate_x, log2_depth) + hsize * interpolate_y;
			const int      shifts_in = CALC_PACK_SHIFTS(interpolate_x, log2_depth);		

			// �o�͑��̍��W(new_x, new_y)�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset_out = CALC_PACK_OFFSET(new_x, log2_depth) + hsize_new * new_y;
			const int      shifts_out = CALC_PACK_SHIFTS(new_x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset_in], depth, shifts_in);
			PACK_STORE(index8_ptr_out[offset_out], index, shifts_out);
		}
		// �R�[���o�b�N
		if(callback_func(new_y, new_height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_resize_rgb8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const RESIZEPARAM *param, callback_t callback_func, long callback_data)
{
	const int new_width  = param->new_width;
	const int new_height = param->new_height;
	const INTERPOLATE_METHOD interpolate_method = param->interpolate_method;

	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	uint32_t pixel_count_in = 0;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in, &pixel_count_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// �V�����C���[�W�̃I�t�Z�b�g
	uint32_t offset_new = 0;

	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;

	try
	{
		// 
		std::valarray<uint8_t> r_plane(pixel_count_in), g_plane(pixel_count_in), b_plane(pixel_count_in), a_plane(pixel_count_in);
		for(uint32_t i = 0; i < pixel_count_in; i++)
		{
			r_plane[i] = pixel_rgb8_ptr_in[i].el.r;
			g_plane[i] = pixel_rgb8_ptr_in[i].el.g;
			b_plane[i] = pixel_rgb8_ptr_in[i].el.b;
			a_plane[i] = pixel_rgb8_ptr_in[i].el.a;
		}

		// �g��^�k��
		const double width_rate  = (new_width  > 1) ? (double)(width  - 1) / (new_width  - 1) : 0;
		const double height_rate = (new_height > 1) ? (double)(height - 1) / (new_height - 1) : 0;
		for(int new_y = 0; new_y < new_height; new_y++)
		{
			for(int new_x = 0; new_x < new_width; new_x++)
			{
				// RGB��3�F��
				const double interpolate_x = new_x * width_rate;
				const double interpolate_y = new_y * height_rate;

				pixel_rgb8_ptr_out[offset_new].el.r = interpolate(&r_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				pixel_rgb8_ptr_out[offset_new].el.g = interpolate(&g_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				pixel_rgb8_ptr_out[offset_new].el.b = interpolate(&b_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				pixel_rgb8_ptr_out[offset_new].el.a = interpolate(&a_plane[0], interpolate_x, interpolate_y, width, height, interpolate_method);
				offset_new++;
			}
			// �R�[���o�b�N
			if(callback_func(new_y, new_height - 1, callback_data))
			{
				throw IR_CANCELED;
			}
		}
	}
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}


////////////////////////////////////////////////////////////////////////////////
// �g���~���O
void _image_trimming_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;
	const int new_width  = out_info_ptr->width;
	const int new_height = out_info_ptr->height;
	const_pixel_gray8_t space_color = {0xffff};

	// �������̈�����b�N
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// ���摜�̃I�t�Z�b�g
	pixel_gray8_ptr_in += CALC_OFFSET(clip_left, clip_top, width);

	// �㕔�]��
	if(TRUE)
	{
		const int fill_count = new_width * y_begin;

		// �󔒂Ŗ��߂�
		std::fill_n(pixel_gray8_ptr_out, fill_count, space_color);
		pixel_gray8_ptr_out += fill_count;
	}

	// �C���[�W��
	for(int y = y_begin; y < y_end; y++)
	{
		// �����]��
		if(TRUE)
		{
			const int fill_count = x_begin;

			// �󔒂Ŗ��߂�
			std::fill_n(pixel_gray8_ptr_out, fill_count, space_color);
			pixel_gray8_ptr_out += fill_count;
		}

		// �C���[�W��
		if(TRUE)
		{
			const int copy_count = x_end - x_begin;

			// �C���[�W�����R�s�[
			std::copy(
				&pixel_gray8_ptr_in[0],
				&pixel_gray8_ptr_in[copy_count],
				pixel_gray8_ptr_out);

			pixel_gray8_ptr_in  += width;
			pixel_gray8_ptr_out += copy_count;
		}

		// �E���]��
		if(TRUE)
		{
			const int fill_count = new_width - x_end;

			// �󔒂Ŗ��߂�
			std::fill_n(pixel_gray8_ptr_out, fill_count, space_color);
			pixel_gray8_ptr_out += fill_count;
		}

		// �R�[���o�b�N
		if(callback_func(y, y_end - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}

	// �����]��
	if(TRUE)
	{
		const int fill_count = new_width * (new_height - y_end);

		// �󔒂Ŗ��߂�
		std::fill_n(pixel_gray8_ptr_out, fill_count, space_color);
		pixel_gray8_ptr_out += fill_count;
	}
}

void _image_trimming_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;
	const int new_width  = out_info_ptr->width;
	const int new_height = out_info_ptr->height;

	// log2(depth)���v�Z
	const_uint_t depth = out_info_ptr->depth;
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// �������̈�����b�N
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size_out = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size_out, &palette_size);

	const_uint32_t hsize     = CALC_HSIZE(    width,  in_info_ptr->depth);
	const_uint32_t hsize_new = CALC_HSIZE(new_width, out_info_ptr->depth);

	memset(index8_ptr_out, 0, index_size_out);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	index8_ptr_in  += clip_top * hsize;
	index8_ptr_out += y_begin  * hsize_new;

	for(int y = y_begin; y < y_end; y++)
	{
		for(int x = x_begin; x < x_end; x++)
		{
			// ���̉摜�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset = CALC_PACK_OFFSET(clip_left + (x - x_begin), log2_depth);
			const int      shifts = CALC_PACK_SHIFTS(clip_left + (x - x_begin), log2_depth);

			// �V�����摜(x, y)�̃I�t�Z�b�g�l�A�V�t�g�ʂ����߂�
			const_uint32_t offset_new = CALC_PACK_OFFSET(x, log2_depth);
			const int      shifts_new = CALC_PACK_SHIFTS(x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset], depth, shifts);
			PACK_STORE(index8_ptr_out[offset_new], index, shifts_new);
		}
		index8_ptr_in  += hsize;
		index8_ptr_out += hsize_new;

		// �R�[���o�b�N
		if(callback_func(y, new_height - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}
}

void _image_trimming_rgb8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const int clip_left, const int clip_top, const int x_begin, const int x_end, const int y_begin, const int y_end, callback_t callback_func, long callback_data)
{
	const int width  = in_info_ptr->width;
	const int height = in_info_ptr->height;
	const int new_width  = out_info_ptr->width;
	const int new_height = out_info_ptr->height;
	const_pixel_rgb8_t space_color = {0xffffffff};

	// �������̈�����b�N
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// ���摜�̃I�t�Z�b�g
	pixel_rgb8_ptr_in += CALC_OFFSET(clip_left, clip_top, width);

	// �㕔�]��
	if(TRUE)
	{
		const int fill_count = new_width * y_begin;

		// �󔒂Ŗ��߂�
		std::fill_n(pixel_rgb8_ptr_out, fill_count, space_color);
		pixel_rgb8_ptr_out += fill_count;
	}

	// �C���[�W��
	for(int y = y_begin; y < y_end; y++)
	{
		// �����]��
		if(TRUE)
		{
			const int fill_count = x_begin;

			// �󔒂Ŗ��߂�
			std::fill_n(pixel_rgb8_ptr_out, fill_count, space_color);
			pixel_rgb8_ptr_out += fill_count;
		}

		// �C���[�W��
		if(TRUE)
		{
			const int copy_count = x_end - x_begin;

			// �C���[�W�����R�s�[
			std::copy(
				&pixel_rgb8_ptr_in[0],
				&pixel_rgb8_ptr_in[copy_count],
				pixel_rgb8_ptr_out);

			pixel_rgb8_ptr_in  += width;
			pixel_rgb8_ptr_out += copy_count;
		}

		// �E���]��
		if(TRUE)
		{
			const int fill_count = new_width - x_end;

			// �󔒂Ŗ��߂�
			std::fill_n(pixel_rgb8_ptr_out, fill_count, space_color);
			pixel_rgb8_ptr_out += fill_count;
		}

		// �R�[���o�b�N
		if(callback_func(y, y_end - 1, callback_data))
		{
			throw IR_CANCELED;
		}
	}

	// �����]��
	if(TRUE)
	{
		const int fill_count = new_width * (new_height - y_end);

		// �󔒂Ŗ��߂�
		std::fill_n(pixel_rgb8_ptr_out, fill_count, space_color);
		pixel_rgb8_ptr_out += fill_count;
	}
}

END_NAMESPACE()
