// Alloc.cpp�coperator new/delete �̃I�[�o�[���[�h
#include <windows.h>                    // ::GlobalAlloc()
#include <new>                          // std::bad_alloc


////////////////////////////////////////////////////////////////////////////////
// operator new/delete ��Ǝ��̎����ɃI�[�o�[���[�h����ꍇ�A
// ���̃t�@�C�����v���W�F�N�g�ɒǉ�����i�ǉ����邾����OK�j
// �I�[�o�[���[�h����Ɓ��݂����Ȃ������Ƃ�����B
//  1. ������
//    �iCRT��operator new/delete�ł́A�������m�ۂ̂ق��ɂ������Ȃ��Ƃ�����Ă�B�f�o�b�O�p�H�j
//  2. �������m�ۂɎ��s�����ꍇ�Athrow std::bad_alloc �̕ۏ�
//    �iVisual C++ �΍�j
//  3. �T�C�Y0�̃��������m�ۂ��悤�Ƃ����ꍇ�̓���̕ۏ�
//    �i�T�C�Y0�̃������̈�ւ̃|�C���^��Ԃ��j
// �������A�����������Ƃ����Ă�new����邽�тɃ��������m�ۂ��Ă���x������A
// ���̂����傫�ȃ������u���b�N��p�ӂ��āAnew����邲�ƂɕK�v�ȕ������؂�o�����肷�邩���B
// �ł��������ʓ|�����獡�̂Ƃ���\��͂Ȃ�


// �P��I�u�W�F�N�g��new
void *operator new(size_t cb)
{
	void *ptr = ::GlobalAlloc(GMEM_FIXED, cb);
	if(ptr == NULL)
	{
		// �m�ۂɎ��s������ std::bad_alloc ���X���[
		throw std::bad_alloc();
	}
	return ptr;
}

// �z���new
void *operator new[](size_t cb)
{
	// operator new �Ɠ���
	return operator new(cb);
}

// �P��I�u�W�F�N�g��delete
void operator delete(void *ptr)
{
	::GlobalFree(ptr);
}

// �z���delete
void operator delete[](void *ptr)
{
	// operator delete �Ɠ���
	operator delete(ptr);
}
