// posterization.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "../filter_common.h"
#include "posterization.h"
#include "posterization_dialog.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// �|�X�^���[�[�V�����p�����[�^
	POSTERIZATIONPARAM g_param =
	{
		8,
	};

	void posterization_proc(COLOR *color, void_ptr_t param);
	void make_table_posterization(const int tone, uint8_t table[256]);
END_NAMESPACE()


// �t�B���^�����s�\��
BOOL executable_posterization(const IMAGE_INFO *info_ptr)
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

// �G�t�F�N�g
IMAGERESULT effect_posterization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data)
{
	// �p�����[�^����e�[�u�����쐬
	uint8_t table[256];
	if(param == NULL)
	{
		make_table_posterization(g_param.tone, table);
	}
	else
	{
		const POSTERIZATIONPARAM *posterizationparam = reinterpret_cast<const POSTERIZATIONPARAM *>(param);
		make_table_posterization(posterizationparam->tone, table);
	}

	return _pop_filter_transform_color(info_ptr, in_data_ptr, out_data_ptr, posterization_proc, table, callback, callback_data);
}

// �I�v�V����
BOOL option_posterization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd)
{
	posterization_dialog theDlg(g_param, info_ptr, data_ptr, hWnd);
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

// �v���V�[�W��
void posterization_proc(COLOR *color, void_ptr_t param)
{
	const_uint8_ptr_t table = reinterpret_cast<const_uint8_ptr_t>(param);

	// �|�X�^���[�[�V�����e�[�u���ŕϊ�
	color->red   = table[color->red  ];
	color->green = table[color->green];
	color->blue  = table[color->blue ];
}

// �|�X�^���[�[�V�����e�[�u���̍쐬
void make_table_posterization(const int tone, uint8_t table[256])
{
	const int division = 256 / tone;

	for(int i = 0; i < 256; i++)
	{
		table[i] = i - (i % division);
	}
}

END_NAMESPACE()
