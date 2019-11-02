/* solarization.h �c�\�����[�[�V�����t�B���^ */
#ifndef __SOLOARIZATION__
#define __SOLOARIZATION__


/* �\�����[�[�V�����̃p�����[�^ */
typedef struct tagSOLARIZATIONPARAM
{
	int threshold;                          /* 臒l */
	BOOL channel_red;                       /* �K�p�`���l���i�ԁj */
	BOOL channel_green;                     /* �K�p�`���l���i�΁j */
	BOOL channel_blue;                      /* �K�p�`���l���i�j */
} SOLARIZATIONPARAM;


/* �G�t�F�N�g�֐��E�I�v�V�����֐� */
BOOL        executable_solarization(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_solarization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
BOOL        option_solarization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __SOLOARIZATION__ */
