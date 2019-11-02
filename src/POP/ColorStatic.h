#if !defined(AFX_COLORSTATIC_H__44741B52_49EF_4409_8701_8810C94A7B20__INCLUDED_)
#define AFX_COLORSTATIC_H__44741B52_49EF_4409_8701_8810C94A7B20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorStatic.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CColorStatic �E�B���h�E

class CColorStatic : public CStatic
{
// �R���X�g���N�V����
public:
	CColorStatic(const COLORREF color = RGB(0, 0, 0), const DWORD dwFlags = CC_FULLOPEN);
	CColorStatic(const int red, const int green, const int blue, const DWORD dwFlags = CC_FULLOPEN);

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	COLORREF GetColor(void) const;
	COLORREF SetColor(const int red, const int green, const int blue);
	COLORREF SetColor(const COLORREF color);
	virtual ~CColorStatic();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CColorStatic)
	afx_msg void OnPaint();
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_color;
	DWORD    m_dwFlags;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_COLORSTATIC_H__44741B52_49EF_4409_8701_8810C94A7B20__INCLUDED_)
