// StaticCtrl.h �c�X�^�e�B�b�N�R���g���[���̃T�u�N���X��
#ifndef __WGC_STATICCTRL__
#define __WGC_STATICCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class static_ctrl : public control
{
public:
	// �R���X�g���N�^�^�f�X�g���N�^
	explicit static_ctrl(void);
	virtual ~static_ctrl(void);

	// �A�C�R��
	HICON GetIcon(void) const;              // ���蓖�Ă�ꂽ�A�C�R�����擾
	HICON SetIcon(HICON hIcon);             // �A�C�R�������蓖��

	// �r�b�g�}�b�v
	HBITMAP GetBitmap(void) const;          // ���蓖�Ă�ꂽ�r�b�g�}�b�v���擾
	HBITMAP SetBitmap(HBITMAP hBitmap);     // �r�b�g�}�b�v�����蓖��

	// �J�[�\��
	HCURSOR GetCursor(void) const;          // ���蓖�Ă�ꂽ�J�[�\�����擾
	HCURSOR SetCursor(HCURSOR hCursor);     // �J�[�\�������蓖��

#if(WINVER >= 0x0400)

	// �g�����^�t�@�C��
	HENHMETAFILE GetEnhMetaFile(void) const;
	HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hEnhMetaFile);

#endif

private:
	// �C���[�W
	HANDLE GetImage(int fImageType) const;          // ���蓖�Ă�ꂽ�C���[�W���擾
	HANDLE SetImage(int fImageType, HANDLE hImage);	// �C���[�W�����蓖��
};

_WGC_END                                // }

#endif // __WGC_STATICCTRL__
