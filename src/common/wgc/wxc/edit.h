// edit.h �c�G�f�B�^�R���|�[�l���g
#ifndef __WXC_EDIT__
#define __WXC_EDIT__

#include "wxc.h"
#include "../window.h"
#include "../devicecontext.h"
#include "../../sgc/edit_manager.h"

// wxc::edit �́A�G�f�B�b�g�R���g���[���E���b�`�G�f�B�b�g�R���g���[���̃��b�Z�[�W���󗝂���
#include <richedit.h>


// �X�^�C��
#define XES_LINENUMBER      0x0001      // �s�ԍ���\��
#define XES_RULER           0x0002      // ���[����\��
#define XES_CURSORUNDERLINE 0x0004      // �J�[�\���s�ɉ�����`��

// �ʒm���b�Z�[�W
#define XEN_READONLYCHANGED 0x0001      // ���������������ύX���ꂽ�iEM_SETREADONLY�j
#define XEN_MODIFYCHANGED   0x0002      // �_�[�e�B�t���O���ύX���ꂽ


_WXC_BEGIN                              // namespace wxc {

class edit : public wgc::window
{
public:
	// �C�e���[�^
	typedef sgc::edit_manager::const_iterator_t const_iterator_t;
	typedef sgc::edit_manager::wstring_t        wstring_t;

	// �e�L�X�g���
	typedef struct _text_info_t
	{
		int nTabSize;                           // �^�u��
		int nLineSpace;                         // �s�Ԋu�i���P�j

		// �F���
		COLORREF color;                         // �F
		COLORREF colorBack;                     // �w�i�F
		COLORREF colorMark;                     // ���ꕶ���̐F
		COLORREF colorLine;                     // �r���̐F
	} text_info_t;

	// �s�ԍ����
	typedef struct _linenumber_info_t
	{
		int      nCols;                         // ����
		int      nMargin;                       // �E���]��
		COLORREF color;                         // �F
		COLORREF colorBack;                     // �w�i�F
	} linenumber_info_t;

	// ���[���[���
	typedef struct _ruler_info_t
	{
		int nHeight;                            // ����
		int nStride;                            // �Ԋu
		COLORREF color;                         // �F
		COLORREF colorBack;                     // �w�i�F
	} ruler_info_t;


	////////////////////////////////////////////////////////////////
	// �R���X�g���N�V����

	explicit edit(
		HCURSOR hCursorMargin = NULL,
		HFONT   hFont         = NULL);
	~edit(void);

	BOOL Create(
		const DWORD dwStyle, const DWORD dwExStyle,
		const DWORD dwXStyle,
		wgc::window *parent,
		const UINT nID,
		HINSTANCE hInstance = NULL,
		const int x = CW_USEDEFAULT,
		const int y = CW_USEDEFAULT,
		const int nWidth  = CW_USEDEFAULT,
		const int nHeight = CW_USEDEFAULT);


	////////////////////////////////////////////////////////////////
	// ���ݒ�
	void SetCursorMargin(HCURSOR hCursorMargin);


	////////////////////////////////////////////////////////////////
	// �X�^�C��
	BOOL IsReadOnly(void) const;

	DWORD GetXStyle(void) const;
	DWORD SetXStyle(const DWORD dwNewStyle);
	BOOL IsLinenumberVisible(void) const;
	BOOL IsRulerVisible     (void) const;
	BOOL IsCursorUnderline  (void) const;

	// ����
	BOOL GetModify(void) const;
	void SetModify(const BOOL bModified = TRUE);
	BOOL SetReadOnly(const BOOL bReadOnly = TRUE);

	void  SetMargins(const UINT nLeft, const UINT nRight);
	DWORD GetMargins(void) const;

	int  GetLineCount(void) const;

	BOOL Undo   (void);
	BOOL CanUndo(void) const;
	BOOL Redo   (void);
	BOOL CanRedo(void) const;
	void Cut     (void);
	void Copy    (void);
	void Clear   (void);
	void Paste   (void);
	BOOL CanPaste(const UINT uFormat = 0) const;

	const_iterator_t GetIteratorBegin(void) const;
	const_iterator_t GetIteratorEnd  (void) const;
	void SetData(const wchar_t *wstr);
	void SetEmpty(void);

protected:
	// �G�f�B�^�G���W��
	sgc::edit_manager m_manager;

	// �_�����
	POINT m_ptCaretLogical;                 // �J���b�g�̘_�����W�i���_(0, 0)�j
	int   m_nInternalX;                     // �����I��X���W

