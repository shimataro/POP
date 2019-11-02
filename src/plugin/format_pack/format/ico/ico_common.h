// ico_common.h
#ifndef __ICO_COMMON__
#define __ICO_COMMON__

#include <windef.h>


// http://www.bd.wakwak.com/%7Eopapa/VC/Specification/Icon.htm
typedef struct tagICONDIRENTRY
{
	BYTE  bWidth;                           // アイコンの幅
	BYTE  bHeight;                          // アイコンの高さ
	BYTE  bColorCount;                      // 色数
	BYTE  bReserved;                        // must be 0x00
	WORD  wPlanes;                          // プレーン数
	WORD  wBitCount;                        // ビットカウント
	DWORD dwBytesInRes;                     // リソースのサイズ
	DWORD dwImageOffset;                    // ICONDIRの先頭からBITMAPINFOHEADERの先頭までのオフセット
} ICONDIRENTRY;

typedef struct tagICONDIR
{
	WORD         idReserved;                // must be 0x0000
	WORD         idType;                    // must be 0x0001
	WORD         idCount;                   // アイコンの数
	ICONDIRENTRY idEntries[1];              // idCountだけ存在
} ICONDIR;

#endif // __ICO_COMMON__
