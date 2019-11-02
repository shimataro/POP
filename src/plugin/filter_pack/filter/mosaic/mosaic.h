/* mosaic.h �c���U�C�N�t�B���^ */
#ifndef __MOSAIC__
#define __MOSAIC__


/* ���U�C�N�p�����[�^ */
typedef struct tagMOSAICPARAM
{
	int size_x;                             /* ���U�C�N�̑傫���i���j */
	int size_y;                             /* ���U�C�N�̑傫���i�c�j */
} MOSAICPARAM;


/* �G�t�F�N�g�֐� */
BOOL        executable_mosaic(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_mosaic(const IMAGE_INFO *info, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);

#endif /* __MOSAIC__ */
