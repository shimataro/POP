// HyperLinkCtrl.cpp �c�C���v�������e�[�V�����t�@�C��
#include "HyperLinkCtrl.h"
#include <shlobj.h>                     // CFSTR_SHELLURL
#include <assert.h>                     // assert()

#include "ole_drop_source.h"            // wgc::ole_drop_source
#include "../devicecontext.h"           // wgc::devicecontext
#include "../wgfunc.h"                  // wgc::wgfMultiByteToWideChar()


#define CF_SHELLURL     (::RegisterClipboardFormat(CFSTR_SHELLURL))

_WXC_BEGIN                              // namespace wxc {


////////////////////////////////////////////////////////////
// �R���X�g���N�V����

hyperlink_ctrl::hyperlink_ctrl(
	HCURSOR hCursor,
	LPCTSTR lpszURL /* = NULL */,
	const COLORREF colorNormal /* = RGB_BLUE */,
	const COLORREF colorPoint  /* = RGB_ORANGE */,
	const COLORREF colorActive /* = RGB_RED */)
{
	m_hCursor = hCursor;
	m_hFont   = NULL;

	// �e�L�X�g�̐F��ݒ�
	m_colorNormal = colorNormal;          // �ʏ�
	m_colorPoint  = colorPoint;           // �|�C���g��
	m_colorActive = colorActive;          // �N���b�N��

	// �����N��ԁ��ʏ�
	m_status = LS_NORMAL;

	// �t�H�[�J�X��`�̕`�恁�Ȃ�
	m_bDrawFocusRect = FALSE;

	// �W�����v��URL��ݒ�
	SetURL(lpszURL);
}

hyperlink_ctrl::~hyperlink_ctrl()
{
	if(m_hFont != NULL)
	{
		::DeleteObject(m_hFont);
		m_hFont = NULL;
	}
}

BOOL hyperlink_ctrl::SetURL(LPCTSTR lpszURL /* = NULL */)
{
	// ���\�[�X�t�@�C������URL���擾
	if(lpszURL != NULL)
	{
		m_url = lpszURL;
		return TRUE;
	}

	// �\������Ă��镶���񂩂�URL���擾
	if(m_hWnd != NULL)
	{
		GetWindowText(m_url);
		return TRUE;
	}

	// ��������擾�ł��Ȃ���΃G���[�I��
	m_url.erase();
	return FALSE;
}

