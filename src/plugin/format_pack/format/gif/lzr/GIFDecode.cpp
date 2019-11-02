/*--------------------------------------------------------------------------------------*/
/*	��LZW���_GIF�f�R�[�_�[�{�� �����t�@�C��	Ver 1.20									*/
/*	for VisualStudio6.0 only															*/
/*	�����ۂ̓W�J�������s���ADIB�Ƃ��ĕԋp���܂��B										*/
/*	Copyright (C) 2000 DJ��Uchi [H.Uchida]												*/
/*--------------------------------------------------------------------------------------*/
#include "GIFDecode.h"
// #pragma inline_depth(255)
// #pragma inline_recursion(on)


// from "image.h"
#define CALC_HSIZE(width, bit)              (((width) * (bit) + 7) / 8)
#define CALC_PACK_OFFSET(pos, log2_depth)   ((pos) >> (~(log2_depth) & 0x03))
#define CALC_PACK_SHIFTS(pos, log2_depth)   ((~(pos) << (log2_depth)) & 0x07)

#define BEGIN_NAMESPACE(name)       namespace name {
#define BEGIN_NAMESPACE_NONAME()    namespace {
#define END_NAMESPACE()             }


/*--------------------------------------------------------------------------------------*/
/*	RGBDECODESTATUS�\����																*/
/*	DIB�f�[�^�������ݗp�\����															*/
/*	��DIB�ɉ摜�f�[�^���������ލۂɕK�v�ȏ����܂Ƃ߂����B								*/
/*--------------------------------------------------------------------------------------*/
typedef struct RGBDECODESTATUS
{
	DWORD rgb_pt;                           // �W�J�f�[�^�������݈ʒu�i���ꃉ�C����ł̃I�t�Z�b�g�l�j
	DWORD rgb_offset;                       // �W�J�f�[�^�������݃I�t�Z�b�g�i���C�����~�A���C�������g�j
	DWORD rgb_width;                        // DIB��pixel��
	DWORD rgb_height;                       // DIB�cpixel��
	DWORD rgb_hsize;                        // DIB�A���C�������g�l�i�P���C�����̃o�C�g���j
	DWORD rgb_depth;                        // DIB�J���[�r�b�g�l
	DWORD rgb_line;                         // �W�J�f�[�^�������݃��C���i�C���^���[�X���Ɏg�p�j
	DWORD image_left;                       // �摜�̃��W�J���X�N���[����ɂ����鍶��̍��W
	DWORD image_top;                        // �V
	DWORD image_width;                      // �摜�̕�
	DWORD image_height;                     // �摜�̍���
	DWORD image_offset;
	DWORD interlace_offset;                 // �C���^���[�X�I�t�Z�b�g�i���C�����j
	BOOL  interlace_flag;                   // �C���^���[�X�t���O
} RGBDECODESTATUS;


/*--------------------------------------------------------------------------------------*/
/*	GIFDECODESTATUS�\����																*/
/*	GIF�f�[�^��͗p�\����																*/
/*	��GIF�̕������R�[�h�擾�ɕK�v�ȏ����܂Ƃ߂����B									*/
/*--------------------------------------------------------------------------------------*/
typedef struct GIFDECODESTATUS
{
	RGBDECODESTATUS *rs;                    // RGBDECODESTATUS�\���̎Q�Ɨp�|�C���^
	DWORD code_size;                        // �R�[�h�T�C�Y�iCS�j
	DWORD bit_size;                         // �r�b�g�T�C�Y�iCBL�j
	DWORD clear_code;                       // �N���A�R�[�h
	DWORD end_code;                         // �G���h�R�[�h
	DWORD entry;                            // ���G���g����
	DWORD bit_pt;                           // ���r�b�g�ʒu
	DWORD next_block;                       // ���u���b�N�J�n�ʒu
	DWORD data_size;                        // �f�[�^�T�C�Y
} GIFDECODESTATUS;


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	DWORD GetCode(const BYTE data[], GIFDECODESTATUS *gs);
	void lzw_decode(BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, const int x, const int y);
	void lzw_string(BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, int x);
	void lzw_char  (BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, int x);
	void rgb_write (BYTE buffer[], RGBDECODESTATUS *rs, const WORD code);
END_NAMESPACE()


