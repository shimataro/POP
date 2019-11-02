// posterization_dialog.h
#ifndef __POSTERIZATION_DIALOG__
#define __POSTERIZATION_DIALOG__

#include "../PreviewDialog.h"
#include "posterization.h"


class posterization_dialog : public preview_dialog
{
public:
	explicit posterization_dialog(const POSTERIZATIONPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */);
	~posterization_dialog(void);

	// ポスタリゼーションパラメータ
	POSTERIZATIONPARAM m_param;

protected:
	enum { IDD = IDD_OPTION_POSTERIZATION };

	// コントロールオブジェクト
	wgc::scrollbar_ctrl m_ctrlTone;             // ポスタリゼーションの階調
	wgc::static_ctrl    m_ctrlToneValue;        // ポスタリゼーションの階調（値表示）

	void AssociateDialogControls(void);
	void UpdateData(const BOOL bUpdate = TRUE);
	void UpdatePreview(void);

	BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);
	void OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);
};

#endif // __POSTERIZATION_DIALOG__
