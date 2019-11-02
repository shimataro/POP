// error_detect.h �c��茟�o���[�`��
#ifndef __ERROR_DETECT__
#define __ERROR_DETECT__

#include <limits.h>                     // CHAR_BIT


////////////////////////////////////////////////////////////////////////////////
// �֐��̃v���g�^�C�v�錾

// �`�F�b�N�T��
template<typename _Type>
_Type checksum(const unsigned char data[], const int size)
{
	_Type sum = 0;

	/* data�̒��g��S�ĉ����� */
	for(int i = 0; i < n; i++)
	{
		sum += data[i];
	}
	return sum;                             /* �f�[�^�̘a��Ԃ� */
}


////////////////////////////////////////////////////////////////////////////////
// ����璷����(Cyclic Redundancy Check)
// XMODEM�����i������ACRC�����l0�A�r�b�g���]���s��Ȃ��j
#define CRCPOLY_CCITT   0x1021U         // CCITT�����̑�����(1 0001 0000 0010 0001)
#define CRCPOLY_ANSI    0x8005U         // CRC-ANSI�̑����� (1 1000 0000 0000 0101)
#define CRCPOLY_PKZIP   0x04c11db7UL    // PKZIP��ish�ȂǂŎg�p����Ă��鑽����(1 0000 0100 1100 0001 0001 1101 1011 0111)

template<typename _Type>
class crc
{
public:
	typedef unsigned char data_type;
	typedef _Type crc_type;

	explicit crc(const _Type &crc_poly, const _Type &crc_value = _Type()) : m_crc_poly(crc_poly)
	{create_table(); reset_crc_value(crc_value);}

	// CRC�l���w��l�Ƀ��Z�b�g
	_Type reset_crc_value(const _Type &crc_value = _Type())
	{_Type tmp = m_crc_value; m_crc_value = crc_value; return tmp;}

	// 1�o�C�g���M
	_Type send_datum(const unsigned char datum)
	{return send_data(&datum, 1);}

	// �����o�C�g���M
	_Type send_data(const unsigned char data[], const int size)
	{
		const _Type *table = m_table;
		_Type crc_value = m_crc_value;
		for(int i = 0; i < size; i++)
		{
			const int shift = CHAR_BIT * (sizeof(_Type) - 1);
			const unsigned char table_index = (crc_value >> shift) ^ data[i];

			crc_value <<= CHAR_BIT;
			crc_value  ^= table[table_index];
		}
		m_crc_value = crc_value;
		return crc_value;
	}

	// CRC�l���擾
	_Type get_crc(void)
	{return m_crc_value;}

private:
	const _Type m_crc_poly;
	_Type m_crc_value;
	_Type m_table[1 << CHAR_BIT];

	// �e�[�u�����쐬
	void create_table(void)
	{
		_Type *table = m_table;
		for(int i = 0; i < sizeof(m_table) / sizeof(m_table[0]); i++)
		{
			const int shift = CHAR_BIT * (sizeof(_Type) - 1);
			_Type r = i << shift;
			for(int j = 0; j < CHAR_BIT; j++)
			{
				const _Type tmp = r;
				r <<= 1;

				// ����r�̍ŏ�ʃr�b�g��1�̏ꍇ
				if((r >> 1) != tmp)
				{
					r ^= m_crc_poly;
				}
			}
			table[i] = r;
		}
	}
};

#endif // __ERROR_DETECT__
