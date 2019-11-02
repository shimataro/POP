/* callback.h �c�R�[���o�b�N�֐��̐錾 */
#ifndef __CALLBACK__
#define __CALLBACK__

#include "resource.h"


/* �֐��̃v���g�^�C�v�錾 */
long         callback_init   (const int id);
int CALLBACK callback_default(const int num, const int denom, const long data);
int CALLBACK callback_none   (const int num, const int denom, const long data);
void         callback_term   (long data);


/******************************************************************************/
/* C++�ł́A�R�[���o�b�N�̏������E�I���������I�ɍs���N���X��� */
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
