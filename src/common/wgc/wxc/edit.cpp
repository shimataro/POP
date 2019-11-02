// edit.cpp �c�C���v�������e�[�V�����t�@�C��
// �@Windows 9x �ł��g�p�\��UNICODE�Ή�API ::TextOutW(), ::ExtTextOutW(), ::GetCharWidthW() ���g�p
#include "edit.h"
#include <windowsx.h>
#include <assert.h>                     // assert()
#include <wchar.h>                      // iswprint(), swprintf()
#include <algorithm>                    // std::max()

#include "../imm.h"                     // wgc::imm
#include "../wgfunc.h"                  // wgc::wgfMultiByteToWideChar()
#include "../../sgc/charcode.h"         // charcode_categorize_init(), charcode_categorize()


#define WXC_EDIT_CLASS  _T("wxc::edit")

#define is_keydown_control()    (::GetKeyState(VK_CONTROL) < 0)

// �X�N���[���o�[���
#define VSCROLL_MIN     0
#define VSCROLL_MAX     65535
#define VSCROLL_PAGE    (VSCROLL_MAX - VSCROLL_MIN + 1)

// �f�t�H���g�l
#define DEFAULT_FONT    GetStockFont(ANSI_VAR_FONT)     // �v���|�[�V���i���t�H���g
//#define DEFAULT_FONT    GetStockFont(OEM_FIXED_FONT)    // �Œ�s�b�`�t�H���g

// �����R�[�h
#define CODE_NUL        L'\0'
#define CODE_TAB        L'\t'
#define CODE_SPACE      L' '
#define CODE_WIDESPACE  L'�@'

#define SYMBOL_ARROW_DOWN   (wchar_t)7  // ���p�́u���v�iUNICODE�ł� 0xffec�j
#define SYMBOL_ARROW_UP     (wchar_t)28 // ���p�́u���v�iUNICODE�ł� 0xffea�j
#define SYMBOL_ARROW_LEFT   (wchar_t)31 // ���p�́u���v�iUNICODE�ł� 0xffe9�j
#define SYMBOL_ARROW_RIGHT  (wchar_t)30 // ���p�́u���v�iUNICODE�ł� 0xffeb�j
#define SYMBOL_LF           (wchar_t)27 // ���s�}�[�N

// �}�[�N
#define MARK_EOF            L"[EOF]"
#define MARK_EOF_LEN        5
#define MARK_WIDESPACE      L"��"
#define MARK_WIDESPACE_LEN  1
#define MARK_TAB            SYMBOL_ARROW_RIGHT
#define MARK_LF             SYMBOL_ARROW_DOWN


_WXC_BEGIN                              // namespace wxc {

const edit::text_info_t default_text_info =
{
	4,                                      // �^�u��
	1,                                      // �s�Ԋu�i���P�j

	RGB(  0,   0,   0),                     // �F
	RGB(255, 255, 255),                     // �w�i�F
	RGB(  0, 128, 255),                     // ���ꕶ���̐F
	RGB(224, 224, 224),                     // �r���̐F
};

const edit::linenumber_info_t default_linenumber_info =
{
	5,                                      // ����
	8,                                      // �E���]��
	RGB(  0,   0,   0),                     // �F
	RGB(196, 196, 196),                     // �w�i�F
};

const edit::ruler_info_t default_ruler_info =
{
	8,                                      // ����
	10,                                     // �Ԋu
	RGB(  0,   0,   0),                     // �F
	RGB(196, 196, 196),                     // �w�i�F
};


////////////////////////////////////////////////////////////
// �R���X�g���N�V����

// �R���X�g���N�^
edit::edit(
	HCURSOR hCursorMargin /* = NULL */,
	HFONT   hFont         /* = NULL */)
{
	static BOOL bInitialized = FALSE;
	if(!bInitialized)
	{
		InitEditComponent();
		bInitialized = TRUE;
	}

	////////////////////////////////////////////////////////////
	// �_�����̏�����

	m_ptCaretLogical.x = 0;
	m_ptCaretLogical.y = 0;
	m_nInternalX       = 0;


	////////////////////////////////////////////////////////////
	// �������̏�����

	// �]�����̃J�[�\��
	m_hCursorMargin = hCursorMargin;
	if(m_hCursorMargin == NULL)
	{
		m_hCursorMargin = ::LoadCursor(NULL, IDC_ARROW);
	}

	// �t�H���g�֘A
	m_hFont = hFont;
	m_sizeFont.cx = 1;
	m_sizeFont.cy = 1;
	m_nCharWidth = new int[WCHAR_MAX - WCHAR_MIN + 1];

	m_sizeTextArea.cx = 1;
	m_sizeTextArea.cy = 1;

	m_bCaretVisible = FALSE;

	m_ptCaretPhysical.x = 0;
	m_ptCaretPhysical.y = 0;
	m_ptCaretPhysicalPrev = m_ptCaretPhysical;

	m_nColumnLeft     = 0;
	m_nColumnLeftPrev = m_nColumnLeft;

	m_nLineNumberTopPrev = 1;
	m_nLineCountPrev     = m_manager.get_line_count();

	// �e�L�X�g / �s�ԍ� / ���[���[���
	m_infoText       = default_text_info;
	m_infoLineNumber = default_linenumber_info;
	m_infoRuler      = default_ruler_info;

	m_dwXStyle = 0;
	m_nTextMarginLeft  = 0;
	m_nTextMarginRight = 0;
}

// �f�X�g���N�^
edit::~edit(void)
{
	delete []m_nCharWidth;
	m_nCharWidth = NULL;

	// ���\�[�X�̊J��
	// �E::LoadCursor() �Ń��[�h�����J�[�\���͔j�������Ⴂ���Ȃ��炵��
	// �E�t�H���g�̔j���`���͌Ăяo�����ɂ���
	// �i�f�t�H���g�ł̓X�g�b�N�t�H���g���g�p����̂Ŕj������K�v�͂Ȃ��j
	// ������A���݂͓��ɂ��邱�Ƃ��Ȃ�
}

BOOL edit::Create(
	const DWORD dwStyle, const DWORD dwExStyle,
	const DWORD dwXStyle,
	wgc::window *parent,
	const UINT nID,
	HINSTANCE hInstance /* = NULL */,
	const int x /* = CW_USEDEFAULT */,
	const int y /* = CW_USEDEFAULT */,
	const int nWidth  /* = CW_USEDEFAULT */,
	const int nHeight /* = CW_USEDEFAULT */)
{
	m_dwXStyle = dwXStyle;
	return window::Create(
		WXC_EDIT_CLASS, _T(""),
		dwStyle, dwExStyle,
		parent,
		reinterpret_cast<HMENU>(nID),
		hInstance,
		x, y, nWidth, nHeight);
}


////////////////////////////////////////////////////////////
// ���ݒ�

// �]�����̃J�[�\����ݒ�
void edit::SetCursorMargin(HCURSOR hCursorMargin)
{
	assert(hCursorMargin != NULL);
	m_hCursorMargin = hCursorMargin;
}


////////////////////////////////////////////////////////////
// �X�^�C��

// �G�f�B�b�g�R���|�[�l���g�̊g���X�^�C�����擾
DWORD edit::GetXStyle(void) const
{
	return m_dwXStyle;
}

// �G�f�B�b�g�R���|�[�l���g�̊g���X�^�C����ݒ�
DWORD edit::SetXStyle(const DWORD dwNewStyle)
{
	// �{���� GetStyle() / SetStyle() ���g�����������񂾂��ǁA
	// WS_xxx �� ES_xxx �ŃX�^�C����32�߂����������疳���ł����B�B
	const DWORD dwOldStyle = m_dwXStyle;
	m_dwXStyle = dwNewStyle;

	UpdateTextAreaSize();
	InvalidateRect();
	return dwOldStyle;
}

// �s�ԍ���\�����邩�H
BOOL edit::IsLinenumberVisible(void) const
{
	return m_dwXStyle & XES_LINENUMBER;
}

// ���[���[��\�����邩�H
BOOL edit::IsRulerVisible(void) const
{
	return m_dwXStyle & XES_RULER;
}

// �J�[�\���ʒu�ɉ������������H
BOOL edit::IsCursorUnderline(void) const
{
	return m_dwXStyle & XES_CURSORUNDERLINE;
}

// �ǂݍ��ݐ�p���[�h���H
BOOL edit::IsReadOnly(void) const
{
	return GetStyle() & ES_READONLY;
}


// ����

// �_�[�e�B�t���O���擾
BOOL edit::GetModify(void) const
{
	return SendMessage(EM_GETMODIFY);
}

// �_�[�e�B�t���O��ݒ�
void edit::SetModify(const BOOL bModified /* = TRUE */)
{
	SendMessage(EM_SETMODIFY, bModified);
}

// �ǂݎ���p�����̕t��/����
BOOL edit::SetReadOnly(const BOOL bReadOnly /* = TRUE */)
{
	return SendMessage(EM_SETREADONLY, bReadOnly);
}

