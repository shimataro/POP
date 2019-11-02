//  gif_common.h …
#ifndef __GIF_COMMON__
#define __GIF_COMMON__


// シグネチャ
#define GIF_SIGNATURE_GIF87A    "GIF87a"
#define GIF_SIGNATURE_GIF89A    "GIF89a"
#define GIF_SIGNATURE_SIZE      6

// ヘッダの定数
#define GIF_INTRODUCER      0x21
#define GIF_LABEL_EXTRA     0xf9
#define GIF_LABEL_NETSCAPE  0xff
#define GIF_TERMINATOR      0x00
#define GIF_SPLIT           0x2c


/*--------------------------------------------------------------------------------------*/
/*	GIFHEADER構造体																		*/
/*	GIFの画面記述情報構造体																*/
/*	※GIF全体の情報を記述したヘッダー部。												*/
/*--------------------------------------------------------------------------------------*/
#include <pshpack1.h>                   // 構造体のメンバ境界を1バイトにする
typedef struct GIFHEADER_GLOBAL
{
	char signature[6];                      // GIF識別記号（"GIF87a" or "GIF89a"）
	WORD width;                             // このGIF全体を表示するのに必要な横pixel数
	WORD height;                            // このGIF全体を表示するのに必要な縦pixel数
	BYTE packet;                            // パケットフィールド（後述のGIFHEADER_PACKET）
	BYTE bgcolor;                           // 背景色
	BYTE aspect;                            // アスペクト比
} GIFHEADER_GLOBAL;
#include <poppack.h>

typedef union
{
	BYTE value;
	struct elements
	{
		unsigned depth      : 3;                // ビット深度-1
		unsigned sort       : 1;                // ソートフラグ
		unsigned resolution : 3;                // 解像度（ビット数-1）
		unsigned palette    : 1;                // パレットフラグ
	} el;
} GIFHEADER_PACKET;


/*--------------------------------------------------------------------------------------*/
/*	GIFEXTRA_NETSCAPE構造体																*/
/*	GIFのアプリケーションエクステンション構造体											*/
/*	※GIFの拡張情報																		*/
/*--------------------------------------------------------------------------------------*/
#include <pshpack1.h>                   // 構造体のメンバ境界を1バイトにする
typedef struct GIFEXTRA_NETSCAPE
{
	BYTE introducer;                        // GIF_INTRODUCER
	BYTE label;                             // GIF_LABEL_NETSCAPE

	BYTE block_size;                        // 11
	char identifier[8];                     // "NETSCAPE"
	char authentication[3];                 // "2.0"
	BYTE block_size2;                       // 3
	BYTE reserved;                          // 0x01
	WORD iteration_count;                   // 繰り返し回数
	BYTE terminator;                        // GIF_TERMINATOR
} GIFEXTRA_NETSCAPE;
#include <poppack.h>


/*--------------------------------------------------------------------------------------*/
/*	GIFEXTRA構造体																		*/
/*	GIFのエクステンション構造体															*/
/*	※GIFの拡張情報																		*/
/*--------------------------------------------------------------------------------------*/
#include <pshpack1.h>                   // 構造体のメンバ境界を1バイトにする
typedef struct GIFEXTRA
{
	BYTE introducer;                        // GIF_INTRODUCER
	BYTE label;                             // GIF_LABEL_EXTRA

	BYTE block_size;                        // 4
	BYTE packet;                            // パケットフィールド（後述の GIFEXTRA_PACKET）
	WORD delay;                             // ディレイタイム（1/100秒）
	BYTE transparent_index;                 // 透過色のインデックス
	BYTE terminator;                        // GIF_TERMINATOR
} GIFEXTRA;
#include <poppack.h>

typedef union
{
	BYTE value;
	struct elements
	{
		unsigned transparent_flag : 1;          // 透過フラグ
		unsigned input            : 1;          // ユーザ入力フラグ
		unsigned disposal_method  : 3;          // 消去法（0:未定義、1:消去しない、2:背景色でクリア、3:表示直前のイメージに戻す）
		unsigned reserved         : 3;          // 予約（未使用）
	} el;
} GIFEXTRA_PACKET;


/*--------------------------------------------------------------------------------------*/
/*	GIFHEADER_LOCAL構造体																*/
/*	GIFのイメージ記述情報構造体															*/
/*	※GIF画像一枚についての情報を記述したもの。											*/
/*--------------------------------------------------------------------------------------*/
#include <pshpack1.h>                   // 構造体のメンバ境界を1バイトにする
typedef struct GIFHEADER_LOCAL
{
	BYTE split;                             // イメージ分離記号（=0x2C）

	WORD left;                              // 画面左上からの表示位置（横pixel数）
	WORD top;                               // 画面左上からの表示位置（縦pixel数）
	WORD width;                             // この画像の横pixel数
	WORD height;                            // この画像の縦pixel数
	BYTE packet;                            // パケットフィールド（後述の GIFIMAGE_PACKET）
} GIFHEADER_LOCAL;
#include <poppack.h>

typedef union
{
	BYTE value;
	struct elements
	{
		unsigned depth     : 3;                 // ビット深度-1
		unsigned reserved  : 2;                 // 予約
		unsigned sort      : 1;                 // ソートフラグ
		unsigned interlace : 1;                 // インターレースフラグ
		unsigned palette   : 1;                 // パレットフラグ
	} el;
} GIFIMAGE_PACKET;


/*--------------------------------------------------------------------------------------*/
/*	GIFRGB構造体																		*/
/*	GIFのパレット構造体																	*/
/*	※RGB順の並びで３バイト構成。														*/
/*--------------------------------------------------------------------------------------*/
#include <pshpack1.h>                   // 構造体のメンバ境界を1バイトにする
typedef struct GIFRGB
{
	BYTE red;                               // 赤成分
	BYTE green;                             // 緑成分
	BYTE blue;                              // 青成分
} GIFRGB;
#include <poppack.h>


#endif // __GIF_COMMON__
