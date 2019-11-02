/* inverse.h …反転フィルタ */
#ifndef __INVERSE__
#define __INVERSE__


/* エフェクト関数 */
BOOL        executable_inverse(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_inverse(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);

#endif /* __INVERSE__ */
