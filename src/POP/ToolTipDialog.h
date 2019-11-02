#if !defined(AFX_TOOLTIPDIALOG_H__D996FCA0_7852_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_TOOLTIPDIALOG_H__D996FCA0_7852_11D5_8205_08004605D14C__INCLUDED_

#include "POP.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTipDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CToolTipDialog �E�B���h�E

class CToolTipDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CToolTipDialog();
	CToolTipDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	CToolTipDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

// �C���v�������e�[�V����
protected:
	void SetToolTips(const UINT arIDs[], const UINT nCount);
	void SetToolTip (const UINT arID)
	{SetToolTips(&arID, 1);}

	virtual ~CToolTipDialog();
	virtual BOOL OnInitDialog();

private:
	CToolTipCtrl m_ctrlToolTip;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TOOLTIPDIALOG_H__D996FCA0_7852_11D5_8205_08004605D14C__INCLUDED_)
