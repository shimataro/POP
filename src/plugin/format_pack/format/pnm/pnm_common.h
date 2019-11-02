// PNM_common.h …
#ifndef __PNM_COMMON__
#define __PNM_COMMON__


////////////////////////////////////////////////////////////////////////////////
// 定数の定義
#define PNM_SIGNATURE_SIZE    2
#define PNM_SIGNATURE_ASCII_BITMAP      "P1"
#define PNM_SIGNATURE_ASCII_GRAYSCALE   "P2"
#define PNM_SIGNATURE_ASCII_COLOR       "P3"
#define PNM_SIGNATURE_BINARY_BITMAP     "P4"
#define PNM_SIGNATURE_BINARY_GRAYSCALE  "P5"
#define PNM_SIGNATURE_BINARY_COLOR      "P6"


////////////////////////////////////////////////////////////////////////////////
// PNM形式独自のデータ
typedef struct tagPNMEXTRA
{
	int max_color_components;               // 最大輝度値
} PNMEXTRA;


////////////////////////////////////////////////////////////////////////////////
// ピクセルモード（アスキー／バイナリ）
typedef enum PNM_PIXELMODE
{
	PP_ASCII,
	PP_BINARY,
} PNM_PIXELMODE;

// カラーモード（ビットマップ／グレイスケール／フルカラー）
typedef enum PNM_COLORMODE
{
	PC_BITMAP,
	PC_GRAYSCALE,
	PC_COLOR,
} PNM_COLORMODE;

// PNGモード
typedef struct tagPNMMODE
{
	char *name;
	PNM_PIXELMODE pixelmode;
	PNM_COLORMODE colormode;
	uint_t bits;
	int mode;
} PNMMODE;


const PNMMODE pnm_mode[] =
{
//	{PNM_SIGNATURE_ASCII_BITMAP    , PP_ASCII , PC_BITMAP   , 8, COLORMODE_INDEX},     // アスキー／ビットマップ
	{PNM_SIGNATURE_ASCII_GRAYSCALE , PP_ASCII , PC_GRAYSCALE, 8, COLORMODE_GRAYSCALE}, // アスキー／グレイスケール
	{PNM_SIGNATURE_ASCII_COLOR     , PP_ASCII , PC_COLOR    , 8, COLORMODE_RGB},       // アスキー／フルカラー
//	{PNM_SIGNATURE_BINARY_BITMAP   , PP_BINARY, PC_BITMAP   , 8, COLORMODE_INDEX},     // バイナリ／ビットマップ
	{PNM_SIGNATURE_BINARY_GRAYSCALE, PP_BINARY, PC_GRAYSCALE, 8, COLORMODE_GRAYSCALE}, // バイナリ／グレイスケール
	{PNM_SIGNATURE_BINARY_COLOR    , PP_BINARY, PC_COLOR    , 8, COLORMODE_RGB},       // バイナリ／フルカラー
};

#endif // __PNM_COMMON__
