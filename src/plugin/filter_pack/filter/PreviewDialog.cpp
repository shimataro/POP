// PreviewDialog.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../stdafx.h"
#include "PreviewDialog.h"


#define PREVIEW_WIDTH   80
#define PREVIEW_HEIGHT  60


// �R���X�g���N�^
preview_dialog::preview_dialog(const UINT nID, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: wgc::dialog(GetInstanceHandle(), MAKEINTRESOURCE(nID), hWndParent)
{
	m_bUpdating = FALSE;

	CreateGammaTable(GAMMA_DEFAULT_FILE, GAMMA_DEFAULT_DECODE, GAMMA_DEFAULT_DISPLAY, m_gamma_table);

	// �v���r���[�p�C���[�W�i�T���l�C���j���쐬
	create_thumbnail(info_ptr, data_ptr, &m_info_preview, &m_data_preview_before, PREVIEW_WIDTH, PREVIEW_HEIGHT);
	create_thumbnail(info_ptr, data_ptr, &m_info_preview, &m_data_preview_after , PREVIEW_WIDTH, PREVIEW_HEIGHT);

	// DIB�Z�N�V�������쐬
	m_hDIBSection = DIBSECTION_Create32bits(
		m_info_preview.width, m_info_preview.height,
		NULL, reinterpret_cast<void **>(&m_buffer));
}

// �f�X�g���N�^
preview_dialog::~preview_dialog(void)
{
	// �v���r���[�p�C���[�W���폜
	image_free(&m_data_preview_before);
	image_free(&m_data_preview_after);

	// DIB�Z�N�V�������폜
	::DeleteObject(m_hDIBSection);
}

// �v���r���[���X�V
void preview_dialog::UpdatePreview(void)
{
	// DIB�Z�N�V�������X�V
	BITMAP_SetDataFromImage(
		&m_info_preview, &m_data_preview_after,
		m_gamma_table,
		2,
		RGB(0x80, 0x80, 0x80), RGB(0xff, 0xff, 0xff),
		m_buffer);

	// ��`�𖳌������A�ĕ`���v��
	m_ctrlPreview.InvalidateRect(NULL, FALSE);
	m_ctrlPreview.UpdateWindow();
}

// 
BOOL preview_dialog::OnCommand(const WORD wNotifyCode, const WORD wID, HWND hWndCtrl)
{
	dialog::OnCommand(wNotifyCode, wID, hWndCtrl);
	if(!m_bUpdating)
	{
		UpdatePreview();
	}
	return TRUE;
}
