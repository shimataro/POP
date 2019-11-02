/* mem.h …メモリ管理用関数 */
#ifndef __MEM__
#define __MEM__

#include <assert.h>
#include "image.h"


/******************************************************************************/
/* 関数のプロトタイプ宣言 */
EXTERN_C BOOL WINAPI image_alloc(const IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr);
EXTERN_C void WINAPI image_free (IMAGE_DATA *data_ptr);


/******************************************************************************/
/* C++では、メモリ領域のロック・解除を自動的に行うクラスを提供 */
#ifdef __cplusplus

class auto_lock
{
public:
	auto_lock(const IMAGE_DATA *data_ptr,
		const_pixel_gray8_ptr_t &pixel_gray8_ptr,
		uint32_ptr_t pixel_count_ptr = NULL) : m_data_ptr(data_ptr)
	{
		assert(data_ptr != NULL);

		pixel_gray8_ptr = reinterpret_cast<const_pixel_gray8_ptr_t>(::GlobalLock(data_ptr->image));
		assert(pixel_gray8_ptr != NULL);

		if(pixel_count_ptr != NULL)
		{
			*pixel_count_ptr = data_ptr->image_size / sizeof(pixel_gray8_t);
		}
	}

	auto_lock(const IMAGE_DATA *data_ptr,
		const_uint8_ptr_t &index_ptr, const_palette8_ptr_t &palette8_ptr,
		uint32_ptr_t index_size_ptr = NULL, uint32_ptr_t palette_count_ptr = NULL) : m_data_ptr(data_ptr)
	{
		assert(data_ptr != NULL);

		index_ptr    = reinterpret_cast<const_uint8_ptr_t   >(::GlobalLock(data_ptr->image));
		palette8_ptr = reinterpret_cast<const_palette8_ptr_t>(::GlobalLock(data_ptr->extra));
		assert(index_ptr    != NULL);
		assert(palette8_ptr != NULL);

		if(index_size_ptr != NULL)
		{
			*index_size_ptr = data_ptr->image_size;
		}
		if(palette_count_ptr != NULL)
		{
			*palette_count_ptr = data_ptr->extra_size / sizeof(palette8_t);
		}
	}

	auto_lock(const IMAGE_DATA *data_ptr,
		const_pixel_rgb8_ptr_t &pixel_rgb8_ptr,
		uint32_ptr_t pixel_count_ptr = NULL) : m_data_ptr(data_ptr)
	{
		assert(data_ptr != NULL);

		pixel_rgb8_ptr = reinterpret_cast<const_pixel_rgb8_ptr_t>(::GlobalLock(data_ptr->image));
		assert(pixel_rgb8_ptr != NULL);

		if(pixel_count_ptr != NULL)
		{
			*pixel_count_ptr = data_ptr->image_size / sizeof(pixel_rgb8_t);
		}
	}

	~auto_lock(void)
	{
		::GlobalUnlock(m_data_ptr->image);
		::GlobalUnlock(m_data_ptr->extra);
	}

private:
	const IMAGE_DATA *m_data_ptr;
};

#endif /* __cplusplus */


#endif /* __MEM__ */
