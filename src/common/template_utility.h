// template_utility.h …テンプレートユーティリティー関数
#ifndef __TEMPLATE_UTILITY__
#define __TEMPLATE_UTILITY__

#include <memory.h>                     // memcmp()


////////////////////////////////////////////////////////////////////////////////
// テンプレート関数

// バイナリ比較で等しいかどうかの判定
template<typename _Type>
bool binary_equal(const _Type &data1, const _Type &data2)
{
	return memcmp(&data1, &data2, sizeof(_Type)) == 0;
}

// dataをlow～highの範囲で飽和させる
template<typename _Type>
_Type saturate(const _Type &data, const _Type &low, const _Type &high)
{
	if(data < low)
	{
		return low;
	}
	if(data > high)
	{
		return high;
	}
	return data;
}


// 整数型限定マルチバイト読み込み関数（ビッグエンディアン）
template<typename integer_type>
void get_multi_bytes_bigendian(integer_type &data, const unsigned char buffer[], const int size)
{
	data = 0;
	for(int i = 0; i < size; i++)
	{
		data <<= 8;
		data |= buffer[i];
	}
}

// 整数型限定マルチバイト読み込み関数（リトルエンディアン）
template<typename integer_type>
void get_multi_bytes_littleendian(integer_type &data, const unsigned char buffer[], const int size)
{
	data = 0;
	for(int i = size - 1; i >= 0; i--)
	{
		data <<= 8;
		data |= buffer[i];
	}
}

// 整数型限定マルチバイト書き込み関数（ビッグエンディアン）
template<typename integer_type>
void set_multi_bytes_bigendian(unsigned char buffer[], integer_type data, const int size)
{
	for(int i = size - 1; i >= 0; i--)
	{
		buffer[i] = static_cast<unsigned char>(data);
		data >>= 8;
	}
}

// 整数型限定マルチバイト書き込み関数（リトルエンディアン）
template<typename integer_type>
void set_multi_bytes_littleendian(unsigned char buffer[], integer_type data, const int size)
{
	for(int i = 0; i < size; i++)
	{
		buffer[i] = static_cast<unsigned char>(data);
		data >>= 8;
	}
}

#endif // __UTILITY__
