/* interpolate.h �c�摜�̕�ԃA���S���Y�� */
#ifndef __INTERPOLATE__
#define __INTERPOLATE__

#include "image.h"


/* ��Ԃ̎�� */
typedef enum
{
	IM_NEAREST,                             /* �ŋߖT��ԁi�O���j */
	IM_LINER,                               /* ���`��ԁi�P���j */
	IM_SPLINE,                              /* �X�v���C����ԁi�R���j */
} INTERPOLATE_METHOD;


/* �֐��̃v���g�^�C�v�錾 */
EXTERN_C uint8_t WINAPI interpolate(const_uint8_ptr_t pixels, const double x, const double y, const int width, const int height, const INTERPOLATE_METHOD type);

#endif /* __INTERPOLATE__ */
