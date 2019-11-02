// mosaic.cpp インプリメンテーションファイル
#include "../../stdafx.h"
#include "../filter_common.h"
#include "mosaic.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// モザイクパラメータ
	MOSAICPARAM g_param =
	{
		8, 8,
	};

	// プロトタイプ宣言
	void mosaic_proc(POINT_2D_INTEGER *point, void_ptr_t param);
END_NAMESPACE()


// フィルタが実行可能か
BOOL executable_mosaic(const IMAGE_INFO *info_ptr)
{
	return TRUE;
}

// モザイク処理
IMAGERESULT effect_mosaic(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data)
{
	MOSAICPARAM *mosaic_param = NULL;
	if(param == NULL)
	{
		mosaic_param = &g_param;
	}
	else
	{
		mosaic_param = reinterpret_cast<MOSAICPARAM *>(param);
	}

	return _pop_filter_transform_geometric_integer(info_ptr, in_data_ptr, out_data_ptr, mosaic_proc, mosaic_param, callback, callback_data);
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// プロシージャ
void mosaic_proc(POINT_2D_INTEGER *point, void_ptr_t param)
{
	// パラメータ
	const MOSAICPARAM *mosaic_param = reinterpret_cast<const MOSAICPARAM *>(param);
	const int size_x = mosaic_param->size_x;
	const int size_y = mosaic_param->size_y;

	point->x -= point->x % size_x;
	point->y -= point->y % size_y;
}

END_NAMESPACE()
