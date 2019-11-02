/* sepia.h �c�Z�s�A�t�B���^ */
#ifndef __SEPIA__
#define __SEPIA__


/* �p�����[�^�̉����A��� */
#define SEPIA_STRENGTH_MIN  (0)
#define SEPIA_STRENGTH_MAX  (9)


/* �Z�s�A�̃p�����[�^ */
typedef struct tagSEPIAPARAM
{
	int strength;                           /* �Z�s�A�̋��x */
} SEPIAPARAM;


/* �G�t�F�N�g�֐��E�I�v�V�����֐� */
BOOL        executable_sepia(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_sepia(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
BOOL        option_sepia(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __SEPIA__ */
