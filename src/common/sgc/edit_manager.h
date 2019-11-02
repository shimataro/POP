// edit_manager.h …エディタコンポーネントのエンジン部
#ifndef __WXC_EDIT_MANAGER__
#define __WXC_EDIT_MANAGER__

#include "sgc.h"
#include <list>                         // std::list<>


_SGC_BEGIN                              // namespace sgc {

class edit_manager
{
public:
	////////////////////////////////////////////////////////////////
	// 型の定義
	typedef std::wstring         wstring_t;
	typedef std::list<wstring_t> stringlist_t;
	typedef stringlist_t::iterator       iterator_t;
	typedef stringlist_t::const_iterator const_iterator_t;
	typedef stringlist_t::size_type      linenumber_t;

	// 行情報
	typedef struct _line_info_t
	{
		iterator_t   pos;                   // 現在行のイテレータ
		linenumber_t linenumber;            // 現在行番号（先頭＝１）
	} line_info_t;

	////////////////////////////////////////////////////////////////
	// コンストラクション
	edit_manager(void);
	~edit_manager(void);


	////////////////////////////////////////////////////////////////
	// 情報
	unsigned int get_line_count(void) const;
	unsigned int get_linenumber_top(void) const;
	unsigned int get_linenumber_now(void) const;

	bool is_begin(void) const;
	bool is_end  (void) const;

	const_iterator_t get_iterator_begin(void) const;
	const_iterator_t get_iterator_end  (void) const;
	const_iterator_t get_iterator_top  (void) const;
	const_iterator_t get_iterator_now  (void) const;


	////////////////////////////////////////////////////////////////
	// 操作

	void set_empty(void);
	int  set_data(const wchar_t *wstr);

	// 挿入
	int  insert_data(int &x, const wchar_t *wstr);
	void insert_string(const unsigned int x, const wstring_t &wstr);
	void insert_linefeed(const unsigned int x);

	// 連結
	void combine_forward (void);
	void combine_backword(void);

	// 削除
	void delete_string(const unsigned int pos, const unsigned int count);

	// スクロール
	int scroll_top(const int count);
	int scroll_now(const int count);

	// 絶対位置
	void set_position(const linenumber_t linenumber, const bool top, const bool now);

protected:
	// 論理情報
	stringlist_t m_list;                    // テキスト情報のリストデータ
	line_info_t  m_top;                     // 画面の上端に表示されている行
	line_info_t  m_now;                     // 現在編集中の行

	int _Scroll(const_iterator_t &pos, const int count) const;
	int _ScrollForward (const_iterator_t &pos, const int count) const;
	int _ScrollBackward(const_iterator_t &pos, const int count) const;

	int _GetPosition(const_iterator_t &pos, const linenumber_t linenumber) const;

	static void _CreateStringList(const wchar_t *wstr, stringlist_t &stringlist);
};

_SGC_END                                // }

#endif // __WXC_EDIT_MANAGER__
