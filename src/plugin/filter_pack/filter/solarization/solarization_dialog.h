// solarization_dialog.h
#ifndef __SOLARIZATION_DIALOG__
#define __SOLARIZATION_DIALOG__

#include "../PreviewDialog.h"
#include "solarization.h"


class solarization_dialog : public preview_dialog
{
public:
	explicit solarization_dialog(const SOLARIZATIONPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */);
	~solarization_dialog(void);

	// ソラリゼーションパラメータ
	SOLARIZATIONPARAM m_param;

protected:
	enum { IDD = IDD_OPTION_SOLARIZATION };

	// コントロールオブジェクト
	wgc::scrollbar_ctrl m_ctrlThreshold;        // 閾値
	wgc::static_ctrl    m_ctrlThresholdValue;   // 閾値（値表示）
	wgc::checkbox_ctrl  m_ctrlChannelRed;       // 適用チャネル（赤）
	wgc::checkbox_ctrl  m_ctrlChannelGreen;     // 適用チャネル（緑）
	wgc::checkbox_ctrl  m_ctrlChannelBlue;      // 適用チャネル（青）

	void AssociateDialogControls(void);
	void UpdateData(const BOOL bUpdate = TRUE);
	void UpdatePreview(void);

	BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);
	void OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);
};

#endif // __SOLARIZATION_DIALOG__
