// thumbnail.cpp …インプリメンテーションファイル
#include "thumbnail.h"
#include "mem.h"
#include "interpolate.h"


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	void create_thumbnail_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr);
	void create_thumbnail_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr);
	void create_thumbnail_rgb8 (const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr);
END_NAMESPACE()


// サムネイルを作成
EXTERN_C
IMAGERESULT create_thumbnail(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr, const_uint16_t new_width, const_uint16_t new_height)
{
	// 指定サイズに収まるようにサムネイルのサイズを決定
	*out_info_ptr = *in_info_ptr;
	const double now_rate = (double)in_info_ptr->width / in_info_ptr->height;
	const double new_rate = (double)new_width          / new_height;
	if(now_rate < new_rate)
	{
		// 高さにあわせる
		out_info_ptr->height = new_height;
		out_info_ptr->width  = static_cast<uint16_t>(new_height * now_rate);
	}
	else
	{
		out_info_ptr->width  = new_width;
		out_info_ptr->height = static_cast<uint16_t>(new_width / now_rate);
	}

	if(out_info_ptr->width == 0)
	{
		out_info_ptr->width = 1;
	}
	if(out_info_ptr->height == 0)
	{
		out_info_ptr->height = 1;
	}

	// メモリを確保
	if(!image_alloc(out_info_ptr, out_data_ptr))
	{
		return IR_NOMEMORY;
	}

	switch(out_info_ptr->mode)
	{
	case COLORMODE_GRAYSCALE:
		create_thumbnail_gray8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr);
		break;

	case COLORMODE_INDEX:
		create_thumbnail_index(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr);
		break;

	case COLORMODE_RGB:
		create_thumbnail_rgb8(in_info_ptr, in_data_ptr, out_info_ptr, out_data_ptr);
		break;
	}
	return IR_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

void create_thumbnail_gray8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr)
{
	const_uint32_t width  = out_info_ptr->width;
	const_uint32_t height = out_info_ptr->height;

	const double width_rate  = (out_info_ptr->width  > 1) ? (double)(in_info_ptr->width  - 1) / (out_info_ptr->width  - 1) : 0;
	const double height_rate = (out_info_ptr->height > 1) ? (double)(in_info_ptr->height - 1) / (out_info_ptr->height - 1) : 0;

	// 新しいイメージのオフセット
	uint32_t offset_new = 0;

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr_in  = NULL;
	pixel_gray8_ptr_t       pixel_gray8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_gray8_ptr_in), lock_out(out_data_ptr, pixel_gray8_ptr_out);

	// 拡大/縮小コピー
	for(uint32_t new_y = 0; new_y < height; new_y++)
	{
		for(uint32_t new_x = 0; new_x < width; new_x++)
		{
			const_uint32_t interpolate_x = static_cast<const_uint32_t>(new_x * width_rate);
			const_uint32_t interpolate_y = static_cast<const_uint32_t>(new_y * height_rate);

			const_uint32_t offset = CALC_OFFSET(interpolate_x, interpolate_y, in_info_ptr->width);
			*pixel_gray8_ptr_out++ = pixel_gray8_ptr_in[offset];
		}
	}
}

void create_thumbnail_index(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr)
{
	const_uint32_t width  = in_info_ptr->width;
	const_uint32_t height = in_info_ptr->height;

	const_uint32_t new_width  = out_info_ptr->width;
	const_uint32_t new_height = out_info_ptr->height;

	// log2(depth)を計算
	const_uint_t depth = out_info_ptr->depth;
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth - 1];

	// メモリ領域をロック
	const_uint8_ptr_t    index8_ptr_in   = NULL;
	const_palette8_ptr_t palette8_ptr_in = NULL;
	uint8_ptr_t    index8_ptr_out   = NULL;
	palette8_ptr_t palette8_ptr_out = NULL;
	uint32_t index_size_in = 0, index_size_out = 0, palette_size = 0;
	auto_lock lock_in(in_data_ptr, index8_ptr_in, palette8_ptr_in, &index_size_in), lock_out(out_data_ptr, index8_ptr_out, palette8_ptr_out, &index_size_out, &palette_size);

	memset(index8_ptr_out, 0, index_size_out);
	memcpy(palette8_ptr_out, palette8_ptr_in, palette_size * sizeof(palette8_t));

	const_uint32_t hsize     = index_size_in  / height;
	const_uint32_t hsize_new = index_size_out / new_height;

	// 拡大/縮小コピー
	const double width_rate  = (new_width  > 1) ? (double)(width  - 1) / (new_width  - 1) : 0;
	const double height_rate = (new_height > 1) ? (double)(height - 1) / (new_height - 1) : 0;
	for(uint32_t new_y = 0; new_y < new_height; new_y++)
	{
		for(uint32_t new_x = 0; new_x < new_width; new_x++)
		{
			const_uint32_t interpolate_x = static_cast<const_uint32_t>(new_x * width_rate);
			const_uint32_t interpolate_y = static_cast<const_uint32_t>(new_y * height_rate);

			// 入力側の座標(interpolate_x, interpolate_y)のオフセット値、シフト量を求める
			const_uint32_t offset_in = CALC_PACK_OFFSET(interpolate_x, log2_depth) + hsize * interpolate_y;
			const int      shifts_in = CALC_PACK_SHIFTS(interpolate_x, log2_depth);		

			// 出力側の座標(new_x, new_y)のオフセット値、シフト量を求める
			const_uint32_t offset_out = CALC_PACK_OFFSET(new_x, log2_depth) + hsize_new * new_y;
			const int      shifts_out = CALC_PACK_SHIFTS(new_x, log2_depth);

			const_uint8_t index = PACK_EXTRACT(index8_ptr_in[offset_in], depth, shifts_in);

			index8_ptr_out[offset_out] |= index << shifts_out;
		}
	}
}

void create_thumbnail_rgb8(const IMAGE_INFO *in_info_ptr, const IMAGE_DATA *in_data_ptr, const IMAGE_INFO *out_info_ptr, IMAGE_DATA *out_data_ptr)
{
	const_uint32_t width  = out_info_ptr->width;
	const_uint32_t height = out_info_ptr->height;

	const double width_rate  = (out_info_ptr->width  > 1) ? (double)(in_info_ptr->width  - 1) / (out_info_ptr->width  - 1) : 0;
	const double height_rate = (out_info_ptr->height > 1) ? (double)(in_info_ptr->height - 1) / (out_info_ptr->height - 1) : 0;

	// 新しいイメージのオフセット
	uint32_t offset_new = 0;

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr_in  = NULL;
	pixel_rgb8_ptr_t       pixel_rgb8_ptr_out = NULL;
	auto_lock lock_in(in_data_ptr, pixel_rgb8_ptr_in), lock_out(out_data_ptr, pixel_rgb8_ptr_out);

	// 拡大/縮小コピー
	for(uint32_t new_y = 0; new_y < height; new_y++)
	{
		for(uint32_t new_x = 0; new_x < width; new_x++)
		{
			const_uint32_t interpolate_x = static_cast<uint32_t>(new_x * width_rate);
			const_uint32_t interpolate_y = static_cast<uint32_t>(new_y * height_rate);

			const_uint32_t offset = CALC_OFFSET(interpolate_x, interpolate_y, in_info_ptr->width);
			*pixel_rgb8_ptr_out++ = pixel_rgb8_ptr_in[offset];
		}
	}
}

END_NAMESPACE()
