// ScrollBarCtrl.h �c�X�N���[���o�[�R���g���[���̃T�u�N���X��
#ifndef __WGC_SCROLLBARCTRL__
#define __WGC_SCROLLBARCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class scrollbar_ctrl : public control
{
public:
	////////////////////////////////////////////////////////////////
	// �R���X�g���N�^�^�f�X�g���N�^
	explicit scrollbar_ctrl(void);
	virtual ~scrollbar_ctrl(void);

	// �X�N���[�����
	BOOL SetScrollInfo(const SCROLLINFO &si, BOOL bRedraw = TRUE);
	BOOL GetScrollInfo(SCROLLINFO &si, UINT nMask = SIF_ALL) const;

	// �y�[�W�T�C�Y
	int GetPageSize(void) const;
	int SetPageSize(int nSize);

	// �͈�
	void GetRange(int &nMin, int &nMax) const;
	void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);

	// �X�N���[���ʒu
	int GetPos(void) const;
	void SetPos(int nPos, BOOL bRedraw = TRUE);

	// 
	BOOL EnableArrow(UINT nArrowFlags = ESB_ENABLE_BOTH);

	int GetLimit(void) const;
};

_WGC_END                                // }

#endif // __WGC_SCROLLBARCTRL__
