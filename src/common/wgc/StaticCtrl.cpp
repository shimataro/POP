// StaticCtrl.cpp �c�C���v�������e�[�V�����t�@�C��
#include "StaticCtrl.h"


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// �\�z

// �R���X�g���N�^
static_ctrl::static_ctrl(void)
{
}

// �f�X�g���N�^
static_ctrl::~static_ctrl(void)
{
}


////////////////////////////////////////////////////////////////////////////////
// �A�C�R��

// ���蓖�Ă�ꂽ�A�C�R�����擾
HICON static_ctrl::GetIcon(void) const
{
	LRESULT result = SendMessage(STM_GETICON);
	return reinterpret_cast<HICON>(result);
}

// �A�C�R�������蓖��
HICON static_ctrl::SetIcon(HICON hIcon)
{
	LRESULT result = SendMessage(
		STM_SETICON,
		reinterpret_cast<WPARAM>(hIcon));

	return reinterpret_cast<HICON>(result);
}


////////////////////////////////////////////////////////////////////////////////
// �r�b�g�}�b�v

// ���蓖�Ă�ꂽ�r�b�g�}�b�v���擾
HBITMAP static_ctrl::GetBitmap(void) const
{
	return reinterpret_cast<HBITMAP>(GetImage(IMAGE_BITMAP));
}

// �r�b�g�}�b�v�����蓖��
HBITMAP static_ctrl::SetBitmap(HBITMAP hBitmap)
{
	return reinterpret_cast<HBITMAP>(SetImage(IMAGE_BITMAP, hBitmap));
}


////////////////////////////////////////////////////////////////////////////////
// �J�[�\��

// ���蓖�Ă�ꂽ�J�[�\�����擾
HCURSOR static_ctrl::GetCursor(void) const
{
	return reinterpret_cast<HCURSOR>(GetImage(IMAGE_CURSOR));
}

// �J�[�\�������蓖��
HCURSOR static_ctrl::SetCursor(HCURSOR hCursor)
{
	return reinterpret_cast<HCURSOR>(SetImage(IMAGE_CURSOR, hCursor));
}


#if(WINVER >= 0x0400)

////////////////////////////////////////////////////////////////////////////////
// �g�����^�t�@�C��
HENHMETAFILE static_ctrl::GetEnhMetaFile(void) const
{
	return reinterpret_cast<HENHMETAFILE>(GetImage(IMAGE_ENHMETAFILE));
}

HENHMETAFILE static_ctrl::SetEnhMetaFile(HENHMETAFILE hEnhMetaFile)
{
	return reinterpret_cast<HENHMETAFILE>(SetImage(IMAGE_ENHMETAFILE, hEnhMetaFile));
}

#endif


////////////////////////////////////////////////////////////////////////////////
// �C���[�W

// ���蓖�Ă�ꂽ�C���[�W���擾
HANDLE static_ctrl::GetImage(int fImageType) const
{
	LRESULT result = SendMessage(STM_GETIMAGE, fImageType);
	return reinterpret_cast<HANDLE>(result);
}

// �C���[�W���蓖��
HANDLE static_ctrl::SetImage(int fImageType, HANDLE hImage)
{
	LRESULT result = SendMessage(STM_SETIMAGE, fImageType, (LPARAM)hImage);
	return reinterpret_cast<HANDLE>(result);
}

_WGC_END                                // }
