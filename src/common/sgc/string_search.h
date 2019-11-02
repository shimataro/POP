// string_search.h �cBoyer-Moore�@�ɂ�镶���񌟍��N���X
#ifndef __STRING_SEARCH__
#define __STRING_SEARCH__

#include "sgc.h"
#include <stddef.h>                     // size_t
#include <algorithm>                    // std::fill_n()

_SGC_BEGIN

// �w���p�N���X
// sgc::string_search �͌����Ƀe�[�u�����g�p����B
// ���̃e�[�u���ɒʏ�̔z����g�p�����ꍇ�A�e���v���[�g�̌^�� char �̂Ƃ���
// �e�[�u���̃C���f�b�N�X�����ɂȂ�\��������B
// �܂��A�e���v���[�g�̌^���N���X�ɂ���ƁA�z����g�����Ƃ��ł��Ȃ��B
// ���������������������邽�߂ɁA�e�[�u���N���X���g�p����B
// �ʏ�̔z��ł͖��̂���^���g�p����Ƃ��ɁA���̌^�̓����o�[�W�������쐬����B
template<typename _Ty>
class _string_search_table
{
public:
	_string_search_table(const size_t &default_value)
	{
		// �������m��
		const size_t size = 1 << (sizeof(_Ty) * 8);
		m_table = new size_t[size];

		// ������
		std::fill_n(m_table, size, default_value);
	}
	~_string_search_table(void) { delete []m_table; }

	size_t &operator[](const _Ty &key)             { return m_table[key]; }
	const size_t &operator[](const _Ty &key) const { return m_table[key]; }

private:
	size_t *m_table;
};

// char �����o�[�W����
template<>
class _string_search_table<char>
{
public:
	_string_search_table(const char &default_value)
	{ std::fill_n(m_table, 256, default_value); }

	size_t &operator[](const char &key)
	{ return m_table[static_cast<unsigned char>(key)]; }

	const size_t &operator[](const char &key) const
	{ return m_table[static_cast<unsigned char>(key)]; }

private:
	size_t m_table[256];
};

// �y���Ӂz
// Boyer-Moore�@�̓e�[�u���̏������������s���K�v������̂ŁA
// �Z����������̌�����1��s�������Ȃ�P���Ȓ��������@�̂ق��������B
// ���̃A���S���Y���́A���ɒ������������������Ƃ���
// ��������������ł������̕����������������Ƃ�
// �ifind() �����o�֐������x���Ăяo���Ƃ��j�ɐ^���𔭊�����B
template<typename _Ty, typename _Table = _string_search_table<_Ty>, const _Ty NUL = _Ty()>
class string_search
{
public:
	// �R���X�g���N�^�i������������w��j
	string_search(const _Ty *search_str)
		: m_search_str(search_str), m_search_len(_Length(search_str)), m_skips(m_search_len + 1)
	{
		// search_str ���̊e�������A���ꂼ���ԍŌ�ɏo�������ꏊ�i�E���牽�Ԗڂ��j���L��
		size_t len = m_search_len;
		while(*search_str != NUL) { m_skips[*search_str++] = len--; }
	}

	// �����i�{���ƕ��������w��j
	int find(const _Ty *base_str, const int base_len) const
	{
		// for�����g���Ώ����Z�������邯�ǂ킩��Â炢�c
		size_t pos = 0;
		while(pos + m_search_len <= base_len)
		{
			// ��r
			if(_IsPrefix(m_search_str, base_str + pos))
			{
				// m_search_str �����S��v
				return pos;
			}

			// ��r�Ɏg��ꂽ���̕���������������Ƀq�b�g����悤�Ɉړ�
			const _Ty next_char = base_str[pos + m_search_len];
			pos += m_skips[next_char];
		}

		// ������Ȃ�����
		return -1;
	}

private:
	const _Ty   *m_search_str;
	const size_t m_search_len;

	_Table m_skips;

	static bool _IsPrefix(const _Ty *strPre, const _Ty *strAll)
	{ while(*strPre != NUL) { if(*strPre++ != *strAll++) { return false; } }
		return true; }

	static size_t _Length(const _Ty *str)
	{ size_t i = 0; while(str[i] != NUL) { i++; }
		return i; }
};

_SGC_END


#endif // __STRING_SEARCH__
