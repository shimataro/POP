/* glasstile.h �c�K���X�^�C���t�B���^ */
#ifndef __GLASSTILE__
#define __GLASSTILE__


/* �p�����[�^�̉����A��� */
#define TILESIZE_MIN  (1)
#define TILESIZE_MAX  (100)


/* �K���X�^�C���̃p�����[�^ */
typedef struct tagGLASSTILEPARAM
{
	int width;                              /* �K���X�^�C���̕� */
	int height;                             /* �K���X�^�C���̍��� */
} GLASSTILEPARAM;


/* �G�t�F�N�g�֐��E�I�v�V�����֐� */
BOOL        executable_glasstile(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_glasstile(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
BOOL        option_glasstile(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __GLASSTILE__ */
