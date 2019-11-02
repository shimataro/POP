// Dialog.h �c�_�C�A���O�{�b�N�X�̃T�u�N���X���ƃc�[���`�b�v
#ifndef __WGC_DIALOG__
#define __WGC_DIALOG__

#include "Window.h"
#include "Control.h"
#include "ToolTipCtrl.h"


_WGC_BEGIN                              // namespace wgc {

class dialog : public window
{
public:
	// �R���X�g���N�^�^�f�X�g���N�^
	dialog(HINSTANCE hResource, LPCTSTR lpTemplate, HWND hWndParent = NULL);
	~dialog(void);

	// �_�C�A���O�̕\���^�I��
	int  DoModal   (const LPARAM dwInitParam = 0);  // ���[�_���\��
	BOOL DoModeless(const LPARAM dwInitParam = 0);  // ���[�h���X�\��
	void EndModeless(void);                         // ���[�h���X�_�C�A���O�̏I��

	// ���b�Z�[�W�֐�
	LONG SendDlgItemMessage(const int nIDDlgItem, const UINT uMsg, const WPARAM wParam = 0, const LPARAM lParam = 0);
	BOOL IsDialogMessage(LPMSG lpMsg);

	// �`�F�b�N�{�^������֐�
	UINT IsDlgButtonChecked   (const int nIDButton) const;
	BOOL CheckDlgButton       (const int nIDButton, const UINT uCheck);
	BOOL CheckRadioButton     (const int nIDFirstButton, const int nIDLastButton, const int nIDCheckButton);
	int  GetCheckedRadioButton(const int nIDFirstButton, const int nIDLastButton);

	// ���b�p�֐�
	HWND GetDlgItem(const int nIDDlgItem);
	UINT GetDlgItemInt(const int nIDDlgItem, BOOL *lpTranslated, const BOOL bSigned = TRUE);
	BOOL SetDlgItemInt(const int nIDDlgItem, const UINT nValue , const BOOL bSigned = TRUE);
	UINT GetDlgItemText(const int nIDDlgItem, tstring_t &tstr);
	BOOL SetDlgItemText(const int nIDDlgItem, const tstring_t &tstr);

private:
	// �c�[���`�b�v
	tooltip_ctrl m_ctrlToolTip;
	void InitToolTips(void);

protected:
	HINSTANCE m_hResource;                  // �_�C�A���O�̃��\�[�X�n���h��
	LPCTSTR   m_lpszTemplateName;           // �_�C�A���O�e���v���[�g
	HWND      m_hWndParent;                 // �e�E�C���h�E

	// ���b�p�֐�
	BOOL EndDialog(const int nResult);

	// �������z�֐�
	virtual void AssociateDialogControls(void) = 0;
	virtual void UpdateData(const BOOL bUpdate = TRUE) = 0;

	// �c�[���`�b�v�֘A
	void SetToolTips(const UINT arID[], const UINT nIDCount);
	void SetToolTip (const UINT nID);

	// �_�C�A���O�v���V�[�W��
	virtual LRESULT CALLBACK WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT _Default(void);


	// �_�C�A���O���b�Z�[�W�n���h��
	virtual BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);

	BOOL OnCommand(const WORD wNotifyCode, const WORD wID, HWND hWndCtrl);

	virtual void OnOK(void);                // OK�{�^��
	virtual void OnCancel(void);            // �L�����Z���{�^��
};

_WGC_END                                // }

#endif // __WGC_DIALOG__
