/* ripple.h …波紋フィルタ */
#ifndef __RIPPLE__
#define __RIPPLE__


/* 波紋パラメータ */
typedef struct tagRIPPLEPARAM
{
	uint32_t center_x, center_y;            /* 中心座標 */
	double amplitude;                       /* 振幅 */
	double cycle;                           /* 周期 */
	double phase;                           /* 位相 */
	double distance;                        /* 元画像との距離 */
	double refraction;                      /* 屈折率 */
} RIPPLEPARAM;


/* エフェクト関数・オプション関数 */
BOOL        executable_ripple(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_ripple(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
BOOL        option_ripple(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __RIPPLE__ */
