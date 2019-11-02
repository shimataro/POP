// sepia.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "../filter_common.h"
#include "glasstile.h"
#include "glasstile_dialog.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// ガラスタイルパラメータ
	GLASSTILEPARAM g_param =
	{
		20,                                     // 幅＝20
		20,                                     // 高さ＝20
	};

	void glasstile_proc(POINT_2D_INTEGER *point, void_ptr_t param);
END_NAMESPACE()


// フィルタが実行可能か
BOOL executable_glasstile(const IMAGE_INFO *info_ptr)
{
	return TRUE;
}

// エフェクト
IMAGERESULT effect_glasstile(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data)
{
	GLASSTILEPARAM *glasstile_param = NULL;

	if(param == NULL)
	{
		glasstile_param = &g_param;
	}
	else
	{
		glasstile_param = reinterpret_cast<GLASSTILEPARAM *>(param);
	}
	return _pop_filter_transform_geometric_integer(info_ptr, in_data_ptr, out_data_ptr, glasstile_proc, glasstile_param, callback, callback_data);
}

// オプション
BOOL option_glasstile(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd)
{
	glasstile_dialog theDlg(g_param, info_ptr, data_ptr, hWnd);

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
void glasstile_proc(POINT_2D_INTEGER *point, void_ptr_t param)
{
	const GLASSTILEPARAM *glasstile_param = reinterpret_cast<const GLASSTILEPARAM *>(param);
	const int width  = glasstile_param->width;
	const int height = glasstile_param->height;

	// 画像を区画分けしたときに属するグループ
	const int group_x = (point->x + width  / 2) / width;
	const int group_y = (point->y + height / 2) / height;

	const_uint32_t center_x = width  * group_x;
	const_uint32_t center_y = height * group_y;

	point->x = 2 * point->x - center_x;
	point->y = 2 * point->y - center_y;
}

END_NAMESPACE()