void edit::SetMargins(const UINT nLeft, const UINT nRight)
{
	SendMessage(
		EM_SETMARGINS,
		EC_LEFTMARGIN | EC_RIGHTMARGIN,
		MAKELONG(nLeft, nRight));
}

DWORD edit::GetMargins(void) const
{
	return SendMessage(EM_GETMARGINS);
}

// �o�b�t�@�̍s�����擾
int edit::GetLineCount(void) const
{
	return SendMessage(EM_GETLINECOUNT);
}

// �A���h�D
BOOL edit::Undo(void)
{
	return SendMessage(EM_UNDO);
}

// �A���h�D�ł��邩�H
BOOL edit::CanUndo(void) const
{
	return SendMessage(EM_CANUNDO);
}

// ���h�D
BOOL edit::Redo(void)
{
	return SendMessage(EM_REDO);
}

// ���h�D�ł��邩�H
BOOL edit::CanRedo(void) const
{
	return SendMessage(EM_CANREDO);
}

// �I��͈͂�؂���A�N���b�v�{�[�h�ɃR�s�[
void edit::Cut(void)
{
	SendMessage(WM_CUT);
}

// �I��͈͂��N���b�v�{�[�h�ɃR�s�[
void edit::Copy(void)
{
	SendMessage(WM_COPY);
}

// �I��͈͂��폜
void edit::Clear(void)
{
	SendMessage(WM_CLEAR);
}

// �N���b�v�{�[�h���̃f�[�^��\��t��
void edit::Paste(void)
{
	SendMessage(WM_PASTE);
}

// �\��t���ł��邩�H
BOOL edit::CanPaste(const UINT uFormat /* = 0 */) const
{
	return SendMessage(EM_CANPASTE, uFormat);
}


// �o�b�t�@�̐擪�������C�e���[�^���擾
edit::const_iterator_t edit::GetIteratorBegin(void) const
{
	return m_manager.get_iterator_begin();
}

// �o�b�t�@�̖����������C�e���[�^���擾
edit::const_iterator_t edit::GetIteratorEnd(void) const
{
	return m_manager.get_iterator_end();
}

// ���݂̃o�b�t�@��j�����Ďw��o�b�t�@������
void edit::SetData(const wchar_t *wstr)
{
	m_manager.set_data(wstr);

	_SyncLineCount();

	m_ptCaretLogical.x = 0;
	m_ptCaretLogical.y = 0;

	LogicalToPhysical();
	EnsureVisible();
	UpdateClient(1);
	MoveCaret();
}

// �o�b�t�@����ɂ���
void edit::SetEmpty(void)
{
	m_manager.set_empty();

	_SyncLineCount();

	m_ptCaretLogical.x = 0;
	m_ptCaretLogical.y = 0;

	LogicalToPhysical();
	EnsureVisible();
	UpdateClient(1);
	MoveCaret();
}


////////////////////////////////////////////////////////////////////////////////
// protected �֐�


////////////////////////////////////////////////////////////
// �R���|�[�l���g�̏�����
BOOL edit::InitEditComponent(void)
{
	HINSTANCE hInstance = ::GetModuleHandle(NULL);
	HCURSOR   hCursor   = ::LoadCursor(NULL, IDC_IBEAM);

	// �E�C���h�E�N���X�̓o�^
	WNDCLASS wc;
	wc.style         = CS_DBLCLKS | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
	wc.lpfnWndProc   = _WindowProcBase;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = NULL;
	wc.hCursor       = hCursor;
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = WXC_EDIT_CLASS;
	if(::RegisterClass(&wc) == 0)
	{
		return FALSE;
	}

	// �������ރe�[�u���̏�����
	charcode_categorize_init();
	return TRUE;
}


////////////////////////////////////////////////////////////
// �_��/�������[�`��

// �����J���b�g�ʒu���_���J���b�g�ʒu
BOOL edit::PhysicalToLogical(void)
{
	const int linenumber_top = m_manager.get_linenumber_top();

	m_manager.set_position(linenumber_top + m_ptCaretPhysical.y, false, true);
	const_iterator_t pos_now = m_manager.get_iterator_now();

	m_ptCaretLogical.x = CalcLogicalLength(pos_now->c_str(), m_ptCaretPhysical.x + m_nColumnLeft * m_sizeFont.cx);
	m_ptCaretLogical.y = m_manager.get_linenumber_now() - 1;
	m_nInternalX = CalcPhysicalLength(pos_now->c_str(), m_ptCaretLogical.x);

	return TRUE;
}

// �_���J���b�g�ʒu�������J���b�g�ʒu
BOOL edit::LogicalToPhysical(const BOOL bUpdateInternalX /* = TRUE */)
{
	const int linenumber_top = m_manager.get_linenumber_top();
	const_iterator_t pos_now = m_manager.get_iterator_now();

	// �J�[�\�����ړ�
	m_ptCaretPhysical.x = CalcPhysicalLength(pos_now->c_str(), m_ptCaretLogical.x) - m_nColumnLeft * m_sizeFont.cx;
	m_ptCaretPhysical.y = m_ptCaretLogical.y - (linenumber_top - 1);

	if(bUpdateInternalX)
	{
		m_nInternalX = m_ptCaretPhysical.x;
	}

	return FALSE;
}


// �����R�[�h code �̕����L�����N�^�P�ʂŌv�Z
int edit::CalcCodeLength(const wchar_t code) const
{
	// ����ȊO
	return m_nCharWidth[WCHAR_MIN + code];
}

// cols ���ڂ̈ʒu�ɂ���^�u�����Ōv�Z
int edit::CalcTabLength(const int cols) const
{
	const int tabsize = m_infoText.nTabSize * m_sizeFont.cx;
	return tabsize - (cols % tabsize);
}


// ������ wstr �̕����������߂�
int edit::CalcPhysicalLength(const wstring_t &wstr) const
{
	return CalcPhysicalLength(wstr.c_str(), wstr.length());
}

// �_���� length �̕����� wstr �̕����������߂�
int edit::CalcPhysicalLength(LPCWSTR wstr, const int length) const
{
	int pos = 0;
	for(int i = 0; i < length; i++)
	{
		const wchar_t code = wstr[i];
		if(code == CODE_TAB)
		{
			pos += CalcTabLength(pos);
		}
		else
		{
			pos += CalcCodeLength(code);
		}
	}
	return pos;
}

// ������ length �𒴂��Ȃ������� wstr �̍ő�ʒu�����߂�
int edit::CalcLogicalLength(LPCWSTR wstr, const int length) const
{
	int pos = 0;
	int i = 0;
	for(i = 0; wstr[i] != CODE_NUL; i++)
	{
		const wchar_t code = wstr[i];
		if(code == CODE_TAB)
		{
			pos += CalcTabLength(pos);
		}
		else
		{
			pos += CalcCodeLength(code);
		}

		// length�𒴂�����A���̎��_�̈ʒu��Ԃ�
		if(pos > length)
		{
			break;
		}
	}
	return i;
}

// ������ wstr ���ŁApos �ɍł��߂������������߂�
int edit::CalcNearestPos(LPCWSTR wstr, const int pos) const
{
	int nearest_pos = 0;
	while(*wstr != CODE_NUL)
	{
		const wchar_t code = *wstr++;
		const int length = (code == CODE_TAB) ? CalcTabLength(nearest_pos) : CalcCodeLength(code);

		if(pos <= nearest_pos + (length + 1) / 2)
		{
			break;
		}
		nearest_pos += length;
	}
	return nearest_pos;
}


////////////////////////////////////////////////////////////
// �X�N���[��

// columns �������X�N���[��
void edit::ScrollHorz(const int columns, const BOOL bPhysical, const BOOL bLogical)
{
	const int dx = columns * m_sizeFont.cx;
	int total = 0;

	// �����X�N���[��
	if(bPhysical)
	{
		m_nColumnLeft += columns;
		total -= dx;
	}

	// �_���X�N���[��
	if(bLogical)
	{
	}

	m_ptCaretPhysical.x += total;
	if(bLogical)
	{
		LogicalToPhysical(FALSE);
	}
	if(bPhysical)
	{
	}
}

// lines �s�����X�N���[��
void edit::ScrollVert(const int dy, const BOOL bPhysical, const BOOL bLogical)
{
	int total = 0;

	// �����X�N���[��
	if(bPhysical)
	{
		const int count = m_manager.scroll_top(dy);
		total -= count;
	}

	// �_���X�N���[��
	if(bLogical)
	{
		const int count = m_manager.scroll_now(dy);

		const_iterator_t pos_now = m_manager.get_iterator_now();
		m_ptCaretLogical.x  = CalcLogicalLength(pos_now->c_str(), m_nInternalX);
		m_ptCaretLogical.y += count;
		total += count;
	}

	m_ptCaretPhysical.y += total;
	if(bLogical)
	{
		LogicalToPhysical(FALSE);
	}
	if(bPhysical)
	{
		_SyncLineNumberTop();
	}
}


