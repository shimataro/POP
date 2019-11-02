// PreviewDialog.h
#ifndef __PREVIEWDIALOG__
#define __PREVIEWDIALOG__


class preview_dialog : public wgc::dialog
{
public:
	explicit preview_dialog(const UINT nID, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */);
	~preview_dialog(void);

private:
	// プレビュー用ガンマテーブル、バッファ
	uint8_t  m_gamma_table[256];
	RGBQUAD *m_buffer;

protected:
	wgc::static_ctrl m_ctrlPreview;         // プレビュー

	BOOL m_bUpdating;

	// プレビュー用イメージ
	HBITMAP    m_hDIBSection;
	IMAGE_INFO m_info_preview;
	IMAGE_DATA m_data_preview_before, m_data_preview_after;

	virtual void UpdatePreview(void);

	BOOL OnCommand(const WORD wNotifyCode, const WORD wID, HWND hWndCtrl);
};

#endif // __PREVIEWDIALOG__
