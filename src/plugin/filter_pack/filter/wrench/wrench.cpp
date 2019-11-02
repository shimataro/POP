// wrench.cpp
#include "../../stdafx.h"
#include "../filter_common.h"
#include "wrench.h"
#include "wrench_dialog.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// ねじりパラメータ
	WRENCHPARAM g_param =
	{
		50, 50,
		0, 0,
		100,
		MATH_PI / 2,
	};

	// プロトタイプ宣言
	void wrench_proc(POINT_2D *point, void_ptr_t param);
END_NAMESPACE()


// フィルタが実行可能か
BOOL executable_wrench(const IMAGE_INFO *info_ptr)
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

// フィルタ関数
IMAGERESULT effect_wrench(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data)
{
	WRENCHPARAM *wrench_param = NULL;
	if(param == NULL)
	{
		wrench_param = &g_param;
	}
	else
	{
		wrench_param = reinterpret_cast<WRENCHPARAM *>(param);
	}

	wrench_param->center_x = info_ptr->width  * wrench_param->center_x_percent / 100;
	wrench_param->center_y = info_ptr->height * wrench_param->center_y_percent / 100;
	return _pop_filter_transform_geometric(info_ptr, in_data_ptr, out_data_ptr, wrench_proc, wrench_param, IM_LINER, callback, callback_data);
}

// オプション
BOOL option_wrench(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd)
{
	wrench_dialog theDlg(g_param, info_ptr, data_ptr, hWnd);
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

// プロシージャ
void wrench_proc(POINT_2D *point, void_ptr_t param)
{
	// パラメータ
	const WRENCHPARAM *wrench_param = reinterpret_cast<const WRENCHPARAM *>(param);

	// 中心点を原点とする
	double dx = point->x - wrench_param->center_x;
	double dy = point->y - wrench_param->center_y;

	// 極座標へ変換
	double r, angle;
	math_xy2pole(dx, dy, &r, &angle);
	if(r >= wrench_param->radius)
	{
		return;
	}

	// 角度にねじりを与える
	angle -= wrench_param->angle * (1 - r / wrench_param->radius);

	// XY座標へ戻す
	math_pole2xy(r, angle, &dx, &dy);

	// 元の画像の対応する位置を求める
	{
		point->x = dx + wrench_param->center_x;
		point->y = dy + wrench_param->center_y;
	}
}

END_NAMESPACE()