////////////////////////////////////////////////////////////
// �_�����[�`��

// �s�ԍ��̌������g��
void edit::ExpandLineNumberCols(void)
{
	// ���݂̌����𐔂���
	int cols = 0;
	int line_count = m_manager.get_line_count();
	while(line_count > 0)
	{
		line_count /= 10;
		cols++;
	}

	// �ݒ肵�Ă��錅�����傫����Ίg��
	if(m_infoLineNumber.nCols < cols)
	{
		m_infoLineNumber.nCols = cols;
		InvalidateRect();
	}
}


////////////////////////////////////////////////////////////
// �������[�`��

// �q�b�g�e�X�g
edit::client_hittest_t edit::ClientHitTest(const POINT &pos_display) const
{
	// �]���̈�
	if(pos_display.x < CalcMarginLeft())
	{
		return CHT_MARGIN;
	}
	return CHT_TEXT;
}

// �e�L�X�g�̈�̃T�C�Y�i�J���b�g�P�ʁj�̍X�V
void edit::UpdateTextAreaSize(void)
{
	RECT rect;
	CalcRectText(rect, FALSE);

	const int cx = rect.right - rect.left;
	const int cy = (rect.bottom - rect.top) / (m_sizeFont.cy + m_infoText.nLineSpace);
	_SetTextAreaSize(
		(cx > 0) ? cx : 1,
		(cy > 0) ? cy : 1);
//	OnSyncTextAreaSize();
}


// �����X�N���[���o�[�𓯊�
void edit::SyncHScroll(void)
{
}

// �����X�N���[���o�[�𓯊�
void edit::SyncVScroll(void)
{
	const LONG cy = m_sizeTextArea.cy;
	assert(cy > 0);

	const int line_count     = m_manager.get_line_count();
	const int linenumber_top = m_manager.get_linenumber_top();
	assert(line_count     > 0);
	assert(linenumber_top > 0);

	// [block]
	// �X�N���[���o�[�̃y�[�W�T�C�Y�ƈʒu���X�V
	{
		SCROLLINFO si = {sizeof(si)};
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_DISABLENOSCROLL;
		si.nMin  = 1;
		si.nMax  = line_count + cy - 1;
		si.nPage = cy;
		si.nPos  = linenumber_top;
		SetVScrollInfo(si);
	}
}


// �ύX�𔽉f
void edit::UpdateClient(const int linenumber_modify)
{
	const int linecount      = m_manager.get_line_count();
	const int linenumber_top = m_manager.get_linenumber_top();

	int  linenumber_redraw  = linenumber_modify;
	BOOL bLinenumber        = FALSE;
	BOOL bRuler             = FALSE;

	// ��ʍ��[�̌����ς���Ă�����e�L�X�g�̈�i�S���j�ƃ��[���[���ĕ`��
	if(m_nColumnLeftPrev != m_nColumnLeft)
	{
		m_nColumnLeftPrev = m_nColumnLeft;
		linenumber_redraw = 1;
		bRuler = TRUE;
	}

	// ��ʏ�[�̍s�ԍ����ς���Ă�����e�L�X�g�̈�i�S���j�ƍs�ԍ����ĕ`��
	if(linenumber_top != m_nLineNumberTopPrev)
	{
		m_nLineNumberTopPrev = linenumber_top;
		linenumber_redraw = 1;
		bLinenumber = TRUE;
	}

	// �s�����ύX����Ă�����e�L�X�g�̈�i�ύX���j�ƍs�ԍ����ĕ`��
	if(linecount != m_nLineCountPrev)
	{
		m_nLineCountPrev = linecount;
		bLinenumber = TRUE;
	}

	// �N���C�A���g�̈��`��
	if(linenumber_redraw > 0 || bLinenumber || bRuler)
	{
		wgc::devicecontext_client dc(m_hWnd);
		PaintClient(dc, linenumber_redraw, bLinenumber, bRuler);
	}
}

// �J���b�g����ʓ��ɕ\��
BOOL edit::EnsureVisible(const BOOL bByScroll /* = TRUE */)
{
	BOOL bMoved = FALSE;

	// ��ʂ��X�N���[��
	if(bByScroll)
	{
		if(m_ptCaretPhysical.x < 0)
		{
			const int dx      = m_ptCaretPhysical.x;
			const int columns = (dx - (m_sizeFont.cx - 1)) / m_sizeFont.cx;
			ScrollHorz(columns, TRUE, FALSE);
			bMoved = TRUE;
		}
		if(m_ptCaretPhysical.x >= m_sizeTextArea.cx - 1)
		{
			const int dx      = m_ptCaretPhysical.x - (m_sizeTextArea.cx - 1);
			const int columns = (dx + (m_sizeFont.cx - 1)) / m_sizeFont.cx;
			ScrollHorz(columns, TRUE, FALSE);
			bMoved = TRUE;
		}

		if(m_ptCaretPhysical.y < 0)
		{
			ScrollVert(m_ptCaretPhysical.y, TRUE, FALSE);
			assert(m_ptCaretPhysical.y == 0);
			bMoved = TRUE;
		}
		if(m_ptCaretPhysical.y > m_sizeTextArea.cy - 1)
		{
			ScrollVert(m_ptCaretPhysical.y - (m_sizeTextArea.cy - 1), TRUE, FALSE);
			assert(m_ptCaretPhysical.y == m_sizeTextArea.cy - 1);
			bMoved = TRUE;
		}
	}
	// �J�[�\�����ړ�
	else
	{
		if(m_ptCaretPhysical.y < 0)
		{
			m_ptCaretPhysical.y = 0;
			bMoved = TRUE;
		}
		if(m_ptCaretPhysical.y > m_sizeTextArea.cy - 1)
		{
			m_ptCaretPhysical.y = m_sizeTextArea.cy - 1;
			bMoved = TRUE;
		}

		if(m_ptCaretPhysical.x < 0)
		{
			m_ptCaretPhysical.x = 0;
			bMoved = TRUE;
		}
		if(m_ptCaretPhysical.x > m_sizeTextArea.cx)
		{
			m_ptCaretPhysical.x = m_sizeTextArea.cx;
			bMoved = TRUE;
		}

		if(bMoved)
		{
			PhysicalToLogical();
			LogicalToPhysical(FALSE);
		}
	}
	return bMoved;
}

// �J���b�g�ʒu�A���ݍs���C��
void edit::AdjustPosition(void)
{
	const int line_count     = m_manager.get_line_count();
	const int linenumber_top = m_manager.get_linenumber_top();

	// Y���W�̃`�F�b�N�i�t�@�C���̖������z���Ă��Ȃ����j
	if(m_ptCaretPhysical.y + linenumber_top > line_count)
	{
		// �J�[�\���ʒu���C��
		m_ptCaretPhysical.y = line_count - linenumber_top;

		m_manager.set_position(0, false, true);
	}

	// X���W�̃`�F�b�N�i�s�����z���Ă��Ȃ����j
	const_iterator_t pos_now = m_manager.get_iterator_now();

	const int length = CalcPhysicalLength(*pos_now);
	if(m_ptCaretPhysical.x > length)
	{
		m_ptCaretPhysical.x = length;
	}
}


////////////////////////////////////////////////////////////
// �̈�v�Z���[�`��

// �e�L�X�g�̍����]�����v�Z
int edit::CalcMarginLeft(const BOOL bAddTextMargin /* = TRUE */) const
{
	int nMargin = 0;
	if(bAddTextMargin)
	{
		nMargin += m_nTextMarginLeft;
	}
	if(IsLinenumberVisible())
	{
		nMargin += m_infoLineNumber.nCols * m_sizeFont.cx + m_infoLineNumber.nMargin;
	}

	return nMargin;
}

// �e�L�X�g�̏㑤�]�����v�Z
int edit::CalcMarginTop(void) const
{
	int nMargin = 0;
	if(IsRulerVisible())
	{
		nMargin += m_infoRuler.nHeight;
	}

	return nMargin;
}

// �s�ԍ��̈�
void edit::CalcRectLineNumber(RECT &rRect) const
{
	GetClientRect(rRect);
	rRect.top   = CalcMarginTop();
	rRect.right = m_infoLineNumber.nCols * m_sizeFont.cx + m_infoLineNumber.nMargin;
}

// ���[���[�̈�
void edit::CalcRectRuler(RECT &rRect) const
{
	GetClientRect(rRect);
	rRect.bottom = CalcMarginTop();
}

// �e�L�X�g�̈�
void edit::CalcRectText(RECT &rRect, const BOOL bIncludeMargin /* = TRUE */) const
{
	// �]���̈���܂ރe�L�X�g�̈���擾
	GetClientRect(rRect);
	rRect.left = CalcMarginLeft(FALSE);
	rRect.top  = CalcMarginTop();

	// �]���̈������
	if(!bIncludeMargin)
	{
		rRect.left  += m_nTextMarginLeft;
		rRect.right -= m_nTextMarginRight;
	}
}


