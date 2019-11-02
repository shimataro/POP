#if !defined(AFX_SETTINGDLG_H__C1131BDB_73A1_4F4C_91DB_5FD0695D01D1__INCLUDED_)
#define AFX_SETTINGDLG_H__C1131BDB_73A1_4F4C_91DB_5FD0695D01D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDlg.h : �w�b�_�[ �t�@�C��
//
#include "ToolTipDialog.h"
#include "ColorStatic.h"
#include "POP.h"	// ClassView �ɂ���Ēǉ�����܂����B
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg �_�C�A���O

class CSettingDlg : public CToolTipDialog
{
// �R���X�g���N�V����
public:
	UINT     m_setting_category;
	COLORREF m_color_background1, m_color_background2;
	CString  m_resource_name;
	CSettingDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSettingDlg)
	enum { IDD = IDD_SETTING };
	CListCtrl	m_listBasicPluginInstalled;
	CTreeCtrl	m_treeCategory;
	BOOL	m_bBasicImplantComment;
	CString	m_strBasicComment;
	BOOL	m_bBasicSaveWindowPlacement;
	UINT	m_nBasicMRUCount;
	CListCtrl	m_listDisplayLanguage;
	BOOL	m_bDisplayActivateGammaFile;
	BOOL	m_bDisplayCustomizeGammaDecode;
	double	m_dDisplayGammaDecode;
	BOOL	m_bDisplayCustomizeGammaDisplay;
	double	m_dDisplayGammaDisplay;
	int		m_nDisplayBackgroundGridSize;
	CColorStatic	m_staticDisplayBackgroundColor1;
	CColorStatic	m_staticDisplayBackgroundColor2;
	BOOL	m_bDisplayToolbarXP;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeCategory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkSettingBasicPluginListInstalled(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBasicCheckImplantComment();
	afx_msg void OnDisplayCheckCustomizeGammaDecode();
	afx_msg void OnDisplayCheckCustomizeGammaDisplay();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	// �J�e�S�����̃R���g���[���̕\��/��\��
	void ShowCategoryControls(const UINT nCategoryIndex, const BOOL bShow = TRUE);

	// �e�J�e�S���̏�����
	void InitCategoryBasic(void);
	void InitCategoryBasicPlugin(void);
	void InitCategoryDisplay(void);
	void InitCategoryDisplayLanguage(void);

	// ���X�g�R���g���[���̐ݒ�
	static void ListCtrlSetHeader(CListCtrl &rListCtrl, CFont &rFont, const UINT title_array[], const int count);
	static BOOL ListCtrlSetItem(CListCtrl &rListCtrl, const int pos, const int sub_pos, LPCTSTR text);

	// �u��{�v���u�v���O�C���v
	CFont m_fontBasicPluginTitle;
	void BasicPluginUpdateList(void);

	// �u�\���v���u����v
	CFont m_fontDisplayLanguageTitle;
	void DisplayLanguageUpdateList(void);
	int  DisplayLanguageInsertItem(const int pos);
	int  DisplayLanguageSelectItem(const CString &rString);
	BOOL DisplayLanguageGetSelectedResourceName(CString &rString);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SETTINGDLG_H__C1131BDB_73A1_4F4C_91DB_5FD0695D01D1__INCLUDED_)
