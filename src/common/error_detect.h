// error_detect.h …誤り検出ルーチン
#ifndef __ERROR_DETECT__
#define __ERROR_DETECT__

#include <limits.h>                     // CHAR_BIT


////////////////////////////////////////////////////////////////////////////////
// 関数のプロトタイプ宣言

// チェックサム
template<typename _Type>
_Type checksum(const unsigned char data[], const int size)
{
	_Type sum = 0;

	/* dataの中身を全て加える */
	for(int i = 0; i < n; i++)
	{
		sum += data[i];
	}
	return sum;                             /* データの和を返す */
}


////////////////////////////////////////////////////////////////////////////////
// 巡回冗長検査(Cyclic Redundancy Check)
// XMODEM方式（左送り、CRC初期値0、ビット反転を行わない）
#define CRCPOLY_CCITT   0x1021U         // CCITT勧告の多項式(1 0001 0000 0010 0001)
#define CRCPOLY_ANSI    0x8005U         // CRC-ANSIの多項式 (1 1000 0000 0000 0101)
#define CRCPOLY_PKZIP   0x04c11db7UL    // PKZIPやishなどで使用されている多項式(1 0000 0100 1100 0001 0001 1101 1011 0111)

template<typename _Type>
class crc
{
public:
	typedef unsigned char data_type;
	typedef _Type crc_type;

	explicit crc(const _Type &crc_poly, const _Type &crc_value = _Type()) : m_crc_poly(crc_poly)
	{create_table(); reset_crc_value(crc_value);}

	// CRC値を指定値にリセット
	_Type reset_crc_value(const _Type &crc_value = _Type())
	{_Type tmp = m_crc_value; m_crc_value = crc_value; return tmp;}

	// 1バイト送信
	_Type send_datum(const unsigned char datum)
	{return send_data(&datum, 1);}

	// 複数バイト送信
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

	// CRC値を取得
	_Type get_crc(void)
	{return m_crc_value;}

private:
	const _Type m_crc_poly;
	_Type m_crc_value;
	_Type m_table[1 << CHAR_BIT];

	// テーブルを作成
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

				// 元のrの最上位ビットが1の場合
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