////////////////////////////////////////////////////////////
// �J���b�g���[�`��

// �Q�ƃJ�E���^���g�킸�ɃJ���b�g�̕\��/��\����؂�ւ�
void edit::DisplayCaret(const BOOL bShow /* = TRUE */)
{
	// ShowCaret() �ŃJ���b�g��\������ɂ́A
	// HideCaret() ���Ăяo�����񐔂Ɠ��������Ăяo���K�v������B
	// DisplayCaret() ���g���΂��̕K�v���Ȃ��Ȃ�B

	// �\��
	if(bShow)
	{
		// ���ݕs���Ȃ�\��
		if(!m_bCaretVisible)
		{
			m_bCaretVisible = TRUE;
			ShowCaret();
		}
	}
	// ��\��
	else
	{
		// ���݉��Ȃ��\��
		if(m_bCaretVisible)
		{
			m_bCaretVisible = FALSE;
			HideCaret();
		}
	}
}

// �J���b�g�ʒu����N���C�A���g���W���v�Z
void edit::CalcDisplayPos(const POINT &pos_caret, POINT &pos_display) const
{
	// ��{���W
	pos_display.x = pos_caret.x;
	pos_display.y = pos_caret.y * (m_sizeFont.cy + m_infoText.nLineSpace);

	// �]�������l��
	pos_display.x += CalcMarginLeft();
	pos_display.y += CalcMarginTop();
}

// �N���C�A���g���W����J���b�g�ʒu���v�Z
void edit::CalcCaretPos(const POINT &pos_display, POINT &pos_caret) const
{
	pos_caret.x = pos_display.x - CalcMarginLeft();
	if(pos_caret.x < 0) { pos_caret.x = 0; } else { pos_caret.x += m_nColumnLeft * m_sizeFont.cx; }

	pos_caret.y = pos_display.y - CalcMarginTop();
	if(pos_caret.y < 0) { pos_caret.y = 0; }

	// y���W���L�����N�^�P�ʂɕ␳
	pos_caret.y /= (m_sizeFont.cy + m_infoText.nLineSpace);

	// �w��ʒu�̃C�e���[�^���擾
	sgc::edit_manager::const_iterator_t p = m_manager.get_iterator_top();
	for(int i = 0; i < pos_caret.y; i++)
	{
		sgc::edit_manager::const_iterator_t tmp = p;
		if(++tmp == m_manager.get_iterator_end())
		{
			pos_caret.y = i;
			break;
		}
		p = tmp;
	}

	// �J���b�g���W���v�Z
	pos_caret.x = CalcNearestPos(p->c_str(), pos_caret.x) - m_nColumnLeft * m_sizeFont.cx;
}

// �J���b�g���ړ�
void edit::MoveCaret(void)
{
	// �e�L�X�g�G���A�͈͓̔�
	if(0 <= m_ptCaretPhysical.x && m_ptCaretPhysical.x <= m_sizeTextArea.cx
	&& 0 <= m_ptCaretPhysical.y && m_ptCaretPhysical.y <= m_sizeTextArea.cy)
	{
		// �J���b�g��\��
		DisplayCaret();

		// �J���b�g���ړ�
		POINT pos_display;
		CalcDisplayPos(m_ptCaretPhysical, pos_display);
		::SetCaretPos(pos_display.x, pos_display.y);

		// [block]
		// IME�̃J���b�g���W��ݒ�
		{
			wgc::imm immcontext(m_hWnd);
			immcontext.SetPos(pos_display);
		}
	}
	else
	{
		// �J���b�g���\��
		DisplayCaret(FALSE);
	}

	// �r����`��
	if(IsCursorUnderline() && ::GetFocus() == m_hWnd)
	{
		wgc::devicecontext_client dc(m_hWnd);
		PaintUnderline(dc, m_ptCaretPhysicalPrev, FALSE);
		PaintUnderline(dc, m_ptCaretPhysical    , TRUE);
	}

	// �J���b�g�̈ʒu���X�V
	m_ptCaretPhysicalPrev = m_ptCaretPhysical;
}


////////////////////////////////////////////////////////////
// �L�[�{�[�h���[�`��

// [Page Up]
void edit::KeyPageUp(void)
{
	ScrollVert(-m_sizeTextArea.cy, TRUE, TRUE);
	EnsureVisible();
}

// [Page Down]
void edit::KeyPageDown(void)
{
	ScrollVert(m_sizeTextArea.cy, TRUE, TRUE);
	EnsureVisible();
}

// [Home]
void edit::KeyHome(void)
{
	// �R���g���[���L�[��������Ă���ꍇ
	if(is_keydown_control())
	{
		// �J�[�\�����o�b�t�@�̐擪�Ɉړ�
		m_manager.set_position(1, false, true);
		m_ptCaretLogical.y = 0;
	}

	// �J�[�\������ʍ��[�Ɉړ�
	m_ptCaretLogical.x = 0;

	LogicalToPhysical();
	EnsureVisible();
}

// [End]
void edit::KeyEnd(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();

	// �R���g���[���L�[��������Ă���ꍇ
	if(is_keydown_control())
	{
		// �o�b�t�@�̖����ɃJ�[�\�����ړ�
		m_manager.set_position(0, false, true);
		pos_now = m_manager.get_iterator_now();
		m_ptCaretLogical.y = m_manager.get_line_count() - 1;
	}

	// �J�[�\������ʉE�[�Ɉړ�
	m_ptCaretLogical.x = pos_now->length();

	LogicalToPhysical();
	EnsureVisible();
}

// [��]
void edit::KeyLeft(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();

	// �f�f
	const long length = pos_now->length();
	assert(m_ptCaretLogical.x <= length);

	// �s���łȂ��ꍇ
	if(m_ptCaretLogical.x > 0)
	{
		LPCWSTR wstr = pos_now->c_str();

		// [Ctrl]��������Ă���ꍇ
		if(is_keydown_control())
		{
			// �P��̋�؂�܂Ŕ�΂�
			const charcode_categorize_t char_type = charcode_categorize(wstr[--m_ptCaretLogical.x]);
			while(m_ptCaretLogical.x > 0)
			{
				if(charcode_categorize(wstr[m_ptCaretLogical.x - 1]) != char_type)
				{
					break;
				}
				m_ptCaretLogical.x--;
			}
		}
		// ������Ă��Ȃ��ꍇ
		else
		{
			// 1���ֈړ�
			m_ptCaretLogical.x--;
		}
	}
	// �s���̏ꍇ
	else
	{
		// ���ݍs���擪�̏ꍇ
		if(m_manager.is_begin())
		{
			if(!EnsureVisible())
			{
				return;
			}
		}
		// �s���ŁA��̍s������ꍇ
		else
		{
			// ���݈ʒu���X�V
			m_manager.scroll_now(-1);
			pos_now--;
			m_ptCaretLogical.x = pos_now->length();
			m_ptCaretLogical.y--;
		}
	}

	LogicalToPhysical();
	EnsureVisible();
	UpdateClient(0);
}

// [��]
void edit::KeyRight(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();
	const long length = pos_now->length();

	// �s���łȂ��ꍇ
	if(m_ptCaretLogical.x < length)
	{
		LPCWSTR wstr = pos_now->c_str();

		// [Ctrl]��������Ă���ꍇ
		if(is_keydown_control())
		{
			// �P��̋�؂�܂Ŕ�΂�
			const charcode_categorize_t char_type = charcode_categorize(wstr[m_ptCaretLogical.x]);
			while(m_ptCaretLogical.x < length)
			{
				m_ptCaretLogical.x++;
				if(charcode_categorize(wstr[m_ptCaretLogical.x]) != char_type)
				{
					break;
				}
			}
		}
		// ������Ă��Ȃ��ꍇ
		else
		{
			// 1�E�ֈړ�
			m_ptCaretLogical.x++;
		}
	}
	// �s���̏ꍇ
	else
	{
		// ���ݍs�������̏ꍇ
		if(m_manager.is_end())
		{
			m_nInternalX = 0;
			if(!EnsureVisible())
			{
				return;
			}
		}
		// �s���ŁA���̍s������ꍇ
		else
		{
			// ���݈ʒu���X�V
			m_manager.scroll_now(1);
			pos_now++;
			m_ptCaretLogical.x = 0;
			m_ptCaretLogical.y++;
		}
	}

	LogicalToPhysical();
	EnsureVisible();
	UpdateClient(0);
}

// [��]
void edit::KeyUp(void)
{
	// �_���X�N���[���p�p�����[�^
	BOOL bPhysical = FALSE;
	BOOL bLogical  = TRUE;
	BOOL bByScroll = TRUE;

	// [Ctrl]��������Ă���ꍇ
	if(is_keydown_control())
	{
		// �����X�N���[���̂ݍs��
		bPhysical = TRUE;
		bLogical  = FALSE;
		bByScroll = FALSE;
	}

	ScrollVert(-1, bPhysical, bLogical);
	EnsureVisible(bByScroll);
	UpdateClient(0);
}

