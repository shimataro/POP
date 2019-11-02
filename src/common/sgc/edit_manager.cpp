// edit_manager.cpp …インプリメンテーションファイル
#include "edit_manager.h"
#include "charcode.h"                   // charcode_search_linefeed()
#include <assert.h>                     // assert()


_SGC_BEGIN                              // namespace sgc {


////////////////////////////////////////////////////////////
// コンストラクション

// コンストラクタ
edit_manager::edit_manager(void)
{
	set_empty();
}

// デストラクタ
edit_manager::~edit_manager(void)
{
}


////////////////////////////////////////////////////////////
// 情報

// 行サイズを取得
unsigned int edit_manager::get_line_count(void) const
{
	return m_list.size();
}

// 画面上端の行番号を取得
unsigned int edit_manager::get_linenumber_top(void) const
{
	return m_top.linenumber;
}

// 編集中の行番号を取得
unsigned int edit_manager::get_linenumber_now(void) const
{
	return m_now.linenumber;
}


// 現在行が先頭か
bool edit_manager::is_begin(void) const
{
	return m_list.begin() == m_now.pos;
}

// 現在行が末尾か
bool edit_manager::is_end(void) const
{
	const_iterator_t p = m_now.pos;
	return m_list.end() == ++p;
}


// 先頭位置のイテレータを取得
edit_manager::const_iterator_t edit_manager::get_iterator_begin(void) const
{
	return m_list.begin();
}

// 末尾位置のイテレータを取得
edit_manager::const_iterator_t edit_manager::get_iterator_end(void) const
{
	return m_list.end();
}

// 画面上端のイテレータを取得
edit_manager::const_iterator_t edit_manager::get_iterator_top(void) const
{
	return m_top.pos;
}

// 現在位置のイテレータを取得
edit_manager::const_iterator_t edit_manager::get_iterator_now(void) const
{
	return m_now.pos;
}


////////////////////////////////////////////////////////////
// 操作

// 現在のバッファを破棄
void edit_manager::set_empty(void)
{
	m_list.clear();
	m_list.push_back(wstring_t());

	m_top.pos        = m_list.begin();
	m_top.linenumber = 1;

	m_now = m_top;
}

// 現在のバッファを破棄して文字列を設定（改行あり）
int edit_manager::set_data(const wchar_t *wstr)
{
	// [block]
	// 文字列を設定
	{
		stringlist_t buffer;
		_CreateStringList(wstr, buffer);

		m_list.clear();
		m_list.splice(m_list.end(), buffer);
	}

	// [block]
	// 現在位置をバッファの先頭にする
	{
		m_top.pos        = m_list.begin();
		m_top.linenumber = 1;
		m_now = m_top;
	}

	// 行数を返す
	return m_list.size();
}


// 現在位置に文字列を挿入（改行あり）
int edit_manager::insert_data(int &x, const wchar_t *wstr)
{
	// 文字列をテンポラリバッファに格納
	stringlist_t buffer;
	_CreateStringList(wstr, buffer);

	iterator_t p = m_now.pos;

	// 現在位置以降の文字列を取得
	const wstring_t after = p->substr(x);
	p->erase(x);

	// バッファ内の1行目を p の後に追加
	*p += *buffer.begin();
	buffer.pop_front();

	const int lines = buffer.size();

	// バッファのデータを追加
	p++;
	m_list.splice(p, buffer);
	assert(buffer.size() == 0);

	// 最終行の後ろに after を追加
	p--;
	x = p->length();
	*p += after;

	m_now.pos         = p;
	m_now.linenumber += lines;

	// 追加した行数を返す
	return lines;
}

// 現在位置に文字列を挿入（改行なし）
void edit_manager::insert_string(const unsigned int x, const wstring_t &wstr)
{
	assert(x <= m_now.pos->length());

	// 入力文字を挿入
	m_now.pos->insert(x, wstr);
}

// 現在位置に改行を挿入
void edit_manager::insert_linefeed(const unsigned int x)
{
	assert(x <= m_now.pos->length());

	// 現在行を現在位置で分割
	const wstring_t string_front = m_now.pos->substr(0, x);
	*m_now.pos                   = m_now.pos->substr(x);

	// 分割した前半を挿入
	m_list.insert(m_now.pos, string_front);
	m_now.linenumber++;

	// 現在行が画面の上端の場合
	if(m_now.pos == m_top.pos)
	{
		m_top.pos--;
	}

	// 画面上端が現在位置より後方の場合、行番号を調整
	if(m_top.linenumber > m_now.linenumber)
	{
		m_top.linenumber++;
	}
}


// 現在行と次の行を連結
void edit_manager::combine_forward(void)
{
	if(is_end())
	{
		return;
	}

	iterator_t p = m_now.pos;
	p++;

	// 現在行(m_pos_now) と1つ下の行(p) を連結
	*m_now.pos += *p;

	// p が画面上端の場合
	if(m_top.pos == p)
	{
		m_top.pos = m_now.pos;
	}

	// 現在行(p) を削除
	m_list.erase(p);

	// 画面上端が現在位置より後方の場合、行番号を調整
	if(m_top.linenumber > m_now.linenumber)
	{
		m_top.linenumber--;
	}
}

