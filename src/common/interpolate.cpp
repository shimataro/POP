// interpolate.cpp
#include "stdafx.h"
#include "interpolate.h"

#include "template_utility.h"           // cram_range()


#define NEVILLE_N   4


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	uint8_t _interpolate_nearest(const_uint8_ptr_t pixels, const double x, const double y, const int width, const int height);
	uint8_t _interpolate_liner  (const_uint8_ptr_t pixels, const double x, const double y, const int width, const int height);
	uint8_t _interpolate_spline (const_uint8_ptr_t pixels, const double x, const double y, const int width, const int height);

	double spline(const double t, double x[], double y[]);
END_NAMESPACE()


// 補間
EXTERN_C
uint8_t WINAPI interpolate(const_uint8_ptr_t pixels, const double x, const double y, const int width, const int height, const INTERPOLATE_METHOD type)
{
	switch(type)
	{
	case IM_NEAREST:                        // 最近傍補間
		return _interpolate_nearest(pixels, x, y, width, height);

	case IM_LINER:                          // 線形補間
		return _interpolate_liner(pixels, x, y, width, height);

	case IM_SPLINE:                         // スプライン補間
		return _interpolate_spline(pixels, x, y, width, height);

	default:                                // ここには来ない
		__assume(0);
	}
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

// 最近傍補間
uint8_t _interpolate_nearest(const_uint8_ptr_t pixels, const double x, const double y, const int width, const int height)
{
	const uint32_t offset = CALC_OFFSET(static_cast<uint32_t>(x), static_cast<uint32_t>(y), width);
	return pixels[offset];
}

// 線形補間
uint8_t _interpolate_liner(const_uint8_ptr_t pixels, const double x, const double y, const int width, const int height)
{
	const uint32_t offset = CALC_OFFSET(static_cast<uint32_t>(x), static_cast<uint32_t>(y), width);

	const double x1 = x - static_cast<uint32_t>(x);
	const double x2 = 1 - x1;

	const double y1 = y - static_cast<uint32_t>(y);
	const double y2 = 1 - y1;

	const int x_plus_one = (x < width -1) ? 1 : 0;
	const int y_plus_one = (y < height-1) ? width : 0;

	const int c00 = pixels[offset];
	const int c01 = pixels[offset + y_plus_one];
	const int c10 = pixels[offset + x_plus_one];
	const int c11 = pixels[offset + x_plus_one + y_plus_one];

	const double intensity =
		c00 * x2 * y2 +
		c01 * x2 * y1 +
		c10 * x1 * y2 +
		c11 * x1 * y1;

	return static_cast<int>(intensity);
}

// スプライン補間
uint8_t _interpolate_spline(const_uint8_ptr_t pixels, const double x, const double y, const int width, const int height)
{
	const uint32_t offset = CALC_OFFSET(static_cast<uint32_t>(x), static_cast<uint32_t>(y), width);

	// Xの増加量
//	const int x_minus_one = (x > 0) ? -1 : 0;
	const int x_minus_one = (x >= 1) ? -1 : 0;
	const int x_plus_one = (x     < width - 1) ? 1              : 0;
	const int x_plus_two = (x + 1 < width - 1) ? x_plus_one + 1 : 0;

	// Yの増加量
//	const int y_minus_one = (y > 0) ? -width : 0;
	const int y_minus_one = (y >= 1) ? -width : 0;
	const int y_plus_one = (y         < height - 1) ? width              : 0;
	const int y_plus_two = (y + width < height - 1) ? y_plus_one + width : 0;

	const int increments[] = {y_minus_one, 0, y_plus_one, y_plus_two};

	const double tx = x - static_cast<uint32_t>(x);
	const double ty = y - static_cast<uint32_t>(y);

	double x_array[] = {-1, 0, 1, 2};
	double z_array[NEVILLE_N];
	for(int i = 0; i < NEVILLE_N; i++)
	{
		double y_array[] =
		{
			pixels[offset + increments[i] + x_minus_one],
			pixels[offset + increments[i]],
			pixels[offset + increments[i] + x_plus_one],
			pixels[offset + increments[i] + x_plus_two],
		};

		z_array[i] = spline(tx, x_array, y_array);
	}

	// 輝度値の調整
	const double intensity = spline(ty, x_array, z_array);
	return saturate<int>(static_cast<int>(intensity), 0, 255);
}

// Neville補間によるスプライン曲線（yは変化する可能性あり）
double spline(const double t, double x[], double y[])
{
	for(int i = 0; i < NEVILLE_N; i++)
	{
		for(int j = i - 1; j >= 0; j--)
		{
			y[j] = y[j + 1] + (y[j + 1] - y[j]) * (t - x[i]) / (x[i] - x[j]);
		}
	}
	return y[0];
}

END_NAMESPACE()
