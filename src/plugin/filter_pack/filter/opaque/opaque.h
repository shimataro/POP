/* opaque.h �c�ڂ����t�B���^ */
#ifndef __OPAQUE__
#define __OPAQUE__


/* �G�t�F�N�g�֐� */
BOOL        executable_opaque(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_opaque(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);

#endif /* __OPAQUE__ */
