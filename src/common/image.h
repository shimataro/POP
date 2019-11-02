/* image.h …Photo Operatorの中核 */
#ifndef __IMAGE__
#define __IMAGE__

#include <windows.h>


/******************************************************************************/
/* 列挙型の定義 */
typedef enum tagIMAGERESULT             /* 処理結果 */
{
	IR_SUCCESS = 0,                         /* 成功 */
	IR_CANCELED,                            /* 処理がキャンセルされた */
	IR_INCORRECTHEADER,                     /* ヘッダが正しくない */
	IR_IMAGEDESTROYED,                      /* イメージが壊れている（ヘッダは正しい） */
	IR_NOMEMORY,                            /* メモリ不足 */
	IR_READERROR,                           /* 読み込みエラー */
	IR_WRITEERROR,                          /* 書き込みエラー */
	IR_ILLEGALPARAM,                        /* 不正なパラメータ */
	IR_APIERROR,                            /* API呼び出しエラー */
	IR_UNKNOWN,                             /* 不明なエラー */
} IMAGERESULT;


/******************************************************************************/
/* 型の定義 */

/* 符号付き8ビット整数 */
typedef       signed char  int8_t;
typedef       signed char *int8_ptr_t;
typedef const signed char  const_int8_t;
typedef const signed char *const_int8_ptr_t;

/* 符号付き16ビット整数 */
typedef       signed short  int16_t;
typedef       signed short *int16_ptr_t;
typedef const signed short  const_int16_t;
typedef const signed short *const_int16_ptr_t;

/* 符号付き32ビット整数 */
typedef       signed long  int32_t;
typedef       signed long *int32_ptr_t;
typedef const signed long  const_int32_t;
typedef const signed long *const_int32_ptr_t;

/* 符号付き64ビット整数 */
typedef       signed __int64  int64_t;
typedef       signed __int64 *int64_ptr_t;
typedef const signed __int64  const_int64_t;
typedef const signed __int64 *const_int64_ptr_t;

/* 符号なし8ビット整数 */
typedef       unsigned char  uint8_t;
typedef       unsigned char *uint8_ptr_t;
typedef const unsigned char  const_uint8_t;
typedef const unsigned char *const_uint8_ptr_t;

/* 符号なし16ビット整数 */
typedef       unsigned short  uint16_t;
typedef       unsigned short *uint16_ptr_t;
typedef const unsigned short  const_uint16_t;
typedef const unsigned short *const_uint16_ptr_t;

/* 符号なし32ビット整数 */
typedef       unsigned long  uint32_t;
typedef       unsigned long *uint32_ptr_t;
typedef const unsigned long  const_uint32_t;
typedef const unsigned long *const_uint32_ptr_t;

/* 符号なし32ビット整数 */
typedef       unsigned __int64  uint64_t;
typedef       unsigned __int64 *uint64_ptr_t;
typedef const unsigned __int64  const_uint64_t;
typedef const unsigned __int64 *const_uint64_ptr_t;

/* unsigned int */
typedef       unsigned int  uint_t;
typedef       unsigned int *uint_ptr_t;
typedef const unsigned int  const_uint_t;
typedef const unsigned int *const_uint_ptr_t;

/* voidポインタ */
typedef       void *void_ptr_t;
typedef const void *const_void_ptr_t;

typedef int (CALLBACK *callback_t)(const int num, const int denom, const long data);


/******************************************************************************/
/* 1画素の輝度値を格納する型 */

/* グレイスケール（8ビット） */
typedef union _pixel_gray8_t
{
	uint16_t value;
	struct elements
	{
		uint8_t gray;
		uint8_t alpha;
	} el;
} pixel_gray8_t;
typedef       pixel_gray8_t *pixel_gray8_ptr_t;
typedef const pixel_gray8_t  const_pixel_gray8_t;
typedef const pixel_gray8_t *const_pixel_gray8_ptr_t;

/* グレイスケール（16ビット） */
typedef union _pixel_gray16_t
{
	uint32_t value;
	struct elements
	{
		uint16_t gray;
		uint16_t alpha;
	} el;
} pixel_gray16_t;
typedef       pixel_gray16_t *pixel_gray16_ptr_t;
typedef const pixel_gray16_t  const_pixel_gray16_t;
typedef const pixel_gray16_t *const_pixel_gray16_ptr_t;

/* カラーパレット（8ビット） */
typedef struct _palette8_t
{
	uint8_t b;                              /* 青 */
	uint8_t g;                              /* 緑 */
	uint8_t r;                              /* 赤 */
	uint8_t a;                              /* 不透明度 */
} palette8_t;
typedef       palette8_t *palette8_ptr_t;
typedef const palette8_t  const_palette8_t;
typedef const palette8_t *const_palette8_ptr_t;

/* カラーパレット（16ビット） */
typedef struct _palette16_t
{
	uint16_t b;                             /* 青 */
	uint16_t g;                             /* 緑 */
	uint16_t r;                             /* 赤 */
	uint16_t a;                             /* 不透明度 */
} palette16_t;
typedef       palette16_t *palette16_ptr_t;
typedef const palette16_t  const_palette16_t;
typedef const palette16_t *const_palette16_ptr_t;

