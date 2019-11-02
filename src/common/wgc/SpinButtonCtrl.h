// SpinButtonCtrl.h �c�X�s���{�^���R���g���[���̃T�u�N���X��
#ifndef __WGC_SPINBUTTONCTRL__
#define __WGC_SPINBUTTONCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class spinbutton_ctrl : public control
{
public:
	////////////////////////////////////////////////////////////////
	// �R���X�g���N�^�^�f�X�g���N�^
	explicit spinbutton_ctrl(void);
	virtual ~spinbutton_ctrl(void);

	// �A�N�Z�����[�V����
	UINT GetAccel(int nAccel, UDACCEL *pAccel) const;
	BOOL SetAccel(int nAccel, UDACCEL *pAccel);

	// ��l
	int GetBase(void) const;
	int SetBase(int nBase);

	// �o�f�B�E�C���h�E
	HWND GetBuddy(void) const;
	HWND SetBuddy(HWND hWndBuddy);

	// �ʒu
	int GetPos(void) const;
	int SetPos(int nPos);

	// �͈�
	DWORD GetRange(void) const;
	void GetRange(int &lower, int &upper) const;
	void GetRange32(int &lower, int &upper) const;

	void SetRange(int nLower, int nUpper);
	void SetRange32(int nLower, int nUpper);
};

_WGC_END                                // }

#endif // __WGC_SPINBUTTONCTRL__
