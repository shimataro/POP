#if !defined(AFX_TOOLBAREX_H__C13B4108_0476_4D61_A452_E5ACB3A4B29D__INCLUDED_)
#define AFX_TOOLBAREX_H__C13B4108_0476_4D61_A452_E5ACB3A4B29D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarEx.h : �w�b�_�[ �t�@�C��
//
#include <AfxTempl.h>

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx �E�B���h�E

class CToolBarEx : public CToolBar
{
// �R���X�g���N�V����
public:
	CToolBarEx();
	BOOL LoadToolBar(LPCTSTR lpszResourceName);
	BOOL LoadToolBar(UINT nIDResource)
	{return LoadToolBar(MAKEINTRESOURCE(nIDResource));}

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CToolBarEx)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void SetContextMenu(CMenu *pContextMenu, CWnd *pWndOwner, const BOOL bAutoDestroy = FALSE);
	void ReplaceIcons(const UINT nIDNormal, const UINT nIDDisabled, const UINT nIDHot);
	BOOL DeleteButtons(const UINT nIDDelete[], const int count);

	virtual ~CToolBarEx();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CToolBarEx)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg void OnNotifyQueryInsert(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyQueryDelete(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyBeginAdjust(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyEndAdjust(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyReset(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyToolBarChange(LPNMHDR lpNotifyStruct, LRESULT *result);
	afx_msg void OnNotifyGetButtonInfo(LPNMHDR lpNotifyStruct, LRESULT *result);
	DECLARE_MESSAGE_MAP()

private:
	int m_nItemCount;                       // �A�C�e���̑���
	int m_nButtonCount;                     // �Z�p���[�^���������{�^����

	// �R���e�L�X�g���j���[�p
	CMenu *m_pContextMenu;
	CWnd  *m_pWndOwner;
	BOOL   m_bAutoDestroy;

	// �J�X�^�}�C�Y�p
	typedef struct tagTOOLBARINFO
	{
		TBBUTTON tbButton;
		CString  strText;
	} TOOLBARINFO;
	CArray<TOOLBARINFO, TOOLBARINFO &> m_toolbarinfos;

	// �w�i�u���V�p
	CBrush m_brushBackGround;

	// �t���J���[�C���[�W�p
	typedef struct tagIMAGELISTINFO
	{
		int width, height;
		UINT flags;
		COLORREF mask;
	} IMAGELISTINFO;
	CImageList m_ilToolbar, m_ilToolbarDisabled, m_ilToolbarHot;
	BOOL CreateToolbarImageList(const UINT nIDBitmap, CImageList &il);
	BOOL GetImageListInfo(CBitmap &rBitmap, IMAGELISTINFO &info);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TOOLBAREX_H__C13B4108_0476_4D61_A452_E5ACB3A4B29D__INCLUDED_)
