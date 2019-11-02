// Window.cpp �c�C���v�������e�[�V�����t�@�C��
#include "Window.h"
#include "wgfunc.h"                     // wgc::wgfLoadString()

#include <windowsx.h>                   // GET_WM_COMMAND_ID()
#include <commctrl.h>                   // WM_MOUSELEAVE
#include <tchar.h>                      // TCHAR
#include <assert.h>                     // assert()


////////////////////////////////////////////////////////////////////////////////
// �}�N����`

////////////////////////////////////////////////////////////////////////////////
// ���b�Z�[�W�ifrom winuser.h�j
#define WM_MOUSEWHEEL     0x020A        // �}�E�X�z�C�[����]

#define WM_XBUTTONDOWN    0x020B        // 5�{�^���}�E�X�̃G�N�X�g���{�^������
#define WM_XBUTTONUP      0x020C        // 5�{�^���}�E�X�̃G�N�X�g���{�^���J��
#define WM_XBUTTONDBLCLK  0x020D        // 5�{�^���}�E�X�̃G�N�X�g���{�^���_�u���N���b�N
#define XBUTTON1       0x0001           // 5�{�^���}�E�X�̃G�N�X�g���{�^���i��j
#define XBUTTON2       0x0002           // 5�{�^���}�E�X�̃G�N�X�g���{�^���i���j


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// static�ϐ�
LONG  window::m_lRefCount = 0;                    // �Q�ƃJ�E���^
DWORD window::m_dwTlsIndex = TLS_OUT_OF_INDEXES;  // Thread Local Storage �̃C���f�b�N�X


////////////////////////////////////////////////////////////////////////////////
// �R���X�g���N�^
window::window(HWND hWnd /* = NULL */)
{
	// �����X���b�h����̎Q��
	::InterlockedIncrement(&m_lRefCount);
	if(m_lRefCount == 1)
	{
		assert(m_dwTlsIndex == TLS_OUT_OF_INDEXES);
		m_dwTlsIndex = ::TlsAlloc();
	}

	// Thread Local Storage �̍쐬
	thread_data_t *pThreadData = _GetThreadData();
	if(pThreadData == NULL)
	{
		// �X���b�h�����i�[
		pThreadData = new thread_data_t;
		pThreadData->ref_count  = 0;
		pThreadData->hHook      = NULL;
		pThreadData->window_ptr = NULL;
		_SetThreadData(pThreadData);

		// �t�b�N�v���V�[�W�����C���X�g�[��
		pThreadData->hHook = ::SetWindowsHookEx(
			WH_CBT,
			_CBTProc,
			NULL,
			::GetCurrentThreadId());;
	}
	pThreadData->ref_count++;

	// �����o�ϐ��̏�����
	m_hWnd           = hWnd;
	m_bOwner         = FALSE;
	m_lpfnOldWndProc = NULL;
}

// �f�X�g���N�^
window::~window(void)
{
	// ���L�����������܂܃f�X�g���N�^���Ăяo���ꂽ�ꍇ�A
	// �E�C���h�E���j�������iOnNcDestroy()���Ăяo�����j�O��
	// �E�C���h�E�I�u�W�F�N�g���j������邱�ƂɂȂ�B
	// ���̂悤�ȏ�Ԃ́A�X�^�b�N��� Create() ����ȂǁA
	// �I�u�W�F�N�g�̊Ǘ��~�X�ȊO�ɂ��肦�Ȃ��̂ŁA
	// �G���[��ߑ��ł���悤�ɃA�T�[�V������������
	assert(!m_bOwner);

	// �X���b�h�̎Q�ƃJ�E���^���f�N�������g
	thread_data_t *pThreadData = _GetThreadData();
	assert(pThreadData != NULL);
	pThreadData->ref_count--;
	if(pThreadData->ref_count == 0)
	{
		// �t�b�N�v���V�[�W�����A���C���X�g�[��
		::UnhookWindowsHookEx(pThreadData->hHook);

		// Thread Local Storage ���N���A
		_SetThreadData(NULL);
		assert(pThreadData->handlemap.empty());
		delete pThreadData;
		pThreadData = NULL;
	}

	// �Q�ƃJ�E���^���f�N�������g
	::InterlockedDecrement(&m_lRefCount);
	if(m_lRefCount == 0)
	{
		// Thread Local Storage ���J��
		::TlsFree(m_dwTlsIndex);
		m_dwTlsIndex = TLS_OUT_OF_INDEXES;
	}
}


window::operator HWND(void) const
{
	return m_hWnd;
}


