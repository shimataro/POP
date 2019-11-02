/* ripple.h �c�g��t�B���^ */
#ifndef __RIPPLE__
#define __RIPPLE__


/* �g��p�����[�^ */
typedef struct tagRIPPLEPARAM
{
	uint32_t center_x, center_y;            /* ���S���W */
	double amplitude;                       /* �U�� */
	double cycle;                           /* ���� */
	double phase;                           /* �ʑ� */
	double distance;                        /* ���摜�Ƃ̋��� */
	double refraction;                      /* ���ܗ� */
} RIPPLEPARAM;


/* �G�t�F�N�g�֐��E�I�v�V�����֐� */
BOOL        executable_ripple(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_ripple(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
BOOL        option_ripple(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __RIPPLE__ */
