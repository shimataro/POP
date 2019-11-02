// export_filter.cpp インプリメンテーションファイル
#include "../stdafx.h"
#include "PictBear_Filter.h"
#include "export_filter.h"

#include <string.h>                     // strchr(), strncpy()
#include <new>                          // std::bad_alloc<>

#undef max
#undef min
#include <valarray>                     // std::valarray<>


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	void CreateFilterString(LPSTR lpszString, const UINT nMax, LPCSTR lpszSourceString);
END_NAMESPACE()


// フィルタの個数
EXTERN_C
int WINAPI pop_filter_count(void)
{
	return get_filter_count();
}

// フィルタの情報
EXTERN_C
DWORD WINAPI pop_filter_info(const int filter_no, LPSTR genre, UINT genre_max, LPSTR name, UINT name_max)
{
	assert(filter_no < get_filter_count());

	char filter_kind[256];
	char filter_name[256];
	call_PBF_GetName_sub(filter_no, filter_kind, SIZE_OF_ARRAY(filter_kind), filter_name, SIZE_OF_ARRAY(filter_name));

	CreateFilterString(genre, genre_max, filter_kind);
	CreateFilterString(name , name_max , filter_name);

	return 0;
}

// 実行可能か
EXTERN_C
BOOL WINAPI pop_filter_executable(const int filter_no, const IMAGE_INFO *info_ptr)
{
	assert(filter_no < get_filter_count());

	switch(info_ptr->mode)
	{
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
EXTERN_C
IMAGERESULT WINAPI pop_filter_effect(const int filter_no, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr_in, IMAGE_DATA *data_ptr_out, callback_t callback, long callback_data)
{
	assert(filter_no < get_filter_count());

	try
	{
		const_uint32_t width  = info_ptr->width;
		const_uint32_t height = info_ptr->height;

		// メモリ領域をロック
		pixel_rgb8_ptr_t src = NULL, dst = NULL;
		uint32_t pixel_count = 0;
		auto_lock lock_src(data_ptr_in, src), lock_dst(data_ptr_out, dst, &pixel_count);

		memcpy(dst, src, pixel_count * sizeof(pixel_rgb8_t));

		const_uint8_t mask_val = 0;
		std::valarray<uint8_t> mask(mask_val, pixel_count);

		// PictBearのプラグインを呼び出す
		FARPROC proc = reinterpret_cast<FARPROC>(callback);
		DWORD maskflag = PBF_MASK_CLIENT;
		RECT area = {0, 0, width, height};

		call_PBF_Execute_sub(
			filter_no, NULL,
			reinterpret_cast<const Pixel32 *>(src),
			reinterpret_cast<      Pixel32 *>(dst),
			&mask[0], &area, &maskflag,
			width, height,
			proc, callback_data);

		return IR_SUCCESS;
	}
	catch(std::bad_alloc)
	{
		return IR_NOMEMORY;
	}
}

// オプション
EXTERN_C
BOOL WINAPI pop_filter_option(const int filter_no, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd)
{
	assert(filter_no < get_filter_count());

	try
	{
		// メモリ領域をロック
		pixel_rgb8_ptr_t preview = NULL;
		auto_lock lock_obj(data_ptr, preview);

		return call_PBF_Setup_sub(
			filter_no, hWnd,
			reinterpret_cast<const Pixel32 *>(preview),
			info_ptr->width, info_ptr->height);
	}
	catch(std::bad_alloc)
	{
		return IR_NOMEMORY;
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

void CreateFilterString(LPSTR lpszString, const UINT nMax, LPCSTR lpszSourceString)
{
	LPSTR lpszJapanese = strchr(lpszSourceString, '|');
	strncpy(lpszString, lpszJapanese + 1, nMax);
	lpszString[nMax - 1] = '\0';
}

END_NAMESPACE()
