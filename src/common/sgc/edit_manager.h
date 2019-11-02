// edit_manager.h �c�G�f�B�^�R���|�[�l���g�̃G���W����
#ifndef __WXC_EDIT_MANAGER__
#define __WXC_EDIT_MANAGER__

#include "sgc.h"
#include <list>                         // std::list<>


_SGC_BEGIN                              // namespace sgc {

class edit_manager
{
public:
	////////////////////////////////////////////////////////////////
	// �^�̒�`
	typedef std::wstring         wstring_t;
	typedef std::list<wstring_t> stringlist_t;
	typedef stringlist_t::iterator       iterator_t;
	typedef stringlist_t::const_iterator const_iterator_t;
	typedef stringlist_t::size_type      linenumber_t;

	// �s���
	typedef struct _line_info_t
	{
		iterator_t   pos;                   // ���ݍs�̃C�e���[�^
		linenumber_t linenumber;            // ���ݍs�ԍ��i�擪���P�j
	} line_info_t;

	////////////////////////////////////////////////////////////////
	// �R���X�g���N�V����
	edit_manager(void);
	~edit_manager(void);


	////////////////////////////////////////////////////////////////
	// ���
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
	// ����

	void set_empty(void);
	int  set_data(const wchar_t *wstr);

	// �}��
	int  insert_data(int &x, const wchar_t *wstr);
	void insert_string(const unsigned int x, const wstring_t &wstr);
	void insert_linefeed(const unsigned int x);

	// �A��
	void combine_forward (void);
	void combine_backword(void);

	// �폜
	void delete_string(const unsigned int pos, const unsigned int count);

	// �X�N���[��
	int scroll_top(const int count);
	int scroll_now(const int count);

	// ��Έʒu
	void set_position(const linenumber_t linenumber, const bool top, const bool now);

protected:
	// �_�����
	stringlist_t m_list;                    // �e�L�X�g���̃��X�g�f�[�^
	line_info_t  m_top;                     // ��ʂ̏�[�ɕ\������Ă���s
	line_info_t  m_now;                     // ���ݕҏW���̍s

	int _Scroll(const_iterator_t &pos, const int count) const;
	int _ScrollForward (const_iterator_t &pos, const int count) const;
	int _ScrollBackward(const_iterator_t &pos, const int count) const;

	int _GetPosition(const_iterator_t &pos, const linenumber_t linenumber) const;

	static void _CreateStringList(const wchar_t *wstr, stringlist_t &stringlist);
};

_SGC_END                                // }

#endif // __WXC_EDIT_MANAGER__
