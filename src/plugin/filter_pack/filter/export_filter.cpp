// export_filter.cpp インプリメンテーションファイル
#include "../stdafx.h"
#include "export_filter.h"

#include "inverse/inverse.h"
#include "sepia/sepia.h"
#include "solarization/solarization.h"
#include "posterization/posterization.h"
#include "ripple/ripple.h"
#include "wrench/wrench.h"
#include "opaque/opaque.h"
#include "sharpen/sharpen.h"
#include "glasstile/glasstile.h"
#include "mosaic/mosaic.h"


typedef BOOL        (*EXECUTABLE)(const IMAGE_INFO *info_ptr);
typedef IMAGERESULT (*EFFECT    )(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
typedef BOOL        (*OPTION    )(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	typedef struct tagFILTERFUNC
	{
		int id_filter_genre;                    // フィルタのジャンルのID
		int id_filter_name;                     // フィルタ名のID
		EXECUTABLE filter_executable;
		EFFECT     filter_effect;
		OPTION     filter_option;
	} FILTERFUNC;

	const FILTERFUNC g_filterfunc_array[] =
	{
		{IDS_FILTER_GENRE_TONE   , IDS_FILTER_NAME_INVERSE      , executable_inverse      , effect_inverse      , NULL                },
		{IDS_FILTER_GENRE_TONE   , IDS_FILTER_NAME_SEPIA        , executable_sepia        , effect_sepia        , option_sepia        },
		{IDS_FILTER_GENRE_TONE   , IDS_FILTER_NAME_SOLARIZATION , executable_solarization , effect_solarization , option_solarization },
		{IDS_FILTER_GENRE_TONE   , IDS_FILTER_NAME_POSTERIZATION, executable_posterization, effect_posterization, option_posterization},
		{IDS_FILTER_GENRE_ALLAXIS, IDS_FILTER_NAME_RIPPLE       , executable_ripple       , effect_ripple       , option_ripple       },
		{IDS_FILTER_GENRE_ALLAXIS, IDS_FILTER_NAME_WRENCH       , executable_wrench       , effect_wrench       , option_wrench       },
		{IDS_FILTER_GENRE_OPAQUE , IDS_FILTER_NAME_OPAQUE       , executable_opaque       , effect_opaque       , NULL                },
		{IDS_FILTER_GENRE_OPAQUE , IDS_FILTER_NAME_SHARPEN      , executable_sharpen      , effect_sharpen      , NULL                },
		{IDS_FILTER_GENRE_GLASS  , IDS_FILTER_NAME_GLASSTILE    , executable_glasstile    , effect_glasstile    , option_glasstile    },
		{IDS_FILTER_GENRE_SPECIAL, IDS_FILTER_NAME_MOSAIC       , executable_mosaic       , effect_mosaic       , NULL                },
	};
END_NAMESPACE()


// フィルタの個数
EXTERN_C
int WINAPI pop_filter_count(void)
{
	return SIZE_OF_ARRAY(g_filterfunc_array);
}

// フィルタの情報
EXTERN_C
DWORD WINAPI pop_filter_info(const int filter_no, LPSTR genre, UINT genre_max, LPSTR name, UINT name_max)
{
	assert(filter_no < SIZE_OF_ARRAY(g_filterfunc_array));

	// ストリングリソースから文字列を読み込む
	if(TRUE)
	{
		HINSTANCE hInstance = GetInstanceHandle();

		::LoadStringA(hInstance, g_filterfunc_array[filter_no].id_filter_genre, genre, genre_max);
		::LoadStringA(hInstance, g_filterfunc_array[filter_no].id_filter_name , name , name_max);
	}

	return 0;
}

// フィルタが実行可能か
EXTERN_C
BOOL WINAPI pop_filter_executable(const int filter_no, const IMAGE_INFO *info_ptr)
{
	assert(filter_no < SIZE_OF_ARRAY(g_filterfunc_array));
	return g_filterfunc_array[filter_no].filter_executable(info_ptr);
}

// エフェクト
EXTERN_C
IMAGERESULT WINAPI pop_filter_effect(const int filter_no, const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, callback_t callback, long callback_data)
{
	assert(filter_no < SIZE_OF_ARRAY(g_filterfunc_array));
	return g_filterfunc_array[filter_no].filter_effect(info_ptr, in_data_ptr, out_data_ptr, NULL, callback, callback_data);
}

// オプション
EXTERN_C
BOOL WINAPI pop_filter_option(const int filter_no, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd)
{
	assert(filter_no < SIZE_OF_ARRAY(g_filterfunc_array));

	OPTION filter_option = g_filterfunc_array[filter_no].filter_option;
	if(filter_option == NULL)
	{
		return TRUE;
	}

	return filter_option(info_ptr, data_ptr, hWnd);
}
