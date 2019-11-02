// mathematics.cpp �c�C���v�������e�[�V�����t�@�C��
#include "stdafx.h"
#include "mathematics.h"

#undef max
#undef min
#include <algorithm>                    // std::swap()
#include <valarray>                     // std::valarray<>


// ���p�O�p�`�̎Εӂ̒����iMoler-Morrison�@�j
EXTERN_C
double math_hypot(double x, double y)
{
	// �ǂ����0�Ȃ�0��Ԃ�
	if(x == 0 && y == 0)
	{
		return 0;
	}

	// �����̐�Βl�����
	if(x < 0) {x = -x;}
	if(y < 0) {y = -y;}

	// x��y �ƂȂ�悤�ɓ���ւ�
	if(x < y)
	{
		std::swap(x, y);
	}

	// �{���x�����^�Ȃ�3��̃��[�v�ŏ\���Ȑ��x��������
	// �œK���ɂ�胋�[�v�A�����[�����O���s����i���Ƃ����ҁj
	const int count = 3;
	for(int i = 0; i < count; i++)
	{
		double t = y / x;
		t *= t;
		t /= 4 + t;
		x += 2 * x * t;
		y *= t;
	}
	return x;
}

// �W�{���֐��isin(x) / x�j
EXTERN_C
double math_sinc(const double x)
{
	const double x2 = x * x;                // x��2��
	double c = 1;                           // �W��
	double val = 1;

	for(int i = 2; c * c > MATH_EPSILON * MATH_EPSILON; i += 2)
	{
		c = -c * x2 / (i * (i + 1));
		val += c;
	}

	return val;
}

// �ő���񐔁iEucrid�̌ݏ��@�j
EXTERN_C
unsigned long math_gcd(unsigned long x, unsigned long y)
{
	while(y != 0)
	{
		unsigned long t = x % y;
		x = y;
		y = t;
	}
	return x;
}

// �l�̌ܓ�
EXTERN_C
unsigned long math_round(const double x)
{
	return static_cast<unsigned long>(x + 0.5);
}

// XY���W����ɍ��W�֕ϊ�
EXTERN_C
void math_xy2pole(const double x, const double y, double *const r, double *const angle)
{
	// ��Βl�y�ъp�x���v�Z
	*r     = math_hypot(x, y);
	*angle = atan2(y, x);
}

// �ɍ��W����XY���W�֕ϊ�
EXTERN_C
void math_pole2xy(const double r, const double angle, double *const x, double *const y)
{
	// X���W�AY���W���v�Z
	*x = r * cos(angle);
	*y = r * sin(angle);
}

// Neville���
EXTERN_C
double math_neville(const double t, const double x[], const double y[], const int n)
{
	std::valarray<double> w(y, n);
	for(int i = 0; i < n; i++)
	{
		for(int j = i - 1; j >= 0; j--)
		{
			w[j] = w[j + 1] + (w[j + 1] - w[j]) * (t - x[i]) / (x[i] - x[j]);
		}
	}
	return w[0];
}
