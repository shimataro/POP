// bmp_common.h �c
#ifndef __BMP_COMMON__
#define __BMP_COMMON__

#include <windows.h>


// �萔�̒�`
#define BMP_TYPE  0x4d42                // BITMAPFILEHEADER::bfType�̎��ʕ����i"BM"�j
#define BOUND_BMP 4                     // �r�b�g�}�b�v�t�@�C���̃��C���o�E���_��


// bit �r�b�g�� data ���A max_bit �ɃX�P�[���A�b�v
#define SCALEUP(data, bit, max_bit)     (((data) << ((max_bit) - (bit))) | (data) >> (2 * (bit) - (max_bit)))


// RGB�e5�r�b�g�̉摜�`��
typedef union
{
	uint16_t value;
	struct elements
	{
		unsigned b : 5;                         // ��:5bit
		unsigned g : 5;                         // ��:5bit
		unsigned r : 5;                         // ��:5bit
		unsigned x : 1;                         // �_�~�[:1bit
	} el;
} bitmap_rgb5_t;

#endif // __BMP_COMMON__
