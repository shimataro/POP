/* posterization.h …ポスタリゼーションフィルタ */
#ifndef __POSTERIZATION__
#define __POSTERIZATION__


/* パラメータの下限、上限 */
#define POSTERIZATION_TONE_MIN  (1)     /* 階調の最小値 */
#define POSTERIZATION_TONE_MAX  (16)    /* 階調の最大値 */


/* ポスタリゼーションのパラメータ */
typedef struct tagPOSTERIZATIONPARAM
{
	int tone;                               /* 階調 */
} POSTERIZATIONPARAM;


/* エフェクト関数・オプション関数 */
BOOL        executable_posterization(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_posterization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
BOOL        option_posterization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __POSTERIZATION__ */
