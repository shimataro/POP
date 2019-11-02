// Undo.h �c�A���h�D�^���h�D
#ifndef __UNDO__
#define __UNDO__

#include <winbase.h>                    // HGLOBAL
#include <deque>                        // std::deque


// �A���h�D���
typedef struct tagUNDOINFO
{
	IMAGE_INFO info;
	IMAGE_DATA data;
} UNDOINFO;

// �A���h�D�N���X
class CUndo
{
public:
	CUndo(void);                            // �R���X�g���N�^
	~CUndo(void);                           // �f�X�g���N�^

	void SetModifiedFlag(BOOL bModified = TRUE) // �_�[�e�B�t���O�̃Z�b�g�i�Z�[�u/���[�h���ɌĂяo���j
	{m_saved = bModified ? -1 : m_now;}
	bool IsModified(void) const             // �ύX���ꂽ���H
	{return m_now != m_saved;}                 // ���݈ʒu���ۑ��ʒu�Ȃ�ύX���ꂽ
	bool IsAbleToUndo(void) const           // �A���h�D�ł��邩�H
	{return m_now > 0;}                       // ���݈ʒu���擪�ʒu�Ȃ�A���h�D�ł���
	bool IsAbleToRedo(void) const           // ���h�D�ł��邩�H
	{return m_now < m_tail;}                  // ���݈ʒu������ʒu�Ȃ烊�h�D�ł���

	void Undo(UNDOINFO &info);              // �A���h�D
	void Redo(UNDOINFO &info);              // ���h�D

	bool AddUndoInfo(const UNDOINFO &info); // ���݈ʒu�ɃA���h�D����������

	void ClearFrontMemory(void);            // ���݈ʒu���O�̃A���h�D�o�b�t�@���N���A
	void ClearBackMemory(void);             // ���݈ʒu����̃A���h�D�o�b�t�@���N���A
	void ClearMemory(void);                 // ���݈ʒu�ȊO�̃A���h�D�o�b�t�@���N���A

private:
	std::deque<UNDOINFO> m_buffer;          // �A���h�D�o�b�t�@�i�f�L���[�j
	int m_now, m_tail, m_saved;             // ���݈ʒu�A�Ō���ʒu�A�ۑ��ʒu
};

#endif
