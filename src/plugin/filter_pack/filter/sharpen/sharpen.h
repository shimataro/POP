/* sharpen.h �c�V���[�v�t�B���^ */
#ifndef __SHARPEN__
#define __SHARPEN__


/* �G�t�F�N�g�֐� */
BOOL        executable_sharpen(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_sharpen(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);

#endif /* __SHARPEN__ */
