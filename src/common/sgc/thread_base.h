// thread_base.h �c �X���b�h�̊�{�N���X
#ifndef __THREAD_BASE__
#define __THREAD_BASE__

#include "sgc.h"
#include <stddef.h>                     // NULL


_SGC_BEGIN

// �X���b�h�N���X
class thread_base
{
public:
	// �X���b�h�̏������
	typedef enum _beginmode_t
	{
		BEGINMODE_NORMAL,                       // �ʏ�
		BEGINMODE_SUSPEND,                      // �T�X�y���h
	} beginmode_t;

	// �R���X�g���N�^/�f�X�g���N�^
	thread_base(void *param = NULL, const beginmode_t beginmode = BEGINMODE_NORMAL);
	virtual ~thread_base(void);

	// ���f/�ĊJ
	int suspend(void);
	int resume (void);

	// �ҋ@
	enum { WAIT_INFINITE = 0xffffffff };
	void wait(const unsigned long milliseconds = WAIT_INFINITE);

protected:
	// �X���b�h�̃��C���֐�
	virtual unsigned _ThreadMain(void *param) = 0;

	// �I��
	void _Exit(const unsigned endcode = 0);

private:
	// �X���b�h���
	unsigned long m_dwThread;               // �X���b�h�n���h��
	unsigned      m_uThreadID;              // �X���b�hID

	typedef struct _this_param_t
	{
		thread_base *this_ptr;
		void   *param;
	} this_param_t;

	static unsigned __stdcall _ThreadProc(void *this_param_ptr);
};

_SGC_END

#endif // __THREAD_BASE__