	// �������
	HCURSOR m_hCursorMargin;                // �]�����̃J�[�\��
	HFONT   m_hFont;                        // �t�H���g
	SIZE    m_sizeFont;                     // �t�H���g�̑傫��
	int    *m_nCharWidth;                   // �����̕�

	SIZE m_sizeTextArea;                    // �e�L�X�g�̈�̃T�C�Y�i�J���b�g�P�ʁj

	BOOL  m_bCaretVisible;                  // �J���b�g������

	POINT m_ptCaretPhysical;                // �J���b�g�̕������W�i��ʏ�̈ʒu ���_(0, 0)�j
	POINT m_ptCaretPhysicalPrev;            // �O��`�F�b�N�����Ƃ��̃J���b�g�̕������W

	int m_nColumnLeft;                      // ��ԍ��̌��ԍ�
	int m_nColumnLeftPrev;                  // �O��`�F�b�N�����Ƃ��̈�ԍ��̌��ԍ�

	int m_nLineNumberTopPrev;               // �O��`�F�b�N�����Ƃ��̉�ʂ̏�[�ɕ\������Ă���s�ԍ�
	int m_nLineCountPrev;                   // �O��`�F�b�N�����Ƃ��̍s��

	text_info_t       m_infoText;
	linenumber_info_t m_infoLineNumber;
	ruler_info_t      m_infoRuler;

	DWORD m_dwXStyle;
	int m_nTextMarginLeft;                  // �����]���i�s�ԍ���\�����j
	int m_nTextMarginRight;                 // �E���]��


	////////////////////////////////////////////////////////////
	// �R���|�[�l���g�̏�����
	BOOL InitEditComponent(void);


	////////////////////////////////////////////////////////////
	// �_��/�������[�`��

	// ���W�ϊ�
	BOOL PhysicalToLogical(void);
	BOOL LogicalToPhysical(const BOOL bUpdateInternalX = TRUE);

	// �������v�Z
	int CalcCodeLength(const wchar_t code) const;
	int CalcTabLength (const int cols) const;
	int CalcPhysicalLength(const wstring_t &wstr) const;
	int CalcPhysicalLength(LPCWSTR wstr, const int length) const;
	int CalcLogicalLength (LPCWSTR wstr, const int length) const;
	int CalcNearestPos    (LPCWSTR wstr, const int pos) const;

	// �X�N���[��
	void ScrollHorz(const int columns, const BOOL bPhysical, const BOOL bLogical);
	void ScrollVert(const int lines  , const BOOL bPhysical, const BOOL bLogical);

	////////////////////////////////////////////////////////////
	// �_�����[�`��
	void ExpandLineNumberCols(void);


	////////////////////////////////////////////////////////////
	// �������[�`��

	// �q�b�g�e�X�g
	typedef enum _client_hittest_t
	{
		CHT_TEXT,                               // �e�L�X�g�̈�
		CHT_MARGIN,                             // �]���̈�
		CHT_SELECTED,                           // �I��̈�
	} client_hittest_t;
	client_hittest_t ClientHitTest(const POINT &pos_display) const;

	void UpdateTextAreaSize(void);          // �e�L�X�g�̈�̃T�C�Y���X�V

	void SyncHScroll(void);                 // �����X�N���[���o�[�𓯊�
	void SyncVScroll(void);                 // �����X�N���[���o�[�𓯊�

	// �ύX�𔽉f
	void UpdateClient(const int linenumber_modify);
	BOOL EnsureVisible(const BOOL bByScroll = TRUE);
	void AdjustPosition(void);              // �J���b�g�ʒu�A���ݍs���C��

	// �̈�v�Z���[�`��
	int  CalcMarginLeft(const BOOL bAddTextMargin = TRUE) const;
	int  CalcMarginTop (void) const;
	void CalcRectLineNumber(RECT &rRect) const;
	void CalcRectRuler     (RECT &rRect) const;
	void CalcRectText      (RECT &rRect, const BOOL bIncludeMargin = TRUE) const;

	// �J���b�g���[�`��
	void DisplayCaret(const BOOL bShow = TRUE);
	void CalcDisplayPos(const POINT &pos_caret  , POINT &pos_display) const;
	void CalcCaretPos  (const POINT &pos_display, POINT &pos_caret  ) const;
	void MoveCaret(void);

