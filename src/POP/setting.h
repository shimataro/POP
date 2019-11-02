// setting.h
#ifndef __SETTING__
#define __SETTING__


////////////////////////////////////////////////////////////////////////////////
// ���ݒ�̃p�����[�^

typedef struct tagSETTING
{
	// ��{�ݒ�
	struct BASIC
	{
		union CHECK
		{
			unsigned long checkflags;               // �`�F�b�N�t���O
			struct ELEMENTS
			{
				unsigned implant_comment       : 1;     // �摜�t�@�C���ɃR�����g�𖄂ߍ��ނ�
				unsigned save_window_placement : 1;     // �E�C���h�E�̈ʒu�ƃT�C�Y��ۑ����邩
			} elements;
		} check;
		CString comment;                        // ���ߍ��ރR�����g
		int     mru_count;                      // MRU���X�g�̐�
	} basic;

	// �\���ݒ�
	struct DISPLAY
	{
		union CHECK
		{
			unsigned long checkflags;               // �`�F�b�N�t���O
			struct ELEMENTS
			{
				unsigned activate_gamma_file     : 1;   // �t�@�C���K���}��L���ɂ���
				unsigned customize_gamma_decode  : 1;   // �f�R�[�f�B���O�K���}���J�X�^�}�C�Y����
				unsigned customize_gamma_display : 1;   // �f�B�X�v���C�K���}���J�X�^�}�C�Y����
			} elements;
		} check;
		double gamma_decode;                    // �f�R�[�f�B���O�K���}�̒l
		double gamma_display;                   // �f�B�X�v���C�K���}�̒l

		int background_grid_size;               // �i�q�̃T�C�Y(bit)
		COLORREF background_color1;             // �i�q�̐F1
		COLORREF background_color2;             // �i�q�̐F2

		BOOL toolbar_XP;                        // XP���c�[���o�[�A�C�R���g�p

		// �\��������
		struct LANGUAGE
		{
			CString resource_name;                  // ����p�b�N�̃��\�[�X��
		} language;
	} display;
} SETTING;


////////////////////////////////////////////////////////////////////////////////
// �֐��̃v���g�^�C�v�錾
void setting_load(      int &category,       SETTING &setting);
void setting_save(const int  category, const SETTING &setting);

#endif // __SETTING__
