// opaque.cpp インプリメンテーションファイル
#include "../../stdafx.h"
#include "../filter_common.h"
#include "opaque.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// プロトタイプ宣言
	uint8_t opaque_proc(const uint8_t pixel_array[], const int pixelcount_x, const int pixelcount_y, void_ptr_t param);
END_NAMESPACE()


// フィルタが実行可能か
BOOL executable_opaque(const IMAGE_INFO *info_ptr)
{
	switch(info_ptr->mode)
	{
	case COLORMODE_GRAYSCALE:
	case COLORMODE_RGB:
		return TRUE;

	default:
		return FALSE;
	}
}

// ぼかし処理
IMAGERESULT effect_opaque(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data)
{
	return _pop_filter_custom(info_ptr, in_data_ptr, out_data_ptr, opaque_proc, 1, 1, NULL, callback, callback_data);
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// プロシージャ
uint8_t opaque_proc(const uint8_t pixel_array[], const int pixelcount_x, const int pixelcount_y, void_ptr_t /* param */)
{
	const int size = (2 * pixelcount_x + 1) * (2 * pixelcount_y + 1);

	// 輝度値の平均をとる
	int val = 0;
	for(int i = 0; i < size; i++)
	{
		val += pixel_array[i];
	}

	return val / size;
}

END_NAMESPACE()
