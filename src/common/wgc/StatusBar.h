// StatusBar.h �c�X���C�_�R���g���[���̃T�u�N���X��
#ifndef __WGC_STATUSBAR__
#define __WGC_STATUSBAR__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class statusbar : public control
{
public:
	////////////////////////////////////////////////////////////////
	// �R���X�g���N�^�^�f�X�g���N�^
	explicit statusbar(void);
	virtual ~statusbar(void);

	BOOL Create(
		HWND hWndParent,
		LPCTSTR lpszWindowName,
		const DWORD dwStyle,
		const UINT nID);

	// ����
	BOOL SetText(const int iPart, const int uType, LPCTSTR lpszText);
};

_WGC_END                                // }

#endif // __WGC_STATUSBAR__
