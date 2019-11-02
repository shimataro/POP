// solarization_dialog.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "solarization_dialog.h"


// �R���X�g���N�^
solarization_dialog::solarization_dialog(const SOLARIZATIONPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: preview_dialog(IDD, info_ptr, data_ptr, hWndParent)
{
	// �p�����[�^��ۑ�
	m_param = param;
}

// �f�X�g���N�^
solarization_dialog::~solarization_dialog(void)
{
}

// �R���g���[�����I�u�W�F�N�g�Ɋ֘A�t��
void solarization_dialog::AssociateDialogControls(void)
{
	m_ctrlThreshold.SubclassDlgItem     (IDC_OPTION_SOLARIZATION_SCROLL_THRESHOLD, this);
	m_ctrlThresholdValue.SubclassDlgItem(IDC_OPTION_SOLARIZATION_STATIC_THRESHOLD, this);
	m_ctrlChannelRed.SubclassDlgItem    (IDC_OPTION_SOLARIZATION_CHANNEL_RED     , this);
	m_ctrlChannelGreen.SubclassDlgItem  (IDC_OPTION_SOLARIZATION_CHANNEL_GREEN   , this);
	m_ctrlChannelBlue.SubclassDlgItem   (IDC_OPTION_SOLARIZATION_CHANNEL_BLUE    , this);
	m_ctrlPreview.SubclassDlgItem       (IDC_OPTION_SOLARIZATION_STATIC_PREVIEW  , this);
}

// �_�C�A���O�p�����[�^���X�V
void solarization_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	m_bUpdating = TRUE;
	if(bUpdate)
	{
		// �_�C�A���O���ϐ�
		m_param.threshold = m_ctrlThreshold.GetPos();
		m_param.channel_red   = (m_ctrlChannelRed  .GetCheck() == BST_CHECKED) ? TRUE : FALSE;
		m_param.channel_green = (m_ctrlChannelGreen.GetCheck() == BST_CHECKED) ? TRUE : FALSE;
		m_param.channel_blue  = (m_ctrlChannelBlue .GetCheck() == BST_CHECKED) ? TRUE : FALSE;
	}
	else
	{
		// �ϐ����_�C�A���O
		m_ctrlThreshold.SetPos(m_param.threshold);
		m_ctrlChannelRed  .SetCheck(m_param.channel_red   ? BST_CHECKED : BST_UNCHECKED);
		m_ctrlChannelGreen.SetCheck(m_param.channel_green ? BST_CHECKED : BST_UNCHECKED);
		m_ctrlChannelBlue .SetCheck(m_param.channel_blue  ? BST_CHECKED : BST_UNCHECKED);
	}
	// 臒l���o��
	SetDlgItemInt(IDC_OPTION_SOLARIZATION_STATIC_THRESHOLD, m_param.threshold, FALSE);

	m_bUpdating = FALSE;
}

// �v���r���[���X�V
void solarization_dialog::UpdatePreview(void)
{
	// ���݂̐ݒ�Ń\�����[�[�V�����t�B���^��������
	UpdateData(TRUE);
	effect_solarization(&m_info_preview, &m_data_preview_before, &m_data_preview_after, &m_param, NULL, 0);

	preview_dialog::UpdatePreview();
}

// �_�C�A���O������
BOOL solarization_dialog::OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam)
{
	preview_dialog::OnInitDialog(hWndFocus, dwInitParam);

	// �R���g���[���̏����ݒ�
	m_ctrlThreshold.SetRange(0, 256, TRUE);
	m_ctrlPreview.SetBitmap(m_hDIBSection); // �s�N�`���[�R���g���[���̃r�b�g�}�b�v��DIB�Z�N�V�����ɐݒ�

	UpdateData(FALSE);
	UpdatePreview();

	// �c�[���`�b�v��ݒ�
	const UINT arIDs[] =
	{
		IDC_OPTION_SOLARIZATION_SCROLL_THRESHOLD,
		IDC_OPTION_SOLARIZATION_CHANNEL_RED,
		IDC_OPTION_SOLARIZATION_CHANNEL_GREEN,
		IDC_OPTION_SOLARIZATION_CHANNEL_BLUE,
		IDC_OPTION_SOLARIZATION_STATIC_PREVIEW,
	};

	SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));

	// OK�{�^���Ƀt�H�[�J�X�����킹��
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}

// �����X�N���[���o�[
void solarization_dialog::OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar)
{
	preview_dialog::OnHScroll(nSBCode, nPos, hWndScrollBar);

	const int nPageSize = 32;

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