////////////////////////////////////////////////////////////////////////////////
// �E�C���h�E���쐬
BOOL window::Create(
	LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
	const DWORD dwStyle, const DWORD dwExStyle,
	window *parent,
	HMENU     hMenu     /* = NULL */,
	HINSTANCE hInstance /* = NULL */,
	const int x       /* = CW_USEDEFAULT */,
	const int y       /* = CW_USEDEFAULT */,
	const int nWidth  /* = CW_USEDEFAULT */,
	const int nHeight /* = CW_USEDEFAULT */,
	LPVOID lpParam /* = NULL */)
{
	// PreCreateWindow()���Ăяo��
	CREATESTRUCT cs;
	cs.dwExStyle      = dwExStyle;
	cs.lpszClass      = lpszClassName;
	cs.lpszName       = lpszWindowName;
	cs.style          = dwStyle;
	cs.x              = x;
	cs.y              = y;
	cs.cx             = nWidth;
	cs.cy             = nHeight;
	cs.hwndParent     = (parent == NULL) ? NULL : parent->m_hWnd;
	cs.hMenu          = hMenu;
	cs.hInstance      = (hInstance == NULL) ? ::GetModuleHandle(NULL) : hInstance;
	cs.lpCreateParams = lpParam;
	if (!PreCreateWindow(cs))
	{
		PostNcDestroy();
		return FALSE;
	}

	// �t�b�N�v���V�[�W�����C���X�g�[���i�A���C���X�g�[���̓t�b�N�v���V�[�W�����ōs���j
	_SetHookTarget(this);
	HWND hWnd = ::CreateWindowEx(
		cs.dwExStyle,
		cs.lpszClass, cs.lpszName,
		cs.style,
		cs.x, cs.y, cs.cx, cs.cy,
		cs.hwndParent, cs.hMenu, cs.hInstance,
		cs.lpCreateParams);

	if(hWnd == NULL)
	{
		_SetHookTarget(NULL);
		return FALSE;
	}

	assert(hWnd == m_hWnd);
	return TRUE;
}


