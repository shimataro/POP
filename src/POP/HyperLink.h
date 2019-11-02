#if !defined(AFX_HYPERLINK_H__70B57841_38FB_4ED8_967A_3EA91A45B39F__INCLUDED_)
#define AFX_HYPERLINK_H__70B57841_38FB_4ED8_967A_3EA91A45B39F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HyperLink.h : �w�b�_�[ �t�@�C��
//


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

/////////////////////////////////////////////////////////////////////////////
// CHyperLink �E�B���h�E

class CHyperLink : public CStatic
{
// �R���X�g���N�V����
public:
	CHyperLink(const UINT nIDResource = 0, const COLORREF colorNormal = RGB_BLUE, const COLORREF colorPoint = RGB_ORANGE, const COLORREF colorActive = RGB_RED);

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	BOOL SetURL(const UINT nIDResource = 0);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CHyperLink)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CHyperLink();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CHyperLink)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

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
	HCURSOR m_hCursorFinger;
	CBrush m_brush;
	CFont m_font;

	// �����N��Ԋ֘A
	LINKSTATE m_status;
	void SetStatus(const LINKSTATE status);

	// URL�֘A
	CString m_url;
	void JumpToURL(void);

	// OLE�h���b�O���h���b�v�֘A
	BOOL m_bDragging;
	void OleDragDrop(void);
	static HGLOBAL CreateGlobalData(LPCVOID pData, const int size);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_HYPERLINK_H__70B57841_38FB_4ED8_967A_3EA91A45B39F__INCLUDED_)
