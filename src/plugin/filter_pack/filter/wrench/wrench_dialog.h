// wrench_dialog.h
#ifndef __WRENCH_DIALOG__
#define __WRENCH_DIALOG__

#include "../PreviewDialog.h"
#include "wrench.h"


class wrench_dialog : public preview_dialog
{
public:
	explicit wrench_dialog(const WRENCHPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data__ptr, HWND hWndParent /* = NULL */);
	~wrench_dialog(void);

	// ねじりパラメータ
	WRENCHPARAM m_param;

protected:
	enum { IDD = IDD_OPTION_WRENCH };

	// コントロールオブジェクト
	wgc::scrollbar_ctrl m_ctrlCenterX;          // ねじり中心のX座標
	wgc::scrollbar_ctrl m_ctrlCenterY;          // ねじり中心のY座標
	wgc::static_ctrl    m_ctrlStaticCenterX;
	wgc::static_ctrl    m_ctrlStaticCenterY;

	void AssociateDialogControls(void);
	void UpdateData(const BOOL bUpdate = TRUE);
	void UpdatePreview(void);

	BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);
	void OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar);
};


/*
/////////////////////////////////////////////////////////////////////////////
// CFilterWrenchDlg ダイアログ

class CFilterWrenchDlg : public CToolTipDialog
{
// コンストラクション
public:
	WRENCHDLGPARAM * m_pParam;
	_INT32 nHeight;
	_INT32 nWidth;
	CFilterWrenchDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CFilterWrenchDlg)
	enum { IDD = IDD_FILTER_WRENCH };
	CString	m_strRadius;
	CString	m_strX;
	CString	m_strY;
	int		m_nAngle;
	int		m_nRadius;
	int		m_nX;
	int		m_nY;
	CString	m_valAngle;
	CString	m_valRadius;
	CString	m_valX;
	CString	m_valY;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFilterWrenchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFilterWrenchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
*/
#endif // __WRENCH_DIALOG__
