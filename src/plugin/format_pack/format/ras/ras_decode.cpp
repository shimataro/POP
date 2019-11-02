// ras_decode.cpp
#include "../../stdafx.h"
#include "../iotemplate.h"
#include "ras_decode.h"

#undef max
#undef min
#include <valarray>                     // std::valarray<>


#define GET_BUFFER(ptr, data)   get_multi_bytes_bigendian(data, ptr, sizeof(data)), ptr += sizeof(data)


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// �������藧���Ȃ���Η�O���X���[
	inline void throw_if_not(bool expression)
	{if(!expression) {throw IR_INCORRECTHEADER;}}

	// �v���g�^�C�v�錾
	void read_RASTERFILEHEADER(HANDLE hFile, RASTERFILEHEADER *header);
END_NAMESPACE()


// �w�b�_����ǂݍ���
void ras_decode_header(HANDLE hFile, IMAGE_INFO *info_ptr, RASTERFILEHEADER *header)
{
	// �w�b�_����ǂݍ���
	read_RASTERFILEHEADER(hFile, header);

	throw_if_not(header->rfMagic == RAS_MAGIC);                                    // �}�W�b�N�i���o�[���قȂ��Ă�����G���[
	throw_if_not(/*header->rfBits==1 || header->rfBits==4 ||*/ header->rfBits==8 || header->rfBits==24);  // �r�b�g����1,4,8,24�ȊO�Ȃ�G���[
	throw_if_not(header->rfMaptype==RMT_NONE || header->rfMaptype==RMT_EQUAL_RGB);  // �J���[�}�b�v�^�C�v��RMT_NONE�ł�RMT_EQUAL_RGB�ł��Ȃ���΃G���[
	throw_if_not(!(header->rfMaptype==RMT_EQUAL_RGB && (header->rfMapsize%3)!=0));  // �J���[�}�b�v��RMT_EQUAL_RGB�ŁA�J���[�}�b�v�T�C�Y���R�̔{���łȂ���΃G���[
	throw_if_not(!(header->rfBits==24 && header->rfMaptype!=RMT_NONE));             // �r�b�g����24�i�t���J���[�j�ŁA�J���[�}�b�v�����݂�����G���[

	// IMAGE_INFO�\���̂̐ݒ�
	info_ptr->width  = static_cast<uint16_t>(header->rfWidth);
	info_ptr->height = static_cast<uint16_t>(header->rfHeight);
	if(header->rfBits == 24)
	{
		info_ptr->depth = 8;
		info_ptr->mode  = COLORMODE_RGB;
	}
	else
	{
		info_ptr->depth = static_cast<uint16_t>(header->rfBits);
		if(header->rfMaptype != RMT_NONE)
		{
			info_ptr->mode = COLORMODE_INDEX;
		}
		else
		{
			info_ptr->mode = COLORMODE_GRAYSCALE;
		}
	}
	info_ptr->alpha      = FALSE;
	info_ptr->colors     = 0;
	info_ptr->density_x  = 0;
	info_ptr->density_y  = 0;
	info_ptr->gamma_file = GAMMA_DEFAULT_FILE;
	info_ptr->comment    = NULL;
}