// [��]
void edit::KeyDown(void)
{
	// �_���X�N���[���p�p�����[�^
	BOOL bPhysical = FALSE;
	BOOL bLogical  = TRUE;
	BOOL bByScroll = TRUE;

	// [Ctrl]��������Ă���ꍇ
	if(is_keydown_control())
	{
		// �����X�N���[���̂ݍs��
		bPhysical = TRUE;
		bLogical  = FALSE;
		bByScroll = FALSE;
	}

	ScrollVert(1, bPhysical, bLogical);
	EnsureVisible(bByScroll);
	UpdateClient(0);
}

// [Enter]
int edit::KeyReturn(void)
{
	const int linenumber_modify = m_manager.get_linenumber_now();

	InsertLineFeed();

	LogicalToPhysical();
	EnsureVisible();

	// �ύX�ӏ����ȑO�J�[�\�����������s
	return linenumber_modify;
}

// [Backspace]
BOOL edit::KeyBack(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();

	// �f�f
	const long length = pos_now->length();
	assert(m_ptCaretLogical.x <= length);

	// �s���̏ꍇ
	if(m_ptCaretLogical.x == 0)
	{
		// �o�b�t�@�̐擪�Ȃ牽�����Ȃ�
		if(m_manager.is_begin())
		{
			EnsureVisible();
			return FALSE;
		}

		// �_�����W���X�V
		pos_now--;
		m_ptCaretLogical.x = pos_now->length();
		m_ptCaretLogical.y--;

		// ���ݍs��1��̍s��A��
		m_manager.combine_backword();
		_SyncLineCount();
	}
	// �s���łȂ��ꍇ
	else
	{
		// ���݈ʒu�̍��̕������폜
		m_ptCaretLogical.x--;
		m_manager.delete_string(m_ptCaretLogical.x, 1);
	}
	LogicalToPhysical();
	EnsureVisible();

	// �ύX�ӏ������݃J�[�\���̂���s
	return m_manager.get_linenumber_now();
}

// [Delete]
BOOL edit::KeyDelete(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();
	const long length = pos_now->length();

	// �s���̏ꍇ
	if(m_ptCaretLogical.x >= length)
	{
		m_ptCaretLogical.x = length;

		// �o�b�t�@�̖����Ȃ牽�����Ȃ�
		if(m_manager.is_end())
		{
			EnsureVisible();
			return 0;
		}

		// ���ݍs��1���̍s��A��
		m_manager.combine_forward();
		_SyncLineCount();
	}
	// �s���łȂ��ꍇ
	else
	{
		// ���݈ʒu�̕������폜
		m_manager.delete_string(m_ptCaretLogical.x, 1);
	}
	LogicalToPhysical();
	EnsureVisible();

	// �ύX�ӏ������݃J�[�\���̂���s
	return m_manager.get_linenumber_now();
}

// ������̓���
void edit::KeyString(const wstring_t &wstr)
{
	if(IsReadOnly())
	{
		return;
	}

	const int linenumber_modify = m_manager.get_linenumber_now();

	InsertString(wstr);

	EnsureVisible();
	UpdateClient(linenumber_modify);
	MoveCaret();
}


// ���݈ʒu�Ƀf�[�^��}��
void edit::InsertData(const wchar_t *wstr)
{
	const int linenumber_modify = m_manager.get_linenumber_now();

	int x = m_ptCaretLogical.x;
	const int lines = m_manager.insert_data(x, wstr);

	m_ptCaretLogical.x  = x;
	m_ptCaretLogical.y += lines;

	if(lines > 0)
	{
		_SyncLineCount();
	}

	LogicalToPhysical();
	EnsureVisible();
	UpdateClient(linenumber_modify);
	MoveCaret();
}


// ���݈ʒu�ɕ������}���i���s�Ȃ��j
void edit::InsertString(const wstring_t &wstr)
{
	m_manager.insert_string(m_ptCaretLogical.x, wstr);

	m_ptCaretLogical.x += wstr.length();

	LogicalToPhysical();
}

// ���݈ʒu�ɉ��s��}��
void edit::InsertLineFeed(void)
{
	const_iterator_t pos_now = m_manager.get_iterator_now();

	// �f�f
	const long length = pos_now->length();
	assert(m_ptCaretLogical.x <= length);

	// ���ݍs�����݈ʒu�ŕ���
	const int pos = m_ptCaretLogical.x;
	m_manager.insert_linefeed(pos);

	// �_�����W���X�V
	m_ptCaretLogical.x = 0;
	m_ptCaretLogical.y++;
	m_nInternalX = 0;

	_SyncLineCount();
}


////////////////////////////////////////////////////////////
// �`�惋�[�`��

// �N���C�A���g�̈��`��
void edit::PaintClient(
	wgc::devicecontext &dc,
	const int linenumber_redraw /* = 1 */,
	const BOOL bLinenumber      /* = TRUE */,
	const BOOL bRuler           /* = TRUE */) const
{
	// �J�[�\�����\��
	HideCaret();
	{
		// �e�L�X�g��`��
		if(linenumber_redraw > 0)
		{
			PaintText(dc, linenumber_redraw);
		}

		// �s�ԍ���`��
		if(bLinenumber && IsLinenumberVisible())
		{
			PaintLineNumber(dc);
		}

		// ���[���[��`��
		if(bRuler && IsRulerVisible())
		{
			PaintRuler(dc);
		}
	}
	// �J�[�\���𕜌�
	ShowCaret();
}


// �e�L�X�g�̈��`��
void edit::PaintText(wgc::devicecontext &dc, const int linenumber_redraw) const
{
	const_iterator_t p = m_manager.get_iterator_top();
	assert(p != m_manager.get_iterator_end());
	assert(linenumber_redraw > 0);

	// [block]
	// �N���b�s���O�̈��ݒ�
	{
		RECT rect;
		CalcRectText(rect, FALSE);
		dc.SetClipRect(rect);
	}

	dc.SetBkColor(m_infoText.colorBack);
	HGDIOBJ hOldObject = dc.SelectObject(GetFont());
	{
		// �\���ʒu
		POINT pos = {0, 0};

		// [block]
		// �ύX�̕s�v�Ș_���s�����v�Z
		{
			const int diff = linenumber_redraw - m_manager.get_linenumber_top();
			if(diff > m_sizeTextArea.cy)
			{
				// �`��s�v
				goto end;
			}
			for(int i = 0; i < diff; i++)
			{
				p++;
				pos.y += 1;
				assert(p != m_manager.get_iterator_end());
			}
		}

		// �_���s�P�ʂ̃��[�v
		while(p != m_manager.get_iterator_end())
		{
			// �\���ʒu���v�Z
			pos.x = -m_nColumnLeft * m_sizeFont.cx;

			// [block]
			// �{���͂����ɐ܂�Ԃ������̂��߂ɕ����s�P�ʂ̃��[�v������
			{
				pos.x += DrawString(
					dc,
					pos,
					p->substr(0, m_nColumnLeft + (m_sizeTextArea.cx / m_sizeFont.cx) + 1),
					m_infoText.color, m_infoText.colorBack);

				// �s���ɉ��s�}�[�N��ǉ�
				PaintMarkLineFeed(dc, pos);

				if(pos.y > m_sizeTextArea.cy)
				{
					// �ŉ��s�܂ŕ`�抮��
					goto end;
				}
				pos.y++;
			}
			p++;
		}

		// �s����EOF�ŏ㏑��
		pos.y--;
		PaintMarkEOF(dc, pos);

		// [block]
		// �ȍ~�̃e�L�X�g�̈��w�i�F�œh��Ԃ�
		{
			RECT rect;
			CalcRectText(rect, FALSE);
			rect.top += (pos.y + 1) * (m_sizeFont.cy + m_infoText.nLineSpace);

			dc.FillSolidRect(rect, m_infoText.colorBack);
		}
	}
end:
	dc.SelectObject(hOldObject);
}

// �e�L�X�g�̈�̔w�i��`��
void edit::PaintTextBack(wgc::devicecontext &dc) const
{
	RECT rect;
	CalcRectText(rect);

	dc.FillSolidRect(rect, m_infoText.colorBack);
}


