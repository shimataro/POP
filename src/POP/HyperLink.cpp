// HyperLink.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "pop.h"
#include "HyperLink.h"

#include "winutil.h"                    // GvxTCHAR2CHAR()

#include <afxole.h>                     // AfxOleInit()


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperLink

CHyperLink::CHyperLink(const UINT nIDResource /* = 0 */, const COLORREF colorNormal /* = RGB_BLUE */, const COLORREF colorPoint /* = RGB_ORANGE */, const COLORREF colorActive /* = RGB_RED */)
{
	// �e�L�X�g�̐F��ݒ�
	m_colorNormal = colorNormal;          // �ʏ�
	m_colorPoint  = colorPoint;           // �|�C���g��
	m_colorActive = colorActive;          // �N���b�N��

	// �����N��ԁ��ʏ�
	m_status = LS_NORMAL;

	// �u���V���쐬
	m_brush.CreateStockObject(NULL_BRUSH);

	// �J�[�\�������[�h
	// Windows 98/Me/2000/XP�Ȃ� ::LoadCursor(NULL, IDC_HAND) �ł悢
	m_hCursorFinger = AfxGetApp()->LoadCursor(IDC_FINGER);

	// �W�����v��URL��ݒ�
	SetURL(nIDResource);
}

CHyperLink::~CHyperLink()
{
}

BOOL CHyperLink::SetURL(const UINT nIDResource /* = 0 */)
{
	// ���\�[�X�t�@�C������URL���擾
	if(m_url.LoadString(nIDResource))
	{
		return TRUE;
	}

	// �\������Ă��镶���񂩂�URL���擾
	if(GetSafeHwnd() != NULL)
	{
		GetWindowText(m_url);
		return TRUE;
	}

	// ��������擾�ł��Ȃ���΃G���[�I��
	m_url.Empty();
	return FALSE;
}


// �����N��փW�����v
void CHyperLink::JumpToURL(void)
{
	::ShellExecute(NULL, NULL, m_url, NULL, NULL, SW_SHOWNORMAL);
}

// �����N��Ԃ�ݒ�
void CHyperLink::SetStatus(const LINKSTATE status)
{
	m_status = status;
	Invalidate();
}

// URL����OLE�h���b�O���h���b�v
void CHyperLink::OleDragDrop(void)
{
	COleDataSource ods;

	// �e�L�X�g�����L���b�V��
	if(TRUE)
	{
		HGLOBAL hGlobal = CreateGlobalData(
			(LPCTSTR)m_url,
			sizeof(TCHAR) * (m_url.GetLength() + 1));

		ods.CacheGlobalData(
			sizeof(TCHAR) == sizeof(char) ? CF_TEXT : CF_UNICODETEXT,
			hGlobal);
	}

	// URL���L���b�V��
	if(TRUE)
	{
		char url[_MAX_PATH];
		GvxTCHAR2CHAR(m_url, url, SIZE_OF_ARRAY(url));

		const CLIPFORMAT cfURL = ::RegisterClipboardFormat(CFSTR_SHELLURL);
		HGLOBAL hGlobal = CreateGlobalData(url, sizeof(url));
		ods.CacheGlobalData(cfURL, hGlobal);
	}

	// OLE�h���b�O���h���b�v���J�n
	ods.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_LINK);
}

// �O���[�o���������Ɏw��f�[�^���i�[
HGLOBAL CHyperLink::CreateGlobalData(LPCVOID pData, const int size)
{
	// ���������m��
	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, size);
	if(hGlobal == NULL)
	{
		return NULL;
	}

	// �f�[�^���R�s�[
	if(TRUE)
	{
		LPVOID pBuffer = ::GlobalLock(hGlobal);
		::CopyMemory(pBuffer, pData, size);
		::GlobalUnlock(hGlobal);
	}

	// �������n���h����Ԃ�
	return hGlobal;
}


BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
	//{{AFX_MSG_MAP(CHyperLink)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperLink ���b�Z�[�W �n���h��

// �R���g���[���̐F�ݒ�
HBRUSH CHyperLink::CtlColor(CDC* pDC, UINT /* nCtlColor */) 
{
	// �ŏ��ɌĂ΂ꂽ�������t�H���g�̐ݒ������
	if(m_font.m_hObject == NULL)
	{
		// �t�H���g�̐ݒ�i�����������j
		LOGFONT lf;
		GetFont()->GetLogFont(&lf);
		lf.lfUnderline = TRUE;
		m_font.CreateFontIndirect(&lf);
		SetFont(&m_font);
	}

	// �����N����єw�i�̐F��ݒ�
	COLORREF color;
	switch(m_status)
	{
	case LS_NORMAL:   color = m_colorNormal; break;
	case LS_HOVER:    color = m_colorPoint;  break;
	case LS_CLICKING: color = m_colorActive; break;
	case LS_DRAGGING: color = m_colorActive; break;
	default: __assume(0);
	}
	pDC->SetTextColor(color);
	pDC->SetBkMode(TRANSPARENT);

	return m_brush;
}

UINT CHyperLink::OnNcHitTest(CPoint point) 
{
	return HTCLIENT;
}

// �J�[�\���̌`��ݒ�
BOOL CHyperLink::OnSetCursor(CWnd* /* pWnd */, UINT /* nHitTest */, UINT /* message */) 
{
	::SetCursor(m_hCursorFinger);
	return TRUE;
}

// �}�E�X�̍��{�^�����������ꂽ
void CHyperLink::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// �����N���N���b�N��
	SetStatus(LS_CLICKING);
	CStatic::OnLButtonDown(nFlags, point);
}

// �}�E�X�̍��{�^���������ꂽ
void CHyperLink::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// WebPage��\��
	if(m_status == LS_CLICKING)
	{
		JumpToURL();
	}

	// �����N���z�o�[
	SetStatus(LS_HOVER);

	CStatic::OnLButtonUp(nFlags, point);
}

// �}�E�X���ړ�����
void CHyperLink::OnMouseMove(UINT nFlags, CPoint point) 
{
	// �����N���N���b�N����Ă���ꍇ
	if(m_status == LS_CLICKING)
	{
		// �����N����OLE�h���b�O���h���b�v���J�n
		SetStatus(LS_DRAGGING);
		OleDragDrop();
	}

	// WM_MOUSELEAVE���b�Z�[�W���L���v�`��
	if(TRUE)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize    = sizeof(tme);
		tme.dwFlags   = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		_TrackMouseEvent(&tme);
	}

	// �����N���z�o�[
	SetStatus(LS_HOVER);

	CStatic::OnMouseMove(nFlags, point);
}

// �}�E�X���R���g���[���O�ɏo��
LRESULT CHyperLink::OnMouseLeave(WPARAM /* wParam */, LPARAM /* lParam */)
{
	// �h���b�O���łȂ���΃����N��ʏ�F�ɖ߂�
	if(m_status != LS_DRAGGING)
	{
		SetStatus(LS_NORMAL);
	}
	return 0;
}
