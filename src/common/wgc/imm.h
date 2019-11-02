// imm.h �cIMM�̐���
#ifndef __WGC_IMM__
#define __WGC_IMM__

#include "wgc.h"
#include <imm.h>                        // HIMC


_WGC_BEGIN                              // namespace wgc {

class imm
{
public:
	// �R���X�g���N�^�^�f�X�g���N�^
	explicit imm(HWND hWnd);
	virtual ~imm(void);

	// ���
	BOOL GetOpenStatus(void) const;
	BOOL SetOpenStatus(const BOOL bOpen = TRUE);

	// ����
	BOOL SetPos(const POINT &point);

	BOOL GetCompositionFont(LPLOGFONT lplf);
	BOOL SetCompositionFont(LPLOGFONT lplf);
	BOOL SetCompositionFont(HFONT hFont);

	// �ϊ�
	LONG GetCompositionString(const DWORD dwIndex, LPVOID lpBuf, const DWORD dwBufLen);
	LONG GetCompositionStringResultStr (tstring_t &tstr);
	LONG GetCompositionStringResultStrW(wstring_t &wstr);

private:
	HWND m_hWnd;
	HIMC m_hIMC;
};

_WGC_END                                // }

#endif // __WGC_IMM__
