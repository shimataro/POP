// Mersenne Twister�@�ɂ��[�����������֐�
// http://www.math.keio.ac.jp/~matumoto/mt.html
#ifndef __MT__
#define __MT__

#include <windows.h>                    // EXTERN_C


#define MT_RAND_MAX   (0xFFFFFFFFU)


// �^�̒�`
typedef unsigned long mt_uint32;
typedef mt_uint32    *mt_uint32_p;


// �֐��̃v���g�^�C�v�錾
EXTERN_C void      mt_srand(mt_uint32 seed);     // MT�@������
EXTERN_C mt_uint32 mt_rand(void);                // �����擾


#endif // __MT__
