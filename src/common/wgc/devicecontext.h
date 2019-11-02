// devicecontext.h �c�f�o�C�X�R���e�L�X�g�N���X
#ifndef __WGC_DEVICECONTEXT__
#define __WGC_DEVICECONTEXT__

#include "wgc.h"
#include <windows.h>


_WGC_BEGIN                              // namespace wgc {

class devicecontext
{
public:
	devicecontext(HDC hDC);
	devicecontext(LPCTSTR lpszDriver, LPCTSTR lpszDevice, CONST DEVMODE *lpInitData);
	virtual ~devicecontext(void);

	operator HDC(void)
	{return m_hDC;}


	// �I�y���[�V����
	HGDIOBJ SelectObject(HGDIOBJ hNewObject);

	COLORREF GetTextColor(void) const;
	COLORREF GetBkColor  (void) const;
	COLORREF SetTextColor(const COLORREF color);
	COLORREF SetBkColor  (const COLORREF color);

	int GetBkMode(void);
	int SetBkMode(const int iBkMode);

	// �N���b�s���O�̈�
	int ClearClip(void);
	int IntersectClipRect(const RECT &rRect);
	int SetClipRect(const RECT &rRect);

	// �h��Ԃ�
	BOOL FillRect(const RECT &rect, HBRUSH hBrush);
	BOOL FillSolidRect(const RECT &rect, const COLORREF color);

	// �`��
	BOOL DrawFocusRect(const RECT &rect);
	BOOL DrawEdge(const RECT &rect, const UINT edge = EDGE_ETCHED, const UINT grfFlags = BF_RECT);

	BOOL MoveToEx(const int x, const int y, LPPOINT lpPoint);
	BOOL MoveTo(const int x, const int y);
	BOOL LineTo(const int x, const int y);

	// �e�L�X�g�`��
	BOOL TextOut (const int x, const int y, const tstring_t &tstr, const int length = -1);
	BOOL TextOutW(const int x, const int y, const wstring_t &wstr, const int length = -1);
	BOOL ExtTextOut (const int x, const int y, const UINT fuOptions, const RECT &rect, const tstring_t &tstr, const UINT length = -1, const int *lpDx = NULL);
	BOOL ExtTextOutW(const int x, const int y, const UINT fuOptions, const RECT &rect, const wstring_t &wstr, const UINT length = -1, const int *lpDx = NULL);

	BOOL GetTextExtent(const tstring_t &tstr, SIZE &size) const;

protected:
	HDC m_hDC;

	BOOL Delete(void);

private:
	BOOL m_bOwner;
};


////////////////////////////////////////////////////////////
// �h��DC

// �N���C�A���gDC
class devicecontext_client : public devicecontext
{
public:
	explicit devicecontext_client(HWND hWnd);
	~devicecontext_client(void);

private:
	HWND m_hWnd;
};

// �E�C���h�EDC
class devicecontext_window : public devicecontext
{
public:
	explicit devicecontext_window(HWND hWnd);
	~devicecontext_window(void);

private:
	HWND m_hWnd;
};

// �y�C���gDC
class devicecontext_paint : public devicecontext
{
public:
	explicit devicecontext_paint(HWND hWnd);
	~devicecontext_paint(void);

private:
	HWND m_hWnd;
	PAINTSTRUCT m_ps;
};

// ������DC
class devicecontext_memory : public devicecontext
{
public:
	explicit devicecontext_memory(HDC hDC);
	~devicecontext_memory(void);
};

_WGC_END                                // }

#endif // __WGC_DEVICECONTEXT__
