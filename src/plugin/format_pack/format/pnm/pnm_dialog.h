// pnm_dialog.h
#ifndef __PNM_DIALOG__
#define __PNM_DIALOG__


typedef struct tagPNMPARAM
{
	uint_t save_format;                     // IDC_SETTING_RADIO_FORMAT_ASCIIÅ`IDC_SETTING_RADIO_FORMAT_BINARY
} PNMPARAM;


class pnm_dialog : public wgc::dialog
{
public:
	pnm_dialog(const PNMPARAM &param, HWND hWndParent = NULL);

	PNMPARAM m_param;

protected:
	enum { IDD = IDD_SETTING_PNM };

	void AssociateDialogControls(void);
	void UpdateData(const BOOL bUpdate = TRUE);

	BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);
};

#endif // __PNM_DIALOG__
