// gif_decode.cpp
#include "../../stdafx.h"
#include "../iotemplate.h"              // ReadData()
#include "gif_decode.h"

#include "lzr/GIFDecode.h"

#include <io.h>                         // filelength()
#include <stdio.h>                      // FILE, fgetc(), fread()
#include <string.h>                     // strncmp()

#undef max
#undef min
#include <valarray>


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	void read_header_global(HANDLE hFile, GIFHEADER_GLOBAL &header);
	void read_header_local (HANDLE hFile, GIFHEADER_LOCAL  &header);
	void read_extra_block_all(HANDLE hFile, GIFEXTRA &extra, GIFEXTRA_NETSCAPE &extra_netscape);

	// �g���u���b�N��1�ǂݍ���
	template<typename _TExtra>
	void read_extra_block(HANDLE hFile, const_uint8_t introducer, const_uint8_t label, _TExtra &extra)
	{
		extra.introducer = introducer;
		extra.label      = label;

		// �\���̂̎c��̃����o��ǂݍ���
		if(TRUE)
		{
			uint8_t buffer[sizeof(_TExtra) - offsetof(_TExtra, block_size)];

			ReadData(
				hFile,
				buffer, SIZE_OF_ARRAY(buffer),
				IR_IMAGEDESTROYED);

			memcpy(&extra.block_size, buffer, sizeof(buffer));
		}
	}
END_NAMESPACE()


// �w�b�_����ǂݍ���
void gif_decode_header(HANDLE hFile, IMAGE_INFO *info_ptr, GIFDECODEDATA &data)
{
	// �O���[�o����ʋL�q�����擾
	GIFHEADER_GLOBAL header;
	read_header_global(hFile, header);

	const GIFHEADER_PACKET packet = {header.packet};

	// ����GIF�̃J���[�r�b�g���i8�r�b�g�ȉ��j
	const int depth = packet.el.depth + 1;
	const int depth_array[] = {1, 2, 4, 4, 8, 8, 8, 8};

	// IMAGE_INFO�\���̂̐ݒ�
	info_ptr->width      = header.width;
	info_ptr->height     = header.height;
	info_ptr->depth      = depth_array[depth - 1];
	info_ptr->mode       = COLORMODE_INDEX;
	info_ptr->alpha      = FALSE;
	info_ptr->colors     = 0;
	info_ptr->density_x  = 0;
	info_ptr->density_y  = 0;
	info_ptr->gamma_file = GAMMA_DEFAULT_FILE;
	info_ptr->comment    = NULL;

	// �p���b�g��ǂݍ���
	if(packet.el.palette)
	{
		std::valarray<GIFRGB> gif_palette(1 << depth);
		ReadData(hFile, &gif_palette[0], gif_palette.size(), IR_IMAGEDESTROYED);

		// �p���b�g�̐ݒ�
		for(size_t i = 0; i < gif_palette.size(); i++)
		{
			data.palette[i].r = gif_palette[i].red;
			data.palette[i].g = gif_palette[i].green;
			data.palette[i].b = gif_palette[i].blue;
			data.palette[i].a = 0xff;
		}
	}

	// GIF�g���u���b�N��ǂݍ���
	if(TRUE)
	{
		GIFEXTRA extra;
		GIFEXTRA_NETSCAPE extra_netscape;
		read_extra_block_all(hFile, extra, extra_netscape);

		// �g���u���b�N���ǂݍ��܂ꂽ�ꍇ
		if(extra.block_size > 0)
		{
			const GIFEXTRA_PACKET extra_packet = {extra.packet};

			// �����F�̐ݒ�
			if(extra_packet.el.transparent_flag && packet.el.palette)
			{
				info_ptr->alpha = TRUE;
				data.palette[extra.transparent_index].a = 0x00;
			}
		}
	}

	data.header = header;
}

