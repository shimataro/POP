// CheckBoxCtrl.cpp �c�C���v�������e�[�V�����t�@�C��
#include "CheckBoxCtrl.h"


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// �\�z

// �R���X�g���N�^
checkbox_ctrl::checkbox_ctrl(void)
{
}

// �f�X�g���N�^
checkbox_ctrl::~checkbox_ctrl(void)
{
}


////////////////////////////////////////////////////////////////////////////////
// ���

UINT checkbox_ctrl::GetState(void) const
{
	return SendMessage(BM_GETSTATE);
}

void checkbox_ctrl::SetState(BOOL bHighlight)
{
	SendMessage(BM_SETSTATE, bHighlight);
}


////////////////////////////////////////////////////////////////////////////////
// �I���^�I�t

int checkbox_ctrl::GetCheck(void) const
{
	return SendMessage(BM_GETCHECK);
}

void checkbox_ctrl::SetCheck(int nCheck)
{
	SendMessage(BM_SETCHECK, nCheck);
}


////////////////////////////////////////////////////////////////////////////////
// �{�^���X�^�C��

void checkbox_ctrl::SetStyle(UINT nStyle, BOOL bRedraw /* = TRUE */)
{
	SendMessage(BM_SETSTYLE, LOWORD(nStyle), MAKELPARAM(bRedraw, 0));
}

_WGC_END                                // }