// �s�ԍ���`��
void edit::PaintLineNumber(wgc::devicecontext &dc) const
{
	assert(IsLinenumberVisible());

	dc.ClearClip();

	dc.SetTextColor(m_infoLineNumber.color);
	dc.SetBkColor  (m_infoLineNumber.colorBack);
	HGDIOBJ hOldObject = dc.SelectObject(GetFont());
	{
		const int line_stride = m_sizeFont.cy + m_infoText.nLineSpace;
		const int line_count = m_manager.get_line_count();

		RECT rect;
		CalcRectLineNumber(rect);
		rect.bottom = rect.top + line_stride;
		rect.right -= m_infoLineNumber.nMargin / 2;

		int linenumber = m_manager.get_linenumber_top();
		for(int i = 0; i <= m_sizeTextArea.cy; i++)
		{
			// �s�ԍ��̕�������擾
			// �iwsprintf() ���g�������������ǁA%*d ���Ďw��ł��Ȃ��̂ˁc�j
			TCHAR line_str[128];
			_stprintf(
				line_str,
				_T("%*d"),
				m_infoLineNumber.nCols, linenumber);

			// �\��
			dc.ExtTextOut(
				rect.left, rect.top,
				ETO_OPAQUE | ETO_CLIPPED,
				rect,
				line_str);

			rect.top    += line_stride;
			rect.bottom += line_stride;

			if(linenumber++ == line_count)
			{
				// �ȍ~�̍s�ԍ��̈��w�i�F�œh��Ԃ�
				rect.bottom += (m_sizeTextArea.cy - i - 1) * line_stride;
				dc.FillSolidRect(rect, m_infoLineNumber.colorBack);
				break;
			}
		}
	}
	dc.SelectObject(hOldObject);
}

// �s�ԍ��̔w�i��`��
void edit::PaintLineNumberBack(wgc::devicecontext &dc) const
{
	assert(IsLinenumberVisible());

	RECT rect;
	CalcRectLineNumber(rect);

	// �w�i�F�œh��Ԃ�
	dc.FillSolidRect(rect, m_infoLineNumber.colorBack);

	// �c��������
	HPEN    hPen       = ::CreatePen(PS_SOLID, 1, m_infoLineNumber.color);
	HGDIOBJ hOldObject = dc.SelectObject(hPen);
	{
		const int x = rect.right - (m_infoLineNumber.nMargin / 2);
		dc.MoveTo(x, rect.top);
		dc.LineTo(x, rect.bottom);
	}
	dc.SelectObject(hOldObject);
	::DeleteObject(hPen);
}


// ���[���[��`��
void edit::PaintRuler(wgc::devicecontext &dc) const
{
	assert(IsRulerVisible());

	dc.ClearClip();

	PaintRulerBack(dc);

	HPEN    hPen       = ::CreatePen(PS_SOLID, 1, m_infoRuler.color);
	HGDIOBJ hOldObject = dc.SelectObject(hPen);
	{
		RECT rect;
		CalcRectRuler(rect);

		// [block]
		// ����������
		{
			const int x1 = rect.left + CalcMarginLeft();
			const int x2 = rect.right;
			const int y  = m_infoRuler.nHeight - 1;

			dc.MoveTo(x1, y);
			dc.LineTo(x2, y);
		}

		POINT pos = {0, 0};
		CalcDisplayPos(pos, pos);
		int column = m_nColumnLeft;
		for(int pos_x = pos.x; pos_x < rect.right; pos_x += m_sizeFont.cx)
		{
			// ���̍���������
			int height = m_infoRuler.nHeight / 2;
			if((column % (m_infoRuler.nStride / 2)) == 0) { height = m_infoRuler.nHeight * 3 / 4; }
			if((column %  m_infoRuler.nStride     ) == 0) { height = m_infoRuler.nHeight; }

			// ��������
			dc.MoveTo(pos_x, m_infoRuler.nHeight - 1);
			dc.LineTo(pos_x, m_infoRuler.nHeight - height);

			column++;
		}
	}
	dc.SelectObject(hOldObject);
	::DeleteObject(hPen);
}

// ���[���[�̔w�i��`��
void edit::PaintRulerBack(wgc::devicecontext &dc) const
{
	assert(IsRulerVisible());

	RECT rect;
	CalcRectRuler(rect);

	dc.FillSolidRect(rect, m_infoRuler.colorBack);
}


// �w��ʒu�ɃA���_�[���C����`��
void edit::PaintUnderline(wgc::devicecontext &dc, const POINT &pos, const BOOL bDraw /* = TRUE */) const
{
	// �e�L�X�g�̈�O�Ȃ牽�����Ȃ�
	if(pos.y < 0 || pos.y >= m_sizeTextArea.cy)
	{
		return;
	}

	// �r����\������ʒu���v�Z
	POINT pos_display;
	CalcDisplayPos(pos, pos_display);
	pos_display.y += m_sizeFont.cy;

	// �r����\�������`���v�Z
	RECT rect;
	CalcRectText(rect);

	const COLORREF color = bDraw ? m_infoText.colorLine : m_infoText.colorBack;
	HPEN    hPen       = ::CreatePen(PS_SOLID, 1, color);
	HGDIOBJ hOldObject = dc.SelectObject(hPen);
	{
		// �r����`��
		dc.MoveTo(rect.left , pos_display.y);
		dc.LineTo(rect.right, pos_display.y);
	}
	dc.SelectObject(hOldObject);
	::DeleteObject(hPen);
}


int edit::DrawString(wgc::devicecontext &dc, const POINT &pos, const wstring_t &wstr, const COLORREF colorText, const COLORREF colorBack) const
{
	const wchar_t special[] =
	{
		CODE_TAB,
		CODE_WIDESPACE,
		CODE_NUL
	};

	wstring_t substring = wstr;
	int x = 0;
	for(;;)
	{
		dc.SetTextColor(colorText);
		dc.SetBkColor(colorBack);

		// ���ꕶ��������
		wstring_t::size_type found_pos = substring.find_first_of(special);

		// [block]
		// ���ꕶ���̑O�܂ŕ`��
		{
			const wstring_t tmp = substring.substr(0, found_pos);
			const POINT pos_draw = {pos.x + x, pos.y};
			PaintString(dc, pos_draw, tmp.c_str(), tmp.length());
			x += CalcPhysicalLength(tmp);
		}

		// ���ꕶ����������Ȃ���΃��[�v�𔲂���
		if(found_pos == wstring_t::npos)
		{
			break;
		}

		const wchar_t code = substring[found_pos];
		switch(code)
		{
		case CODE_TAB:
			x += PaintMarkTab(dc, pos, x);
			break;

		case CODE_WIDESPACE:
			x += PaintMarkWideSpace(dc, pos, x);
			break;
		}

		substring = substring.substr(found_pos + 1);
	}

	// �ړ������������i�J���b�g�P�ʁj��Ԃ�
	return x;
}

// �������`��
void edit::PaintString(wgc::devicecontext &dc, const POINT &pos, LPCWSTR wstr, const int length) const
{
	POINT pos_display;
	CalcDisplayPos(pos, pos_display);

	dc.TextOutW(pos_display.x, pos_display.y, wstr, length);
}

// ���s�}�[�N��`��
void edit::PaintMarkLineFeed(wgc::devicecontext &dc, const POINT &pos) const
{
	dc.SetTextColor(m_infoText.colorMark);

	const int fill_length = m_sizeTextArea.cx - pos.x + 2;
	if(fill_length > 0)
	{
		// ����������`��
		wstring_t linefeed(fill_length, CODE_SPACE);
		linefeed[0] = MARK_LF;

		PaintString(dc, pos, linefeed.c_str(), linefeed.length());
	}
}

// EOF�}�[�N��`��
void edit::PaintMarkEOF(wgc::devicecontext &dc, const POINT &pos) const
{
	dc.SetTextColor(m_infoText.colorMark);
	PaintString(dc, pos, MARK_EOF, MARK_EOF_LEN);
}

// �^�u�}�[�N��`��
int edit::PaintMarkTab(wgc::devicecontext &dc, const POINT &pos_base, const int x) const
{
	dc.SetTextColor(m_infoText.colorMark);

	// �E��������`��
	const int length = CalcTabLength(x);
	wstring_t tab(length, CODE_SPACE);
	tab[0] = MARK_TAB;

	const POINT pos = {pos_base.x + x, pos_base.y};
	PaintString(dc, pos, tab.c_str(), tab.length());

	return length;
}

// �S�p�󔒂�`��
int edit::PaintMarkWideSpace(wgc::devicecontext &dc, const POINT &pos_base, const int x) const
{
	dc.SetTextColor(m_infoText.colorMark);

	const POINT pos = {pos_base.x + x, pos_base.y};
	PaintString(dc, pos, MARK_WIDESPACE, MARK_WIDESPACE_LEN);

	return CalcCodeLength(CODE_WIDESPACE);
}


////////////////////////////////////////////////////////////
// �E�C���h�E���b�Z�[�W�֘A

// �e�E�C���h�E�ɒʒm���b�Z�[�W�𑗐M
LRESULT edit::SendNotifyCommandToParent(const WORD wNotify)
{
	HWND hWndParent = GetParent();
	WORD wCtrlID    = GetDlgCtrlID(m_hWnd);

	return ::SendMessage(
		hWndParent,
		WM_COMMAND,
		GET_WM_COMMAND_MPS(wCtrlID, m_hWnd, wNotify));
}

