/* solarization.h …ソラリゼーションフィルタ */
#ifndef __SOLOARIZATION__
#define __SOLOARIZATION__


/* ソラリゼーションのパラメータ */
typedef struct tagSOLARIZATIONPARAM
{
	int threshold;                          /* 閾値 */
	BOOL channel_red;                       /* 適用チャネル（赤） */
	BOOL channel_green;                     /* 適用チャネル（緑） */
	BOOL channel_blue;                      /* 適用チャネル（青） */
} SOLARIZATIONPARAM;


/* エフェクト関数・オプション関数 */
BOOL        executable_solarization(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_solarization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
BOOL        option_solarization(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __SOLOARIZATION__ */
