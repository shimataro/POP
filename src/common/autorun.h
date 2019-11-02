// autorun.h …main関数の前後で実行させる関数の登録
#ifndef __AUTORUN__
#define __AUTORUN__

#include <stddef.h>                     // NULL


// 自動実行クラス
class autorun
{
public:
	typedef void (*AUTORUN_FUNC)(void);

	// 開始時に実行
	autorun(AUTORUN_FUNC start_func, AUTORUN_FUNC end_func = NULL)
	{
		if(start_func != NULL) start_func();
		m_end_func = end_func;                  // 終了関数を登録
	}

	// 終了時に実行
	~autorun(void)
	{
		if(m_end_func != NULL) m_end_func();
	}

private:
	AUTORUN_FUNC m_end_func;
};

#endif // __AUTORUN__