// �E�C���h�E�v���V�[�W��
LRESULT edit::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case EM_GETMODIFY: return OnEmGetModify();
	case EM_SETMODIFY: OnEmSetModify(wParam); return 0;

	case EM_SETREADONLY: return OnEmSetReadOnly(wParam);

	case EM_GETMARGINS: return OnEmGetMargins();
	case EM_SETMARGINS: OnEmSetMargins(wParam, LOWORD(lParam), HIWORD(lParam)); return 0;

	case EM_GETLINECOUNT: return OnEmGetLineCount();

	case WM_UNDO:    // no break
	case EM_UNDO:    return OnEmUndo();
	case EM_CANUNDO: return OnEmCanUndo();

	case EM_REDO:    return OnEmRedo();
	case EM_CANREDO: return OnEmCanRedo();

	case WM_CUT:      OnCut  (); return 0;
	case WM_COPY:     OnCopy (); return 0;
	case WM_CLEAR:    OnClear(); return 0;
	case WM_PASTE:    OnPaste(); return 0;
	case EM_CANPASTE: return OnEmCanPaste(wParam);
	}

	// ���N���X�̃v���V�[�W�����Ăяo��
	return window::WindowProc(uMsg, wParam, lParam);
}


// �E�C���h�E�쐬
int edit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(window::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// [block]
	// �t�H���g�̐ݒ�
	{
		HFONT hFont = (m_hFont != NULL) ? m_hFont : DEFAULT_FONT;
		SetFont(hFont, FALSE);
	}

	// [block]
	// �X�N���[���o�[�̐ݒ�
	{
		SyncHScroll();
		SyncVScroll();
	}

	return 0;
}

UINT edit::OnGetDlgCode(LPMSG /* lpMsg */)
{
	return DLGC_WANTARROWS | DLGC_WANTCHARS;
}


// �T�C�Y�ύX
void edit::OnSize(const UINT nType, const int cx, const int cy)
{
	window::OnSize(nType, cx, cy);
	UpdateTextAreaSize();
}


// �t�H�[�J�X�̐ݒ�
void edit::OnSetFocus(HWND /* hWndLoseFocus */)
{
	// �J���b�g���ړ�
	CreateSolidCaret(2, m_sizeFont.cy);
	MoveCaret();

	// [block]
	// IME�̃t�H���g��ύX
	{
		wgc::imm immcontext(m_hWnd);
		immcontext.SetCompositionFont(GetFont());
	}
}

void edit::OnKillFocus(HWND /* hWndGetFocus */)
{
	// �J���b�g��j��
	DisplayCaret(FALSE);
	::DestroyCaret();

	// [block]
	// �r��������
	{
		wgc::devicecontext_client dc(m_hWnd);
		PaintUnderline(dc, m_ptCaretPhysicalPrev, FALSE);
	}
}


// �t�H���g�̐ݒ�
void edit::OnSetFont(HFONT hFont, const BOOL bRedraw)
{
	// �Â��t�H���g�̔j���`���͌Ăяo����
	m_hFont = hFont;

	// [block]
	{
		HDC hDC = GetDC(m_hWnd);
		HGDIOBJ hOldObject = ::SelectObject(hDC, m_hFont);

		// �t�H���g�̕����擾
		::GetCharWidthW(
			hDC,
			WCHAR_MIN, WCHAR_MAX,
			m_nCharWidth);

		// [block]
		// �t�H���g�T�C�Y���擾
		{
			TEXTMETRIC tm;
			::GetTextMetrics(hDC, &tm);

			_SetFontSize(tm.tmAveCharWidth, tm.tmHeight);
		}

		::SelectObject(hDC, hOldObject);
		ReleaseDC(m_hWnd, hDC);
	}

	// �ĕ`��
	if(bRedraw)
	{
		InvalidateRect();
	}
}

HFONT edit::OnGetFont(void)
{
	return m_hFont;
}


// �����X�N���[��
void edit::OnHScroll(const UINT /* nSBCode */, const UINT /* nPos */, HWND /* hWndScrollBar */)
{
}

// �����X�N���[��
void edit::OnVScroll(const UINT nSBCode, const UINT /* nPos */, HWND /* hWndScrollBar */)
{
	int dy = 0;                             // �X�N���[����

	// �X�N���[���ʂ�����
	switch(nSBCode)
	{
	case SB_TOP:    dy = 1 - m_nLineNumberTopPrev; break;
	case SB_BOTTOM: dy = m_manager.get_line_count() - m_nLineNumberTopPrev; break;

	case SB_LINEUP:   dy = -1; break;
	case SB_LINEDOWN: dy = +1; break;

	case SB_PAGEUP:   dy = -m_sizeTextArea.cy; break;
	case SB_PAGEDOWN: dy = +m_sizeTextArea.cy; break;

	case SB_THUMBTRACK:
		{
			// nPos �� 16bit �Ȃ̂ŁA����𒴂���l�� GetScrollInfo() ���g���Ď擾
			SCROLLINFO si = {sizeof(si), SIF_POS | SIF_TRACKPOS};
			GetVScrollInfo(si);
			dy = si.nTrackPos - si.nPos;
		}
		break;
	}

	// �X�N���[��
	ScrollVert(dy, TRUE, FALSE);
	UpdateClient(0);
	MoveCaret();
}


// �J�[�\���`��̕ύX
BOOL edit::OnSetCursor(const HWND hWnd, const UINT nHitTest, const UINT message)
{
	// �N���C�A���g�̈���ɂ���ꍇ
	if(nHitTest == HTCLIENT)
	{
		// �J�[�\���̃N���C�A���g���W���擾
		POINT point;
		::GetCursorPos(&point);
		::ScreenToClient(hWnd, &point);

		switch(ClientHitTest(point))
		{
		case CHT_MARGIN:
			::SetCursor(m_hCursorMargin);
			return TRUE;
		}
	}

	return window::OnSetCursor(hWnd, nHitTest, message);
}


////////////////////////////////////////////////////////////
// �}�E�X���b�Z�[�W

BOOL edit::OnMouseWheel(const UINT /* nFlags */, const short zDelta, const POINT & /* point */)
{
	const int scroll = -zDelta / WHEEL_DELTA * 3;

	// �����X�N���[���̂ݍs��
	ScrollVert(scroll, TRUE, FALSE);

	UpdateClient(0);
	MoveCaret();
	return TRUE;
}

void edit::OnLButtonDown(const UINT /* nFlags */, const POINT &point)
{
	SetFocus();

	// �}�E�X���W����J���b�g�̕������W���v�Z
	CalcCaretPos(point, m_ptCaretPhysical);

	// �������W����_�����W���v�Z
	PhysicalToLogical();

	AdjustPosition();

	EnsureVisible();
	UpdateClient(0);
	MoveCaret();
}

void edit::OnRButtonDown(const UINT /* nFlags */, const POINT &point)
{
	SetFocus();

	// �}�E�X���W����J���b�g�̕������W���v�Z
	CalcCaretPos(point, m_ptCaretPhysical);

	// �������W����_�����W���v�Z
	PhysicalToLogical();

	AdjustPosition();

	EnsureVisible();
	UpdateClient(0);
	MoveCaret();
}

void edit::OnMButtonDown(const UINT /* nFlags */, const POINT & /* point */)
{
	SetFocus();
}


////////////////////////////////////////////////////////////
// �L�[�{�[�h���b�Z�[�W

// �L�[�{�[�h����
void edit::OnKeyDown(const UINT nChar, const UINT nRepeatCount, const UINT nFlags)
{
	int linenumber_modify = 0;

	switch(nChar)
	{
	case VK_PRIOR: KeyPageUp();   break;
	case VK_NEXT:  KeyPageDown(); break;
	case VK_HOME:  KeyHome();     break;
	case VK_END:   KeyEnd();      break;

	case VK_LEFT:  KeyLeft();  goto caret;
	case VK_RIGHT: KeyRight(); goto caret;
	case VK_UP:    KeyUp();    goto caret;
	case VK_DOWN:  KeyDown();  goto caret;

	case VK_RETURN: if(!IsReadOnly()) { linenumber_modify = KeyReturn(); } break;
	case VK_BACK:   if(!IsReadOnly()) { linenumber_modify = KeyBack();   } break;
	case VK_DELETE: if(!IsReadOnly()) { linenumber_modify = KeyDelete(); } break;
/*
	case VK_TAB:                                // VC++ 7 �ł� WM_CHAR �ɂ��͂��H
		KeyString(wstring_t(nRepeatCount, CODE_TAB));
		return;
*/
	default:
		window::OnKeyDown(nChar, nRepeatCount, nFlags);
		return;
	}

	UpdateClient(linenumber_modify);

caret:
	MoveCaret();
}

// ��������
void edit::OnChar(const UINT nChar, const UINT nRepeatCount, const UINT nFlags)
{
	const wchar_t code = nChar;
	if(iswprint(code))
	{
		KeyString(wstring_t(nRepeatCount, code));
	}

	window::OnChar(nChar, nRepeatCount, nFlags);
}

