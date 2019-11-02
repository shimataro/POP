// sepia_dialog.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "sepia_dialog.h"


// �R���X�g���N�^
sepia_dialog::sepia_dialog(const SEPIAPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: preview_dialog(IDD, info_ptr, data_ptr, hWndParent)
{
	// �p�����[�^��ۑ�
	m_param = param;
}

// �f�X�g���N�^
sepia_dialog::~sepia_dialog(void)
{
}

// �R���g���[�����I�u�W�F�N�g�Ɋ֘A�t��
void sepia_dialog::AssociateDialogControls(void)
{
	m_ctrlStrength.SubclassDlgItem(IDC_OPTION_SEPIA_SLIDER_STRENGTH, this); // �Z�s�A�̋��x
	m_ctrlPreview .SubclassDlgItem(IDC_OPTION_SEPIA_STATIC_PREVIEW , this); // �v���r���[
}

// �_�C�A���O�p�����[�^���X�V
void sepia_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	m_bUpdating = TRUE;
	if(bUpdate)
	{
		// �_�C�A���O���ϐ�
		m_param.strength = m_ctrlStrength.GetPos();
	}
	else
	{
		// �ϐ����_�C�A���O
		m_ctrlStrength.SetPos(m_param.strength);
	}
	m_bUpdating = FALSE;
}

// �v���r���[���X�V
void sepia_dialog::UpdatePreview(void)
{
	// ���݂̐ݒ�ŃZ�s�A�t�B���^��������
	UpdateData(TRUE);
	effect_sepia(&m_info_preview, &m_data_preview_before, &m_data_preview_after, &m_param, NULL, 0);

	preview_dialog::UpdatePreview();
}

// �_�C�A���O������
BOOL sepia_dialog::OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam)
{
	preview_dialog::OnInitDialog(hWndFocus, dwInitParam);

	// �R���g���[���̏����ݒ�
	m_ctrlStrength.SetRange(SEPIA_STRENGTH_MIN, SEPIA_STRENGTH_MAX, TRUE);  // �X���C�_�R���g���[���̐ݒ�
	m_ctrlPreview.SetBitmap(m_hDIBSection);                     // �s�N�`���[�R���g���[���̃r�b�g�}�b�v��DIB�Z�N�V�����ɐݒ�

	UpdateData(FALSE);
	UpdatePreview();

	// �c�[���`�b�v��ݒ�
	const UINT arIDs[] =
	{
		IDC_OPTION_SEPIA_SLIDER_STRENGTH,
		IDC_OPTION_SEPIA_STATIC_PREVIEW,
	};

	SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));

	// OK�{�^���Ƀt�H�[�J�X�����킹��
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}

// �����X�N���[���o�[
void sepia_dialog::OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar)
{
	preview_dialog::OnHScroll(nSBCode, nPos, hWndScrollBar);
	UpdatePreview();
}
