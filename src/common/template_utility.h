// template_utility.h �c�e���v���[�g���[�e�B���e�B�[�֐�
#ifndef __TEMPLATE_UTILITY__
#define __TEMPLATE_UTILITY__

#include <memory.h>                     // memcmp()


////////////////////////////////////////////////////////////////////////////////
// �e���v���[�g�֐�

// �o�C�i����r�œ��������ǂ����̔���
template<typename _Type>
bool binary_equal(const _Type &data1, const _Type &data2)
{
	return memcmp(&data1, &data2, sizeof(_Type)) == 0;
}

// data��low�`high�͈̔͂ŖO�a������
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


// �����^����}���`�o�C�g�ǂݍ��݊֐��i�r�b�O�G���f�B�A���j
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

// �����^����}���`�o�C�g�ǂݍ��݊֐��i���g���G���f�B�A���j
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

// �����^����}���`�o�C�g�������݊֐��i�r�b�O�G���f�B�A���j
template<typename integer_type>
void set_multi_bytes_bigendian(unsigned char buffer[], integer_type data, const int size)
{
	for(int i = size - 1; i >= 0; i--)
	{
		buffer[i] = static_cast<unsigned char>(data);
		data >>= 8;
	}
}

// �����^����}���`�o�C�g�������݊֐��i���g���G���f�B�A���j
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
