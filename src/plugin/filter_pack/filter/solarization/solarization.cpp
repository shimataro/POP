// solarization.cpp �C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "../filter_common.h"
#include "solarization.h"
#include "solarization_dialog.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// �\�����[�[�V�����p�����[�^
	SOLARIZATIONPARAM g_param =
	{
		128,                                    // �\�����[�[�V������臒l
		TRUE,
		TRUE,
		TRUE,
	};

	// �v���g�^�C�v�錾
	void solarization_proc(COLOR *color, void_ptr_t param);
END_NAMESPACE()


// �t�B���^�����s�\��
BOOL executable_solarization(const IMAGE_INFO *info_ptr)
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

// �\�����[�[�V��������
IMAGERESULT effect_solarization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data)
{
	// �p�����[�^����e�[�u�����쐬
	SOLARIZATIONPARAM *solarizationparam = NULL;
	if(param == NULL)
	{
		solarizationparam = &g_param;
	}
	else
	{
		solarizationparam = reinterpret_cast<SOLARIZATIONPARAM *>(param);
	}

	return _pop_filter_transform_color(info_ptr, in_data_ptr, out_data_ptr, solarization_proc, solarizationparam, callback, callback_data);
}

// �I�v�V����
BOOL option_solarization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd)
{
	solarization_dialog theDlg(g_param, info_ptr, data_ptr, hWnd);
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
void solarization_proc(COLOR *color, void_ptr_t param)
{
	const SOLARIZATIONPARAM *solarizationparam = reinterpret_cast<const SOLARIZATIONPARAM *>(param);

	const int threshold = solarizationparam->threshold;
	const int red   = color->red;
	const int green = color->green;
	const int blue  = color->blue;

	// �K�v�ȃ`���l���𔽓]
	if(solarizationparam->channel_red && red >= threshold)
	{
		color->red = ~red;
	}

	if(solarizationparam->channel_green && red >= threshold)
	{
		color->green = ~green;
	}

	if(solarizationparam->channel_blue && red >= threshold)
	{
		color->blue = ~blue;
	}
}

END_NAMESPACE()
