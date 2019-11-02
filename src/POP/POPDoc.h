// POPDoc.h : CPOPDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_POPDOC_H__BF5410B2_332D_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_POPDOC_H__BF5410B2_332D_11D5_8205_08004605D14C__INCLUDED_

#include "Undo.h"  // ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// ドキュメントクラス
class CPOPDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CPOPDoc();
	DECLARE_DYNCREATE(CPOPDoc)

// アトリビュート
public:

// オペレーション
public:

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPOPDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	CRect m_rect;
	IMAGE_INFO m_info;
	IMAGE_DATA m_data;

	virtual ~CPOPDoc();
	BOOL PasteAsNew(HBITMAP hBitmap);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CPOPDoc)
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSaveAll();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditUndoClear();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndoClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnOperationAlphaAppend();
	afx_msg void OnOperationAlphaInverse();
	afx_msg void OnOperationResize();
	afx_msg void OnOperationTrimming();
	afx_msg void OnOperationQuantize();
	afx_msg void OnUpdateOperationAlphaAppend(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOperationQuantize(CCmdUI* pCmdUI);
	afx_msg void OnFilterReapply();
	afx_msg void OnFilterResubmit();
	//}}AFX_MSG
	afx_msg void OnOperationColormode(UINT nID);
	afx_msg void OnOperationAlphaFill(UINT nID);
	afx_msg void OnOperationTurn(UINT nID);
	afx_msg void OnOperationSwap(UINT nID);
	afx_msg void OnOperationSelect(UINT nID);
	afx_msg void OnUpdateOperationColormode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOperationAlpha(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOperationSwap(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOperationSelect(CCmdUI* pCmdUI);
	afx_msg void OnFilterCommand(UINT nID);
	afx_msg void OnUpdateFilterRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilterCommand(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

private:
	static int m_select_type;
	static int m_last_filter_no;

	int   m_format;
	CUndo m_undo;

	int GetOptimumColormode(uint_t support_mode);
	bool AddToUndoBuffer(const IMAGE_INFO &info, const IMAGE_DATA &data);
	void UpdateDocument(void);
	BOOL DoFilter(const UINT filter_no, const BOOL bSetup = TRUE);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POPDOC_H__BF5410B2_332D_11D5_8205_08004605D14C__INCLUDED_)
