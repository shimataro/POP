// Control.cpp �c�C���v�������e�[�V�����t�@�C��
#include "Control.h"
#include "Dialog.h"
#include <assert.h>                     // assert()


// ���C�u�����̃����N
#pragma comment(lib, "comctl32.lib")    // �R�����R���g���[���p���C�u����


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// �\�z

// �R���X�g���N�^
control::control(void)
{
	// �R�����R���g���[����������
	::InitCommonControls();
}

// �f�X�g���N�^
control::~control(void)
{
}



// �T�u�N���X��
BOOL control::SubclassDlgItem(const UINT uID, dialog *parent)
{
	assert(parent != NULL);

	// �R���g���[���E�C���h�E�n���h�����擾
	HWND hWnd = parent->GetDlgItem(uID);
	if(hWnd == NULL)
	{
		return FALSE;
	}

	// �T�u�N���X��
	return Subclass(hWnd);
}


BOOL control::Create(
	LPCTSTR lpszClassName,
	LPCTSTR lpszWindowName,
	const DWORD dwStyle,
	const DWORD dwCtlStyle,
	const DWORD dwExStyle,
	window *parent,
	const UINT nID,
	HINSTANCE hInstance /* = NULL */,
	const int x /* = CW_USEDEFAULT */,
	const int y /* = CW_USEDEFAULT */,
	const int nWidth  /* = CW_USEDEFAULT */,
	const int nHeight /* = CW_USEDEFAULT */)
{
	return window::Create(
		lpszClassName, lpszWindowName,
		dwStyle | dwCtlStyle,
		dwExStyle,
		parent,
		reinterpret_cast<HMENU>(nID),
		hInstance,
		x, y, nWidth, nHeight);
}

_WGC_END                                // }
