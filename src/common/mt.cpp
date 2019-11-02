// mt.cpp …インプリメンテーションファイル
#include "stdafx.h"
#include "mt.h"


// 定数の定義
#define N              (624)                // length of state vector
#define M              (397)                // a period parameter
#define K              (0x9908B0DFU)        // a magic constant
#define hiBit(u)       ((u) & 0x80000000U)  // mask all but highest   bit of u
#define loBit(u)       ((u) & 0x00000001U)  // mask all but lowest    bit of u
#define loBits(u)      ((u) & 0x7FFFFFFFU)  // mask     the highest   bit of u
#define mixBits(u, v)  (hiBit(u)|loBits(v)) // move hi bit of u to hi bit of v


// anonymous namespace
namespace
{
	mt_uint32   state[N+ 1 ];               // state vector + 1 extra to not violate ANSI C
	mt_uint32_p next = state + 1;           // next random value is computed from here
}


// MT法を初期化
EXTERN_C
void mt_srand(mt_uint32 seed)
{
	mt_uint32_p s = state;

	*s++ = seed;
	for(int i = 0; i < N - 1; i++)
	{
		seed *= 69069U;
		*s++  = seed & 0xFFFFFFFFU;
	}
}

// MT法による乱数生成
EXTERN_C
mt_uint32 mt_rand(void)
{
	mt_uint32 x;

	static int count = N;
	if(++count > N - 1)
	{
		count = 0;
		next = state + 1;

		// 
		mt_uint32 s0 = state[0];
		mt_uint32 s1 = state[1];
		mt_uint32_p p0 = state;
		mt_uint32_p p2 = state + 2;
		mt_uint32_p pM = state + M;
		for(int i = 0; i < N - M; i++)
		{
			*p0++ = *pM++ ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);
			s0 = s1;
			s1 = *p2++;
		}

		// 
		pM = state;
		for(int j = 0; j < M - 1; j++)
		{
			*p0++ = *pM++ ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);
			s0 = s1;
			s1 = *p2++;
		}

		s1  = state[0];
		*p0 = *pM ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);

		x = s1;
	}
	else
	{
		x = *next++;
	}

	x ^= (x >> 11);
	x ^= (x <<  7) & 0x9D2C5680U;
	x ^= (x << 15) & 0xEFC60000U;

	return x ^ (x >> 18);
}
