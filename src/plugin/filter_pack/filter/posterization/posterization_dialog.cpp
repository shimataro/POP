// posterization_dialog.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "posterization_dialog.h"


// �R���X�g���N�^
posterization_dialog::posterization_dialog(const POSTERIZATIONPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: preview_dialog(IDD, info_ptr, data_ptr, hWndParent)
{
	// �p�����[�^��ۑ�
	m_param = param;
}

// �f�X�g���N�^
posterization_dialog::~posterization_dialog(void)
{
}

// �R���g���[�����I�u�W�F�N�g�Ɋ֘A�t��
void posterization_dialog::AssociateDialogControls(void)
{
	m_ctrlTone     .SubclassDlgItem(IDC_OPTION_POSTERIZATION_SCROLL_TONE   , this); // �|�X�^���[�[�V�����̊K��
	m_ctrlToneValue.SubclassDlgItem(IDC_OPTION_POSTERIZATION_STATIC_TONE   , this);
	m_ctrlPreview  .SubclassDlgItem(IDC_OPTION_POSTERIZATION_STATIC_PREVIEW, this); // �v���r���[
}

// �_�C�A���O�p�����[�^���X�V
void posterization_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	m_bUpdating = TRUE;
	if(bUpdate)
	{
		// �_�C�A���O���ϐ�
		m_param.tone = m_ctrlTone.GetPos();
	}
	else
	{
		// �ϐ����_�C�A���O
		m_ctrlTone.SetPos(m_param.tone);
	}
	// �K�����o��
	SetDlgItemInt(IDC_OPTION_POSTERIZATION_STATIC_TONE, m_param.tone, FALSE);

	m_bUpdating = FALSE;
}

// �v���r���[���X�V
void posterization_dialog::UpdatePreview(void)
{
	// ���݂̐ݒ�Ń|�X�^���[�[�V�����t�B���^��������
	UpdateData(TRUE);
	effect_posterization(&m_info_preview, &m_data_preview_before, &m_data_preview_after, &m_param, NULL, 0);

	preview_dialog::UpdatePreview();
}

// �_�C�A���O������
BOOL posterization_dialog::OnInitDialog(HWND hWndFocus, LPARAM dwInitParam)
{
	preview_dialog::OnInitDialog(hWndFocus, dwInitParam);

	// �R���g���[���̏����ݒ�
	m_ctrlTone.SetRange(POSTERIZATION_TONE_MIN, POSTERIZATION_TONE_MAX, TRUE);  // �X�N���[���o�[�R���g���[���̐ݒ�
	m_ctrlPreview.SetBitmap(m_hDIBSection);                     // �s�N�`���[�R���g���[���̃r�b�g�}�b�v��DIB�Z�N�V�����ɐݒ�

	UpdateData(FALSE);
	UpdatePreview();

	// �c�[���`�b�v��ݒ�
	const UINT arIDs[] =
	{
		IDC_OPTION_POSTERIZATION_SCROLL_TONE,
		IDC_OPTION_POSTERIZATION_STATIC_PREVIEW,
	};

	SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));

	// OK�{�^���Ƀt�H�[�J�X�����킹��
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}

// �����X�N���[���o�[
void posterization_dialog::OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar)
{
	preview_dialog::OnHScroll(nSBCode, nPos, hWndScrollBar);

	const int nPageSize = 8;

	// �X�N���[�������擾
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask  = SIF_RANGE | SIF_POS;
	::GetScrollInfo(hWndScrollBar, SB_CTL, &si);

	// ���݈ʒu���X�V
	int position = si.nPos;
	switch(nSBCode)
	{
	case SB_LEFT:  position = si.nMin; break;
	case SB_RIGHT: position = si.nMax; break;

	case SB_PAGELEFT:  position -= nPageSize; break;
	case SB_PAGERIGHT: position += nPageSize; break;

	case SB_LINELEFT:  position--; break;
	case SB_LINERIGHT: position++; break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		position = nPos;
		break;

	case SB_ENDSCROLL: break;
	}

	// �X�N���[���ʒu��ύX
	::SetScrollPos(hWndScrollBar, SB_CTL, position, TRUE);

	// �v���r���[���X�V
	UpdatePreview();
}
