// inverse.cpp �C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "../filter_common.h"
#include "inverse.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// �v���g�^�C�v�錾
	void inverse_proc(COLOR *color, void_ptr_t param);
END_NAMESPACE()


// �t�B���^�����s�\��
BOOL executable_inverse(const IMAGE_INFO *info_ptr)
{
	switch(info_ptr->mode)
	{
	case COLORMODE_INDEX:
		return TRUE;

	case COLORMODE_RGB:
		switch(info_ptr->depth)
		{
		case 8:
			return TRUE;

		default:
			return FALSE;
		}

	default:
		return FALSE;
	}
}

// ���]����
IMAGERESULT effect_inverse(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data)
{
	return _pop_filter_transform_color(info_ptr, in_data_ptr, out_data_ptr, inverse_proc, NULL, callback, callback_data);
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// �v���V�[�W��
void inverse_proc(COLOR *color, void_ptr_t /* param */)
{
	color->red   = ~color->red;
	color->green = ~color->green;
	color->blue  = ~color->blue;
}

END_NAMESPACE()
