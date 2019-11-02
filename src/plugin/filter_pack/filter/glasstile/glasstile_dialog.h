// glasstile_dialog.h
#ifndef __GLASSTILE_DIALOG__
#define __GLASSTILE_DIALOG__

#include "../PreviewDialog.h"
#include "glasstile.h"


class glasstile_dialog : public preview_dialog
{
public:
	explicit glasstile_dialog(const GLASSTILEPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */);
	~glasstile_dialog(void);

	// ガラスタイルパラメータ
	GLASSTILEPARAM m_param;

protected:
	enum { IDD = IDD_OPTION_GLASSTILE };

	// コントロールオブジェクト
	wgc::spinbutton_ctrl m_ctrlSpinWidth;   // タイルの幅
	wgc::spinbutton_ctrl m_ctrlSpinHeight;  // タイルの高さ

	const uint32_t m_width;
	const uint32_t m_height;

	void AssociateDialogControls(void);
	void UpdateData(const BOOL bUpdate = TRUE);
	void UpdatePreview(void);

	BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);
};

#endif // __GLASSTILE_DIALOG__
