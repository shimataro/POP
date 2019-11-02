// ripple_dialog.h
#ifndef __RIPPLE_DIALOG__
#define __RIPPLE_DIALOG__

#include "../PreviewDialog.h"
#include "ripple.h"


/* �g��p�����[�^ */
typedef struct tagRIPPLEPARAM_DIALOG
{
	int center_x, center_y;                 /* ���S���W */
	int amplitude;                          /* �U�� */
	int cycle;                              /* ���� */
	int phase;                              /* �ʑ� */
	int distance;                           /* ���摜�Ƃ̋��� */
	double refraction;                      /* ���ܗ� */
} RIPPLEPARAM_DIALOG;


class ripple_dialog : public preview_dialog
{
public:
	explicit ripple_dialog(const RIPPLEPARAM_DIALOG &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */);
	~ripple_dialog(void);

	// �g��p�����[�^
	RIPPLEPARAM_DIALOG m_param;

protected:
	enum { IDD = IDD_OPTION_RIPPLE };

	// �R���g���[���I�u�W�F�N�g
	wgc::scrollbar_ctrl m_ctrlScrollCenterX;    // �g�䒆�S��X���W
	wgc::scrollbar_ctrl m_ctrlScrollCenterY;    // �g�䒆�S��Y���W
	wgc::scrollbar_ctrl m_ctrlScrollAmplitude;
	wgc::scrollbar_ctrl m_ctrlScrollCycle;
	wgc::scrollbar_ctrl m_ctrlScrollPhase;
	wgc::scrollbar_ctrl m_ctrlScrollDistance;

	void AssociateDialogControls(void);
	void UpdateData(const BOOL bUpdate = TRUE);
	void UpdatePreview(void);

	BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);
	void OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);
};

#endif // __RIPPLE_DIALOG__
