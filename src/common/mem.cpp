/* mem.cpp …インプリメンテーションファイル */
#include "stdafx.h"
#include "mem.h"

#include <assert.h>                     // assert()


/* イメージ用のメモリを確保 */
EXTERN_C
BOOL WINAPI image_alloc(const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr)
{
	const_uint32_t width  = info_ptr->width;
	const_uint32_t height = info_ptr->height;
	const_uint_t   depth  = info_ptr->depth;

	switch(info_ptr->mode)
	{
	case COLORMODE_GRAYSCALE:
		data_ptr->image_size = width * height * sizeof(pixel_gray8_t);
		data_ptr->image      = ::GlobalAlloc(GMEM_MOVEABLE, data_ptr->image_size);
		data_ptr->extra_size = 0;
		data_ptr->extra      = NULL;
		break;

	case COLORMODE_INDEX:
		data_ptr->image_size = CALC_IMAGE_SIZE(width, height, depth, 1);
		data_ptr->image      = ::GlobalAlloc(GMEM_MOVEABLE, data_ptr->image_size);
		data_ptr->extra_size = (1 << depth) * sizeof(palette8_t);
		data_ptr->extra      = ::GlobalAlloc(GMEM_MOVEABLE, data_ptr->extra_size);
		break;

	case COLORMODE_RGB:
		data_ptr->image_size = width * height * sizeof(pixel_rgb8_t);
		data_ptr->image      = ::GlobalAlloc(GMEM_MOVEABLE, data_ptr->image_size);
		data_ptr->extra_size = 0;
		data_ptr->extra      = NULL;
		break;
	}

	/*  */
	if(data_ptr->image == NULL || (data_ptr->extra_size > 0 && data_ptr->extra == NULL))
	{
		::GlobalFree(data_ptr->image);
		::GlobalFree(data_ptr->extra);
		data_ptr->image_size = 0;
		data_ptr->image = NULL;
		data_ptr->extra_size = 0;
		data_ptr->extra = NULL;
		return FALSE;
	}
	return TRUE;
}

/* イメージ用のメモリを解放 */
EXTERN_C
void WINAPI image_free(IMAGE_DATA *data_ptr)
{
	assert(data_ptr != NULL);
	HGLOBAL image = data_ptr->image;
	HGLOBAL extra = data_ptr->extra;

	/* メモリが廃棄されていなければ解放 */
	if(image != NULL)
	{
		/* ロックカウントの確認 */
		const UINT lock_count = GlobalFlags(image) & GMEM_LOCKCOUNT;
		assert(lock_count == 0);

		::GlobalFree(image);
		data_ptr->image = NULL;
	}

	/* メモリが廃棄されていなければ解放 */
	if(extra != NULL)
	{
		/* ロックカウントの確認 */
		const UINT lock_count = GlobalFlags(extra) & GMEM_LOCKCOUNT;
		assert(lock_count == 0);

		::GlobalFree(extra);
		data_ptr->extra = NULL;
	}
}