/* RGB（8ビット） */
typedef union _pixel_rgb8_t
{
	uint32_t value;
	struct elements
	{
		uint8_t b;                              /* 青 */
		uint8_t g;                              /* 緑 */
		uint8_t r;                              /* 赤 */
		uint8_t a;                              /* 不透明度 */
	} el;
} pixel_rgb8_t;
typedef       pixel_rgb8_t *pixel_rgb8_ptr_t;
typedef const pixel_rgb8_t  const_pixel_rgb8_t;
typedef const pixel_rgb8_t *const_pixel_rgb8_ptr_t;

/* RGB（16ビット） */
typedef union _pixel_rgb16_t
{
	uint64_t value;
	struct elements
	{
		uint16_t b;                             /* 青 */
		uint16_t g;                             /* 緑 */
		uint16_t r;                             /* 赤 */
		uint16_t a;                             /* 不透明度 */
	} el;
} pixel_rgb16_t;
typedef       pixel_rgb16_t *pixel_rgb16_ptr_t;
typedef const pixel_rgb16_t  const_pixel_rgb16_t;
typedef const pixel_rgb16_t *const_pixel_rgb16_ptr_t;


/******************************************************************************/
/* 定数の定義 */

/* サポートしているビット数 */
#define SUPPORT_GRAYSCALE (0x0001)      /* グレイスケール256階調 */
#define SUPPORT_INDEX     (0x0002)      /* インデックスカラー256色 */
#define SUPPORT_RGB       (0x0004)      /* RGB24bit */

/* カラーモード */
#define COLORMODE_GRAYSCALE  0          /* グレイスケール */
#define COLORMODE_INDEX      1          /* インデックスカラー */
#define COLORMODE_RGB        2          /* RGBカラー */


/* γ値 */
#define GAMMA_MIN              (0.01)   /* 最小値 */
#define GAMMA_MAX              (1.00)   /* 最大値 */
#define GAMMA_DEFAULT_FILE     (1 / 2.2)/* デフォルトのファイルガンマ */
#define GAMMA_DEFAULT_DECODE   (1.0)    /* デフォルトのデコーディングガンマ */
#define GAMMA_DEFAULT_DISPLAY  (2.2)    /* デフォルトのディスプレイガンマ */


/******************************************************************************/
/* 構造体の定義 */
typedef struct tagIMAGE_INFO            /* 画像情報 */
{
	uint16_t width, height;                 /* 幅、高さ */
	uint16_t depth;                         /* ビット深度 */
	uint16_t mode;                          /* カラーモード（COLORMODE_???） */
	BOOL     alpha;                         /* アルファチャネルを含んでいるか */
	uint32_t colors;                        /* 実際に使われている色数 */
	uint16_t density_x, density_y;          /* 水平、垂直方向密度 */
	double   gamma_file;                    /* ファイルガンマ */
	char    *comment;                       /* 画像ファイル内のテキスト情報（保存時にのみ使用） */
} IMAGE_INFO;

typedef struct tagIMAGE_DATA            /* 画像データ */
{
	uint32_t image_size;                    /* 画素数 */
	uint32_t extra_size;                    /* 追加データサイズ */
	HGLOBAL image;                          /* 画像データ */
	HGLOBAL extra;                          /* 追加データ（カラーパレットなど） */
} IMAGE_DATA;


/******************************************************************************/
/* マクロ関数の定義 */

/* 配列のサイズ */
#define SIZE_OF_ARRAY(array)    (sizeof(array) / sizeof(array[0]))

/* 画像に関する計算 */
#define CALC_OFFSET(x, y, hsize)                    ((x) + (y) * (hsize))
#define CALC_BOUND(hsize, bound)                    (((hsize) + (bound) - 1) / (bound) * (bound))
#define CALC_HSIZE(width, bit)                      (((width) * (bit) + 7) / 8)
#define CALC_IMAGE_SIZE(width, height, bit, bound)  (CALC_BOUND(CALC_HSIZE(width, bit), bound) * (height))
#define CALC_IMAGE_PIXELS(width, height)            ((width) * (height))

#define CALC_PACK_OFFSET(pos, log2_depth)   ((pos) >> (~(log2_depth) & 0x03))
#define CALC_PACK_SHIFTS(pos, log2_depth)   ((~(pos) << (log2_depth)) & 0x07)

#define PACK_EXTRACT(pack, depth, shifts)   (((pack) >> (shifts)) & (0xff >> (8 - depth)))
#define PACK_STORE(pack, data, shifts)      (pack |= (data) << (shifts))

/* ハッシュ */
#define MAX_COLOR_HASH  0x0fff
#define HASH_COLOR_RGB(r, g, b)     (((r) << 4) & 0x0f00) | ((g) & 0x00f0) | (((b) >> 4) & 0x000f)
#define HASH_COLOR_REF(rgb)         (((rgb) >> 12) & 0x0f00) | (((rgb) >> 8) & 0x00f0) | (((rgb) >> 4) & 0x000f)

/* グレイスケール値 */
/* r * 0.3 + g * 0.59 + b * 0.11 */
#define GRAYSCALE(r, g, b)  (((r) * 77 + (g) * 151 + (b) * 28) >> 8)


/* 名前空間の定義 */
#ifdef __cplusplus

#define BEGIN_NAMESPACE(name)       namespace name {
#define BEGIN_NAMESPACE_NONAME()    namespace {
#define END_NAMESPACE()             }

#endif /* __cplusplus */

#endif
