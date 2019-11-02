/* glasstile.h …ガラスタイルフィルタ */
#ifndef __GLASSTILE__
#define __GLASSTILE__


/* パラメータの下限、上限 */
#define TILESIZE_MIN  (1)
#define TILESIZE_MAX  (100)


/* ガラスタイルのパラメータ */
typedef struct tagGLASSTILEPARAM
{
	int width;                              /* ガラスタイルの幅 */
	int height;                             /* ガラスタイルの高さ */
} GLASSTILEPARAM;


/* エフェクト関数・オプション関数 */
BOOL        executable_glasstile(const IMAGE_INFO *info_ptr);
IMAGERESULT effect_glasstile(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, void_ptr_t param, callback_t callback, long callback_data);
BOOL        option_glasstile(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWnd);

#endif /* __GLASSTILE__ */