// �C���[�W����ǂݍ���
void gif_decode_image(HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, GIFDECODEDATA &data, callback_t callback, long callback_data)
{
	// �������̈�����b�N
	uint8_ptr_t    index8_ptr = NULL;
	palette8_ptr_t palette_ptr = NULL;
	uint32_t       index_size = 0, palette_count = 0;
	auto_lock lock_obj(data_ptr, index8_ptr, palette_ptr, &index_size, &palette_count);

	// �O���[�o���w�b�_�̃p�P�b�g���
	const GIFHEADER_PACKET packet = {data.header.packet};
	const int depth = packet.el.depth + 1;

	// �p���b�g���R�s�[
	if(packet.el.palette)
	{
		memcpy(palette_ptr, data.palette, sizeof(palette_ptr[0]) * palette_count);
	}

	// �ꖇ�ڂ̃C���[�W�L�q�����擾
	GIFHEADER_LOCAL header;
	read_header_local(hFile, header);

	// ���[�J���p���b�g��ǂݍ���
	const GIFIMAGE_PACKET packet_image = {header.packet};
	if(packet_image.el.palette)
	{
		GIFRGB gif_palette[256];
		ReadData(hFile, &gif_palette[0], SIZE_OF_ARRAY(gif_palette), IR_IMAGEDESTROYED);
		for(int i = 0; i < 1 << depth; i++)
		{
			palette_ptr[i].r = gif_palette[i].red;
			palette_ptr[i].g = gif_palette[i].green;
			palette_ptr[i].b = gif_palette[i].blue;
			palette_ptr[i].a = 0xff;
		}
	}

	// ���k�f�[�^��ǂݍ���
	if(TRUE)
	{
		const DWORD gif_size = ::GetFileSize(hFile, NULL) - ::SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		std::valarray<BYTE> gif_data(gif_size);
		ReadData(hFile, &gif_data[0], gif_data.size(), IR_IMAGEDESTROYED);

		// �w�i�F�œh��Ԃ�
		BYTE bgc = 0;
		for(int i = 0; i < 8; i += info_ptr->depth)
		{
			bgc <<= info_ptr->depth;
			bgc  |= data.header.bgcolor;
		}
		memset(index8_ptr, bgc, index_size);

		// GIF�W�J�p�֐��R�[��
		GifDecode(
			index8_ptr, &gif_data[0],
			info_ptr->width, info_ptr->height, info_ptr->depth,
			header.left, header.top, header.width, header.height,
			packet_image.el.interlace, gif_size);
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

void read_header_global(HANDLE hFile, GIFHEADER_GLOBAL &header)
{
	// �O���[�o����ʋL�q�����擾
	ReadData(hFile, &header, 1, IR_INCORRECTHEADER);

	// signature���`�F�b�N
	if(strncmp(header.signature, GIF_SIGNATURE_GIF87A, GIF_SIGNATURE_SIZE) != 0
	&& strncmp(header.signature, GIF_SIGNATURE_GIF89A, GIF_SIGNATURE_SIZE) != 0)
	{
		throw IR_INCORRECTHEADER;
	}
}

void read_header_local(HANDLE hFile, GIFHEADER_LOCAL &header)
{
	ReadData(hFile, &header, 1, IR_INCORRECTHEADER);
}

// GIF�g���u���b�N��ǂݍ���
void read_extra_block_all(HANDLE hFile, GIFEXTRA &extra, GIFEXTRA_NETSCAPE &extra_netscape)
{
	extra.block_size = 0;
	extra_netscape.block_size = 0;

	uint8_t ch = 0;
	while(ReadData(hFile, &ch, 1, IR_IMAGEDESTROYED), ch != GIF_SPLIT)
	{
		if(ch != GIF_INTRODUCER)
		{
			throw IR_IMAGEDESTROYED;
		}

		uint8_t label = 0;
		ReadData(hFile, &label, 1, IR_IMAGEDESTROYED);
		switch(label)
		{
		case GIF_LABEL_NETSCAPE:                // NETSCAPE 2.0 �A�v���P�[�V�����G�N�X�e���V����
			read_extra_block(hFile, ch, label, extra_netscape);
			break;

		case GIF_LABEL_EXTRA:                   // �O���t�B�b�N�R���g���[���G�N�X�e���V����
			read_extra_block(hFile, ch, label, extra);
			break;

		default:                                // ����ȊO�̃u���b�N�͖�������
			uint8_t block_size = 0;
			while(ReadData(hFile, &block_size, 1, IR_IMAGEDESTROYED), block_size != GIF_TERMINATOR)
			{
				// �u���b�N�T�C�Y���ǂݔ�΂�
				::SetFilePointer(hFile, block_size, NULL, FILE_CURRENT);
			}
			break;
		}
	}

	// 1�o�C�g�ǂݖ߂�
	::SetFilePointer(hFile, -1, NULL, FILE_CURRENT);
}

END_NAMESPACE()