// �g�b�v���X�g�\��
void window::SetTopmost(const BOOL bTopmost)
{
	// �g�b�v���X�g�^��g�b�v���X�g�\��
	SetWindowPos(
		bTopmost ? HWND_TOPMOST : HWND_NOTOPMOST,
		0, 0, 0, 0,
		SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
}


////////////////////////////////////////////////////////////////////////////////
// �f�f�֐�

BOOL window::IsWindow(void) const
{
	return this != NULL && ::IsWindow(m_hWnd);
}

BOOL window::IsChild(HWND hWnd) const
{
	return ::IsChild(m_hWnd, hWnd);
}


////////////////////////////////////////////////////////////////////////////////
// �X�^�C���֐�

DWORD window::GetStyle(void) const
{
	return ::GetWindowLong(m_hWnd, GWL_STYLE);
}

void window::SetStyle(const DWORD dwStyle)
{
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
}

BOOL window::ModifyStyle(const DWORD dwRemove, const DWORD dwAdd, const BOOL bRedraw /* = TRUE */)
{
	// �X�^�C����ύX
	DWORD dwStyle = GetStyle();
	dwStyle &= ~dwRemove;
	dwStyle |= dwAdd;
	SetStyle(dwStyle);

	// �ĕ`��
	if(bRedraw)
	{
		InvalidateRect();
		UpdateWindow();
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
// �^�C�}�֐�

UINT window::SetTimer(const UINT uIDEvent, const UINT uElapse, TIMERPROC lpTimerFunc /* = NULL */)
{
	return ::SetTimer(m_hWnd, uIDEvent, uElapse, lpTimerFunc);
}

BOOL window::KillTimer(const UINT uIDEvent)
{
	return ::KillTimer(m_hWnd, uIDEvent);
}


////////////////////////////////////////////////////////////////////////////////
// �`��֐�

BOOL window::InvalidateRect(LPCRECT lpRect /* = NULL */, const BOOL bErase /* = TRUE */)
{
	return ::InvalidateRect(m_hWnd, lpRect, bErase);
}

HDC window::BeginPaint(LPPAINTSTRUCT ps)
{
	return ::BeginPaint(m_hWnd, ps);
}

BOOL window::EndPaint(const PAINTSTRUCT *ps)
{
	return ::EndPaint(m_hWnd, ps);
}


////////////////////////////////////////////////////////////////////////////////
// �t�H�[�J�X�֐�

HWND window::SetFocus(void)
{
	return ::SetFocus(m_hWnd);
}

BOOL window::SetForegroundWindow(void)
{
	return ::SetForegroundWindow(m_hWnd);
}


////////////////////////////////////////////////////////////////////////////////
// �J���b�g�֐�
BOOL window::CreateCaret(HBITMAP hBitmap, const int nWidth, const int nHeight) const
{
	return ::CreateCaret(m_hWnd, hBitmap, nWidth, nHeight);
}

BOOL window::CreateSolidCaret(const int nWidth, const int nHeight) const
{
	return CreateCaret(NULL, nWidth, nHeight);
}

BOOL window::ShowCaret(void) const
{
	return ::ShowCaret(m_hWnd);
}

BOOL window::HideCaret(void) const
{
	return ::HideCaret(m_hWnd);
}


////////////////////////////////////////////////////////////////////////////////
// �e�E�C���h�E�֐�
HWND window::GetParent(void) const
{
	return ::GetParent(m_hWnd);
}

HWND window::SetParent(HWND hWndNewParent)
{
	return ::SetParent(m_hWnd, hWndNewParent);
}


////////////////////////////////////////////////////////////////////////////////
// ���j���[�֐�
HMENU window::GetMenu(void) const
{
	return ::GetMenu(m_hWnd);
}

BOOL window::SetMenu(HMENU hMenu)
{
	return ::SetMenu(m_hWnd, hMenu);
}


////////////////////////////////////////////////////////////////////////////////
// �t�H���g�֐�
HFONT window::GetFont(void) const
{
	const LRESULT lResult = SendMessage(WM_GETFONT);
	return reinterpret_cast<HFONT>(lResult);
}

void window::SetFont(HFONT hFont, const BOOL bRedraw /* = TRUE */)
{
	SendMessage(
		WM_SETFONT,
		reinterpret_cast<WPARAM>(hFont),
		MAKELPARAM(bRedraw, 0));
}


////////////////////////////////////////////////////////////////////////////////
// �X�N���[���֐�

int window::SetHScrollInfo(const SCROLLINFO &si, const BOOL bRedraw /* = TRUE */)
{
	return ::SetScrollInfo(m_hWnd, SB_HORZ, &si, bRedraw);
}

int window::SetVScrollInfo(const SCROLLINFO &si, const BOOL bRedraw /* = TRUE */)
{
	return ::SetScrollInfo(m_hWnd, SB_VERT, &si, bRedraw);
}

BOOL window::GetHScrollInfo(SCROLLINFO &si)
{
	return ::GetScrollInfo(m_hWnd, SB_HORZ, &si);
}

BOOL window::GetVScrollInfo(SCROLLINFO &si)
{
	return ::GetScrollInfo(m_hWnd, SB_VERT, &si);
}


////////////////////////////////////////////////////////////////////////////////
// �E�C���h�E����

// �E�C���h�E�Ƀe�L�X�g���o��
BOOL window::SetWindowText(const tstring_t &tstr)
{
	return ::SetWindowText(m_hWnd, tstr.c_str());
}

int window::GetWindowText(tstring_t &tstr) const
{
	// �e�L�X�g�̒����������������m��
	const int size  = GetWindowTextLength() + 1;
	LPTSTR    lpStr = new TCHAR[size];

	// �e�L�X�g�̓��e���擾
	const int result = ::GetWindowText(m_hWnd, lpStr, size);
	tstr = lpStr;

	// ���������J��
	delete []lpStr;
	lpStr = NULL;

	return result;
}

int window::GetWindowTextLength(void) const
{
	return ::GetWindowTextLength(m_hWnd);
}


// �E�C���h�E�̈ʒu��ݒ�
BOOL window::SetWindowPos(HWND hWndInsertAfter, const int x, const int y, const int cx, const int cy, const UINT uFlags)
{
	return ::SetWindowPos(m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
}

BOOL window::MoveWindow(const int x, const int y, const int nWidth, const int nHeight, const BOOL bRepaint /* = TRUE */)
{
	return ::MoveWindow(m_hWnd, x, y, nWidth, nHeight, bRepaint);
}

BOOL window::MoveWindow(const RECT &rect, const BOOL bRepaint /* = TRUE */)
{
	return MoveWindow(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, bRepaint);
}

BOOL window::MoveWindow(const POINT &point, const SIZE &size, const BOOL bRepaint /* = TRUE */)
{
	return MoveWindow(point.x, point.y, size.cx, size.cy, bRepaint);
}


BOOL window::ShowWindow(const int nCmdShow)
{
	return ::ShowWindow(m_hWnd, nCmdShow);
}

BOOL window::UpdateWindow(void)
{
	return ::UpdateWindow(m_hWnd);
}

BOOL window::DestroyWindow(void)
{
	return ::DestroyWindow(m_hWnd);
}


////////////////////////////////////////////////////////////////////////////////
// �E�C���h�E�̈�֐�

BOOL window::GetClientRect(RECT &rRect) const
{
	return ::GetClientRect(m_hWnd, &rRect);
}

BOOL window::GetWindowRect(RECT &rRect) const
{
	return ::GetWindowRect(m_hWnd, &rRect);
}


////////////////////////////////////////////////////////////////////////////////
// ���b�Z�[�W�֐�

// ���b�Z�[�W�𑗐M
LRESULT window::SendMessage(UINT uMsg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */)
{
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
}

LRESULT window::SendMessage(UINT uMsg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */) const
{
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
}

// ���b�Z�[�W���|�X�g
BOOL window::PostMessage(UINT uMsg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */)
{
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

BOOL window::PostMessage(UINT uMsg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */) const
{
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

// ���b�Z�[�W���擾
BOOL window::GetMessage(LPMSG lpMsg, UINT wMsgFilterMin /* = 0 */, UINT wMsgFilterMax /* = 0 */) const
{
	return ::GetMessage(lpMsg, m_hWnd, wMsgFilterMin, wMsgFilterMax);
}

BOOL window::PeekMessage(LPMSG lpMsg, UINT wMsgFilterMin /* = 0 */, UINT wMsgFilterMax /* = 0 */, UINT wRemoveMsg /* = PM_REMOVE */) const
{
	return ::PeekMessage(lpMsg, m_hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

BOOL window::TranslateAccelerator(HACCEL hAccTable, LPMSG lpMsg)
{
	return ::TranslateAccelerator(m_hWnd, hAccTable, lpMsg);
}


// ���b�Z�[�W�{�b�N�X�֐�
int window::MessageBox(HINSTANCE hResource, const UINT uText, const UINT uCaption, const UINT uType /* = MB_OK */)
{
	tstring_t text;
	wgfLoadString(hResource, uText, text);

	tstring_t caption;
	wgfLoadString(hResource, uCaption, caption);

	return MessageBox(text, caption, uType);
}

int window::MessageBox(const tstring_t &text, const tstring_t &caption, const UINT uType /* = MB_OK */)
{
	return ::MessageBox(m_hWnd, text.c_str(), caption.c_str(), uType);
}


////////////////////////////////////////////////////////////////////////////////
// �}�b�v�֘A

window *window::_MapFind(HWND hWnd)
{
	thread_data_t *data = _GetThreadData();
	assert(data != NULL);

	handlemap_iterator_t p = data->handlemap.find(hWnd);
	if(p == data->handlemap.end())
	{
		return NULL;
	}
	return p->second;
}

BOOL window::_MapAttach(HWND hWnd, window *window_ptr)
{
	assert(hWnd       != NULL);
	assert(window_ptr != NULL);

	// �n���h���ƃI�u�W�F�N�g��Ή��t����
	thread_data_t *data = _GetThreadData();
	assert(data != NULL);

	data->handlemap[hWnd] = window_ptr;
	window_ptr->m_hWnd   = hWnd;
	window_ptr->m_bOwner = TRUE;
	return TRUE;
}

HWND window::_MapDetach(window *window_ptr)
{
	assert(window_ptr != NULL);

	HWND hWndTmp = window_ptr->m_hWnd;
	window_ptr->m_hWnd   = NULL;
	window_ptr->m_bOwner = FALSE;

	thread_data_t *data = _GetThreadData();
	assert(data != NULL);

	data->handlemap.erase(hWndTmp);
	return hWndTmp;
}


////////////////////////////////////////////////////////////////////////////////
// Thread Local Storage

window::thread_data_t *window::_GetThreadData(void)
{
	if(m_dwTlsIndex == TLS_OUT_OF_INDEXES)
	{
		return NULL;
	}

	LPVOID lpData = ::TlsGetValue(m_dwTlsIndex);
	return reinterpret_cast<thread_data_t *>(lpData);
}

BOOL window::_SetThreadData(thread_data_t *data)
{
	assert(m_dwTlsIndex != TLS_OUT_OF_INDEXES);
	return ::TlsSetValue(m_dwTlsIndex, data);
}


////////////////////////////////////////////////////////////////////////////////
// �t�b�N�֘A

// �֘A�t���̃^�[�Q�b�g�E�C���h�E��ݒ�
BOOL window::_SetHookTarget(window *pTargetWindow)
{
	thread_data_t *data = _GetThreadData();
	if(data == NULL)
	{
		return FALSE;
	}
	data->window_ptr = pTargetWindow;
	return TRUE;
}

// �t�b�N�v���V�[�W��
LRESULT CALLBACK window::_CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	thread_data_t *data = _GetThreadData();
	assert(data != NULL);

	// �t�b�N�n���h�����擾
	HHOOK hHook = data->hHook;
	assert(hHook != NULL);

	if(nCode == HCBT_CREATEWND)
	{
		// �֘A�t���̃^�[�Q�b�g�E�C���h�E���擾
		window *pTargetWindow = data->window_ptr;
		if(pTargetWindow != NULL)
		{
			HWND hWnd = reinterpret_cast<HWND>(wParam);

			LPCBT_CREATEWND lpCreateWnd = reinterpret_cast<LPCBT_CREATEWND>(lParam);
			LPCREATESTRUCT  lpCreateStruct = lpCreateWnd->lpcs;

			if(lpCreateStruct->lpszClass == WC_DIALOG)
			{
				// �_�C�A���O�{�b�N�X�̓A�^�b�`�̂�
				_MapAttach(hWnd, pTargetWindow);
			}
			else
			{
				// �_�C�A���O�{�b�N�X�ȊO�̓T�u�N���X�����s��
				pTargetWindow->Subclass(hWnd);
			}

			// �^�[�Q�b�g�E�C���h�E�̐ݒ���͂���
			_SetHookTarget(NULL);
		}
	}

	// �t�b�N�`�F�C�����̎��̃t�b�N�v���V�[�W�����R�[��
	return ::CallNextHookEx(hHook, nCode, wParam, lParam);
}


////////////////////////////////////////////////////////////////////////////////
// �T�u�N���X��

// �E�C���h�E���T�u�N���X��
BOOL window::Subclass(HWND hWnd)
{
	assert(m_hWnd == NULL);
	assert(hWnd   != NULL);
	assert(m_lpfnOldWndProc == NULL);

	// �}�b�s���O�Ɏ��s������G���[
	if(!_MapAttach(hWnd, this))
	{
		return FALSE;
	}

	// �E�C���h�E�v���V�[�W����u������
	WNDPROC lpfnOldWndProc = SubclassWindow(m_hWnd, _WindowProcBase);
	if(lpfnOldWndProc == _WindowProcBase)
	{
		// �i::RegisterClass() �ȂǂŁj���ł� _WindowProcBase() ��
		// �E�C���h�E�v���V�[�W���Ƃ��Ċ��蓖�Ă��Ă���Ύ��s
		return FALSE;
	}

	m_lpfnOldWndProc = lpfnOldWndProc;
	return TRUE;
}

// �T�u�N���X��������
BOOL window::Unsubclass(void)
{
	// �E�C���h�E�v���V�[�W�������ɖ߂�
	if(m_lpfnOldWndProc == NULL)
	{
		return FALSE;
	}

	// �A���T�u�N���X��
	SubclassWindow(m_hWnd, m_lpfnOldWndProc);
	m_lpfnOldWndProc = NULL;

	// �E�C���h�E�̑Ή���؂�
	_MapDetach(this);
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
// ���b�Z�[�W�֘A

// ���b�Z�[�W�p�����[�^���擾
void window::GetMessageParam(MSGPARAM &rMsgParam) const
{
	rMsgParam = m_msgparam;
}


////////////////////////////////////////////////////////////////////////////////
// �E�C���h�E�v���V�[�W��

// ���b�Z�[�W�U�蕪���v���V�[�W��
LRESULT CALLBACK window::_WindowProcBase(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �E�C���h�E�Ɗ֘A�t����ꂽ�I�u�W�F�N�g���擾
	window *pTargetWindow = _MapFind(hWnd);
	assert(pTargetWindow->IsWindow());

	// ���b�Z�[�W��ۑ�
	pTargetWindow->m_msgparam.uMsg   = uMsg;
	pTargetWindow->m_msgparam.wParam = wParam;
	pTargetWindow->m_msgparam.lParam = lParam;

	// �I�u�W�F�N�g�̃v���V�[�W���Ƀ��b�Z�[�W��n��
	return pTargetWindow->WindowProc(uMsg, wParam, lParam);
}

// �E�C���h�E�v���V�[�W��
LRESULT window::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �}�E�X���W
	const POINT point = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	// ���b�Z�[�W�ɂ���ĕ���
	switch(uMsg)
	{
	case WM_GETMINMAXINFO:
		OnGetMinMaxInfo(
			reinterpret_cast<LPMINMAXINFO>(lParam));    // lpMMI
		return 0;

	case WM_GETDLGCODE:
		return OnGetDlgCode(
			reinterpret_cast<LPMSG>(lParam));           // lpMsg

	case WM_NCCREATE:
		return OnNcCreate(
			reinterpret_cast<LPCREATESTRUCT>(lParam));  // lpCreateStruct

	case WM_CREATE:
		return OnCreate(
			reinterpret_cast<LPCREATESTRUCT>(lParam));  // lpCreateStruct

	case WM_CLOSE:
		OnClose();
		return 0;                               // �����ς�

	case WM_DESTROY:
		OnDestroy();
		return 0;                               // �����ς�

	case WM_NCDESTROY:
		OnNcDestroy();
		return 0;

	case WM_SIZE:
		OnSize(
			wParam,                                 // nType
			GET_X_LPARAM(lParam),                   // cx
			GET_Y_LPARAM(lParam));                   // cy
		return 0;

	case WM_SIZING:
		OnSizing(
			wParam,                                 // nSide
			reinterpret_cast<LPRECT>(lParam));      // lpRect
		return 0;


	case WM_ACTIVATE:
		OnActivate(
			LOWORD(wParam),                         // nState
			HIWORD(wParam),                         // bMinimized
			reinterpret_cast<HWND>(lParam));        // hWndPrevious
		return 0;

	case WM_SETFOCUS:
		OnSetFocus(
			reinterpret_cast<HWND>(wParam));        // hWndLoseFocus
		return 0;

	case WM_KILLFOCUS:
		OnKillFocus(
			reinterpret_cast<HWND>(wParam));        // hWndGetFocus
		return 0;


	case WM_SETFONT:
		OnSetFont(
			reinterpret_cast<HFONT>(wParam),        // hFont
			LOWORD(lParam));                        // bRedraw
		return 0;

	case WM_GETFONT:
		return reinterpret_cast<LRESULT>(
			OnGetFont());


	case WM_INITMENU:
		OnInitMenu(
			reinterpret_cast<HMENU>(wParam));       // hMenu
		return 0;

	case WM_INITMENUPOPUP:
		OnInitMenuPopup(
			reinterpret_cast<HMENU>(wParam),        // hMenu
			LOWORD(lParam),                         // uIndex
			HIWORD(lParam));                        // bSystemMenu
		return 0;

	case WM_MENUSELECT:
		OnMenuSelect(
			GET_WM_MENUSELECT_CMD  (wParam, lParam),    // nItemID
			GET_WM_MENUSELECT_FLAGS(wParam, lParam),    // nFlags
			GET_WM_MENUSELECT_HMENU(wParam, lParam));   // hSysMenu
		return 0;

	case WM_DROPFILES:
		OnDropFiles(
			reinterpret_cast<HDROP>(wParam));       // hDrop
		return 0;

	case WM_NOTIFY:
		return OnNotify(
			wParam,                                 // nIdCtrl
			reinterpret_cast<LPNMHDR>(lParam));     // lpNmHeader

	case WM_COMMAND:
		return OnCommand(
			GET_WM_COMMAND_CMD (wParam, lParam),    // wNotifyCode
			GET_WM_COMMAND_ID  (wParam, lParam),    // wID
			GET_WM_COMMAND_HWND(wParam, lParam));   // hWndCtrl


	case WM_NCHITTEST:
		return OnNcHitTest(
			point);                                 // point


	case WM_TIMER:
		OnTimer(wParam);                            // uIDEvent
		return 0;


	case WM_PAINT:
		OnPaint();
		return 0;

	case WM_NCPAINT:
		OnNcPaint();
		return 0;

	case WM_ERASEBKGND:
		return OnEraseBkgnd(
			reinterpret_cast<HDC>(wParam));         // hDC


	case WM_CTLCOLORSTATIC:
		return reinterpret_cast<LRESULT>(
			OnCtlColorStatic(
				GET_WM_CTLCOLOR_HDC (wParam, lParam, uMsg),
				GET_WM_CTLCOLOR_HWND(wParam, lParam, uMsg)));


	case WM_PARENTNOTIFY:
		OnParentNotify(
			GET_WM_PARENTNOTIFY_MSG(wParam, lParam),// message
			lParam);                                // lParam
		return TRUE;

	case WM_HSCROLL:
		OnHScroll(
			GET_WM_HSCROLL_CODE(wParam, lParam),    // nSBCode
			GET_WM_HSCROLL_POS (wParam, lParam),    // nPos
			GET_WM_HSCROLL_HWND(wParam, lParam));   // hWndScrollBar
		return TRUE;

	case WM_VSCROLL:
		OnVScroll(
			GET_WM_VSCROLL_CODE(wParam, lParam),    // nSBCode
			GET_WM_VSCROLL_POS (wParam, lParam),    // nPos
			GET_WM_VSCROLL_HWND(wParam, lParam));   // hWndScrollBar
		return TRUE;


	////////////////////////////////////////////////////////////
	// �J�[�\�����b�Z�[�W
	case WM_SETCURSOR:
		return OnSetCursor(
			reinterpret_cast<HWND>(wParam),
			LOWORD(lParam),
			HIWORD(lParam));


	////////////////////////////////////////////////////////////
	// �L�[�{�[�h���b�Z�[�W
	case WM_KEYDOWN:
		OnKeyDown(
			wParam,                             // nChar
			LOWORD(lParam),                     // nRepeatCount
			HIWORD(lParam));                    // nFlags
		return 0;

	case WM_CHAR:
		OnChar(
			wParam,                             // nChar
			LOWORD(lParam),                     // nRepeatCount
			HIWORD(lParam));                    // nFlags
		return 0;


	////////////////////////////////////////////////////////////
	// IME���b�Z�[�W
	case WM_IME_COMPOSITION:
		OnImeComposition(
			wParam,                             // chDBCS
			lParam);                            // nFlags
		return 0;


	////////////////////////////////////////////////////////////
	// �}�E�X���b�Z�[�W
	case WM_MOUSEWHEEL:                     // �}�E�X�z�C�[����]
		return OnMouseWheel(
			LOWORD(wParam),                     // nFlags
			HIWORD(wParam),                     // zDelta
			point);                             // point

	case WM_MOUSELEAVE:                     // �}�E�X����
		OnMouseLeave();
		return 0;

	case WM_MOUSEMOVE:                      // �}�E�X�ړ�
		OnMouseMove(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_LBUTTONDOWN:                    // �}�E�X�̍��{�^������
		OnLButtonDown(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_LBUTTONUP:                      // �}�E�X�̍��{�^���J��
		OnLButtonUp(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_LBUTTONDBLCLK:                  // �}�E�X�̍��{�^���_�u���N���b�N
		OnLButtonDblClk(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_RBUTTONDOWN:                    // �}�E�X�̉E�{�^������
		OnRButtonDown(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_RBUTTONUP:                      // �}�E�X�̉E�{�^���J��
		OnRButtonUp(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_RBUTTONDBLCLK:                  // �}�E�X�̉E�{�^���_�u���N���b�N
		OnRButtonDblClk(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_MBUTTONDOWN:                    // �}�E�X�̒��{�^������
		OnMButtonDown(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_MBUTTONUP:                      // �}�E�X�̒��{�^���J��
		OnMButtonUp(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_MBUTTONDBLCLK:                  // �}�E�X�̒��{�^���_�u���N���b�N
		OnMButtonDblClk(
			wParam,                             // nFlags
			point);                             // point
		return 0;

	case WM_XBUTTONDOWN:                    // 5�{�^���}�E�X�̃{�^������
		switch(GET_WM_COMMAND_CMD(wParam, lParam))
		{
		case XBUTTON1:
			OnXButton1Down(
				wParam,                             // nFlags
				point);                             // point
			return 0;

		case XBUTTON2:
			OnXButton2Down(
				wParam,                             // nFlags
				point);                             // point
			return 0;
		}
		break;

	case WM_XBUTTONUP:                      // 5�{�^���}�E�X�̃{�^���J��
		switch(GET_WM_COMMAND_CMD(wParam, lParam))
		{
		case XBUTTON1:
			OnXButton1Up(
				wParam,                             // nFlags
				point);                             // point
			return 0;

		case XBUTTON2:
			OnXButton2Up(
				wParam,                             // nFlags
				point);                             // point
			return 0;
		}
		break;

	case WM_XBUTTONDBLCLK:                  // 5�{�^���}�E�X�̃{�^���_�u���N���b�N
		switch(GET_WM_COMMAND_CMD(wParam, lParam))
		{
		case XBUTTON1:
			OnXButton1DblClk(
				wParam,                             // nFlags
				point);                             // point
			return 0;

		case XBUTTON2:
			OnXButton2DblClk(
				wParam,                             // nFlags
				point);                             // point
			return 0;
		}
		break;
	}

	// �f�t�H���g�̃v���V�[�W�����Ăяo��
	return _Default();
}

// ���b�Z�[�W�̃f�t�H���g����
LRESULT window::_Default(void)
{
	MSGPARAM param;
	GetMessageParam(param);

	// �E�C���h�E�v���V�[�W�����ۑ�����Ă���ꍇ
	if(m_lpfnOldWndProc != NULL)
	{
		return ::CallWindowProc(
			m_lpfnOldWndProc,
			m_hWnd,
			param.uMsg,
			param.wParam,
			param.lParam);
	}

	return ::DefWindowProc(
		m_hWnd,
		param.uMsg,
		param.wParam,
		param.lParam);
}


////////////////////////////////////////////////////////////////////////////////
// ���z�֐�

// �E�C���h�E���쐬�����O�ɌĂяo�����
BOOL window::PreCreateWindow(CREATESTRUCT & /* cs */)
{
	return TRUE;
}

// �E�C���h�E���j�����ꂽ��ɌĂяo�����
void window::PostNcDestroy(void)
{
}


////////////////////////////////////////////////////////////////////////////////
// ���b�Z�[�W�n���h��

void window::OnGetMinMaxInfo(LPMINMAXINFO /* lpMMI */)
{
	_Default();
}

UINT window::OnGetDlgCode(LPMSG /* lpMsg */)
{
	return _Default();
}

BOOL window::OnNcCreate(LPCREATESTRUCT /* lpCreateStruct */)
{
	return _Default();
}

// �E�C���h�E������
int window::OnCreate(LPCREATESTRUCT /* lpCreateStruct */)
{
	return _Default();
}

// �E�C���h�E�����
void window::OnClose(void)
{
	_Default();
}

// �E�C���h�E�̔j��
void window::OnDestroy(void)
{
	_Default();
}

// �E�C���h�E�̃m���N���C�A���g�̈�̔j���i�E�C���h�E�̔j�����ɍŌ�ɌĂяo�����j
void window::OnNcDestroy(void)
{
	_Default();
	_MapDetach(this);
	PostNcDestroy();
}


// �T�C�Y�ύX
void window::OnSize(const UINT /* nType */, const int /* cx */, const int /* cy */)
{
	_Default();
}

void window::OnSizing(const UINT /* nSide */, LPRECT /* lpRect */)
{
	_Default();
}


void window::OnActivate(const UINT /* nState */, const BOOL /* bMinimized */, HWND /* hWndPrevious */)
{
	_Default();
}

// �t�H�[�J�X
void window::OnSetFocus(HWND /* hWndLoseFocus */)
{
	_Default();
}

void window::OnKillFocus(HWND /* hWndGetFocus */)
{
	_Default();
}


// �t�H���g
void window::OnSetFont(HFONT /* hFont */, const BOOL /* bRedraw */)
{
	_Default();
}

HFONT window::OnGetFont(void)
{
	const LRESULT lResult = _Default();
	return reinterpret_cast<HFONT>(lResult);
}


// ���j���[
void window::OnInitMenu(HMENU /* hMenu */)
{
	_Default();
}

void window::OnInitMenuPopup(HMENU /* hMenu */, const UINT /* uIndex */, const BOOL /* bSystemMenu */)
{
	_Default();
}

void window::OnMenuSelect(const UINT /* nItemID */, const UINT /* nFlags */, HMENU /* hSysMenu */)
{
	_Default();
}


// �t�@�C���̃h���b�v
void window::OnDropFiles(HDROP /* hDrop */)
{
	_Default();
}


// �ʒm���b�Z�[�W
BOOL window::OnNotify(const int /* nIdCtrl */, LPNMHDR /* lpNmHeader */)
{
	return _Default();
}

// �R�}���h���b�Z�[�W
BOOL window::OnCommand(const WORD /* wNotifyCode */, const WORD /* wID */, HWND /* hWndCtrl */)
{
	return _Default();
}


// �q�b�g�e�X�g
UINT window::OnNcHitTest(const POINT & /* point */)
{
	return _Default();
}


// �^�C�}
void window::OnTimer(const UINT /* uIDEvent */)
{
	_Default();
}


// �E�C���h�E�`��
void window::OnPaint(void)
{
	_Default();
}

// �E�C���h�E�̔�N���C�A���g�̈��`��
void window::OnNcPaint(void)
{
	_Default();
}

// �w�i����
BOOL window::OnEraseBkgnd(HDC /* hDC */)
{
	return _Default();
}


HBRUSH window::OnCtlColorStatic(HDC /* hDC */, HWND /* hWnd */)
{
	const LRESULT lResult = _Default();
	return reinterpret_cast<HBRUSH>(lResult);
}


// �ʒm���b�Z�[�W
void window::OnParentNotify(const UINT /* message */, const LPARAM /* lParam */)
{
	_Default();
}

// �����X�N���[���o�[
void window::OnHScroll(const UINT /* nSBCode */, const UINT /* nPos */, HWND /* hWndScrollBar */)
{
	_Default();
}

// �����X�N���[���o�[
void window::OnVScroll(const UINT /* nSBCode */, const UINT /* nPos */, HWND /* hWndScrollBar */)
{
	_Default();
}


////////////////////////////////////////////////////////////
// �J�[�\�����b�Z�[�W

BOOL window::OnSetCursor(HWND /* hWnd */, const UINT /* nHitTest */, const UINT /* message */)
{
	return _Default();
}


////////////////////////////////////////////////////////////
// �L�[�{�[�h���b�Z�[�W

void window::OnKeyDown(const UINT /* nChar */, const UINT /* nRepCnt */, const UINT /* nFlags */)
{
	_Default();
}

// �L�[�{�[�h����
void window::OnChar(const UINT /* nChar */, const UINT /* nRepCnt */, const UINT /* nFlags */)
{
	_Default();
}


////////////////////////////////////////////////////////////
// IME���b�Z�[�W

// IME�ϊ�
void window::OnImeComposition(const WORD /* chDBCS */, const UINT /* nFlags */)
{
	_Default();
}


////////////////////////////////////////////////////////////
// �}�E�X���b�Z�[�W

// �z�C�[����]
BOOL window::OnMouseWheel(const UINT /* nFlags */, const short /* zDelta */, const POINT & /* point */)
{
	return _Default();
}

// �}�E�X����
void window::OnMouseLeave(void)
{
	_Default();
}

// �}�E�X�ړ�
void window::OnMouseMove(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// �}�E�X�̍��{�^������
void window::OnLButtonDown(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// �}�E�X�̍��{�^���J��
void window::OnLButtonUp(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// �}�E�X�̍��{�^���_�u���N���b�N
void window::OnLButtonDblClk(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// �}�E�X�̉E�{�^������
void window::OnRButtonDown(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// �}�E�X�̉E�{�^���J��
void window::OnRButtonUp(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// �}�E�X�̉E�{�^���_�u���N���b�N
void window::OnRButtonDblClk(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// �}�E�X�̒��{�^������
void window::OnMButtonDown(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// �}�E�X�̒��{�^���J��
void window::OnMButtonUp(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// �}�E�X�̒��{�^���_�u���N���b�N
void window::OnMButtonDblClk(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5�{�^���}�E�X�̑�{�^������
void window::OnXButton1Down(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5�{�^���}�E�X�̑�{�^���J��
void window::OnXButton1Up(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5�{�^���}�E�X�̑�{�^���_�u���N���b�N
void window::OnXButton1DblClk(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5�{�^���}�E�X�̏��{�^������
void window::OnXButton2Down(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5�{�^���}�E�X�̏��{�^���J��
void window::OnXButton2Up(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

// 5�{�^���}�E�X�̏��{�^���_�u���N���b�N
void window::OnXButton2DblClk(const UINT /* nFlags */, const POINT & /* point */)
{
	_Default();
}

_WGC_END                                // }
