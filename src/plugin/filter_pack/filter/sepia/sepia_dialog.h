// sepia_dialog.h
#ifndef __SEPIA_DIALOG__
#define __SEPIA_DIALOG__

#include "../PreviewDialog.h"
#include "sepia.h"


class sepia_dialog : public preview_dialog
{
public:
	explicit sepia_dialog(const SEPIAPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data__ptr, HWND hWndParent /* = NULL */);
	~sepia_dialog(void);

	// セピアパラメータ
	SEPIAPARAM m_param;

protected:
	enum { IDD = IDD_OPTION_SEPIA };

	// コントロールオブジェクト
	wgc::slider_ctrl m_ctrlStrength;        // セピアの強度

	void AssociateDialogControls(void);
	void UpdateData(const BOOL bUpdate = TRUE);
	void UpdatePreview(void);

	BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);
	void OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);
};

#endif // __SEPIA_DIALOG__
