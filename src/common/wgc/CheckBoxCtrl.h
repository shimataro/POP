// CheckBoxCtrl.h �c�`�F�b�N�{�b�N�X�R���g���[���̃T�u�N���X��
#ifndef __WGC_CHECKBOXCTRL__
#define __WGC_CHECKBOXCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class checkbox_ctrl : public control
{
public:
	// �R���X�g���N�^�^�f�X�g���N�^
	explicit checkbox_ctrl(void);
	virtual ~checkbox_ctrl(void);

	// ���
	UINT GetState(void) const;
	void SetState(BOOL bHighlight);

	// �I���^�I�t
	int GetCheck(void) const;
	void SetCheck(int nCheck);

	// �{�^���X�^�C��
	void SetStyle(UINT nStyle, BOOL bRedraw = TRUE);
};

_WGC_END                                // }

#endif // __WGC_CHECKBOXCTRL__
