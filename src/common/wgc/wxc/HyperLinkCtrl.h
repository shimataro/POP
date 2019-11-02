// HyperLinkCtrl.h �c�n�C�p�[�����N�R���|�[�l���g
#ifndef __WXC_HYPERLINKCTRL__
#define __WXC_HYPERLINKCTRL__

#include "wxc.h"
#include "../StaticCtrl.h"


// ��{�F
#define RGB_BLACK     RGB(  0,   0,   0)  // ��
#define RGB_RED       RGB(255,   0,   0)  // ��
#define RGB_GREEN     RGB(  0, 255,   0)  // ��
#define RGB_BLUE      RGB(  0,   0, 255)  // ��
#define RGB_YELLOW    RGB(255, 255,   0)  // ��
#define RGB_MAGENTA   RGB(255,   0, 255)  // �}�[���^
#define RGB_CYAN      RGB(  0, 255, 255)  // �V�A��
#define RGB_WHITE     RGB(255, 255, 255)  // ��
#define RGB_ORANGE    RGB(255, 128,   0)  // �I�����W


_WXC_BEGIN                              // namespace wxc {

class hyperlink_ctrl : public wgc::static_ctrl
{
public:
	hyperlink_ctrl(
		HCURSOR hCursor,
		LPCTSTR lpszURL = NULL,
		const COLORREF colorNormal = RGB_BLUE,
		const COLORREF colorPoint  = RGB_ORANGE,
		const COLORREF colorActive = RGB_RED);

	~hyperlink_ctrl(void);

	BOOL SetURL(LPCTSTR lpszURL = NULL);
	void SetFontStyle(const BOOL bUnderline = TRUE, const BOOL bItalic = FALSE);

protected:
	BOOL OnSetCursor(HWND hWnd, const UINT nHitTest, const UINT message);
	void OnCaptureChanged(HWND hWnd);

	void OnSetFocus(HWND hWndLoseFocus);
	void OnKillFocus(HWND hWndLoseFocus);

	UINT OnNcHitTest(const POINT &point);

	void OnMouseLeave(void);
	void OnMouseMove  (const UINT nFlags, const POINT &point);
	void OnLButtonDown(const UINT nFlags, const POINT &point);
	void OnLButtonUp  (const UINT nFlags, const POINT &point);

	void OnPaint(void);

private:
	typedef enum tagLINKSTATE
	{
		LS_NORMAL,
		LS_HOVER,
		LS_CLICKING,
		LS_DRAGGING,
	} LINKSTATE;

	// �����N�F
	COLORREF m_colorNormal;                 // �ʏ�̃����N�F
	COLORREF m_colorPoint;                  // �|�C���g��
	COLORREF m_colorActive;                 // �N���b�N��

	// ���\�[�X
	HCURSOR m_hCursor;
	HFONT   m_hFont;

	// �t�H�[�J�X��`��`��H
	BOOL m_bDrawFocusRect;

	// URL�֘A
	wgc::tstring_t m_url;
	void JumpToURL(void) const;

	// �����N��Ԋ֘A
	LINKSTATE m_status;
	void SetStatus(const LINKSTATE status);

	COLORREF GetLinkColor(void) const;

	// OLE�h���b�O���h���b�v�֘A
	DWORD OleDragDrop(void) const;
	static HGLOBAL CreateGlobalData(LPCVOID pData, const int size);
};

_WXC_END                                // }

#endif // __WXC_HYPERLINKCTRL__
