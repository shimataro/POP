/* callback.h …コールバック関数の宣言 */
#ifndef __CALLBACK__
#define __CALLBACK__

#include "resource.h"


/* 関数のプロトタイプ宣言 */
long         callback_init   (const int id);
int CALLBACK callback_default(const int num, const int denom, const long data);
int CALLBACK callback_none   (const int num, const int denom, const long data);
void         callback_term   (long data);


/******************************************************************************/
/* C++では、コールバックの初期化・終了を自動的に行うクラスを提供 */
#ifdef __cplusplus

class CCallBack
{
public:
	explicit CCallBack(const int id = IDS_PROCESSING)
	{ m_data = callback_init(id); }

	~CCallBack(void)
	{ callback_term(m_data); }

	callback_t GetCallBackProc(void) const
	{ return callback_default; }

	long GetCallBackData(void) const
	{ return m_data; }

private:
	long m_data;
};

#endif /* __cplusplus */

#endif
