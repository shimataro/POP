// Mersenne Twister法による擬似乱数発生関数
// http://www.math.keio.ac.jp/~matumoto/mt.html
#ifndef __MT__
#define __MT__

#include <windows.h>                    // EXTERN_C


#define MT_RAND_MAX   (0xFFFFFFFFU)


// 型の定義
typedef unsigned long mt_uint32;
typedef mt_uint32    *mt_uint32_p;


// 関数のプロトタイプ宣言
EXTERN_C void      mt_srand(mt_uint32 seed);     // MT法初期化
EXTERN_C mt_uint32 mt_rand(void);                // 乱数取得


#endif // __MT__
