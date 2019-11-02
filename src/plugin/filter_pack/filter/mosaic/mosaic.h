/* mosaic.h …モザイクフィルタ */
#ifndef __MOSAIC__
#define __MOSAIC__


/* モザイクパラメータ */
typedef struct tagMOSAICPARAM
{
	int size_x;                             /* モザイクの大きさ（横） */
	int size_y;                             /* モザイクの大きさ（縦） */
} MOSAICPARAM;


/* エフェクト関数 */
BOOL        executable_mosaic(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_mosaic(const IMAGE_INFO *info, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);

#endif /* __MOSAIC__ */
