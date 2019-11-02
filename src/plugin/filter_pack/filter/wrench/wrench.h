/* wrench.h �c�˂���t�B���^ */
#ifndef __WRENCH__
#define __WRENCH__


/* �˂���p�����[�^�p�����[�^ */
typedef struct tagWRENCHPARAM
{
	int center_x_percent, center_y_percent;
	int center_x, center_y;                 /* �˂���̒��S */
	double radius;                          /* ���a */
	double angle;                           /* �˂���p�x(rad) */
} WRENCHPARAM;


/* �G�t�F�N�g�֐��E�I�v�V�����֐� */
BOOL        executable_wrench(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_wrench(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
BOOL        option_wrench(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __WRENCH__ */
