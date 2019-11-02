// pnm_dialog.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "pnm_dialog.h"


// �R���X�g���N�^
pnm_dialog::pnm_dialog(const PNMPARAM &param, HWND hWndParent /* = NULL */)
	: dialog(GetInstanceHandle(), MAKEINTRESOURCE(IDD), hWndParent)
{
	m_param = param;
}

// �R���g���[�����I�u�W�F�N�g�Ɋ֘A�t��
void pnm_dialog::AssociateDialogControls(void)
{
}

// �_�C�A���O�p�����[�^���X�V
void pnm_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	if(bUpdate)
	{
		// �_�C�A���O���ϐ�
		const UINT nSaveFormat = GetCheckedRadioButton(IDC_PNM_SETTING_RADIO_FORMAT_ASCII, IDC_PNM_SETTING_RADIO_FORMAT_BINARY);

		m_param.save_format = nSaveFormat - IDC_PNM_SETTING_RADIO_FORMAT_ASCII;
	}
	else
	{
		// �ϐ����_�C�A���O
		const UINT nSaveFormat = m_param.save_format + IDC_PNM_SETTING_RADIO_FORMAT_ASCII;

		CheckRadioButton(IDC_PNM_SETTING_RADIO_FORMAT_ASCII, IDC_PNM_SETTING_RADIO_FORMAT_BINARY, nSaveFormat);
	}
}

// �_�C�A���O������
BOOL pnm_dialog::OnInitDialog(HWND hWndFocus, LPARAM dwInitParam)
{
	dialog::OnInitDialog(hWndFocus, dwInitParam);

	// �R���g���[���̏�Ԃ�ݒ�
	UpdateData(FALSE);

	// �c�[���`�b�v��ݒ�
	const UINT arID[] =
	{
		IDC_PNM_SETTING_RADIO_FORMAT_ASCII,
		IDC_PNM_SETTING_RADIO_FORMAT_BINARY,
	};

	SetToolTips(arID, SIZE_OF_ARRAY(arID));

	// OK�{�^���Ƀt�H�[�J�X�����킹��
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}
