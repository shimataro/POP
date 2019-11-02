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

	// �\�����[�[�V�����p�����[�^
	SOLARIZATIONPARAM m_param;

protected:
	enum { IDD = IDD_OPTION_SOLARIZATION };

	// �R���g���[���I�u�W�F�N�g
	wgc::scrollbar_ctrl m_ctrlThreshold;        // 臒l
	wgc::static_ctrl    m_ctrlThresholdValue;   // 臒l�i�l�\���j
	wgc::checkbox_ctrl  m_ctrlChannelRed;       // �K�p�`���l���i�ԁj
	wgc::checkbox_ctrl  m_ctrlChannelGreen;     // �K�p�`���l���i�΁j
	wgc::checkbox_ctrl  m_ctrlChannelBlue;      // �K�p�`���l���i�j

	void AssociateDialogControls(void);
	void UpdateData(const BOOL bUpdate = TRUE);
	void UpdatePreview(void);

	BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);
	void OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);
};

#endif // __SOLARIZATION_DIALOG__
