// thread_base.h … スレッドの基本クラス
#ifndef __THREAD_BASE__
#define __THREAD_BASE__

#include "sgc.h"
#include <stddef.h>                     // NULL


_SGC_BEGIN

// スレッドクラス
class thread_base
{
public:
	// スレッドの初期状態
	typedef enum _beginmode_t
	{
		BEGINMODE_NORMAL,                       // 通常
		BEGINMODE_SUSPEND,                      // サスペンド
	} beginmode_t;

	// コンストラクタ/デストラクタ
	thread_base(void *param = NULL, const beginmode_t beginmode = BEGINMODE_NORMAL);
	virtual ~thread_base(void);

	// 中断/再開
	int suspend(void);
	int resume (void);

	// 待機
	enum { WAIT_INFINITE = 0xffffffff };
	void wait(const unsigned long milliseconds = WAIT_INFINITE);

protected:
	// スレッドのメイン関数
	virtual unsigned _ThreadMain(void *param) = 0;

	// 終了
	void _Exit(const unsigned endcode = 0);

private:
	// スレッド情報
	unsigned long m_dwThread;               // スレッドハンドル
	unsigned      m_uThreadID;              // スレッドID

	typedef struct _this_param_t
	{
		thread_base *this_ptr;
		void   *param;
	} this_param_t;

	static unsigned __stdcall _ThreadProc(void *this_param_ptr);
};

_SGC_END

#endif // __THREAD_BASE__
