// ToolTipCtrl.h �c�c�[���`�b�v�R���g���[���̃T�u�N���X��
#ifndef __WGC_TOOLTIPCTRL__
#define __WGC_TOOLTIPCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class tooltip_ctrl : public control
{
public:
	// �R���X�g���N�^�^�f�X�g���N�^
	explicit tooltip_ctrl(void);
	virtual ~tooltip_ctrl(void);

	// �쐬
	BOOL Create(window *parent, const DWORD dwStyle = 0, HINSTANCE hInstance = NULL);

	////////////////////////////////////////
	// ����

	// �c�[���`�b�v��ǉ�
	BOOL AddTool(const TOOLINFO &ti);
	BOOL AddTool(HWND hWnd, HINSTANCE hResource, const UINT nIDText);
	BOOL AddTool(HWND hWnd, HINSTANCE hResource, LPCTSTR lpszText);

	// �ő啝�iInternet Explorer 3.0�ȍ~�j
	int GetMaxTipWidth(void) const;
	int SetMaxTipWidth(const int iWidth);

private:
	HINSTANCE m_hInstance;
};

_WGC_END                                // }

#endif // __WGC_TOOLTIPCTRL__
