// ripple.cpp
#include "../../stdafx.h"
#include "../filter_common.h"
#include "ripple_dialog.h"
#include "ripple.h"


// ���ܗ�
#define REF_WATER    1.33                 // ����1.33
#define REF_GLASS    1.5                  // �K���X��1.5
#define REF_CRYSTAL  1.54                 // ������1.54
#define REF_SAPPHIRE 1.77                 // �T�t�@�C�A��1.77
#define REF_DIAMOND  2.42                 // �_�C�������h��2.42

// ���ܗ��̔z��
const double refraction[] =
{
	REF_WATER,
	REF_GLASS,
	REF_CRYSTAL,
	REF_SAPPHIRE,
	REF_DIAMOND,
};


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// �g��p�����[�^
	RIPPLEPARAM_DIALOG g_param =
	{
		50, 50,
		100,
		100,
		0,
		100,
		REF_DIAMOND,
	};

	// �v���g�^�C�v�錾
	void ripple_proc(POINT_2D *point, void_ptr_t param);
	double ripple_expression(const double t, const double amplitude, const double cycle, const double phase, const double distance, const double refraction);
END_NAMESPACE()


// �t�B���^�����s�\��
BOOL executable_ripple(const IMAGE_INFO *info_ptr)
{
	switch(info_ptr->mode)
	{
	case COLORMODE_GRAYSCALE:
	case COLORMODE_INDEX:
	case COLORMODE_RGB:
		return TRUE;

	default:
		return FALSE;
	}
}

// �t�B���^�֐�
IMAGERESULT effect_ripple(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data)
{
	RIPPLEPARAM_DIALOG *ripple_param = NULL;
	if(param == NULL)
	{
		ripple_param = &g_param;
	}
	else
	{
		ripple_param = reinterpret_cast<RIPPLEPARAM_DIALOG *>(param);
	}

	const int width  = info_ptr->width;
	const int height = info_ptr->height;
//	const double diagonal = math_hypot(width, height);
	RIPPLEPARAM ripple =
	{
		static_cast<uint32_t>(ripple_param->center_x * width / 100.0),
		static_cast<uint32_t>(ripple_param->center_y * height/ 100.0),
		ripple_param->amplitude * 10          / 100.0,
		ripple_param->cycle     * 50          / 100.0,
		ripple_param->phase     * 2 * MATH_PI / 100.0,
		ripple_param->distance  * 15          / 100.0,
		ripple_param->refraction,
	};
	return _pop_filter_transform_geometric(info_ptr, in_data_ptr, out_data_ptr, ripple_proc, &ripple, IM_LINER, callback, callback_data);
}

// �I�v�V����
BOOL option_ripple(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd)
{
	ripple_dialog theDlg(g_param, info_ptr, data_ptr, hWnd);
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
void ripple_proc(POINT_2D *point, void_ptr_t param)
{
	// �p�����[�^
	const RIPPLEPARAM *ripple_param = reinterpret_cast<const RIPPLEPARAM *>(param);

	const uint32_t center_x = ripple_param->center_x;
	const uint32_t center_y = ripple_param->center_y;

	// ���_���W������
	const double dx = point->x - center_x;
	const double dy = point->y - center_y;

	// ���_����_�܂ł̋������v�Z
	const double t = math_hypot(dx, dy);

	const double u = ripple_expression(
		t,
		ripple_param->amplitude,                // �U��
		ripple_param->cycle,                    // ����
		ripple_param->phase,                    // �ʑ�
		ripple_param->distance,                 // ����
		ripple_param->refraction);              // ���ܗ�

	point->x = dx * u + center_x;
	point->y = dy * u + center_y;
}

double ripple_expression(const double t, const double amplitude, const double cycle, const double phase, const double distance, const double refraction)
{
	const double omega = 2 * MATH_PI / cycle;

	// �g��̎�
	//  f(t)  = A sin(��t + p) + d
	//  f'(t) = A�� cos(��t + p)
	const double y      = amplitude * sin(omega * t + phase) + distance;
	const double y_diff = amplitude * omega * cos(omega * t + phase);

	// u = t + f(t) * (tan�� - tan��) / (1 + tan��tan��)
	//  tan�� = f'(t)
	//  tan�� = sin�� / ��(��^2 - sin^2 ��)
	//  sin�� = tan�� / ��(1 + tan^2 ��)
	const double tan_1 = y_diff;
	const double sin_1 = tan_1 / math_hypot(1, tan_1);
	const double tan_2 = sin_1 / sqrt(refraction * refraction - sin_1 * sin_1);

	const double u = 1 + y * (tan_1 - tan_2) / ((1 + tan_1 * tan_2) * t);
	return u;
}

END_NAMESPACE()