void hyperlink_ctrl::SetFontStyle(const BOOL bUnderline /* = TRUE */, const BOOL bItalic /* = FALSE */)
{
	if(m_hFont != NULL)
	{
		::DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	// �w�肳�ꂽ�X�^�C���̃t�H���g���쐬
	LOGFONT lf;
	::GetObject(GetFont(), sizeof(lf), &lf);
	lf.lfUnderline = bUnderline;
	lf.lfItalic    = bItalic;

	m_hFont = ::CreateFontIndirect(&lf);
}


// �����N��փW�����v
void hyperlink_ctrl::JumpToURL(void) const
{
	::ShellExecute(NULL, NULL, m_url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

// �����N��Ԃ�ݒ�
void hyperlink_ctrl::SetStatus(const LINKSTATE status)
{
	m_status = status;
	InvalidateRect();
}

// �����N�F���擾
COLORREF hyperlink_ctrl::GetLinkColor(void) const
{
	COLORREF color;
	switch(m_status)
	{
	case LS_NORMAL:   color = m_colorNormal; break;
	case LS_HOVER:    color = m_colorPoint;  break;
	case LS_CLICKING: color = m_colorActive; break;
	case LS_DRAGGING: color = m_colorActive; break;
	default: __assume(0);
	}
	return color;
}

// URL����OLE�h���b�O���h���b�v
DWORD hyperlink_ctrl::OleDragDrop(void) const
{
	ole_drop_source ods;

	// [block]
	// �e�L�X�g�����L���b�V��
	{
		HGLOBAL hGlobal = CreateGlobalData(
			m_url.c_str(),
			sizeof(TCHAR) * (m_url.length() + 1));

		ods.CacheGlobalData(
			(sizeof(TCHAR) == sizeof(char)) ? CF_TEXT : CF_UNICODETEXT,
			hGlobal);
	}

	// [block]
	// URL���L���b�V��
	{
		wgc::string_t url;
#ifndef UNICODE
		url = m_url;
#else // UNICODE
		wgc::wgfWideCharToMultiByte(m_url, url, CP_ACP, 0);
#endif // UNICODE

		HGLOBAL hGlobal = CreateGlobalData(
			url.c_str(),
			url.length() + 1);
		ods.CacheGlobalData(CF_SHELLURL, hGlobal);
	}

	// OLE�h���b�O���h���b�v���J�n
	return ods.DoDragDrop(DROPEFFECT_COPY);
}

// �O���[�o���������Ɏw��f�[�^���i�[
HGLOBAL hyperlink_ctrl::CreateGlobalData(LPCVOID pData, const int size)
{
	// ���������m��
	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, size);
	if(hGlobal == NULL)
	{
		return NULL;
	}

	// [block]
	// �f�[�^���R�s�[
	{
		LPVOID pBuffer = ::GlobalLock(hGlobal);
		::CopyMemory(pBuffer, pData, size);
		::GlobalUnlock(hGlobal);
	}

	// �������n���h����Ԃ�
	return hGlobal;
}


/////////////////////////////////////////////////////////////////////////////
// hyperlink_ctrl ���b�Z�[�W �n���h��

// �J�[�\���̌`��ݒ�
BOOL hyperlink_ctrl::OnSetCursor(HWND /* hWnd */, const UINT /* nHitTest */, const UINT /* message */) 
{
	::SetCursor(m_hCursor);
	return TRUE;
}

// �}�E�X���R���g���[���O�ɏo��
void hyperlink_ctrl::OnMouseLeave(void)
{
	// �h���b�O���łȂ���΃����N��ʏ�F�ɖ߂�
	if(m_status != LS_DRAGGING)
	{
		SetStatus(LS_NORMAL);
	}

	static_ctrl::OnMouseLeave();
}


void hyperlink_ctrl::OnSetFocus(HWND hWndLoseFocus)
{
	m_bDrawFocusRect = TRUE;
	InvalidateRect();

	static_ctrl::OnSetFocus(hWndLoseFocus);
}

void hyperlink_ctrl::OnKillFocus(HWND hWndLoseFocus)
{
	m_bDrawFocusRect = TRUE;
	InvalidateRect();

	static_ctrl::OnKillFocus(hWndLoseFocus);
}


// �q�b�g�e�X�g
UINT hyperlink_ctrl::OnNcHitTest(const POINT & /* point */)
{
	// �����ŏ�ɃN���C�A���g�̈����Ԃ����Ƃɂ��A
	// �X�^�e�B�b�N�R���g���[���� SS_NOTIFY �X�^�C����ǉ�����K�v���Ȃ��Ȃ�
	// �iSS_NOTIFY ��ݒ肵�Ă��Ȃ��X�^�e�B�b�N�R���g���[���͏�� HTTRANSPARENT ��Ԃ��j
	return HTCLIENT;
}


// �}�E�X���ړ�����
void hyperlink_ctrl::OnMouseMove(const UINT nFlags, const POINT &point) 
{
	// �����N���N���b�N����Ă���ꍇ
	if(m_status == LS_CLICKING)
	{
		// �����N����OLE�h���b�O���h���b�v���J�n
		SetStatus(LS_DRAGGING);
		OleDragDrop();
	}

	if(m_status != LS_HOVER)
	{
		// �����N���z�o�[
		SetStatus(LS_HOVER);

		// WM_MOUSELEAVE���b�Z�[�W���L���v�`��
		TRACKMOUSEEVENT tme;
		tme.cbSize    = sizeof(tme);
		tme.dwFlags   = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		_TrackMouseEvent(&tme);
	}

	static_ctrl::OnMouseMove(nFlags, point);
}

// �}�E�X�̍��{�^�����������ꂽ
void hyperlink_ctrl::OnLButtonDown(const UINT nFlags, const POINT &point) 
{
	// �����N���N���b�N��
	SetStatus(LS_CLICKING);
	SetFocus();

	static_ctrl::OnLButtonDown(nFlags, point);
}

// �}�E�X�̍��{�^���������ꂽ
void hyperlink_ctrl::OnLButtonUp(const UINT nFlags, const POINT &point) 
{
	// WebPage��\��
	if(m_status == LS_CLICKING)
	{
		JumpToURL();
	}

	// �����N���z�o�[
	SetStatus(LS_HOVER);

	static_ctrl::OnLButtonUp(nFlags, point);
}

// �`��
void hyperlink_ctrl::OnPaint(void)
{
	wgc::devicecontext_paint dc(m_hWnd);

	// �t�H�[�J�X��`��`��/����
	// DrawFocusRect() ��XOR���Z���g�p���Ă���̂ŁA2�x�Ăяo���Ώ�����
	if(m_bDrawFocusRect)
	{
		RECT rect;
		GetClientRect(rect);
		dc.DrawFocusRect(rect);

		m_bDrawFocusRect = FALSE;
	}

	HFONT   hFont      = (m_hFont != NULL) ? m_hFont : GetFont();
	HGDIOBJ hOldObject = dc.SelectObject(hFont);
	{
		// �\���e�L�X�g���擾
		wgc::tstring_t tstr;
		GetWindowText(tstr);

		// �e�L�X�g��`��
		dc.SetTextColor(GetLinkColor());
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(0, 0, tstr);
	}
	dc.SelectObject(hOldObject);
}

_WXC_END                                // }
