// Dialog.cpp �c�C���v�������e�[�V�����t�@�C��
#include "Dialog.h"
#include <windowsx.h>                   // GET_WM_COMMAND_ID()
#include <assert.h>                     // assert()
#include <limits.h>                     // INT_MAX


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// �\�z

// �R���X�g���N�^
dialog::dialog(HINSTANCE hResource, LPCTSTR lpszTemplateName, HWND hWndParent /* = NULL */)
{
	m_hResource        = hResource;
	m_lpszTemplateName = lpszTemplateName;
	m_hWndParent       = hWndParent;
}

// �f�X�g���N�^
dialog::~dialog(void)
{
}


////////////////////////////////////////////////////////////////////////////////
// �\��

// ���[�_���_�C�A���O��\��
int dialog::DoModal(const LPARAM dwInitParam /* = 0 */)
{
	_SetHookTarget(this);
	const int nResult = ::DialogBoxParam(
		m_hResource,
		m_lpszTemplateName,
		m_hWndParent,
		reinterpret_cast<DLGPROC>(_WindowProcBase),
		dwInitParam);

	if(nResult == -1)
	{
		_SetHookTarget(NULL);
	}
	return nResult;
}

// �_�C�A���O�쐬
BOOL dialog::DoModeless(const LPARAM dwInitParam /* = 0 */)
{
	_SetHookTarget(this);
	HWND hWnd = ::CreateDialogParam(
		m_hResource,
		m_lpszTemplateName,
		m_hWndParent,
		reinterpret_cast<DLGPROC>(_WindowProcBase),
		dwInitParam);

	if(hWnd == NULL)
	{
		_SetHookTarget(NULL);
		return FALSE;
	}
	assert(hWnd == m_hWnd);
	return TRUE;
}

// �_�C�A���O�����i���[�h���X�_�C�A���O�j
void dialog::EndModeless(void)
{
	DestroyWindow();
}


////////////////////////////////////////////////////////////////////////////////
// ���b�Z�[�W�֐�

// �_�C�A���O�A�C�e���փ��b�Z�[�W�𑗂�
LONG dialog::SendDlgItemMessage(const int nIDDlgItem, const UINT uMsg, const WPARAM wParam /* = 0 */, const LPARAM lParam /* = 0*/)
{
	return ::SendDlgItemMessage(m_hWnd, nIDDlgItem, uMsg, wParam, lParam);
}

// ���[�h���X�_�C�A���O�p���b�Z�[�W�̔���
BOOL dialog::IsDialogMessage(LPMSG lpMsg)
{
	return ::IsDialogMessage(m_hWnd, lpMsg);
}


////////////////////////////////////////////////////////////////////////////////
// �`�F�b�N�{�^������֐�

UINT dialog::IsDlgButtonChecked(const int nIDButton) const
{
	return ::IsDlgButtonChecked(m_hWnd, nIDButton);
}

BOOL dialog::CheckDlgButton(const int nIDButton, const UINT uCheck)
{
	return ::CheckDlgButton(m_hWnd, nIDButton, uCheck);
}

BOOL dialog::CheckRadioButton(const int nIDFirstButton, const int nIDLastButton, const int nIDCheckButton)
{
	return ::CheckRadioButton(m_hWnd, nIDFirstButton, nIDLastButton, nIDCheckButton);
}

int dialog::GetCheckedRadioButton(const int nIDFirstButton, const int nIDLastButton)
{
	for(int id = nIDFirstButton; id <= nIDLastButton; id++)
	{
		if(IsDlgButtonChecked(id) == BST_CHECKED)
		{
			return id;
		}
	}
	return -1;
}


////////////////////////////////////////////////////////////////////////////////
// �c�[���`�b�v�֐�

// �c�[���`�b�v�̏�����
void dialog::InitToolTips(void)
{
	m_ctrlToolTip.Create(this, WS_POPUP | TTS_ALWAYSTIP, m_hResource);
	m_ctrlToolTip.SetMaxTipWidth(INT_MAX);
}

// �c�[���`�b�v��ݒ�
void dialog::SetToolTips(const UINT arID[], const UINT nIDCount)
{
	// ID���ƂɃc�[���`�b�v��ݒ�
	for(UINT i = 0; i < nIDCount; i++)
	{
		HWND hWnd = GetDlgItem(arID[i]);
		if(::IsWindow(hWnd))
		{
			// �c�[���`�b�v��ǉ�
			m_ctrlToolTip.AddTool(hWnd, m_hResource, arID[i]);
		}
	}
}

void dialog::SetToolTip(const UINT nID)
{
	SetToolTips(&nID, 1);
}


// ���b�p�֐�
HWND dialog::GetDlgItem(const int nIDDlgItem)
{
	return ::GetDlgItem(m_hWnd, nIDDlgItem);
}

UINT dialog::GetDlgItemInt(const int nIDDlgItem, BOOL *lpTranslated, const BOOL bSigned /* = TRUE */)
{
	return ::GetDlgItemInt(m_hWnd, nIDDlgItem, lpTranslated, bSigned);
}

BOOL dialog::SetDlgItemInt(const int nIDDlgItem, const UINT nValue, const BOOL bSigned /* = TRUE */)
{
	return ::SetDlgItemInt(m_hWnd, nIDDlgItem, nValue, bSigned);
}

UINT dialog::GetDlgItemText(const int nIDDlgItem, tstring_t &tstr)
{
	TCHAR buffer[256];
	const UINT result = ::GetDlgItemText(m_hWnd, nIDDlgItem, buffer, SIZE_OF_ARRAY(buffer));

	tstr = buffer;
	return result;
}

BOOL dialog::SetDlgItemText(const int nIDDlgItem, const tstring_t &tstr)
{
	return ::SetDlgItemText(m_hWnd, nIDDlgItem, tstr.c_str());
}

BOOL dialog::EndDialog(const int nResult)
{
	return ::EndDialog(m_hWnd, nResult);
}


////////////////////////////////////////////////////////////////////////////////
// �_�C�A���O�v���V�[�W��

LRESULT CALLBACK dialog::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		InitToolTips();
		return OnInitDialog(
			reinterpret_cast<HWND>(wParam),              // hWndFocus
			lParam);                                     // dwInitParam
	}
	return window::WindowProc(uMsg, wParam, lParam);
}

// �_�C�A���O�v���V�[�W���̃f�t�H���g����
LRESULT dialog::_Default(void)
{
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
// �_�C�A���O���b�Z�[�W�n���h��

// �_�C�A���O������
BOOL dialog::OnInitDialog(HWND /* hWndFocus */, const LPARAM /* dwInitParam */)
{
	AssociateDialogControls();
	return _Default();
}

// �R�}���h
BOOL dialog::OnCommand(const WORD /* wNotifyCode */, const WORD wID, HWND /* hWndCtrl */)
{
	switch(wID)
	{
	case IDOK:
		OnOK();
		return FALSE;

	case IDCANCEL:
		OnCancel();
		return FALSE;
	}
	return TRUE;
}

// OK
void dialog::OnOK(void)
{
	UpdateData(TRUE);
	EndDialog(IDOK);
}

// �L�����Z��
void dialog::OnCancel(void)
{
	EndDialog(IDCANCEL);
}

_WGC_END                                // }
