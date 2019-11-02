/*--------------------------------------------------------------------------------------*/
/*	��LZW���_GIF�f�R�[�_�[�{�� �w�b�_�t�@�C��	Ver 1.20								*/
/*	for VisualStudio6.0 only															*/
/*	���W�J�p�p�����[�^�i�[�\���̂Ɗ֐��̃v���g�^�C�v�錾�B								*/
/*	Copyright (C) 2000 DJ��Uchi [H.Uchida]												*/
/*--------------------------------------------------------------------------------------*/
#ifndef __GIF_LZR__
#define __GIF_LZR__

#include <windows.h>
#include <stdio.h>


/*--------------------------------------------------------------------------------------*/
/*	GifDecode()																			*/
/*	GIF�摜�i�ŏ��̈ꖇ�̂݁j��W�J����B												*/
/*	�����͂�GIF�f�[�^�A�o�͂�index�i�g�b�v�_�E���^�A���C�������g�Ȃ�DIB�j				*/
/*	������̃v���O�C���̗v�BLZW���_���g�p���Ă��܂���B									*/
/*--------------------------------------------------------------------------------------*/
DWORD GifDecode(
	BYTE buffer[], const BYTE gif[],
	const DWORD width, const DWORD height, const WORD depth,
	const DWORD left, const DWORD top, const DWORD image_width, const DWORD image_height,
	const BOOL interlace, const DWORD size);


#endif // __GIF_LZR__