	// �L�[�{�[�h���[�`��
	void KeyPageUp  (void);
	void KeyPageDown(void);
	void KeyHome    (void);
	void KeyEnd     (void);

	void KeyLeft (void);
	void KeyRight(void);
	void KeyUp   (void);
	void KeyDown (void);

	int  KeyReturn(void);
	int  KeyBack(void);
	int  KeyDelete(void);
	void KeyString(const wstring_t &wstr);

	void InsertData(const wchar_t *wstr);
	void InsertString(const wstring_t &wstr);
	void InsertLineFeed(void);

	// �`�惋�[�`��
	void PaintClient(
		wgc::devicecontext &dc,
		const int  linenumber_redraw = 1,
		const BOOL bLinenumber = TRUE,
		const BOOL bRuler      = TRUE) const;

	void PaintText    (wgc::devicecontext &dc, const int linenumber_redraw) const;
	void PaintTextBack(wgc::devicecontext &dc) const;
	void PaintLineNumber    (wgc::devicecontext &dc) const;
	void PaintLineNumberBack(wgc::devicecontext &dc) const;
	void PaintRuler    (wgc::devicecontext &dc) const;
	void PaintRulerBack(wgc::devicecontext &dc) const;
	void PaintUnderline(wgc::devicecontext &dc, const POINT &pos, const BOOL bDraw = TRUE) const;

	int  DrawString        (wgc::devicecontext &dc, const POINT &pos, const wstring_t &wstr, const COLORREF colorText, const COLORREF colorBack) const;
	void PaintString       (wgc::devicecontext &dc, const POINT &pos, LPCWSTR wstr, const int length) const;
	void PaintMarkLineFeed (wgc::devicecontext &dc, const POINT &pos) const;
	void PaintMarkEOF      (wgc::devicecontext &dc, const POINT &pos) const;
	int  PaintMarkTab      (wgc::devicecontext &dc, const POINT &pos_base, const int x) const;
	int  PaintMarkWideSpace(wgc::devicecontext &dc, const POINT &pos_base, const int x) const;


	////////////////////////////////////////////////////////////
	// ���b�Z�[�W�֘A
	LRESULT SendNotifyCommandToParent(const WORD wNotify);

	LRESULT CALLBACK WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	UINT OnGetDlgCode(LPMSG lpMsg);

	void OnSize(const UINT nType, const int cx, const int cy);

	void OnSetFocus (HWND hWndLoseFocus);
	void OnKillFocus(HWND hWndGetFocus);

	void  OnSetFont(HFONT hFont, const BOOL bRedraw);
	HFONT OnGetFont(void);

	void OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);
	void OnVScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);

	BOOL OnSetCursor(HWND hWnd, const UINT nHitTest, const UINT message);

	void OnKeyDown(const UINT nChar, const UINT nRepeatCount, const UINT nFlags);
	void OnChar   (const UINT nChar, const UINT nRepeatCount, const UINT nFlags);

	void OnImeComposition(const WORD chDBCS, const UINT nFlags);

	BOOL OnMouseWheel(const UINT nFlags, const short zDelta, const POINT &point);
	void OnLButtonDown(const UINT nFlags, const POINT &point);
	void OnRButtonDown(const UINT nFlags, const POINT &point);
	void OnMButtonDown(const UINT nFlags, const POINT &point);

	void OnPaint(void);
	BOOL OnEraseBkgnd(HDC hDC);

	// ����
	BOOL OnEmGetModify(void) const;
	void OnEmSetModify(const BOOL bModified);
	BOOL OnEmSetReadOnly(const BOOL bReadOnly);

	void  OnEmSetMargins(const WORD fwMargin, const WORD wLeft, const WORD wRight);
	DWORD OnEmGetMargins(void) const;

	int  OnEmGetLineCount(void) const;

	BOOL OnEmUndo   (void);
	BOOL OnEmCanRedo(void) const;
	BOOL OnEmRedo   (void);
	BOOL OnEmCanUndo(void) const;
	void OnCut       (void);
	void OnCopy      (void);
	void OnClear     (void);
	void OnPaste     (void);
	BOOL OnEmCanPaste(const UINT uFormat) const;

private:
	// �����o�ϐ��̐ݒ�
	void _SetFontSize    (const int cx, const int cy);
	void _SetTextAreaSize(const int cx, const int cy);

	// �����n���h��
	void _SyncLineCount    (void);
	void _SyncLineNumberTop(void);
};

_WXC_END                                // }

#endif // __WXC_EDIT__