// �C���[�W�������
void ras_decode_image_gray8(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, RASTERFILEHEADER *header)
{
	try
	{
		const_uint32_t width  = info_ptr->width;  // �C���[�W�̕�
		const_uint32_t height = info_ptr->height; // �C���[�W�̍���
		const_uint_t   depth  = info_ptr->depth;

		// �������̈�����b�N
		pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
		auto_lock lock_obj(data_ptr, pixel_gray8_ptr);

		// 1���C��������̃T�C�Y
		const_uint32_t hsize     = CALC_HSIZE(width, depth);
		const_uint32_t hsize_RAS = CALC_BOUND(hsize, BOUND_RAS);
		std::valarray<uint8_t> buffer(hsize_RAS);

		// �C���[�W��z��Ɋi�[
		for(uint32_t y = 0; y < height; y++)
		{
			ReadData(hFile, &buffer[0], buffer.size(), IR_IMAGEDESTROYED);
			for(uint32_t x = 0; x < width; x++)
			{
				pixel_gray8_ptr->el.gray = buffer[x];
				pixel_gray8_ptr++;
			}
		}
	}

	// ��O����
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

void ras_decode_image_index(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, RASTERFILEHEADER *header)
{
	try
	{
		const_uint32_t width  = info_ptr->width;  // �C���[�W�̕�
		const_uint32_t height = info_ptr->height; // �C���[�W�̍���
		const_uint_t   depth  = info_ptr->depth;

		// �������̈�����b�N
		uint8_ptr_t    index_ptr    = NULL;
		palette8_ptr_t palette8_ptr = NULL;
		auto_lock lock_obj(data_ptr, index_ptr, palette8_ptr);


		////////////////////////////////////////////////////////////////////////////////
		// �J���[�}�b�v��ǂݍ���
		if(TRUE)
		{
			// RAS�J���[�}�b�v�̑傫���A�W���J���[�}�b�v�̑傫���A�J���[�}�b�v�x�N�g��
			const_uint32_t map_size   = header->rfMapsize;
			const_uint32_t table_size = map_size / 3;
			std::valarray<uint8_t> map_buffer(map_size);

			// �J���[�}�b�v��ǂݍ���
			ReadData(hFile, &map_buffer[0], map_buffer.size(), IR_IMAGEDESTROYED);

			// �W���J���[�}�b�v�փZ�b�g
			for(uint32_t i = 0; i < table_size; i++)
			{
				palette8_ptr[i].r = map_buffer[i + 0 * table_size];
				palette8_ptr[i].g = map_buffer[i + 1 * table_size];
				palette8_ptr[i].b = map_buffer[i + 2 * table_size];
				palette8_ptr[i].a = 255;
			}
		}


		////////////////////////////////////////////////////////////////////////////////
		// �C���[�W�f�[�^��ǂݍ���
		if(TRUE)
		{
			// 1���C��������̃T�C�Y
			const_uint32_t hsize     = CALC_HSIZE(width, depth);
			const_uint32_t hsize_RAS = CALC_BOUND(hsize, BOUND_RAS);
			std::valarray<uint8_t> buffer(hsize_RAS);

			// �C���[�W��z��Ɋi�[
			for(uint32_t y = 0; y < height; y++)
			{
				ReadData(hFile, &buffer[0], buffer.size(), IR_IMAGEDESTROYED);
				memcpy(&index_ptr[y * hsize], &buffer[0], hsize);
			}
		}
	}

	// ��O����
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}

void ras_decode_image_rgb8(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, RASTERFILEHEADER *header)
{
	try
	{
		const_uint32_t width  = info_ptr->width;  // �C���[�W�̕�
		const_uint32_t height = info_ptr->height; // �C���[�W�̍���

		// �������̈�����b�N
		pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
		auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

		if(TRUE)
		{
			// 1���C��������̃T�C�Y
			const_uint32_t hsize     = CALC_HSIZE(width, 24);
			const_uint32_t hsize_RAS = CALC_BOUND(hsize, BOUND_RAS);
			std::valarray<uint8_t> buffer(hsize_RAS);

			// �C���[�W��z��Ɋi�[
			for(uint32_t y = 0; y < height; y++)
			{
				ReadData(hFile, &buffer[0], buffer.size(), IR_IMAGEDESTROYED);

				uint8_ptr_t ptr = &buffer[0];
				for(uint32_t x = 0; x < width; x++)
				{
					pixel_rgb8_ptr->el.b = *ptr++;
					pixel_rgb8_ptr->el.g = *ptr++;
					pixel_rgb8_ptr->el.r = *ptr++;
					pixel_rgb8_ptr++;
				}
			}
		}
	}

	// ��O����
	catch(std::bad_alloc)
	{
		throw IR_NOMEMORY;
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// �V���A���C�[�[�V����
void read_RASTERFILEHEADER(HANDLE hFile, RASTERFILEHEADER *header)
{
	// �w�b�_��ǂݍ���
	uint8_t buffer[sizeofRASTERFILEHEADER];
	uint8_ptr_t p = buffer;
	ReadData(hFile, buffer, SIZE_OF_ARRAY(buffer), IR_INCORRECTHEADER);

	// �\���̂֊i�[
	GET_BUFFER(p, header->rfMagic);
	GET_BUFFER(p, header->rfWidth);
	GET_BUFFER(p, header->rfHeight);
	GET_BUFFER(p, header->rfBits);
	GET_BUFFER(p, header->rfSize);
	GET_BUFFER(p, header->rfType);
	GET_BUFFER(p, header->rfMaptype);
	GET_BUFFER(p, header->rfMapsize);
}

END_NAMESPACE()
