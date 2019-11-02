/* posterization.h �c�|�X�^���[�[�V�����t�B���^ */
#ifndef __POSTERIZATION__
#define __POSTERIZATION__


/* �p�����[�^�̉����A��� */
#define POSTERIZATION_TONE_MIN  (1)     /* �K���̍ŏ��l */
#define POSTERIZATION_TONE_MAX  (16)    /* �K���̍ő�l */


/* �|�X�^���[�[�V�����̃p�����[�^ */
typedef struct tagPOSTERIZATIONPARAM
{
	int tone;                               /* �K�� */
} POSTERIZATIONPARAM;


/* �G�t�F�N�g�֐��E�I�v�V�����֐� */
BOOL        executable_posterization(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_posterization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
BOOL        option_posterization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __POSTERIZATION__ */
