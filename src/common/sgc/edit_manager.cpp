// edit_manager.cpp �c�C���v�������e�[�V�����t�@�C��
#include "edit_manager.h"
#include "charcode.h"                   // charcode_search_linefeed()
#include <assert.h>                     // assert()


_SGC_BEGIN                              // namespace sgc {


////////////////////////////////////////////////////////////
// �R���X�g���N�V����

// �R���X�g���N�^
edit_manager::edit_manager(void)
{
	set_empty();
}

// �f�X�g���N�^
edit_manager::~edit_manager(void)
{
}


////////////////////////////////////////////////////////////
// ���

// �s�T�C�Y���擾
unsigned int edit_manager::get_line_count(void) const
{
	return m_list.size();
}

// ��ʏ�[�̍s�ԍ����擾
unsigned int edit_manager::get_linenumber_top(void) const
{
	return m_top.linenumber;
}

// �ҏW���̍s�ԍ����擾
unsigned int edit_manager::get_linenumber_now(void) const
{
	return m_now.linenumber;
}


// ���ݍs���擪��
bool edit_manager::is_begin(void) const
{
	return m_list.begin() == m_now.pos;
}

// ���ݍs��������
bool edit_manager::is_end(void) const
{
	const_iterator_t p = m_now.pos;
	return m_list.end() == ++p;
}


// �擪�ʒu�̃C�e���[�^���擾
edit_manager::const_iterator_t edit_manager::get_iterator_begin(void) const
{
	return m_list.begin();
}

// �����ʒu�̃C�e���[�^���擾
edit_manager::const_iterator_t edit_manager::get_iterator_end(void) const
{
	return m_list.end();
}

// ��ʏ�[�̃C�e���[�^���擾
edit_manager::const_iterator_t edit_manager::get_iterator_top(void) const
{
	return m_top.pos;
}

// ���݈ʒu�̃C�e���[�^���擾
edit_manager::const_iterator_t edit_manager::get_iterator_now(void) const
{
	return m_now.pos;
}


////////////////////////////////////////////////////////////
// ����

// ���݂̃o�b�t�@��j��
void edit_manager::set_empty(void)
{
	m_list.clear();
	m_list.push_back(wstring_t());

	m_top.pos        = m_list.begin();
	m_top.linenumber = 1;

	m_now = m_top;
}

// ���݂̃o�b�t�@��j�����ĕ������ݒ�i���s����j
int edit_manager::set_data(const wchar_t *wstr)
{
	// [block]
	// �������ݒ�
	{
		stringlist_t buffer;
		_CreateStringList(wstr, buffer);

		m_list.clear();
		m_list.splice(m_list.end(), buffer);
	}

	// [block]
	// ���݈ʒu���o�b�t�@�̐擪�ɂ���
	{
		m_top.pos        = m_list.begin();
		m_top.linenumber = 1;
		m_now = m_top;
	}

	// �s����Ԃ�
	return m_list.size();
}


// ���݈ʒu�ɕ������}���i���s����j
int edit_manager::insert_data(int &x, const wchar_t *wstr)
{
	// ��������e���|�����o�b�t�@�Ɋi�[
	stringlist_t buffer;
	_CreateStringList(wstr, buffer);

	iterator_t p = m_now.pos;

	// ���݈ʒu�ȍ~�̕�������擾
	const wstring_t after = p->substr(x);
	p->erase(x);

	// �o�b�t�@����1�s�ڂ� p �̌�ɒǉ�
	*p += *buffer.begin();
	buffer.pop_front();

	const int lines = buffer.size();

	// �o�b�t�@�̃f�[�^��ǉ�
	p++;
	m_list.splice(p, buffer);
	assert(buffer.size() == 0);

	// �ŏI�s�̌��� after ��ǉ�
	p--;
	x = p->length();
	*p += after;

	m_now.pos         = p;
	m_now.linenumber += lines;

	// �ǉ������s����Ԃ�
	return lines;
}

// ���݈ʒu�ɕ������}���i���s�Ȃ��j
void edit_manager::insert_string(const unsigned int x, const wstring_t &wstr)
{
	assert(x <= m_now.pos->length());

	// ���͕�����}��
	m_now.pos->insert(x, wstr);
}

// ���݈ʒu�ɉ��s��}��
void edit_manager::insert_linefeed(const unsigned int x)
{
	assert(x <= m_now.pos->length());

	// ���ݍs�����݈ʒu�ŕ���
	const wstring_t string_front = m_now.pos->substr(0, x);
	*m_now.pos                   = m_now.pos->substr(x);

	// ���������O����}��
	m_list.insert(m_now.pos, string_front);
	m_now.linenumber++;

	// ���ݍs����ʂ̏�[�̏ꍇ
	if(m_now.pos == m_top.pos)
	{
		m_top.pos--;
	}

	// ��ʏ�[�����݈ʒu������̏ꍇ�A�s�ԍ��𒲐�
	if(m_top.linenumber > m_now.linenumber)
	{
		m_top.linenumber++;
	}
}


// ���ݍs�Ǝ��̍s��A��
void edit_manager::combine_forward(void)
{
	if(is_end())
	{
		return;
	}

	iterator_t p = m_now.pos;
	p++;

	// ���ݍs(m_pos_now) ��1���̍s(p) ��A��
	*m_now.pos += *p;

	// p ����ʏ�[�̏ꍇ
	if(m_top.pos == p)
	{
		m_top.pos = m_now.pos;
	}

	// ���ݍs(p) ���폜
	m_list.erase(p);

	// ��ʏ�[�����݈ʒu������̏ꍇ�A�s�ԍ��𒲐�
	if(m_top.linenumber > m_now.linenumber)
	{
		m_top.linenumber--;
	}
}

