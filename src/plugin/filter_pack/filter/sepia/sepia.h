/* sepia.h …セピアフィルタ */
#ifndef __SEPIA__
#define __SEPIA__


/* パラメータの下限、上限 */
#define SEPIA_STRENGTH_MIN  (0)
#define SEPIA_STRENGTH_MAX  (9)


/* セピアのパラメータ */
typedef struct tagSEPIAPARAM
{
	int strength;                           /* セピアの強度 */
} SEPIAPARAM;


/* エフェクト関数・オプション関数 */
BOOL        executable_sepia(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_sepia(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
BOOL        option_sepia(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __SEPIA__ */
