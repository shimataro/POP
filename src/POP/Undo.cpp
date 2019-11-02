// Undo.cpp �c�C���v�������e�[�V�����t�@�C��
#include "stdafx.h"
#include "Undo.h"


////////////////////////////////////////////////////////////////////////////////
// �R���X�g���N�^�i�ϐ��̏������j
CUndo::CUndo(void)
{
	m_now   = -1;                           // ���݈ʒu��-1
	m_tail  = -1;                           // �Ō���ʒu��-1
	m_saved = 0;                            // �ۑ��ʒu���f�L���[�̐擪
}

// �f�X�g���N�^�i�������̊J���j
CUndo::~CUndo(void)
{
	if(m_tail != -1)
	{
		ClearFrontMemory();
		ClearBackMemory();
		image_free(&m_buffer[m_now].data);
	}
}

// �A���h�D
void CUndo::Undo(UNDOINFO &info)
{
	if(IsAbleToUndo())
	{
		m_now--;
		info = m_buffer[m_now];
	}
}

// ���h�D
void CUndo::Redo(UNDOINFO &info)
{
	if(IsAbleToRedo())
	{
		m_now++;
		info = m_buffer[m_now];
	}
}

// ���݈ʒu�ɃA���h�D����������
bool CUndo::AddUndoInfo(const UNDOINFO &info)
{
	// ���݈ʒu����̃A���h�D���������N���A
	ClearBackMemory();
	ASSERT(m_now == m_tail);

	try
	{
		// �o�b�t�@�̍Ō�ɃA���h�D����ǉ�
		m_buffer.push_back(info);
		m_now++;
		m_tail++;
		return true;
	}
	// �ǉ��Ɏ��s���邱�Ƃ͂܂��Ȃ����ǁc
	catch(CMemoryException *e)
	{
		e->Delete();
		return false;
	}
}

// ���݈ʒu���O�̃A���h�D���������N���A
void CUndo::ClearFrontMemory(void)
{
	while(m_now > 0)
	{
		if(!binary_equal(m_buffer[0].data, m_buffer[1].data))
		{
			image_free(&m_buffer[0].data);
		}
		m_buffer.pop_front();
		m_now--;
		m_tail--;
		m_saved--;
	}
}

// ���݈ʒu����̃A���h�D���������N���A
void CUndo::ClearBackMemory(void)
{
	while(m_tail > m_now)
	{
		if(!binary_equal(m_buffer[m_tail].data, m_buffer[m_tail - 1].data))
		{
			image_free(&m_buffer[m_tail].data);
		}
		m_buffer.pop_back();
		m_tail--;
	}

	if(m_saved > m_now)
	{
		m_saved = -1;
	}
}

// ���݈ʒu�ȊO�̃��������N���A
void CUndo::ClearMemory(void)
{
	ClearFrontMemory();
	ClearBackMemory();
	if(m_saved != m_now)
	{
		m_saved = -1;
	}
}
