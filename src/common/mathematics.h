/* mathematics.h �c���w�֐� */
#ifndef __MATHEMATICS__
#define __MATHEMATICS__

#include <windows.h>                    /* EXTERN_C */
#include <math.h>                       /* �W�����w���C�u�������g����悤�ɂ��� */


/******************************************************************************/
/* �萔�̒�` */
#define MATH_PI       3.14159265358979323846264338327950288419716939937510  /* �~���� */
#define MATH_EPSILON  (1.0e-2)          /* ���e�덷(0.01) */
#define RAD2DEG       (180 / MATH_PI)   /* �ʓx���x�� */
#define DEG2RAD       (1 / RAD2DEG)     /* �x�����ʓx */


/******************************************************************************/
/* �֐��̃v���g�^�C�v�錾 */
EXTERN_C double        math_hypot(double x, double y);
EXTERN_C double        math_sinc(const double x);
EXTERN_C unsigned long math_gcd(unsigned long x, unsigned long y);
EXTERN_C unsigned long math_round(const double x);
EXTERN_C void          math_xy2pole(const double x, const double y, double *const r, double *const angle);
EXTERN_C void          math_pole2xy(const double r, const double angle, double *const x, double *const y);
EXTERN_C double        math_neville(const double t, const double x[], const double y[], const int n);


#endif  /* __MATHEMATICS__ */
