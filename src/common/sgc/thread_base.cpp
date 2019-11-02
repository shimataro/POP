// thread_base.cpp
#include "thread_base.h"
#include <windows.h>                    // ::xxxThread(), ::WaitForSingleObject(), ::CloseHandle()
#include <process.h>                    // _beginthreadex(), _endthreadex()
#include <assert.h>                     // assert()


_SGC_BEGIN                              // namespace sgc {

////////////////////////////////////////////////////////////
// �R���X�g���N�V����

// �R���X�g���N�^
thread_base::thread_base(
	void *param /* = NULL */,
	const thread_base::beginmode_t beginmode /* = BEGINMODE_NORMAL */)
{
	this_param_t *this_param_ptr = new this_param_t;
	this_param_ptr->this_ptr = this;
	this_param_ptr->param    = param;

	// �X���b�h�̏�����Ԃ�����
	DWORD dwCreationFlags = 0;
	switch(beginmode)
	{
	case BEGINMODE_SUSPEND:
		dwCreationFlags = CREATE_SUSPENDED;
		break;
	}

	// �X���b�h���쐬
	m_dwThread = _beginthreadex(
		NULL, 0,
		_ThreadProc, this_param_ptr,
		dwCreationFlags,
		&m_uThreadID);

	// �X���b�h�̍쐬�Ɏ��s�����ꍇ
	if(m_dwThread == 0)
	{
		// ���������ꍇ�́Athread_base::_ThreadProc() �̒��� delete ����
		delete this_param_ptr;
	}
}

// �f�X�g���N�^
thread_base::~thread_base(void)
{
	// �n���h�����J��
	HANDLE hThread = reinterpret_cast<HANDLE>(m_dwThread);
	::CloseHandle(hThread);
}


////////////////////////////////////////////////////////////
// �I�y���[�V����

// ���f
int thread_base::suspend(void)
{
	HANDLE hThread  = reinterpret_cast<HANDLE>(m_dwThread);
	DWORD  dwResult = ::SuspendThread(hThread);
	return static_cast<int>(dwResult);
}

// �ĊJ
int thread_base::resume(void)
{
	HANDLE hThread  = reinterpret_cast<HANDLE>(m_dwThread);
	DWORD  dwResult = ::ResumeThread(hThread);
	return static_cast<int>(dwResult);
}

// �ҋ@
void thread_base::wait(const unsigned long milliseconds /* = WAIT_INFINITE */)
{
	HANDLE hThread  = reinterpret_cast<HANDLE>(m_dwThread);
	::WaitForSingleObject(hThread, milliseconds);
}


////////////////////////////////////////////////////////////
// protected �֐�

// �I��
void thread_base::_Exit(const unsigned endcode /* = 0 */)
{
	_endthreadex(endcode);
}


////////////////////////////////////////////////////////////
// private �֐�

// �X���b�h�v���V�[�W��
unsigned __stdcall thread_base::_ThreadProc(void *this_param_ptr)
{
	assert(this_param_ptr != NULL);

	// this �|�C���^�ƃp�����[�^���擾
	this_param_t *ptr = reinterpret_cast<this_param_t *>(this_param_ptr);
	thread_base *this_ptr = ptr->this_ptr;
	void   *param    = ptr->param;
	delete ptr;

	// �X���b�h���J�n
	return this_ptr->_ThreadMain(param);
}

_SGC_END                                // }



////////////////////////////////////////////////////////////////////////////////
// �P�̃e�X�g�p�R�[�h
#ifdef __UNITTEST_THREAD__

#include <iostream>


// �h���N���X���쐬
class test_thread : public sgc::thread_base
{
public:
	test_thread(const int num, beginmode_t mode = BEGINMODE_NORMAL)
		: thread_base(reinterpret_cast<void *>(num), mode)
	{
		m_num = num;
		std::cout << "Thread(" << m_num << ") begin" << std::endl;
	}

	~test_thread(void)
	{
		std::cout << "Thread(" << m_num << ") end" << std::endl;
	}

protected:
	// �X���b�h�̃��C���֐�
	unsigned _ThreadMain(void *param)
	{
		const int thread_num = reinterpret_cast<int>(param);

		// �����J�E���g
		for(int i = 0; i < 100; i++)
		{
			std::cout << "Thread(" << thread_num << ") : " << i << std::endl;
		}

		return 0;
	}

private:
	int m_num;
};


// main�֐�
int main(void)
{
	int test_no = 1;

	// �Ƃ肠�������ʂɎ��s
	{
		std::cout << "============================================================" << std::endl;
		std::cout << "�e�X�g" << test_no++ << "�i�Ƃ肠�������ʂɎ��s�j" << std::endl;

		// �X���b�h���쐬
		test_thread newthread1(1);

		// �J�E���g
		for(int i = 0; i < 100; i++)
		{
			std::cout << "Main : " << i << std::endl;
		}

		// �I���҂�
		newthread1.wait();
	}

	// �X���b�h��2�쐬
	{
		std::cout << "============================================================" << std::endl;
		std::cout << "�e�X�g" << test_no++ << "�i�X���b�h��2�쐬�j" << std::endl;

		test_thread newthread1(1);
		test_thread newthread2(2);

		for(int i = 0; i < 100; i++)
		{
			std::cout << "Main : " << i << std::endl;
		}

		newthread1.wait();
		newthread2.wait();
	}

	// wait() ���Ȃ��Ă��I�����邩�H
	{
		std::cout << "============================================================" << std::endl;
		std::cout << "�e�X�g" << test_no++ << "�iwait() ���Ȃ��Ă��I�����邩�H�j" << std::endl;

		test_thread newthread1(1);

		for(int i = 0; i < 100; i++)
		{
			std::cout << "Main : " << i << std::endl;
		}

//		newthread1.wait();
	}

	// �X���b�h���T�X�y���h��Ԃ̂Ƃ��ł��I�����邩�H
	{
		std::cout << "============================================================" << std::endl;
		std::cout << "�e�X�g" << test_no++ << "�i�X���b�h���T�X�y���h��Ԃ̂Ƃ��ł��I�����邩�H�j" << std::endl;

		// �T�X�y���h���[�h�ŋN��
		test_thread newthread1(1, sgc::thread_base::BEGINMODE_SUSPEND);

		for(int i = 0; i < 100; i++)
		{
			std::cout << "Main : " << i << std::endl;
		}
	}

	// thread_base::suspend() / thread_base::resume() �̃e�X�g
	{
		std::cout << "============================================================" << std::endl;
		std::cout << "�e�X�g" << test_no++ << "�ithread_base::suspend() / thread_base::resume() �̃e�X�g�j" << std::endl;

		test_thread newthread1(1);

		newthread1.suspend();
		{
			// ���̊ԁA�X���b�h���̏o�͂͂Ȃ��͂�
			for(int i = 0; i < 100; i++)
			{
				std::cout << "Main : " << i << std::endl;
			}
		}
		newthread1.resume();
		newthread1.wait();
	}

	return 0;
}

#endif // __UNITTEST_THREAD__
