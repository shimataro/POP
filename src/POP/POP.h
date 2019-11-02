// POP.h : POP �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_POP_H__BF5410AA_332D_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_POP_H__BF5410AA_332D_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��
#include "setting.h"
#include <vector>                       // std::vector<>
#include <stack>                        // std::stack<>
#include <map>                          // std::map<>


// �u�t�@�C�����J���v�u���O�����ĕۑ��v�Ŏg�p����t�B���^
#define TCHAR_FILTER_DELIMITER      _T('|')
#define TCHAR_FILTER_EXT_DELIMITER  _T(';')

// ���䕶���i�����萔�j
#define TCHAR_CTRL_NIL              _T('\0')

// ����p�b�N/�v���O�C��/�w���v�t�@�C��
#define TCHAR_LANGUAGE_DIRECTORY    _T("languages")
#define TCHAR_LANGUAGE_WILDCARD     _T("*.dll")

#define TCHAR_PLUGIN_DIRECTORY      _T("plugins")
#define TCHAR_PLUGIN_WILDCARD       _T("*.dll")

#define TCHAR_HELP_DIRECTORY        _T("help")


////////////////////////////////////////////////////////////////////////////////
// ���W�X�g���֌W

// �v���O�C��
#define STRING_REG_SECTION_PLUGIN    _T("Software\\SHIMATARO\\Photo Operator\\Plugin Setting")

// �A�v���P�[�V����
#define TCHAR_REG_COMPANY               _T("SHIMATARO")
#define TCHAR_REG_APPLICATION           _T("Photo Operator")


// �t�H�[�}�b�gID�̍ŏ��l�A�ő�l
#define MAX_FORMAT_COUNT        900
#define ID_SETTING_FORMAT_MIN   (ID_SETTING_ENVIRONMENT + 1)
#define ID_SETTING_FORMAT_MAX   (ID_SETTING_FORMAT_MIN + MAX_FORMAT_COUNT - 1)

// �t�B���^ID�̍ŏ��l�A�ő�l
#define MAX_FILTER_COUNT        900
#define ID_FILTER_MIN   (ID_FILTER_REAPPLY + 2)	// ID_FILTER_REAPPLY, ID_FILTER_RESUBMIT�̂Ԃ������
#define ID_FILTER_MAX   (ID_FILTER_MIN + MAX_FILTER_COUNT - 1)


/////////////////////////////////////////////////////////////////////////////
// CPOPApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� POP.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CPOPApp : public CWinApp
{
public:
	CPOPApp();

	void GetHMenuArray(HMENU hMenuArray[], const int nMenuMaxCount);
	void ReflectSetting(void);
	void GetSetting(SETTING &setting) {setting = m_setting;}

	void NewImageFromClipboard(void);
	void NewImageFromWindow(HWND hWnd, const BOOL bAltogether, const DWORD dwMilliSeconds);

	////////////////////////////////////////
	// �v���O�C���֘A
	BOOL InstallPlugin(HMODULE hModule, HKEY hKeyPlugin);

	UINT GetPluginCount(void) const {return m_plugin_count;}
	const PLUGIN_INFO &GetPluginInfo(const UINT num) const
	{ASSERT(num < m_plugin_count); return m_plugin_info[num];}

	UINT GetPluginFilterCount(void) const {return m_plugin_filter_count;}
	UINT GetPluginFormatCount(void) const {return m_plugin_format_count;}
	UINT GetPluginFilterDetailCount(void) const {return m_plugin_filter_detail_count;}
	UINT GetPluginFormatDetailCount(void) const {return m_plugin_format_detail_count;}

	// num�Ԗڂ̃t�B���^�v���O�C���\���̂��擾
	const FILTERPLUGIN &GetPluginFilter(const UINT num) const
	{ASSERT(num < m_plugin_filter_count); return m_plugin_filter[num];}

	const FILTERPLUGIN_DETAIL &GetPluginFilterDetail(const UINT num) const
	{ASSERT(num < m_plugin_filter_detail_count); return m_plugin_filter_detail[num];}

	// num�Ԗڂ̃t�H�[�}�b�g�v���O�C���\���̂��擾
	const FORMATPLUGIN &GetPluginFormat(const UINT num) const
	{ASSERT(num < m_plugin_format_count); return m_plugin_format[num];}

	const FORMATPLUGIN_DETAIL &GetPluginFormatDetail(const UINT num) const
	{ASSERT(num < m_plugin_format_detail_count); return m_plugin_format_detail[num];}

	// �R�����_�C�A���O�Ŏg�p����t�B���^���擾
	CString GetFileFilterLoad(void) const;
	CString GetFileFilterSave(void) const;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CPOPApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CPOPApp)
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg void OnFileCloseAll();
	afx_msg void OnUpdateFileCloseAll(CCmdUI* pCmdUI);
	afx_msg void OnFileImportClipboard();
	afx_msg void OnUpdateFileImportClipboard(CCmdUI* pCmdUI);
	afx_msg void OnFileImportDisplay();
	afx_msg void OnSettingEnvironment();
	afx_msg void OnHelpWebpage();
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	afx_msg void OnSettingFormat(UINT nID);
	afx_msg void OnUpdateSettingFormat(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

private:
	// �ݒ�
	int     m_setting_category;
	SETTING m_setting;

	HINSTANCE m_hResource;
	BOOL SetResource(LPCTSTR lpszResourceName);

	// ��荞��
	UINT m_nClipboardCount;
	UINT m_nDisplayCount;
	void ImportFromHandle(HBITMAP hBitmap, LPCTSTR lpszTitle);

	// CFileDialog::m_ofn::m_nFilterIndex�̒l�i�u�t�@�C���v���u�J���v�Ŏg�p�j
	DWORD m_nFilterIndex;

	////////////////////////////////////////
	// �v���O�C���֘A
	UINT                     m_plugin_count;
	std::vector<PLUGIN_INFO> m_plugin_info;
	std::stack<HMODULE>      m_plugin_handle_stack;

	// �t�B���^�v���O�C��
	HMENU m_hFilterMenu;
	std::map<CString, HMENU>  m_plugin_filter_map;  // ���j���[�}�b�v�i�W�������̐����Ɏg�p�j
	UINT                      m_plugin_filter_count;
	std::vector<FILTERPLUGIN> m_plugin_filter;      // �t�B���^�v���O�C���֐��̊i�[�p
	UINT                      m_plugin_filter_detail_count;
	std::vector<FILTERPLUGIN_DETAIL>   m_plugin_filter_detail; // �t�B���^���̊i�[�p

	// �t�H�[�}�b�g�v���O�C��
	CString m_file_filter_load;             // 
	CString m_file_filter_load_all;         // 
	CString m_file_filter_save;             // 
	HMENU m_hFormatMenuMainframe;
	HMENU m_hFormatMenuPoptype;
	UINT                             m_plugin_format_count;           // 
	std::vector<FORMATPLUGIN>        m_plugin_format;                 // �t�H�[�}�b�g�v���O�C���֐��̊i�[�p
	UINT                             m_plugin_format_detail_count;    // 
	std::vector<FORMATPLUGIN_DETAIL> m_plugin_format_detail;          // �t�H�[�}�b�g�v���O�C���֐��̊i�[�p

	BOOL ReadPluginInfo(HMODULE hModule, PLUGIN_INFO &plugin_info, HKEY hKeyPlugin);
	void LoadPlugin(void);
	void FreePlugin(void);
	int  LoadPluginFormat(HMODULE hModule);
	int  LoadPluginFilter(HMODULE hModule);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_POP_H__BF5410AA_332D_11D5_8205_08004605D14C__INCLUDED_)
