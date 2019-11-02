// ras_common.h …
#ifndef __RAS_COMMON__
#define __RAS_COMMON__


// 定数の定義
#define RAS_MAGIC     0x59a66a95        // マジックナンバー
#define sizeofRASTERFILEHEADER  32      // ヘッダのサイズ
#define BOUND_RAS     2                 // Rasterファイルのラインバウンダリ

// カラーマップのタイプ
#define RMT_NONE	    0                 // カラーマップなし
#define RMT_EQUAL_RGB	1                 // red[maplength/3],green[],blue[]
#define RMT_RAW         2                 // 

// RASイメージのタイプ
#define RT_OLD            0             // Raw pixrect image in 68000 byte order
#define RT_STANDARD       1             // Raw pixrect image in 68000 byte order
#define RT_BYTE_ENCODED   2             // Run-length compression of bytes
#define RT_FORMAT_RGB     3             // XRGB or RGB instead of XBGR or BGR
#define RT_FORMAT_TIFF    4             // tiff <-> standard rasterfile
#define RT_FORMAT_IFF     5             // iff (TAAC format) <-> standard rasterfile
#define RT_EXPERIMENTAL   0xffff        // Reserved for testing


// ヘッダ
typedef struct tagRASTERFILEHEADER
{
	uint32_t rfMagic;                       // マジックナンバー
	uint32_t rfWidth;                       // 画像の幅
	uint32_t rfHeight;                      // 画像の高さ
	uint32_t rfBits;                        // 画像のビット数（1,8,24）
	uint32_t rfSize;                        // 画像のサイズ
	uint32_t rfType;                        // 画像のタイプ
	uint32_t rfMaptype;                     // カラーマップのタイプ
	uint32_t rfMapsize;                     // カラーマップのサイズ
} RASTERFILEHEADER;

#endif // __RAS_COMMON__
