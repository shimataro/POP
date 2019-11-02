// ToolTipCtrl.cpp �c�C���v�������e�[�V�����t�@�C��
#include "ToolTipCtrl.h"


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// �\�z

// �R���X�g���N�^
tooltip_ctrl::tooltip_ctrl(void)
{
}

// �f�X�g���N�^
tooltip_ctrl::~tooltip_ctrl(void)
{
}


////////////////////////////////////////////////////////////////////////////////
// �쐬

BOOL tooltip_ctrl::Create(window *parent, const DWORD dwStyle /* = 0 */, HINSTANCE hInstance /* = NULL */)
{
	// �c�[���`�b�v�E�C���h�E�̍쐬
	return control::Create(
		TOOLTIPS_CLASS,
		NULL,
		dwStyle, 0, 0,
		parent,
		0,
		hInstance);
}


// �c�[���`�b�v��ǉ��iTOOLINFO�\���̂��g�p�j
BOOL tooltip_ctrl::AddTool(const TOOLINFO &ti)
{
	return SendMessage(TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&ti));
}

// �q�E�C���h�E�Ƀc�[���`�b�v��ǉ��i�X�g�����O���\�[�X�g�p�j
BOOL tooltip_ctrl::AddTool(HWND hWnd, HINSTANCE hResource, const UINT nIDText)
{
	return AddTool(hWnd, hResource, MAKEINTRESOURCE(nIDText));
}

// �q�E�C���h�E�Ƀc�[���`�b�v��ǉ��i������𒼐ڎw��j
BOOL tooltip_ctrl::AddTool(HWND hWnd, HINSTANCE hResource, LPCTSTR lpszText)
{
	// TOOLINFO�\���̂̊�{�ݒ�
	TOOLINFO ti;
	ti.cbSize = sizeof(ti);                 // TOOLINFO�\���̂̃T�C�Y
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;// ID���E�C���h�E�n���h�� �{ �T�u�N���X��
	ti.hwnd   = GetParent();                // �c�[���`�b�v�����L����E�C���h�E���e�E�C���h�E
	ti.hinst  = hResource;                  // �X�g�����O���\�[�X�����C���X�^���X

	ti.uId      = reinterpret_cast<UINT>(hWnd);
	ti.lpszText = const_cast<LPTSTR>(lpszText);

	return AddTool(ti);
}


////////////////////////////////////////////////////////////////////////////////
// �ő啝�iInternet Explorer 3.0�ȍ~�j

// �ő啝���擾
int tooltip_ctrl::GetMaxTipWidth(void) const
{
#if (_WIN32_IE >= 0x0300)
	return SendMessage(TTM_GETMAXTIPWIDTH);
#else
	return -1;
#endif
}

// �ő啝��ݒ�
int tooltip_ctrl::SetMaxTipWidth(const int iWidth)
{
#if (_WIN32_IE >= 0x0300)
	return SendMessage(TTM_SETMAXTIPWIDTH, 0, iWidth);
#else
	return -1;
#endif
}

_WGC_END                                // }
