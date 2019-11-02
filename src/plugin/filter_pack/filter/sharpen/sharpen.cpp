// sharpen.cpp インプリメンテーションファイル
#include "../../stdafx.h"
#include "../filter_common.h"
#include "sharpen.h"

#include <stdlib.h>                     // abs()


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// プロトタイプ宣言
	uint8_t sharpen_proc(const uint8_t pixel_array[], const int pixelcount_x, const int pixelcount_y, void_ptr_t param);
END_NAMESPACE()


// フィルタが実行可能か
BOOL executable_sharpen(const IMAGE_INFO *info_ptr)
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

// シャープ処理
IMAGERESULT effect_sharpen(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data)
{
	return _pop_filter_custom(info_ptr, in_data_ptr, out_data_ptr, sharpen_proc, 1, 1, NULL, callback, callback_data);
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// プロシージャ
uint8_t sharpen_proc(const uint8_t pixel_array[], const int pixelcount_x, const int pixelcount_y, void_ptr_t /* param */)
{
//	const int coef_array[] = {13, -2, -1};
	const int coef_array[] = {5, -1, 0};

	// 輝度値の加重平均をとる
	const int sum1 = pixel_array[4];
	const int sum2 = pixel_array[1] + pixel_array[3] + pixel_array[5] + pixel_array[7];
	const int sum3 = pixel_array[0] + pixel_array[2] + pixel_array[6] + pixel_array[8];

	const int value = sum1 * coef_array[0] + sum2 * coef_array[1] + sum3 * coef_array[2];

	return saturate(value, 0, 255);
}

END_NAMESPACE()
