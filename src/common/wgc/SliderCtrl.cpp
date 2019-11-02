// SliderCtrl.cpp �c�C���v�������e�[�V�����t�@�C��
#include "SliderCtrl.h"


_WGC_BEGIN                              // namespace wgc {

// �R���X�g���N�^
slider_ctrl::slider_ctrl(void)
{
}

// �f�X�g���N�^
slider_ctrl::~slider_ctrl(void)
{
}


// �s�̃T�C�Y
int slider_ctrl::GetLineSize(void) const
{
	return SendMessage(TBM_GETLINESIZE);
}

int slider_ctrl::SetLineSize(int nSize)
{
	return SendMessage(TBM_SETLINESIZE, 0, nSize);
}


// �y�[�W�T�C�Y
int slider_ctrl::GetPageSize(void) const
{
	return SendMessage(TBM_GETPAGESIZE);
}

int slider_ctrl::SetPageSize(int nSize)
{
	return SendMessage(TBM_SETPAGESIZE, 0, nSize);
}


// �ŏ��l
int slider_ctrl::GetRangeMin(void) const
{
	return SendMessage(TBM_GETRANGEMIN);
}

void slider_ctrl::SetRangeMin(int nMin, BOOL bRedraw /* = FALSE */)
{
	SendMessage(TBM_SETRANGEMIN, bRedraw, nMin);
}


// �ő�l
int slider_ctrl::GetRangeMax(void) const
{
	return SendMessage(TBM_GETRANGEMAX);
}

void slider_ctrl::SetRangeMax(int nMax, BOOL bRedraw /* = FALSE */)
{
	SendMessage(TBM_SETRANGEMAX, bRedraw, nMax);
}


// �͈�
void slider_ctrl::GetRange(int &nMin, int &nMax) const
{
	nMin = GetRangeMin();
	nMax = GetRangeMax();
}

void slider_ctrl::SetRange(int nMin, int nMax, BOOL bRedraw /* = FALSE */)
{
	SendMessage(TBM_SETRANGE, (WPARAM)bRedraw, (LPARAM)MAKELONG(nMin, nMax));
}


// ��`
void slider_ctrl::GetChannelRect(LPRECT lprc) const
{
	SendMessage(TBM_GETCHANNELRECT, 0, (LPARAM)lprc);
}

void slider_ctrl::GetThumbRect(LPRECT lprc) const
{
	SendMessage(TBM_GETTHUMBRECT, 0, (LPARAM)lprc);
}


// �܂݂̈ʒu
int slider_ctrl::GetPos(void) const
{
	return SendMessage(TBM_GETPOS, 0, 0);
}

void slider_ctrl::SetPos(int nPos)
{
	SendMessage(TBM_SETPOS, (WPARAM)TRUE, (LPARAM)nPos);
}


// �ڐ���}�[�N
UINT slider_ctrl::GetNumTics(void) const
{
	return SendMessage(TBM_GETNUMTICS);
}

int slider_ctrl::GetTic(int nTic) const
{
	return SendMessage(TBM_GETTIC, nTic);
}

BOOL slider_ctrl::SetTic(int nTic)
{
	return SendMessage(TBM_SETTIC, 0, nTic);
}

int slider_ctrl::GetTicPos(int nTic) const
{
	return SendMessage(TBM_GETTICPOS, nTic);
}

void slider_ctrl::SetTicFreq(int nFreq)
{
	SendMessage(TBM_SETTICFREQ, nFreq);
}


// �o�f�B
HWND slider_ctrl::GetBuddy(BOOL fLocation /* = TRUE */) const
{
	LRESULT result = SendMessage(TBM_GETBUDDY, fLocation);
	return reinterpret_cast<HWND>(result);
}

HWND slider_ctrl::SetBuddy(HWND hWndBuddy, BOOL fLocation /* = TRUE */)
{
	LRESULT result = SendMessage(TBM_SETBUDDY, fLocation, (LPARAM)hWndBuddy);
	return reinterpret_cast<HWND>(result);
}


// ����
void slider_ctrl::ClearSel(BOOL bRedraw /* = FALSE */)
{
	SendMessage(TBM_CLEARSEL, bRedraw);
}

void slider_ctrl::ClearTics(BOOL bRedraw /* = FALSE */)
{
	SendMessage(TBM_CLEARTICS, bRedraw);
}

_WGC_END                                // }
