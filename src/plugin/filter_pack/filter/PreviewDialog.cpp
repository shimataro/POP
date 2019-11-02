// PreviewDialog.cpp …インプリメンテーションファイル
#include "../stdafx.h"
#include "PreviewDialog.h"


#define PREVIEW_WIDTH   80
#define PREVIEW_HEIGHT  60


// コンストラクタ
preview_dialog::preview_dialog(const UINT nID, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: wgc::dialog(GetInstanceHandle(), MAKEINTRESOURCE(nID), hWndParent)
{
	m_bUpdating = FALSE;

	CreateGammaTable(GAMMA_DEFAULT_FILE, GAMMA_DEFAULT_DECODE, GAMMA_DEFAULT_DISPLAY, m_gamma_table);

	// プレビュー用イメージ（サムネイル）を作成
	create_thumbnail(info_ptr, data_ptr, &m_info_preview, &m_data_preview_before, PREVIEW_WIDTH, PREVIEW_HEIGHT);
	create_thumbnail(info_ptr, data_ptr, &m_info_preview, &m_data_preview_after , PREVIEW_WIDTH, PREVIEW_HEIGHT);

	// DIBセクションを作成
	m_hDIBSection = DIBSECTION_Create32bits(
		m_info_preview.width, m_info_preview.height,
		NULL, reinterpret_cast<void **>(&m_buffer));
}

// デストラクタ
preview_dialog::~preview_dialog(void)
{
	// プレビュー用イメージを削除
	image_free(&m_data_preview_before);
	image_free(&m_data_preview_after);

	// DIBセクションを削除
	::DeleteObject(m_hDIBSection);
}

// プレビューを更新
void preview_dialog::UpdatePreview(void)
{
	// DIBセクションを更新
	BITMAP_SetDataFromImage(
		&m_info_preview, &m_data_preview_after,
		m_gamma_table,
		2,
		RGB(0x80, 0x80, 0x80), RGB(0xff, 0xff, 0xff),
		m_buffer);

	// 矩形を無効化し、再描画を要求
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
