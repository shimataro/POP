// glasstile_dialog.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "glasstile_dialog.h"


// �R���X�g���N�^
glasstile_dialog::glasstile_dialog(const GLASSTILEPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: m_width(info_ptr->width), m_height(info_ptr->height),
	preview_dialog(IDD, info_ptr, data_ptr, hWndParent)
{
	// �p�����[�^��ۑ�
	m_param = param;
}

// �f�X�g���N�^
glasstile_dialog::~glasstile_dialog(void)
{
}

// �R���g���[�����I�u�W�F�N�g�Ɋ֘A�t��
void glasstile_dialog::AssociateDialogControls(void)
{
	m_ctrlSpinWidth .SubclassDlgItem(IDC_OPTION_GLASSTILE_SPIN_WIDTH    , this);
	m_ctrlSpinHeight.SubclassDlgItem(IDC_OPTION_GLASSTILE_SPIN_HEIGHT   , this);
	m_ctrlPreview   .SubclassDlgItem(IDC_OPTION_GLASSTILE_STATIC_PREVIEW, this);
}

// �_�C�A���O�p�����[�^���X�V
void glasstile_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	m_bUpdating = TRUE;
	if(bUpdate)
	{
		// �_�C�A���O���ϐ�
		m_param.width  = m_ctrlSpinWidth.GetPos();
		m_param.height = m_ctrlSpinHeight.GetPos();
	}
	else
	{
		// �ϐ����_�C�A���O
		m_ctrlSpinWidth.SetPos(m_param.width);
		m_ctrlSpinHeight.SetPos(m_param.height);
	}
	m_bUpdating = FALSE;
}

// �v���r���[���X�V
void glasstile_dialog::UpdatePreview(void)
{
	// ���݂̐ݒ�ŃK���X�^�C���t�B���^��������
	UpdateData(TRUE);

	// �v���r���[�T�C�Y�ɍ��킹�ăp�����[�^�𒲐�
	GLASSTILEPARAM param = m_param;
	param.width  = saturate<int>(param.width  * m_info_preview.width  / m_width , TILESIZE_MIN, TILESIZE_MAX);
	param.height = saturate<int>(param.height * m_info_preview.height / m_height, TILESIZE_MIN, TILESIZE_MAX);
	effect_glasstile(&m_info_preview, &m_data_preview_before, &m_data_preview_after, &param, NULL, 0);

	preview_dialog::UpdatePreview();
}

// �_�C�A���O������
BOOL glasstile_dialog::OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam)
{
	preview_dialog::OnInitDialog(hWndFocus, dwInitParam);

	// �R���g���[���̏����ݒ�
	m_ctrlSpinWidth .SetRange(TILESIZE_MIN, TILESIZE_MAX);
	m_ctrlSpinHeight.SetRange(TILESIZE_MIN, TILESIZE_MAX);
	m_ctrlPreview.SetBitmap(m_hDIBSection);                     // �s�N�`���[�R���g���[���̃r�b�g�}�b�v��DIB�Z�N�V�����ɐݒ�

	UpdateData(FALSE);
	UpdatePreview();

	// �c�[���`�b�v��ݒ�
	const UINT arIDs[] =
	{
		IDC_OPTION_GLASSTILE_EDIT_WIDTH,
		IDC_OPTION_GLASSTILE_EDIT_HEIGHT,
		IDC_OPTION_GLASSTILE_STATIC_PREVIEW,
	};

	SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));

	// OK�{�^���Ƀt�H�[�J�X�����킹��
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}
