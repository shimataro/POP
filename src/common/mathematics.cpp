// mathematics.cpp …インプリメンテーションファイル
#include "stdafx.h"
#include "mathematics.h"

#undef max
#undef min
#include <algorithm>                    // std::swap()
#include <valarray>                     // std::valarray<>


// 直角三角形の斜辺の長さ（Moler-Morrison法）
EXTERN_C
double math_hypot(double x, double y)
{
	// どちらも0なら0を返す
	if(x == 0 && y == 0)
	{
		return 0;
	}

	// 長さの絶対値を取る
	if(x < 0) {x = -x;}
	if(y < 0) {y = -y;}

	// x≧y となるように入れ替え
	if(x < y)
	{
		std::swap(x, y);
	}

	// 倍精度実数型なら3回のループで十分な精度が得られる
	// 最適化によりループアンローリングが行われる（ことを期待）
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

// 標本化関数（sin(x) / x）
EXTERN_C
double math_sinc(const double x)
{
	const double x2 = x * x;                // xの2乗
	double c = 1;                           // 係数
	double val = 1;

	for(int i = 2; c * c > MATH_EPSILON * MATH_EPSILON; i += 2)
	{
		c = -c * x2 / (i * (i + 1));
		val += c;
	}

	return val;
}

// 最大公約数（Eucridの互助法）
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

// 四捨五入
EXTERN_C
unsigned long math_round(const double x)
{
	return static_cast<unsigned long>(x + 0.5);
}

// XY座標から極座標へ変換
EXTERN_C
void math_xy2pole(const double x, const double y, double *const r, double *const angle)
{
	// 絶対値及び角度を計算
	*r     = math_hypot(x, y);
	*angle = atan2(y, x);
}

// 極座標からXY座標へ変換
EXTERN_C
void math_pole2xy(const double r, const double angle, double *const x, double *const y)
{
	// X座標、Y座標を計算
	*x = r * cos(angle);
	*y = r * sin(angle);
}

// Neville補間
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
