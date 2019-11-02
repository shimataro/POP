// StatusBar.cpp �c�C���v�������e�[�V�����t�@�C��
#include "StatusBar.h"
#include "devicecontext.h"              // wgc::devicecontext
#include "wgfunc.h"                     // wgc::wgfLoadString()
#include <assert.h>                     // assert()


_WGC_BEGIN                              // namespace wgc {

// �R���X�g���N�^
statusbar::statusbar(void)
{
}

// �f�X�g���N�^
statusbar::~statusbar(void)
{
}


// �X�e�[�^�X�o�[�̍쐬
BOOL statusbar::Create(
	window *parent,
	LPCTSTR lpszWindowName,
	const DWORD dwStyle          /* = WS_CHILD | WS_VISIBLE */,
	const DWORD dwCtlStyle       /* = CCS_BOTTOM */,
	const DWORD dwStatusBarStyle /* = SBARS_SIZEGRIP */,
	const UINT  nID              /* = IDW_STD_STATUSBAR */)
{
	assert(parent != NULL);

	_SetHookTarget(this);
	HWND hWnd = ::CreateStatusWindow(
		dwStyle | dwCtlStyle | dwStatusBarStyle,
		lpszWindowName,
		*parent,
		nID);

	if(hWnd == NULL)
	{
		_SetHookTarget(NULL);
		return FALSE;
	}
	assert(hWnd == m_hWnd);
	return TRUE;
}


// �C���W�P�[�^��ݒ�
BOOL statusbar::SetIndicators(HINSTANCE hResource, const UINT nIDArray[], const int nCount)
{
	m_pane_width.resize(nCount);

	// �e�����̕����擾
	for(int i = 0; i < nCount; i++)
	{
		tstring_t tstr;
		wgfLoadString(hResource, nIDArray[i], tstr);

		SIZE size;
		devicecontext_client dc(m_hWnd);
		dc.GetTextExtent(tstr, size);

		m_pane_width[i] = size.cx;
	}

	return UpdatePanePosition();
}


// ���E�����擾�i�S�v�f�𓯎��Ɂj
BOOL statusbar::GetBorders(int *pBorders) const
{
	assert(pBorders != NULL);
	return SendMessage(
		SB_GETBORDERS,
		0,
		reinterpret_cast<LPARAM>(pBorders));
}

// ���E�����擾�i��v�f���j
BOOL statusbar::GetBorders(int *pHorz, int *pVert, int *nSpacing) const
{
	int arBorders[3];
	if(!GetBorders(arBorders))
	{
		return FALSE;
	}

	if(pHorz    != NULL) { *pHorz    = arBorders[0]; }
	if(pVert    != NULL) { *pVert    = arBorders[1]; }
	if(nSpacing != NULL) { *nSpacing = arBorders[2]; }
	return TRUE;
}

// �y�C�����쐬
BOOL statusbar::SetParts(const int nParts, int *pWidths)
{
	assert(pWidths != NULL);
	return SendMessage(
		SB_SETPARTS,
		nParts,
		reinterpret_cast<LPARAM>(pWidths));
}

// �y�C�������擾
BOOL statusbar::GetParts(const int nParts, int *pWidths) const
{
	assert(pWidths != NULL);
	return SendMessage(
		SB_GETPARTS,
		nParts,
		reinterpret_cast<LPARAM>(pWidths));
}

// �e�L�X�g���o��
BOOL statusbar::SetText(const int iPart, const int uType, const tstring_t &text)
{
	return SendMessage(
		SB_SETTEXT,
		iPart | uType,
		reinterpret_cast<LPARAM>(text.c_str()));
}


////////////////////////////////////////////////////////////
// ���b�Z�[�W�n���h��
void statusbar::OnSize(const UINT nType, const int cx, const int cy)
{
	control::OnSize(nType, cx, cy);
	UpdatePanePosition();
}


////////////////////////////////////////////////////////////
// private

// �����̗̈���v�Z
void statusbar::CalcInsideRect(RECT &rect) const
{
	GetClientRect(rect);

	// �ő�\���łȂ���΃T�C�Y�ύX�̈���l��
	if(!::IsZoomed(GetParent()))
	{
		// �T�C�Y�ύX�̈�́A�قڃN���C�A���g�̈�̍����Ɠ�����
		rect.right -= (rect.bottom - rect.top);
	}
}

// �e�y�C���̈ʒu���X�V
BOOL statusbar::UpdatePanePosition(void)
{
	// �A�C�h��������\���̈�{�C���W�P�[�^
	const size_t count = m_pane_width.size();
	intarray_t right_pos(count + 1);

	// [block]
	// ��ԉE�̃y�C���̍��W������
	{
		RECT rect;
		CalcInsideRect(rect);
		right_pos[count] = rect.right;
	}

	// ���̑��̃y�C���̍��W������
	for(int j = count - 1; j >= 0; j--)
	{
		right_pos[j] = right_pos[j + 1] - m_pane_width[j];
	}

	return SetParts(right_pos.size(), &right_pos[0]);
}

_WGC_END                                // }
