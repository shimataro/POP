// string_search.h …Boyer-Moore法による文字列検索クラス
#ifndef __STRING_SEARCH__
#define __STRING_SEARCH__

#include "sgc.h"
#include <stddef.h>                     // size_t
#include <algorithm>                    // std::fill_n()

_SGC_BEGIN

// ヘルパクラス
// sgc::string_search は検索にテーブルを使用する。
// このテーブルに通常の配列を使用した場合、テンプレートの型が char のときに
// テーブルのインデックスが負になる可能性がある。
// また、テンプレートの型をクラスにすると、配列を使うことができない。
// こういった問題を解消するために、テーブルクラスを使用する。
// 通常の配列では問題のある型を使用するときに、その型の特化バージョンを作成する。
template<typename _Ty>
class _string_search_table
{
public:
	_string_search_table(const size_t &default_value)
	{
		// メモリ確保
		const size_t size = 1 << (sizeof(_Ty) * 8);
		m_table = new size_t[size];

		// 初期化
		std::fill_n(m_table, size, default_value);
	}
	~_string_search_table(void) { delete []m_table; }

	size_t &operator[](const _Ty &key)             { return m_table[key]; }
	const size_t &operator[](const _Ty &key) const { return m_table[key]; }

private:
	size_t *m_table;
};

// char 特化バージョン
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

// 【注意】
// Boyer-Moore法はテーブルの初期化処理を行う必要があるので、
// 短い文字列内の検索を1回行うだけなら単純な逐次検索法のほうが速い。
// このアルゴリズムは、非常に長い文字列を検索するときや
// 同じ検索文字列でいくつもの文字列内を検索するとき
// （find() メンバ関数を何度も呼び出すとき）に真価を発揮する。
template<typename _Ty, typename _Table = _string_search_table<_Ty>, const _Ty NUL = _Ty()>
class string_search
{
public:
	// コンストラクタ（検索文字列を指定）
	string_search(const _Ty *search_str)
		: m_search_str(search_str), m_search_len(_Length(search_str)), m_skips(m_search_len + 1)
	{
		// search_str 中の各文字が、それぞれ一番最後に出現した場所（右から何番目か）を記憶
		size_t len = m_search_len;
		while(*search_str != NUL) { m_skips[*search_str++] = len--; }
	}

	// 検索（本文と文字数を指定）
	int find(const _Ty *base_str, const int base_len) const
	{
		// for文を使えば少し短く書けるけどわかりづらい…
		size_t pos = 0;
		while(pos + m_search_len <= base_len)
		{
			// 比較
			if(_IsPrefix(m_search_str, base_str + pos))
			{
				// m_search_str が完全一致
				return pos;
			}

			// 比較に使われた次の文字が検索文字列にヒットするように移動
			const _Ty next_char = base_str[pos + m_search_len];
			pos += m_skips[next_char];
		}

		// 見つからなかった
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