// 現在行と前の行を連結
void edit_manager::combine_backword(void)
{
	if(is_begin())
	{
		return;
	}

	iterator_t p = m_now.pos;
	m_now.pos--;
	m_now.linenumber--;

	// 現在行(p) と1つ上の行(m_pos_now) を連結
	*m_now.pos += *p;

	// p が画面上端の場合
	if(m_top.pos == p)
	{
		m_top.pos = m_now.pos;
	}

	// 現在行(p) を削除
	m_list.erase(p);

	// 画面上端が現在位置より後方の場合、行番号を調整
	if(m_top.linenumber > m_now.linenumber)
	{
		m_top.linenumber--;
	}
}


// 現在行の pos 文字目から count 文字を削除
void edit_manager::delete_string(const unsigned int pos, const unsigned int count)
{
	m_now.pos->erase(pos, count);
}


// 先頭位置からスクロール（正なら末尾方向、負なら先頭方向）
int edit_manager::scroll_top(const int count)
{
	const int lines = _Scroll(m_top.pos, count);
	m_top.linenumber += lines;

	return lines;
}

// 現在位置からスクロール（正なら末尾方向、負なら先頭方向）
int edit_manager::scroll_now(const int count)
{
	const int lines = _Scroll(m_now.pos, count);
	m_now.linenumber += lines;

	return lines;
}

// 絶対位置
void edit_manager::set_position(const linenumber_t linenumber, const bool top, const bool now)
{
	line_info_t info;
	info.linenumber = _GetPosition(info.pos, linenumber);

	if(top) { m_top = info; }
	if(now) { m_now = info; }
}


////////////////////////////////////////////////////////////
// protected関数

int edit_manager::_Scroll(const_iterator_t &pos, const int count) const
{
	if(count > 0)
	{
		return _ScrollForward(pos, count);
	}
	else
	{
		return -_ScrollBackward(pos, -count);
	}
}

// 末尾方向にスクロール
int edit_manager::_ScrollForward(const_iterator_t &pos, const int count) const
{
	assert(count >= 0);

	// 現在位置を更新
	for(int i = 0; i < count; i++)
	{
		const_iterator_t p = pos;
		if(++p == get_iterator_end())
		{
			return i;
		}
		pos++;
	}

	// 実際にスクロールした行数を返す
	return count;
}

// 先頭方向にスクロール
int edit_manager::_ScrollBackward(const_iterator_t &pos, const int count) const
{
	assert(count >= 0);

	// 現在位置を更新
	for(int i = 0; i < count; i++)
	{
		if(get_iterator_begin() == pos)
		{
			return i;
		}
		pos--;
	}

	// 実際にスクロールした行数を返す
	return count;
}


// 絶対位置を取得（linenumber==0 なら末尾、linenumber==1 なら先頭）
int edit_manager::_GetPosition(const_iterator_t &pos, const linenumber_t linenumber) const
{
	const unsigned int line_count = get_line_count();
	const line_info_t &base = m_top;        // 基準位置＝画面の上端

	// 先頭
	if(linenumber == 1)
	{
		pos = get_iterator_begin();
		return 1;
	}
	// 末尾
	if(linenumber == 0 || linenumber >= line_count)
	{
		pos = get_iterator_end();
		pos--;
		return line_count;
	}

	// 基準位置より先頭方向
	if(linenumber < base.linenumber)
	{
		const linenumber_t diff = base.linenumber - linenumber;

		// 先頭に近い
		if(linenumber < diff)
		{
			// 先頭から検索
			pos = get_iterator_begin();
			_ScrollForward(pos, linenumber - 1);
			return linenumber;
		}
		// 基準位置に近い
		else
		{
			// 基準位置から検索
			pos = base.pos;
			_ScrollBackward(pos, diff);
			return linenumber;
		}
	}
	// 基準位置より末尾方向
	if(linenumber > base.linenumber)
	{
		const linenumber_t diff = linenumber - base.linenumber;

		// 末尾に近い
		if(line_count - linenumber < diff)
		{
			// 末尾から検索
			pos = get_iterator_end();
			pos--;
			_ScrollBackward(pos, line_count - linenumber);
			return linenumber;
		}
		// 基準位置に近い
		else
		{
			// 基準位置から検索
			pos = base.pos;
			_ScrollForward(pos, diff);
			return linenumber;
		}
	}

	// 基準位置
	pos = base.pos;
	return linenumber;
}

void edit_manager::_CreateStringList(const wchar_t *wstr, stringlist_t &stringlist)
{
	for(;;)
	{
		// 改行位置を検索
		charcode_linefeed_t type;
		const int pos = charcode_search_linefeed(wstr, type);
		if(pos == -1)
		{
			// 最終行
			stringlist.push_back(wstr);
			break;
		}

		// 文字列を切り出す
		stringlist.push_back(wstring_t(wstr, pos));

		// [block]
		// 次の位置を計算
		{
			const int skip[] = {0, 1, 1, 2};
			wstr += pos + skip[type];
		}
	}
}

_SGC_END                                // }
