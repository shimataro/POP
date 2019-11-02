//  gif_common.h �c
#ifndef __GIF_COMMON__
#define __GIF_COMMON__


// �V�O�l�`��
#define GIF_SIGNATURE_GIF87A    "GIF87a"
#define GIF_SIGNATURE_GIF89A    "GIF89a"
#define GIF_SIGNATURE_SIZE      6

// �w�b�_�̒萔
#define GIF_INTRODUCER      0x21
#define GIF_LABEL_EXTRA     0xf9
#define GIF_LABEL_NETSCAPE  0xff
#define GIF_TERMINATOR      0x00
#define GIF_SPLIT           0x2c


/*--------------------------------------------------------------------------------------*/
/*	GIFHEADER�\����																		*/
/*	GIF�̉�ʋL�q���\����																*/
/*	��GIF�S�̂̏����L�q�����w�b�_�[���B												*/
/*--------------------------------------------------------------------------------------*/
#include <pshpack1.h>                   // �\���̂̃����o���E��1�o�C�g�ɂ���
typedef struct GIFHEADER_GLOBAL
{
	char signature[6];                      // GIF���ʋL���i"GIF87a" or "GIF89a"�j
	WORD width;                             // ����GIF�S�̂�\������̂ɕK�v�ȉ�pixel��
	WORD height;                            // ����GIF�S�̂�\������̂ɕK�v�ȏcpixel��
	BYTE packet;                            // �p�P�b�g�t�B�[���h�i��q��GIFHEADER_PACKET�j
	BYTE bgcolor;                           // �w�i�F
	BYTE aspect;                            // �A�X�y�N�g��
} GIFHEADER_GLOBAL;
#include <poppack.h>

typedef union
{
	BYTE value;
	struct elements
	{
		unsigned depth      : 3;                // �r�b�g�[�x-1
		unsigned sort       : 1;                // �\�[�g�t���O
		unsigned resolution : 3;                // �𑜓x�i�r�b�g��-1�j
		unsigned palette    : 1;                // �p���b�g�t���O
	} el;
} GIFHEADER_PACKET;


/*--------------------------------------------------------------------------------------*/
/*	GIFEXTRA_NETSCAPE�\����																*/
/*	GIF�̃A�v���P�[�V�����G�N�X�e���V�����\����											*/
/*	��GIF�̊g�����																		*/
/*--------------------------------------------------------------------------------------*/
#include <pshpack1.h>                   // �\���̂̃����o���E��1�o�C�g�ɂ���
typedef struct GIFEXTRA_NETSCAPE
{
	BYTE introducer;                        // GIF_INTRODUCER
	BYTE label;                             // GIF_LABEL_NETSCAPE

	BYTE block_size;                        // 11
	char identifier[8];                     // "NETSCAPE"
	char authentication[3];                 // "2.0"
	BYTE block_size2;                       // 3
	BYTE reserved;                          // 0x01
	WORD iteration_count;                   // �J��Ԃ���
	BYTE terminator;                        // GIF_TERMINATOR
} GIFEXTRA_NETSCAPE;
#include <poppack.h>


/*--------------------------------------------------------------------------------------*/
/*	GIFEXTRA�\����																		*/
/*	GIF�̃G�N�X�e���V�����\����															*/
/*	��GIF�̊g�����																		*/
/*--------------------------------------------------------------------------------------*/
#include <pshpack1.h>                   // �\���̂̃����o���E��1�o�C�g�ɂ���
typedef struct GIFEXTRA
{
	BYTE introducer;                        // GIF_INTRODUCER
	BYTE label;                             // GIF_LABEL_EXTRA

	BYTE block_size;                        // 4
	BYTE packet;                            // �p�P�b�g�t�B�[���h�i��q�� GIFEXTRA_PACKET�j
	WORD delay;                             // �f�B���C�^�C���i1/100�b�j
	BYTE transparent_index;                 // ���ߐF�̃C���f�b�N�X
	BYTE terminator;                        // GIF_TERMINATOR
} GIFEXTRA;
#include <poppack.h>

typedef union
{
	BYTE value;
	struct elements
	{
		unsigned transparent_flag : 1;          // ���߃t���O
		unsigned input            : 1;          // ���[�U���̓t���O
		unsigned disposal_method  : 3;          // �����@�i0:����`�A1:�������Ȃ��A2:�w�i�F�ŃN���A�A3:�\�����O�̃C���[�W�ɖ߂��j
		unsigned reserved         : 3;          // �\��i���g�p�j
	} el;
} GIFEXTRA_PACKET;


/*--------------------------------------------------------------------------------------*/
/*	GIFHEADER_LOCAL�\����																*/
/*	GIF�̃C���[�W�L�q���\����															*/
/*	��GIF�摜�ꖇ�ɂ��Ă̏����L�q�������́B											*/
/*--------------------------------------------------------------------------------------*/
#include <pshpack1.h>                   // �\���̂̃����o���E��1�o�C�g�ɂ���
typedef struct GIFHEADER_LOCAL
{
	BYTE split;                             // �C���[�W�����L���i=0x2C�j

	WORD left;                              // ��ʍ��ォ��̕\���ʒu�i��pixel���j
	WORD top;                               // ��ʍ��ォ��̕\���ʒu�i�cpixel���j
	WORD width;                             // ���̉摜�̉�pixel��
	WORD height;                            // ���̉摜�̏cpixel��
	BYTE packet;                            // �p�P�b�g�t�B�[���h�i��q�� GIFIMAGE_PACKET�j
} GIFHEADER_LOCAL;
#include <poppack.h>

typedef union
{
	BYTE value;
	struct elements
	{
		unsigned depth     : 3;                 // �r�b�g�[�x-1
		unsigned reserved  : 2;                 // �\��
		unsigned sort      : 1;                 // �\�[�g�t���O
		unsigned interlace : 1;                 // �C���^�[���[�X�t���O
		unsigned palette   : 1;                 // �p���b�g�t���O
	} el;
} GIFIMAGE_PACKET;


/*--------------------------------------------------------------------------------------*/
/*	GIFRGB�\����																		*/
/*	GIF�̃p���b�g�\����																	*/
/*	��RGB���̕��тłR�o�C�g�\���B														*/
/*--------------------------------------------------------------------------------------*/
#include <pshpack1.h>                   // �\���̂̃����o���E��1�o�C�g�ɂ���
typedef struct GIFRGB
{
	BYTE red;                               // �Ԑ���
	BYTE green;                             // �ΐ���
	BYTE blue;                              // ����
} GIFRGB;
#include <poppack.h>


#endif // __GIF_COMMON__
