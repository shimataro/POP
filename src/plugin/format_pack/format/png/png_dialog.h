// png_dialog.h
#ifndef __PNG_DIALOG__
#define __PNG_DIALOG__


typedef struct tagPNGPARAM
{
	union CHECK
	{
		uint32_t checkflags;                    // �`�F�b�N�t���O
		struct ELEMENTS
		{
			unsigned compress_text : 1;             // �摜�t�@�C���ɃR�����g�𖄂ߍ��ނ�
			unsigned save_gamma    : 1;             // �E�C���h�E�̃T�C�Y�ƈʒu��ۑ����邩
			unsigned save_time     : 1;             // �E�C���h�E�̃T�C�Y�ƈʒu��ۑ����邩
		} elements;
	} check;
	uint_t interlace_type;                  // �C���^�[���[�X�^�C�v�i0�`1�j
	uint_t filter_type;                     // �t�B���^�^�C�v(0�`4�j
	uint_t compression_level;               // ���k���x���i0�`9�j
} PNGPARAM;


class png_dialog : public wgc::dialog
{
public:
	png_dialog(const PNGPARAM &param, HWND hWndParent = NULL);

	PNGPARAM m_param;

protected:
	enum { IDD = IDD_SETTING_PNG };

	// �R���g���[���I�u�W�F�N�g
	wgc::checkbox_ctrl m_ctrlCompressText;  // �e�L�X�g�f�[�^�����k���邩�H
	wgc::checkbox_ctrl m_ctrlSaveGamma;     // �K���}�l��ۑ����邩�H
	wgc::checkbox_ctrl m_ctrlSaveTime;      // �X�V������ۑ����邩�H
	wgc::slider_ctrl   m_ctrlCompressLevel; // ���k���x��

	void AssociateDialogControls(void);
	void UpdateData(const BOOL bUpdate = TRUE);

	BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);
};

#endif // __OPTIONDIALOGPNG__
