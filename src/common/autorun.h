// autorun.h �cmain�֐��̑O��Ŏ��s������֐��̓o�^
#ifndef __AUTORUN__
#define __AUTORUN__

#include <stddef.h>                     // NULL


// �������s�N���X
class autorun
{
public:
	typedef void (*AUTORUN_FUNC)(void);

	// �J�n���Ɏ��s
	autorun(AUTORUN_FUNC start_func, AUTORUN_FUNC end_func = NULL)
	{
		if(start_func != NULL) start_func();
		m_end_func = end_func;                  // �I���֐���o�^
	}

	// �I�����Ɏ��s
	~autorun(void)
	{
		if(m_end_func != NULL) m_end_func();
	}

private:
	AUTORUN_FUNC m_end_func;
};

#endif // __AUTORUN__
