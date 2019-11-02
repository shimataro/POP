// thread_base.cpp
#include "thread_base.h"
#include <windows.h>                    // ::xxxThread(), ::WaitForSingleObject(), ::CloseHandle()
#include <process.h>                    // _beginthreadex(), _endthreadex()
#include <assert.h>                     // assert()


_SGC_BEGIN                              // namespace sgc {

////////////////////////////////////////////////////////////
// コンストラクション

// コンストラクタ
thread_base::thread_base(
	void *param /* = NULL */,
	const thread_base::beginmode_t beginmode /* = BEGINMODE_NORMAL */)
{
	this_param_t *this_param_ptr = new this_param_t;
	this_param_ptr->this_ptr = this;
	this_param_ptr->param    = param;

	// スレッドの初期状態を決定
	DWORD dwCreationFlags = 0;
	switch(beginmode)
	{
	case BEGINMODE_SUSPEND:
		dwCreationFlags = CREATE_SUSPENDED;
		break;
	}

	// スレッドを作成
	m_dwThread = _beginthreadex(
		NULL, 0,
		_ThreadProc, this_param_ptr,
		dwCreationFlags,
		&m_uThreadID);

	// スレッドの作成に失敗した場合
	if(m_dwThread == 0)
	{
		// 成功した場合は、thread_base::_ThreadProc() の中で delete する
		delete this_param_ptr;
	}
}

// デストラクタ
thread_base::~thread_base(void)
{
	// ハンドルを開放
	HANDLE hThread = reinterpret_cast<HANDLE>(m_dwThread);
	::CloseHandle(hThread);
}


////////////////////////////////////////////////////////////
// オペレーション

// 中断
int thread_base::suspend(void)
{
	HANDLE hThread  = reinterpret_cast<HANDLE>(m_dwThread);
	DWORD  dwResult = ::SuspendThread(hThread);
	return static_cast<int>(dwResult);
}

// 再開
int thread_base::resume(void)
{
	HANDLE hThread  = reinterpret_cast<HANDLE>(m_dwThread);
	DWORD  dwResult = ::ResumeThread(hThread);
	return static_cast<int>(dwResult);
}

// 待機
void thread_base::wait(const unsigned long milliseconds /* = WAIT_INFINITE */)
{
	HANDLE hThread  = reinterpret_cast<HANDLE>(m_dwThread);
	::WaitForSingleObject(hThread, milliseconds);
}


////////////////////////////////////////////////////////////
// protected 関数

// 終了
void thread_base::_Exit(const unsigned endcode /* = 0 */)
{
	_endthreadex(endcode);
}


////////////////////////////////////////////////////////////
// private 関数

// スレッドプロシージャ
unsigned __stdcall thread_base::_ThreadProc(void *this_param_ptr)
{
	assert(this_param_ptr != NULL);

	// this ポインタとパラメータを取得
	this_param_t *ptr = reinterpret_cast<this_param_t *>(this_param_ptr);
	thread_base *this_ptr = ptr->this_ptr;
	void   *param    = ptr->param;
	delete ptr;

	// スレッドを開始
	return this_ptr->_ThreadMain(param);
}

_SGC_END                                // }



////////////////////////////////////////////////////////////////////////////////
// 単体テスト用コード
#ifdef __UNITTEST_THREAD__

#include <iostream>


// 派生クラスを作成
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
	// スレッドのメイン関数
	unsigned _ThreadMain(void *param)
	{
		const int thread_num = reinterpret_cast<int>(param);

		// 数をカウント
		for(int i = 0; i < 100; i++)
		{
			std::cout << "Thread(" << thread_num << ") : " << i << std::endl;
		}

		return 0;
	}

private:
	int m_num;
};


// main関数
int main(void)
{
	int test_no = 1;

	// とりあえず普通に実行
	{
		std::cout << "============================================================" << std::endl;
		std::cout << "テスト" << test_no++ << "（とりあえず普通に実行）" << std::endl;

		// スレッドを作成
		test_thread newthread1(1);

		// カウント
		for(int i = 0; i < 100; i++)
		{
			std::cout << "Main : " << i << std::endl;
		}

		// 終了待ち
		newthread1.wait();
	}

	// スレッドを2つ作成
	{
		std::cout << "============================================================" << std::endl;
		std::cout << "テスト" << test_no++ << "（スレッドを2つ作成）" << std::endl;

		test_thread newthread1(1);
		test_thread newthread2(2);

		for(int i = 0; i < 100; i++)
		{
			std::cout << "Main : " << i << std::endl;
		}

		newthread1.wait();
		newthread2.wait();
	}

	// wait() しなくても終了するか？
	{
		std::cout << "============================================================" << std::endl;
		std::cout << "テスト" << test_no++ << "（wait() しなくても終了するか？）" << std::endl;

		test_thread newthread1(1);

		for(int i = 0; i < 100; i++)
		{
			std::cout << "Main : " << i << std::endl;
		}

//		newthread1.wait();
	}

	// スレッドがサスペンド状態のときでも終了するか？
	{
		std::cout << "============================================================" << std::endl;
		std::cout << "テスト" << test_no++ << "（スレッドがサスペンド状態のときでも終了するか？）" << std::endl;

		// サスペンドモードで起動
		test_thread newthread1(1, sgc::thread_base::BEGINMODE_SUSPEND);

		for(int i = 0; i < 100; i++)
		{
			std::cout << "Main : " << i << std::endl;
		}
	}

	// thread_base::suspend() / thread_base::resume() のテスト
	{
		std::cout << "============================================================" << std::endl;
		std::cout << "テスト" << test_no++ << "（thread_base::suspend() / thread_base::resume() のテスト）" << std::endl;

		test_thread newthread1(1);

		newthread1.suspend();
		{
			// この間、スレッド側の出力はないはず
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