/*--------------------------------------------------------------------------------------*/
/*	GifDecode()																			*/
/*	GIF�摜�i�ŏ��̈ꖇ�̂݁j��W�J����B												*/
/*	�����͂�GIF�f�[�^�A�o�͂�index�i�g�b�v�_�E���^�A���C�������g�Ȃ�DIB�j				*/
/*	�������ł͎�Ƀf�R�[�h�̑O�������s���܂��B											*/
/*--------------------------------------------------------------------------------------*/
DWORD GifDecode(
	BYTE buffer[], const BYTE data[],
	const DWORD width, const DWORD height, const WORD depth,
	const DWORD left, const DWORD top, const DWORD image_width, const DWORD image_height,
	const BOOL interlace, const DWORD size)
{
	const DWORD hsize = CALC_HSIZE(width, depth);

	// �f�[�^�������ݗp�\���̏�����
	RGBDECODESTATUS rs;
	rs.rgb_pt     = 0;                      // �W�J�f�[�^�������݈ʒu������
	rs.rgb_offset = 0;                      // �������݈ʒu�I�t�Z�b�g
	rs.rgb_width  = width;                  // ��pixel���擾
	rs.rgb_height = height;                 // �cpixel���擾
	rs.rgb_hsize  = hsize;                  // �A���C�������g�l�擾
	rs.rgb_depth  = depth;                  // �J���[�r�b�g�l�擾
	rs.rgb_line   = 0;                      // �W�J�f�[�^�������݃��C��������
	rs.image_left   = left;
	rs.image_top    = top;
	rs.image_width  = image_width;
	rs.image_height = image_height;
	rs.image_offset = hsize * top;
	rs.interlace_offset = 8;                // �C���^���[�X�I�t�Z�b�g������
	rs.interlace_flag   = interlace;        // �C���^���[�X�t���O�擾

	// GIF�f�[�^��͗p�\���̏�����
	GIFDECODESTATUS gs;
	gs.rs         = &rs;                    // RGBDECODESTATUS�\���̎Q�Ɨp�|�C���^�擾
	gs.code_size  = data[0];                // �R�[�h�T�C�Y�擾
	gs.bit_size   = gs.code_size + 1;       // CBL�������i�R�[�h�T�C�Y+1�j
	gs.clear_code = 1 << gs.code_size;      // �N���A�R�[�h�ݒ�i2^�R�[�h�T�C�Y�j
	gs.end_code   = gs.clear_code + 1;      // �G���h�R�[�h�ݒ�i2^�R�[�h�T�C�Y+1�j
	gs.entry      = gs.end_code + 1;        // �G���g�����������i2^�R�[�h�T�C�Y+2�j
	gs.bit_pt     = 8;                      // �r�b�g�|�C���^������(8bit=1byte)
	gs.next_block = 1;                      // ���u���b�N�ʒu�������i�擪�u���b�N�j
	gs.data_size  = size;                   // GIF�f�[�^�T�C�Y�擾

	int x = 0;								// �W�J�񐔏�����
	int y = 0;								// �W�J�J�n�ʒu������

	WORD lzw[8192];                         // ���k�f�[�^�i�[�p�z��i�����e�[�u���ł͂Ȃ��I�j
	DWORD code = 0;                         // �������R�[�h������

	// �G���h�R�[�h�������܂łЂ����烋�[�v
	while((code = GetCode(data, &gs)) != gs.end_code)
	{
		// �N���A�R�[�h�����ꂽ�ꍇ
		if(code == gs.clear_code)
		{
			// �G���g������CBL������
			gs.entry = gs.end_code + 1;
			gs.bit_size = gs.code_size + 1;

			// �W�J�֐����R�[���B
			lzw_decode(buffer, lzw, &gs, x, y);
			x = 0;
			y = 0;
		}
		else
		{
			// WORD�T�C�Y�z��Ɏ擾�����������R�[�h�𗭂ߍ��ށB
			// ����͓W�J��Ƃ����~���ɍs���ׂł���A�����e�[�u���Ƃ͈قȂ�܂��B
			// �z��ɗ��ߍ��܂��Ɏ擾�����������R�[�h�𒼂ɓW�J���邱�Ƃ��o���܂����A
			// ���x�I�ɖ�肪����ׁA���̕��@���̗p���Ă��܂��B
			lzw[x++] = (WORD)code;

			// ���܂ł��N���A�R�[�h������Ȃ��Ɣz�񂪈��邽�߁A
			// �������R�[�h��8192���܂������_�œW�J��Ƃ��s���B
			// �񈳏kGIF�΍􂩁H
			if(x == 8192)
			{
				lzw_decode(buffer, lzw, &gs, x, y);
				x = 4096;
				y = 4096;
			}
			if(gs.entry < 4095)
			{
				gs.entry++;
			}
		}
	}
	// �z��Ɏc���Ă��镄�����R�[�h��W�J����B
	lzw_decode(buffer, lzw, &gs, x, y);

	// ���̃f�[�^�ʒu��Ԃ�
	return (gs.bit_pt + 7) / 8;
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

/*--------------------------------------------------------------------------------------*/
/*	GetCode()																			*/
/*	�σr�b�g�����͊֐��B																*/
/*	���������R�[�h������o���āA�r�b�g�ʒu���C���N�������g����B					*/
/*	�����o�����f�[�^��WORD�T�C�Y�̔z��Ɉ��ʒ��߂Ă���f�R�[�h����B				*/
/*--------------------------------------------------------------------------------------*/
DWORD GetCode(const BYTE data[], GIFDECODESTATUS *gs)
{
	DWORD code = 0;					// �������R�[�h������
	DWORD pt = gs->bit_pt >> 3;		// �ǂݍ��݈ʒu�i�o�C�g�P�ʁj�擾

	// �T�C�Y�I�[�o�[�t���[�̏ꍇ�A�����I�ɃG���h�R�[�h��Ԃ��i�j���t�@�C���΍�j
	if(pt + 2 > gs->data_size)
	{
		return gs->end_code;
	}

	// �������R�[�h�擾
	int i = 0;	// �ǂݍ��݃o�C�g��������
	while((gs->bit_pt + gs->bit_size - 1) >> 3 >= pt)
	{
		// �ǂݍ��ݒ��Ƀu���b�N���I�������ꍇ
		if(pt == gs->next_block)
		{
			gs->next_block += data[pt++] + 1;   // ���u���b�N�ʒu�X�V
			gs->bit_pt += 8;                    // �r�b�g�|�C���^���P�o�C�g�����Z
		}
		code += data[pt++] << i;            // �R�[�h�擾
		i += 8;
	}

	// ����ꂽ�R�[�h�̗]���ȃr�b�g��؂�Ƃ΂��B�i�}�X�L���O�����j
	code = (code >> (gs->bit_pt & 0x07)) & ((1 << gs->bit_size) - 1);

	// �r�b�g�|�C���^�X�V
	gs->bit_pt += gs->bit_size;

	// CBL���C���N�������g����K�v�����邩�ǂ����m�F����B
	if(gs->entry > (DWORD)((1 << gs->bit_size) - 1))
	{
		gs->bit_size++;
	}

	return code;
}


/*--------------------------------------------------------------------------------------*/
/*	lzw_decode()																		*/
/*	��LZW���_�W�J�֐��i���C�����[�v�j													*/
/*	��WORD�T�C�Y�̔z��Ɋi�[���ꂽ�������R�[�h���f�R�[�h���܂��B						*/
/*--------------------------------------------------------------------------------------*/
void lzw_decode(BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, const int x, const int y)
{
	// �P�Ƀ��[�v���񂵂ēW�J�֐����Ă�ł邾���B
	for (int i = y; i < x; i++)
	{
		lzw_string(buffer, lzw, gs, i);
	}
}


/*--------------------------------------------------------------------------------------*/
/*	lzw_string()																		*/
/*	��LZW���_�W�J�֐��i�R�A�j															*/
/*	����LZW���_�̊j�B																	*/
/*	���w�肳�ꂽ�������R�[�h�ɑ΂���W�J�f�[�^��Ԃ��܂��B								*/
/*--------------------------------------------------------------------------------------*/
void lzw_string(BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, int x)
{
	// �z�񂩂畄�����R�[�h������o���܂��B
	WORD code = lzw[x];

	// �������R�[�h���h�F���h��菬�����ꍇ
	if(code < gs->clear_code)
	{
		// �������R�[�h�����̂܂�DIB�ɏ������ށB
		rgb_write(buffer, gs->rs, code);
	}
	// �������R�[�h�����m�̂��̂ł���ꍇ
	else if(code > gs->end_code + x--)
	{
		// ��O�̓W�J�f�[�^
		lzw_string(buffer, lzw, gs, x);

		// ��O�̓W�J�f�[�^�̐擪���
		lzw_char(buffer, lzw, gs, x);
	}
	// �������R�[�h���h�F��+1�h���傫���ꍇ
	// ���Ȃ݂ɁA���̊֐��ɓ����Ă���R�[�h�ɃG���h�R�[�h��N���A�R�[�h��
	// ��΂Ɍ���܂���̂ŁA���̏ꍇ�̏����͍l������Ă��܂���B
	else
	{
		// �i�������R�[�h-�F��+1�j�̓W�J�f�[�^
		lzw_string(buffer, lzw, gs, code - gs->end_code - 1);

		// �i�������R�[�h-�F��+2�j�̓W�J�f�[�^�̐擪���
		lzw_char(buffer, lzw, gs, code - gs->end_code);
	}
}


/*--------------------------------------------------------------------------------------*/
/*	lzw_char()																			*/
/*	��LZW���_�W�J�֐��i�T�u�j															*/
/*	����LZW���_�̊j�B																	*/
/*	���w�肳�ꂽ�������R�[�h�ɑ΂���W�J�f�[�^�̐擪�P��Ԃ��܂��B					*/
/*--------------------------------------------------------------------------------------*/
void lzw_char(BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, int x)
{
	WORD code;
	// �z�񂩂畄�����R�[�h������o���A�h�F���h��菬�����ǂ����m�F�B
	while((code = lzw[x]) >= gs->clear_code)
	{
		// �������R�[�h�����m�̂��̂ł���ꍇ
		if(code > gs->end_code + x)
		{
			// ��O�̓W�J�f�[�^�̐擪���
			x--;
		}
		// �������R�[�h���h�F��+1�h���傫���ꍇ
		else
		{
			// �i�������R�[�h-�F��+1�j�̓W�J�f�[�^�̐擪���
			x = code - gs->end_code - 1;
		}
	}

	// ����ꂽ�W�J�f�[�^��DIB�ɏ������ށB
	rgb_write(buffer, gs->rs, code);
}


/*--------------------------------------------------------------------------------------*/
/*	rgb_write()																			*/
/*	DIB�摜�f�[�^�������݊֐��B															*/
/*	���W�J���ꂽ�摜�f�[�^��DIB�Ƃ��ď������݂܂��B										*/
/*--------------------------------------------------------------------------------------*/
void rgb_write(BYTE buffer[], RGBDECODESTATUS *rs, const WORD code)
{
	// log2(depth)���v�Z
	const int log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const int log2_depth = log2_depth_array[rs->rgb_depth - 1];

	// �I�t�Z�b�g�ƃV�t�g�ʂ����߂�
	const int x = rs->image_left + rs->rgb_pt++;
	const int offset = rs->image_offset + rs->rgb_offset + CALC_PACK_OFFSET(x, log2_depth);
	const int shifts = CALC_PACK_SHIFTS(x, log2_depth);

	// �}�X�N�l�����߂�
	const BYTE mask_array[] = {0x01, 0x03, 0x0f, 0xff}; // 00000001, 00000011, 00001111, 11111111
	const BYTE mask = ~(mask_array[log2_depth] << shifts);

	// code���p�b�L���O���ď�������
	buffer[offset] &= mask;
	buffer[offset] |= code << shifts;

	// �������݈ʒu�����C���̏I�[�ɒB�����ꍇ
	if(rs->rgb_pt == rs->image_width)
	{
		// �C���^���[�XGIF�̏ꍇ
		if(rs->interlace_flag)
		{
			// �C���^���[�X���C������ʉ��[�ɒB�����ꍇ
			if(rs->rgb_line + rs->interlace_offset >= rs->image_height)
			{
				if((rs->rgb_line & 0x07) == 0)
				{
					rs->rgb_offset = rs->rgb_hsize * 4;
					rs->rgb_line   = 4;
				}
				else
				{
					switch(rs->interlace_offset)
					{
					case 8:
						rs->rgb_offset = rs->rgb_hsize * 2;
						rs->rgb_line   = 2;
						rs->interlace_offset = 4;
						break;

					case 4:
						rs->rgb_offset = rs->rgb_hsize * 1;
						rs->rgb_line   = 1;
						rs->interlace_offset = 2;
						break;
					}
				}
			}
			else
			{
				rs->rgb_offset += rs->rgb_hsize * rs->interlace_offset;
				rs->rgb_line   += rs->interlace_offset;
			}
		}
		// ���j�AGIF�̏ꍇ
		else
		{
			rs->rgb_offset += rs->rgb_hsize;
		}
		rs->rgb_pt = 0;
	}
}

END_NAMESPACE()
