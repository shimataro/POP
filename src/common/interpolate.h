/* interpolate.h …画像の補間アルゴリズム */
#ifndef __INTERPOLATE__
#define __INTERPOLATE__

#include "image.h"


/* 補間の種類 */
typedef enum
{
	IM_NEAREST,                             /* 最近傍補間（０次） */
	IM_LINER,                               /* 線形補間（１次） */
	IM_SPLINE,                              /* スプライン補間（３次） */
} INTERPOLATE_METHOD;


/* 関数のプロトタイプ宣言 */
EXTERN_C uint8_t WINAPI interpolate(const_uint8_ptr_t pixels, const double x, const double y, const int width, const int height, const INTERPOLATE_METHOD type);

#endif /* __INTERPOLATE__ */