// IME����̕�������
void edit::OnImeComposition(const WORD chDBCS, const UINT nFlags)
{
	if((nFlags & GCS_RESULTSTR) != GCS_RESULTSTR)
	{
		window::OnImeComposition(chDBCS, nFlags);
		return;
	}

	wgc::imm immcontext(m_hWnd);
	wgc::wstring_t wstr;

	// UNICODE�`���ŕϊ���̕�������擾�iWindows 98�ȍ~�j
	const LONG result = immcontext.GetCompositionStringResultStrW(wstr);

#ifndef UNICODE
	if(result == IMM_ERROR_GENERAL)
	{
		// Windows 95�ł�UNICODE�`���Ŏ擾�ł��Ȃ��̂ŁAAPI���g���ĕϊ�
		wgc::string_t tstr;
		immcontext.GetCompositionStringResultStr(tstr);

		wgc::wgfMultiByteToWideChar(tstr, wstr);
	}
#endif

	KeyString(wstr);
}


// ��ʕ`��
void edit::OnPaint(void)
{
	wgc::devicecontext_paint dc(m_hWnd);
	PaintClient(dc);
}

// �w�i����
BOOL edit::OnEraseBkgnd(HDC hDC)
{
	wgc::devicecontext dc = hDC;

	PaintTextBack(dc);
	if(IsLinenumberVisible())
	{
		PaintLineNumberBack(dc);
	}
	if(IsRulerVisible())
	{
		PaintRulerBack(dc);
	}

	// �r����`��
	if(IsCursorUnderline() && ::GetFocus() == m_hWnd)
	{
		PaintUnderline(dc, m_ptCaretPhysicalPrev, FALSE);
		PaintUnderline(dc, m_ptCaretPhysical    , TRUE);
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
// �ҏW���b�Z�[�W�n���h��

// �_�[�e�B�t���O���擾(EM_GETMODIFY)
BOOL edit::OnEmGetModify(void) const
{
	return FALSE;
}

// �_�[�e�B�t���O��ݒ�(EM_SETMODIFY)
void edit::OnEmSetModify(const BOOL /* bModified */)
{
}

// �ǂݎ���p������t��/�����iEM_SETREADONLY�j
BOOL edit::OnEmSetReadOnly(const BOOL bReadOnly)
{
	if(bReadOnly)
	{
		// �ǂݎ���p������t��
		ModifyStyle(0, ES_READONLY, FALSE);
	}
	else
	{
		// �ǂݎ���p����������
		ModifyStyle(ES_READONLY, 0, FALSE);
	}

	// �e�E�C���h�E�� XEN_READONLYCHANGED �ʒm���b�Z�[�W�𑗂�
	SendNotifyCommandToParent(XEN_READONLYCHANGED);

	// WM_KEYDOWN, WM_PASTE, KeyString() ��3�ӏ��Ń`�F�b�N���s��
	return TRUE;
}

// ���E�̗]����ݒ�iEM_SETMARGINS�j
void edit::OnEmSetMargins(const WORD fwMargin, const WORD wLeft, const WORD wRight)
{
	// �t�H���g�̕��ɂ���ė]��������
	if(fwMargin == EC_USEFONTINFO)
	{
		m_nTextMarginLeft  = CalcCodeLength(L'C');
		m_nTextMarginRight = CalcCodeLength(L'A');
	}
	else
	{
		if(fwMargin & EC_LEFTMARGIN ) { m_nTextMarginLeft  = wLeft;  }
		if(fwMargin & EC_RIGHTMARGIN) { m_nTextMarginRight = wRight; }
	}
	UpdateTextAreaSize();
}

// ���E�̗]�����擾�iEM_GETMARGINS�j
DWORD edit::OnEmGetMargins(void) const
{
	return MAKELONG(m_nTextMarginLeft, m_nTextMarginRight);
}

// �o�b�t�@�̍s�����擾�iEM_GETLINECOUNT�j
int edit::OnEmGetLineCount(void) const
{
	return m_manager.get_line_count();
}

// �A���h�D(WM_UNDO, EM_UNDO)
BOOL edit::OnEmUndo(void)
{
	return FALSE;
}

// �A���h�D�ł��邩�H(EM_CANUNDO)
BOOL edit::OnEmCanUndo(void) const
{
	return FALSE;
}

// ���h�D(EM_REDO)
BOOL edit::OnEmRedo(void)
{
	return FALSE;
}

// ���h�D�ł��邩�H(EM_CANREDO)
BOOL edit::OnEmCanRedo(void) const
{
	return FALSE;
}

// �J�b�g(WM_CUT)
void edit::OnCut(void)
{
	// �I��͈͂�؂���A�N���b�v�{�[�h�ɃR�s�[
	Copy();
	Clear();
}

// �I��͈͂��N���b�v�{�[�h�ɃR�s�[(WM_COPY)
void edit::OnCopy(void)
{
}

// �I��͈͂��폜(WM_CLEAR)
void edit::OnClear(void)
{
}

// �N���b�v�{�[�h�̃f�[�^���y�[�X�g(WM_PASTE)
void edit::OnPaste(void)
{
	if(IsReadOnly() || !::OpenClipboard(m_hWnd))
	{
		return;
	}

	// UNICODE�`���Ńf�[�^���擾
	HANDLE hData = ::GetClipboardData(CF_UNICODETEXT);
	if(hData != NULL)
	{
		// �f�[�^���擾
		LPVOID lpData = ::GlobalLock(hData);
		InsertData(reinterpret_cast<LPCWSTR>(lpData));
		::GlobalUnlock(hData);
	}

	::CloseClipboard();
}

// �w��t�H�[�}�b�g���y�[�X�g�ł��邩�H(EM_CANPASTE)
BOOL edit::OnEmCanPaste(const UINT uFormat) const
{
	// uFormat ��0�ȊO�̂Ƃ��́AuFormat �𒲂ׂ�
	if(uFormat != 0)
	{
		// CF_TEXT �`���� CF_UNICODETEXT �`���Ȃ�y�[�X�g�\
		return (uFormat == CF_TEXT) || (uFormat == CF_UNICODETEXT);
	}
	// uFormat ��0�̂Ƃ��́A�N���b�v�{�[�h�̃f�[�^�𒲂ׂ�
	else
	{
		// NT�nOS�ł́A�O���� CF_TEXT �����Ŕ���\�B
		// 9x�nOS�ł��ʏ�� CF_TEXT �����ł悢���A
		// CF_UNICODETEXT �`���Ŋi�[����A�v�������邩������Ȃ��̂�
		// ���̏ꍇ�͌㔼�Ŕ��肵�Ă���B
		return ::IsClipboardFormatAvailable(CF_TEXT) || ::IsClipboardFormatAvailable(CF_UNICODETEXT);
	}
}


////////////////////////////////////////////////////////////////////////////////
// �����o�ϐ��̐ݒ�
// �����o�ϐ��̒��ɂ́A�ύX����Ƃ��ɑ��̃I�u�W�F�N�g�Ɠ������������
// �e�E�C���h�E�֒ʒm����K�v�̂�����̂�����B
// ���̂悤�ȕϐ���ύX����Ƃ��́A���ڕύX����̂ł͂Ȃ��ύX�p�̃����o�֐������K�v������B
// ���́u�ύX�p�̃����o�֐��v�������B
// �������G�Ȃ��͎̂��́u�����n���h���v�œ��������B

// �t�H���g�T�C�Y�̕ύX
void edit::_SetFontSize(const int cx, const int cy)
{
	m_sizeFont.cx = cx;
	m_sizeFont.cy = cy;

	UpdateTextAreaSize();
}

// �e�L�X�g�̈�̃T�C�Y�̕ύX
void edit::_SetTextAreaSize(const int cx, const int cy)
{
	m_sizeTextArea.cx = cx;
	m_sizeTextArea.cy = cy;

	SyncVScroll();
}

////////////////////////////////////////////////////////////////////////////////
// �����n���h��
// ��́u�����o�ϐ��̐ݒ�v���g�p�ł��Ȃ����G�ȃI�u�W�F�N�g�̓��������Ƃ��́A
// �ύX���ꂽ��ɂ��̓����n���h�����ĂԁB
// �ǂ��ŕύX����邩���킩��悤�ɁA�R�����g�Ƃ��ČĂяo�����������Ă������ƁB

// �s�����ύX���ꂽ�iSetData() / SetEmpty() / InsertLineFeed() / KeyDelete() / KeyBack()�j
void edit::_SyncLineCount(void)
{
	ExpandLineNumberCols();
	SyncVScroll();
}

// ��ʏ�[�̍s�ԍ����ύX���ꂽ�iScrollVert()�j
void edit::_SyncLineNumberTop(void)
{
	SyncVScroll();
}

_WGC_END                                // }
