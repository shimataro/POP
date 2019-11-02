// POPView.h : CPOPView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_POPVIEW_H__BF5410B4_332D_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_POPVIEW_H__BF5410B4_332D_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// 5�{�^���}�E�X�̃T�C�h�{�^���֌W
// ���b�Z�[�W
#define WM_XBUTTONDOWN    0x020B        // �T�C�h�{�^������
#define WM_XBUTTONUP      0x020C        // �T�C�h�{�^���J��
#define WM_XBUTTONDBLCLK  0x020D        // �T�C�h�{�^���_�u���N���b�N
// HIWORD(wParam)
#define XBUTTON1          0x0001        // �T�C�h�{�^����
#define XBUTTON2          0x0002        // �T�C�h�{�^����
// LOWORD(wParam)
#define MK_XBUTTON1       0x0020
#define MK_XBUTTON2       0x0040


class CPOPView : public CScrollView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CPOPView();
	DECLARE_DYNCREATE(CPOPView)

// �A�g���r���[�g
public:
	CPOPDoc* GetDocument();

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CPOPView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	virtual void OnInitialUpdate(); // �\�z��̍ŏ��̂P�x�����Ăяo����܂��B
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CPOPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	LRESULT OnXButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnXButtonUp(WPARAM wParam, LPARAM lParam);

	void OnXButton1Down(UINT nFlags, CPoint point);
	void OnXButton1Up(UINT nFlags, CPoint point);
	void OnXButton2Down(UINT nFlags, CPoint point);
	void OnXButton2Up(UINT nFlags, CPoint point);

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CPOPView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	// �r���[�̍ő�T�C�Y
	SIZE m_sizeViewMax;

	// �}�E�X�C�x���g���g���b�L���O���Ă��邩
	BOOL m_bTrackingMouseEvent;

	// �}�E�X�{�^���̏��
	BOOL m_bXButton1Down;                   // �G�N�X�g���{�^��1�i5�{�^���}�E�X�̃T�C�h�{�^����j��������Ă��邩
	BOOL m_bXButton2Down;                   // �G�N�X�g���{�^��2�i5�{�^���}�E�X�̃T�C�h�{�^�����j��������Ă��邩

	// �X�e�[�^�X�o�[�A�y�C��
	CStatusBar* m_pStatusBar;               // �X�e�[�^�X�o�[
	int m_nIndexColor;
	int m_nIndexPoint;

	// �K���}
	uint8_t m_gamma_table[256];             // �K���}�e�[�u��

	// DIB�Z�N�V����
	CBitmap     m_dibsection;
	BITMAPINFO  m_bitmapinfo;
	RGBQUAD    *m_buffer;

	void ResizeWindow(void);
	void UpdateStatusPaneColor(const POINT &point);
	void UpdateStatusPanePoint(const POINT &point);

	void ConvertToLogicalPoint(POINT &point);
	void ConvertToLogicalPoint(RECT  &rect);
	void ConvertToLogicalPoint(SIZE  &size);

	void ConvertToDevicePoint(POINT &point);
	void ConvertToDevicePoint(RECT  &rect);
	void ConvertToDevicePoint(SIZE  &size);
};

#ifndef _DEBUG  // POPView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CPOPDoc* CPOPView::GetDocument()
	 { return (CPOPDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_POPVIEW_H__BF5410B4_332D_11D5_8205_08004605D14C__INCLUDED_)
