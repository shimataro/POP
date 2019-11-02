// ColorStatic.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "pop.h"
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic(COLORREF color /* = RGB(0, 0, 0) */, DWORD dwFlags /* = CC_FULLOPEN */)
{
	m_color   = color;
	m_dwFlags = dwFlags;
}

CColorStatic::CColorStatic(int red, int green, int blue, DWORD dwFlags /* = CC_FULLOPEN */)
{
	m_color   = RGB(red, green, blue);
	m_dwFlags = dwFlags;
}

CColorStatic::~CColorStatic()
{
}


// ���݂̐F���擾
COLORREF CColorStatic::GetColor(void) const
{
	return m_color;
}

// �V�����F��RGB�R���|�[�l���g�Ŏw��
COLORREF CColorStatic::SetColor(const int red, const int green, const int blue)
{
	return SetColor(RGB(red, green, blue));
}

// �V�����F�� COLORREF �Őݒ�
COLORREF CColorStatic::SetColor(const COLORREF color)
{
	const COLORREF color_old = m_color;

	// �F��ς��čĕ`��
	m_color = color;
	Invalidate(FALSE);

	// �Â��F��Ԃ�
	return color_old;
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic ���b�Z�[�W �n���h��

void CColorStatic::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

	// �J���[�{�b�N�X��h��Ԃ�
	if(TRUE)
	{
		CRect rect;
		GetClientRect(rect);
		dc.FillSolidRect(rect, m_color);
	}
}

// �N���b�N���ꂽ��F�I���_�C�A���O��\��
void CColorStatic::OnClicked() 
{
	CColorDialog theDlg(m_color, m_dwFlags);
	if(theDlg.DoModal() == IDOK)
	{
		SetColor(theDlg.GetColor());
	}
}
