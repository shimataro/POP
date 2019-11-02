// ripple_dialog.h
#ifndef __RIPPLE_DIALOG__
#define __RIPPLE_DIALOG__

#include "../PreviewDialog.h"
#include "ripple.h"


/* 波紋パラメータ */
typedef struct tagRIPPLEPARAM_DIALOG
{
	int center_x, center_y;                 /* 中心座標 */
	int amplitude;                          /* 振幅 */
	int cycle;                              /* 周期 */
	int phase;                              /* 位相 */
	int distance;                           /* 元画像との距離 */
	double refraction;                      /* 屈折率 */
} RIPPLEPARAM_DIALOG;


class ripple_dialog : public preview_dialog
{
public:
	explicit ripple_dialog(const RIPPLEPARAM_DIALOG &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */);
	~ripple_dialog(void);

	// 波紋パラメータ
	RIPPLEPARAM_DIALOG m_param;

protected:
	enum { IDD = IDD_OPTION_RIPPLE };

	// コントロールオブジェクト
	wgc::scrollbar_ctrl m_ctrlScrollCenterX;    // 波紋中心のX座標
	wgc::scrollbar_ctrl m_ctrlScrollCenterY;    // 波紋中心のY座標
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
