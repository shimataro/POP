/* mathematics.h …数学関数 */
#ifndef __MATHEMATICS__
#define __MATHEMATICS__

#include <windows.h>                    /* EXTERN_C */
#include <math.h>                       /* 標準数学ライブラリも使えるようにする */


/******************************************************************************/
/* 定数の定義 */
#define MATH_PI       3.14159265358979323846264338327950288419716939937510  /* 円周率 */
#define MATH_EPSILON  (1.0e-2)          /* 許容誤差(0.01) */
#define RAD2DEG       (180 / MATH_PI)   /* 弧度→度数 */
#define DEG2RAD       (1 / RAD2DEG)     /* 度数→弧度 */


/******************************************************************************/
/* 関数のプロトタイプ宣言 */
EXTERN_C double        math_hypot(double x, double y);
EXTERN_C double        math_sinc(const double x);
EXTERN_C unsigned long math_gcd(unsigned long x, unsigned long y);
EXTERN_C unsigned long math_round(const double x);
EXTERN_C void          math_xy2pole(const double x, const double y, double *const r, double *const angle);
EXTERN_C void          math_pole2xy(const double r, const double angle, double *const x, double *const y);
EXTERN_C double        math_neville(const double t, const double x[], const double y[], const int n);


#endif  /* __MATHEMATICS__ */
