// sepia.cpp …インプリメンテーションファイル
#include "../../stdafx.h"
#include "../filter_common.h"
#include "sepia.h"
#include "sepia_dialog.h"
#include <math.h>


typedef struct tagSEPIA_TABLE
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} SEPIA_TABLE;


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// セピアパラメータ
	SEPIAPARAM g_param =
	{
		4,                                      // 強度＝4（log(1.5) = 0.405465）
	};

	void sepia_proc(COLOR *color, void_ptr_t param);
	void make_table_sepia(const UINT strength, SEPIA_TABLE table[256]);
END_NAMESPACE()


// フィルタが実行可能か
BOOL executable_sepia(const IMAGE_INFO *info_ptr)
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

// エフェクト
IMAGERESULT effect_sepia(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data)
{
	// パラメータからテーブルを作成
	SEPIA_TABLE table[256];
	if(param == NULL)
	{
		make_table_sepia(g_param.strength, table);
	}
	else
	{
		const SEPIAPARAM *sepiaparam = reinterpret_cast<const SEPIAPARAM *>(param);
		make_table_sepia(sepiaparam->strength, table);
	}

	return _pop_filter_transform_color(info_ptr, in_data_ptr, out_data_ptr, sepia_proc, table, callback, callback_data);
}

// オプション
BOOL option_sepia(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd)
{
	sepia_dialog theDlg(g_param, info_ptr, data_ptr, hWnd);

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
void sepia_proc(COLOR *color, void_ptr_t param)
{
	const SEPIA_TABLE *table = reinterpret_cast<const SEPIA_TABLE *>(param);
	const int grayscale = GRAYSCALE(color->red, color->green, color->blue);

	// セピアテーブルで変換
	color->red   = table[grayscale].r;
	color->green = table[grayscale].g;
	color->blue  = table[grayscale].b;
}

// セピアテーブルの作成
void make_table_sepia(const UINT strength, SEPIA_TABLE table[256])
{
	// セピアパラメータ（≧１）
	const double power = exp(static_cast<double>(strength) / SEPIA_STRENGTH_MAX);

	for(int i = 0; i < 256; i++)
	{
		const int r = static_cast<int>(pow(i / 255.0, 1 / power) * 255);    // 輝度を1/power乗
		const int b = static_cast<int>(pow(i / 255.0,     power) * 255);    // 輝度をpower乗

		// R≧G≧B となる
		table[i].r = r;
		table[i].g = i;
		table[i].b = b;
	}
}

END_NAMESPACE()