// ���ݍs�ƑO�̍s��A��
void edit_manager::combine_backword(void)
{
	if(is_begin())
	{
		return;
	}

	iterator_t p = m_now.pos;
	m_now.pos--;
	m_now.linenumber--;

	// ���ݍs(p) ��1��̍s(m_pos_now) ��A��
	*m_now.pos += *p;

	// p ����ʏ�[�̏ꍇ
	if(m_top.pos == p)
	{
		m_top.pos = m_now.pos;
	}

	// ���ݍs(p) ���폜
	m_list.erase(p);

	// ��ʏ�[�����݈ʒu������̏ꍇ�A�s�ԍ��𒲐�
	if(m_top.linenumber > m_now.linenumber)
	{
		m_top.linenumber--;
	}
}


// ���ݍs�� pos �����ڂ��� count �������폜
void edit_manager::delete_string(const unsigned int pos, const unsigned int count)
{
	m_now.pos->erase(pos, count);
}


// �擪�ʒu����X�N���[���i���Ȃ疖�������A���Ȃ�擪�����j
int edit_manager::scroll_top(const int count)
{
	const int lines = _Scroll(m_top.pos, count);
	m_top.linenumber += lines;

	return lines;
}

// ���݈ʒu����X�N���[���i���Ȃ疖�������A���Ȃ�擪�����j
int edit_manager::scroll_now(const int count)
{
	const int lines = _Scroll(m_now.pos, count);
	m_now.linenumber += lines;

	return lines;
}

// ��Έʒu
void edit_manager::set_position(const linenumber_t linenumber, const bool top, const bool now)
{
	line_info_t info;
	info.linenumber = _GetPosition(info.pos, linenumber);

	if(top) { m_top = info; }
	if(now) { m_now = info; }
}


////////////////////////////////////////////////////////////
// protected�֐�

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

// ���������ɃX�N���[��
int edit_manager::_ScrollForward(const_iterator_t &pos, const int count) const
{
	assert(count >= 0);

	// ���݈ʒu���X�V
	for(int i = 0; i < count; i++)
	{
		const_iterator_t p = pos;
		if(++p == get_iterator_end())
		{
			return i;
		}
		pos++;
	}

	// ���ۂɃX�N���[�������s����Ԃ�
	return count;
}

// �擪�����ɃX�N���[��
int edit_manager::_ScrollBackward(const_iterator_t &pos, const int count) const
{
	assert(count >= 0);

	// ���݈ʒu���X�V
	for(int i = 0; i < count; i++)
	{
		if(get_iterator_begin() == pos)
		{
			return i;
		}
		pos--;
	}

	// ���ۂɃX�N���[�������s����Ԃ�
	return count;
}


// ��Έʒu���擾�ilinenumber==0 �Ȃ疖���Alinenumber==1 �Ȃ�擪�j
int edit_manager::_GetPosition(const_iterator_t &pos, const linenumber_t linenumber) const
{
	const unsigned int line_count = get_line_count();
	const line_info_t &base = m_top;        // ��ʒu����ʂ̏�[

	// �擪
	if(linenumber == 1)
	{
		pos = get_iterator_begin();
		return 1;
	}
	// ����
	if(linenumber == 0 || linenumber >= line_count)
	{
		pos = get_iterator_end();
		pos--;
		return line_count;
	}

	// ��ʒu���擪����
	if(linenumber < base.linenumber)
	{
		const linenumber_t diff = base.linenumber - linenumber;

		// �擪�ɋ߂�
		if(linenumber < diff)
		{
			// �擪���猟��
			pos = get_iterator_begin();
			_ScrollForward(pos, linenumber - 1);
			return linenumber;
		}
		// ��ʒu�ɋ߂�
		else
		{
			// ��ʒu���猟��
			pos = base.pos;
			_ScrollBackward(pos, diff);
			return linenumber;
		}
	}
	// ��ʒu��薖������
	if(linenumber > base.linenumber)
	{
		const linenumber_t diff = linenumber - base.linenumber;

		// �����ɋ߂�
		if(line_count - linenumber < diff)
		{
			// �������猟��
			pos = get_iterator_end();
			pos--;
			_ScrollBackward(pos, line_count - linenumber);
			return linenumber;
		}
		// ��ʒu�ɋ߂�
		else
		{
			// ��ʒu���猟��
			pos = base.pos;
			_ScrollForward(pos, diff);
			return linenumber;
		}
	}

	// ��ʒu
	pos = base.pos;
	return linenumber;
}

void edit_manager::_CreateStringList(const wchar_t *wstr, stringlist_t &stringlist)
{
	for(;;)
	{
		// ���s�ʒu������
		charcode_linefeed_t type;
		const int pos = charcode_search_linefeed(wstr, type);
		if(pos == -1)
		{
			// �ŏI�s
			stringlist.push_back(wstr);
			break;
		}

		// �������؂�o��
		stringlist.push_back(wstring_t(wstr, pos));

		// [block]
		// ���̈ʒu���v�Z
		{
			const int skip[] = {0, 1, 1, 2};
			wstr += pos + skip[type];
		}
	}
}

_SGC_END                                // }
