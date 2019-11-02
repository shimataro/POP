// imm.cpp �c�C���v�������e�[�V�����t�@�C��
#include "imm.h"


// ���C�u�����̃����N
#pragma comment(lib, "imm32.lib")       // IMM����p���C�u����


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// �\�z

// �R���X�g���N�^
imm::imm(HWND hWnd)
{
	m_hWnd = hWnd;
	m_hIMC = ::ImmGetContext(m_hWnd);
}

// �f�X�g���N�^
imm::~imm(void)
{
	::ImmReleaseContext(m_hWnd, m_hIMC);
	m_hIMC = NULL;
}


////////////////////////////////////////////////////////////////////////////////
// ���

BOOL imm::GetOpenStatus(void) const
{
	return ::ImmGetOpenStatus(m_hIMC);
}

BOOL imm::SetOpenStatus(const BOOL bOpen /* = TRUE */)
{
	return ::ImmSetOpenStatus(m_hIMC, bOpen);
}


////////////////////////////////////////////////////////////////////////////////
// ����

// IME�̃J���b�g���W��ݒ�
BOOL imm::SetPos(const POINT &point)
{
	COMPOSITIONFORM cf;
	cf.dwStyle = CFS_POINT;
	cf.ptCurrentPos = point;

	return ::ImmSetCompositionWindow(m_hIMC, &cf);
}


// �t�H���g����
BOOL imm::GetCompositionFont(LPLOGFONT lplf)
{
	return ::ImmGetCompositionFont(m_hIMC, lplf);
}

BOOL imm::SetCompositionFont(LPLOGFONT lplf)
{
	return ::ImmSetCompositionFont(m_hIMC, lplf);
}

BOOL imm::SetCompositionFont(HFONT hFont)
{
	LOGFONT lf;
	::GetObject(hFont, sizeof(LOGFONT), &lf);
	return SetCompositionFont(&lf);
}


// �ϊ�
LONG imm::GetCompositionString(const DWORD dwIndex, LPVOID lpBuf, const DWORD dwBufLen)
{
	return ::ImmGetCompositionString(m_hIMC, dwIndex, lpBuf, dwBufLen);
}

LONG imm::GetCompositionStringResultStr(tstring_t &tstr)
{
	// �o�b�t�@�T�C�Y���擾
	const int size = GetCompositionString(GCS_RESULTSTR, NULL, 0);
	if(size < 0)
	{
		return size;
	}

	// �o�b�t�@���������m��
	const size_t length = size / sizeof(TCHAR);
	TCHAR *buf = new TCHAR[length + 1];

	// �ϊ���������擾
	const LONG lResult = ::ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, buf, size);
	buf[length] = _T('\0');
	tstr = buf;

	// �㏈��
	delete []buf;
	return lResult;
}

// UNICODE������ŕϊ���̒l���擾
LONG imm::GetCompositionStringResultStrW(wstring_t &wstr)
{
	// �o�b�t�@�T�C�Y���擾
	const int size = ::ImmGetCompositionStringW(m_hIMC, GCS_RESULTSTR, NULL, 0);
	if(size < 0)
	{
		return size;
	}

	// �o�b�t�@���������m��
	const size_t length = size / sizeof(wchar_t);
	wchar_t *buf = new wchar_t[length + 1];

	// �ϊ���������擾
	const LONG lResult = ::ImmGetCompositionStringW(m_hIMC, GCS_RESULTSTR, buf, size);
	buf[length] = L'\0';
	wstr = buf;

	// �㏈��
	delete []buf;
	return lResult;
}

_WGC_END                                // }
