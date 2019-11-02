// SliderCtrl.h �c�X���C�_�R���g���[���̃T�u�N���X��
#ifndef __WGC_SLIDERCTRL__
#define __WGC_SLIDERCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class slider_ctrl : public control
{
public:
	////////////////////////////////////////////////////////////////
	// �R���X�g���N�^�^�f�X�g���N�^
	explicit slider_ctrl(void);
	virtual ~slider_ctrl(void);

	// �s�̃T�C�Y
	int GetLineSize(void) const;
	int SetLineSize(int nSize);

	// �y�[�W�T�C�Y
	int GetPageSize(void) const;
	int SetPageSize(int nSize);

	// �ŏ��l
	int GetRangeMin(void) const;
	void SetRangeMin(int nMin, BOOL bRedraw = FALSE);

	// �ő�l
	int GetRangeMax(void) const;
	void SetRangeMax(int nMax, BOOL bRedraw = FALSE);

	// �͈�
	void GetRange(int &nMin, int &nMax) const;
	void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);

	// ��`
	void GetChannelRect(LPRECT lprc) const;
	void GetThumbRect(LPRECT lprc) const;

	// �܂݂̈ʒu
	int GetPos(void) const;
	void SetPos(int nPos);

	// �ڐ���}�[�N
	UINT GetNumTics(void) const;
	int  GetTic(int nTic) const;
	BOOL SetTic(int nTic);
	int  GetTicPos(int nTic) const;
	void SetTicFreq(int nFreq);

	// �o�f�B
	HWND GetBuddy(BOOL fLocation = TRUE) const;
	HWND SetBuddy(HWND hWndBuddy, BOOL fLocation = TRUE);

	// ����
	void ClearSel(BOOL bRedraw = FALSE);
	void ClearTics(BOOL bRedraw = FALSE);
};

_WGC_END                                // }

#endif // __WGC_SLIDERCTRL__
