// POP.cpp : �A�v���P�[�V�����p�N���X�̋@�\��`���s���܂��B
//

#include "stdafx.h"
#include "POP.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "POPDoc.h"
#include "POPView.h"

#include "winutil.h"                    // GvxPathAppendToModuleDir(), GvxGetFileVersion(), GvxCHAR2TCHAR()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPOPApp

BEGIN_MESSAGE_MAP(CPOPApp, CWinApp)
	//{{AFX_MSG_MAP(CPOPApp)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_CLOSE_ALL, OnFileCloseAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE_ALL, OnUpdateFileCloseAll)
	ON_COMMAND(ID_FILE_IMPORT_CLIPBOARD, OnFileImportClipboard)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_CLIPBOARD, OnUpdateFileImportClipboard)
	ON_COMMAND(ID_FILE_IMPORT_DISPLAY, OnFileImportDisplay)
	ON_COMMAND(ID_SETTING_ENVIRONMENT, OnSettingEnvironment)
	ON_COMMAND(ID_HELP_WEBPAGE, OnHelpWebpage)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// �W���̈���Z�b�g�A�b�v �R�}���h
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)

	// �W���̃w���v�R�}���h
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)

	ON_COMMAND_RANGE          (ID_SETTING_FORMAT_MIN, ID_SETTING_FORMAT_MAX, OnSettingFormat)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SETTING_FORMAT_MIN, ID_SETTING_FORMAT_MAX, OnUpdateSettingFormat)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// �萔

// �����E������
#define TCHAR_DEFAULT_ALL_READ      _T("*.")

// std::vector<>�̗\��l
#define RESERVE_FILTER  128
#define RESERVE_FORMAT  32
#define RESERVE_PLUGIN  (RESERVE_FILTER + RESERVE_FORMAT)

#define TCHAR_WEBPAGE   _T("http://shima-t.cool.ne.jp/")


/////////////////////////////////////////////////////////////////////////////
// CPOPApp �N���X�̍\�z

CPOPApp::CPOPApp()
{
	m_nClipboardCount = 1;
	m_nDisplayCount   = 1;

	m_nFilterIndex    = 1;

	// ���\�[�X�n���h��
	m_hResource = NULL;

	// std::vector<>�̃������\��
	m_plugin_filter.reserve(RESERVE_FILTER);
	m_plugin_filter_detail.reserve(RESERVE_FILTER);

	m_plugin_format.reserve(RESERVE_FORMAT);
	m_plugin_format_detail.reserve(RESERVE_FORMAT);

	m_plugin_info.reserve(RESERVE_PLUGIN);
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CPOPApp �I�u�W�F�N�g

CPOPApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPOPApp �N���X�̏�����

BOOL CPOPApp::InitInstance()
{
	if(!CWinApp::InitInstance())
	{
		return FALSE;
	}

	InitCommonControls();
	AfxEnableControlContainer();
	if(!AfxOleInit())
	{
		return FALSE;
	}

	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������
	// ��������Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ���
	// ���폜���Ă��������B

	AfxEnableControlContainer();
/*
#ifdef _AFXDLL
	Enable3dControls();    // ���L DLL �̒��� MFC ���g�p����ꍇ�ɂ͂������Ăяo���Ă��������B
#else
	Enable3dControlsStatic();  // MFC �ƐÓI�Ƀ����N���Ă���ꍇ�ɂ͂������Ăяo���Ă��������B
#endif
*/
	// ���W�X�g���L�[�o�^
	SetRegistryKey(TCHAR_REG_COMPANY);

	// �ݒ�̃��[�h
	setting_load(m_setting_category, m_setting);
	LoadStdProfileSettings(m_setting.basic.mru_count);
	SetResource(m_setting.display.language.resource_name);

	// �h�L�������g�e���v���[�g�̓o�^
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_POPTYPE,
		RUNTIME_CLASS(CPOPDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CPOPView));
	AddDocTemplate(pDocTemplate);

	// ���C�� MDI �t���[�� �E�B���h�E���쐬
	CMainFrame* pMainFrame = new CMainFrame;
	if(!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// �h���b�O&�h���b�v������
	pMainFrame->DragAcceptFiles(TRUE);

	// �v���O�C����ǂݍ���
	LoadPlugin();

	// ���ׂĂ̈������t�@�C���Ƃ݂Ȃ��ĊJ���i�I�v�V�����ɑΉ����Ă��Ȃ��j
	for(int i = 1; i < __argc; i++)
	{
		OpenDocumentFile(__targv[i]);
	}

	return TRUE;
}

int CPOPApp::ExitInstance() 
{
	// �v���O�C�����J��
	FreePlugin();

	// �ݒ��ۑ�
	setting_save(m_setting_category, m_setting);

	// ���\�[�X�n���h�����J��
	if(m_hResource != NULL)
	{
		::FreeLibrary(m_hResource);
		m_hResource = NULL;
	}

	return CWinApp::ExitInstance();
}

// �w���v�t�@�C���̌Ăяo��
void CPOPApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	HH_FTS_QUERY query = {sizeof(query)};

	// �R�}���h���擾
	UINT uCommand = HH_DISPLAY_TOPIC;
	switch(nCmd)
	{
	case HELP_CONTEXT:                      // �ڎ�
		uCommand = HH_DISPLAY_TOC;
		break;

	case HELP_INDEX:                        // �L�[���[�h
		uCommand = HH_DISPLAY_INDEX;
		break;

	case HELP_FINDER:                       // ����
		uCommand = HH_DISPLAY_SEARCH;
		dwData = reinterpret_cast<DWORD>(&query);
		break;
	}

	// HTML�w���v�̃p�X�����쐬
	CString filename;
	filename.LoadString(IDS_HELPFILE);

	TCHAR szHelpPath[_MAX_PATH];
	GvxPathAppendToModuleDir(
		AfxGetInstanceHandle(),
		szHelpPath, SIZE_OF_ARRAY(szHelpPath),
		TCHAR_HELP_DIRECTORY,
		(LPCTSTR)filename,
		NULL);

	// HTML�w���v���Ăяo��
	::HtmlHelp(NULL, szHelpPath, uCommand, dwData);
}

// ���\�[�X�t�@�C����ݒ�
BOOL CPOPApp::SetResource(LPCTSTR lpszResourceName)
{
	// EXE�t�@�C���̃t���p�X���擾
	TCHAR szExePathName[_MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(), szExePathName, SIZE_OF_ARRAY(szExePathName));

	// ����p�b�N�̃t���p�X���擾
	TCHAR szResourcePath[_MAX_PATH];
	GvxPathAppendToModuleDir(
		AfxGetInstanceHandle(),
		szResourcePath, SIZE_OF_ARRAY(szResourcePath),
		TCHAR_LANGUAGE_DIRECTORY,
		lpszResourceName,
		NULL);

	// �{�̂ƌ���p�b�N�̃v���_�N�g�o�[�W�������r
	VS_FIXEDFILEINFO vffi_this, vffi_resource;
	GvxGetFileVersion(szExePathName , &vffi_this    , NULL, 0, NULL);
	GvxGetFileVersion(szResourcePath, &vffi_resource, NULL, 0, NULL);
	if(vffi_this.dwProductVersionMS != vffi_resource.dwProductVersionMS
	|| vffi_this.dwProductVersionLS != vffi_resource.dwProductVersionLS)
	{
		return FALSE;
	}

	// ����p�b�N�����[�h
	m_hResource = ::LoadLibrary(szResourcePath);
	if(szResourcePath == NULL)
	{
		return FALSE;
	}

	// ����p�b�N���C���X�g�[��
	AfxSetResourceHandle(m_hResource);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CPOPApp ���b�Z�[�W �n���h��

// �u�t�@�C���v���u���ׂĕ���v
void CPOPApp::OnFileCloseAll() 
{
	// ���ׂẴh�L�������g�e���v���[�g���܂��
	POSITION doctemplate_pos = GetFirstDocTemplatePosition();
	while(doctemplate_pos != NULL)
	{
		CDocTemplate *pDocTemplate = GetNextDocTemplate(doctemplate_pos);

		// ���ׂẴh�L�������g���܂��
		POSITION doc_pos = pDocTemplate->GetFirstDocPosition();
		while(doc_pos != NULL)
		{
			CDocument *pDocument = pDocTemplate->GetNextDoc(doc_pos);

			// �h�L�������g�����
			if(pDocument->SaveModified())
			{
				pDocument->OnCloseDocument();
			}
		}
	}
}

void CPOPApp::OnUpdateFileCloseAll(CCmdUI* pCmdUI) 
{
	// ���ׂẴh�L�������g�e���v���[�g���܂��
	POSITION pos = GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		CDocTemplate *pDocTemplate = GetNextDocTemplate(pos);
		ASSERT(pDocTemplate != NULL);

		// �h�L�������g��1�ł�����ΑI���\�ɂ���
		if(pDocTemplate->GetFirstDocPosition() != NULL)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}

	// �h�L�������g��1���Ȃ���ΑI��s�ɂ���
	pCmdUI->Enable(FALSE);
}

// �u�w���v�v���u��҂̃z�[���y�[�W��\���v
void CPOPApp::OnHelpWebpage() 
{
	::ShellExecute(
		NULL, NULL,
		TCHAR_WEBPAGE,
		NULL, NULL,
		SW_SHOWNORMAL);
}

// �u�w���v�v���uPhoto Operator�̃o�[�W�������v
#include "AboutDlg.h"
void CPOPApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// ���j���[���ő�nMenuMaxCount�擾
void CPOPApp::GetHMenuArray(HMENU hMenuArray[], const int nMenuMaxCount)
{
	ASSERT(nMenuMaxCount > 0);

	// ���C���t���[���E�C���h�E���擾
	CFrameWnd *pFrameWnd = STATIC_DOWNCAST(CFrameWnd, GetMainWnd());
	ASSERT(pFrameWnd != NULL);

	// ���C���t���[���E�C���h�E�̃��j���[���擾
	hMenuArray[0] = pFrameWnd->m_hMenuDefault;

	// �h�L�������g�e���v���[�g���擾
	POSITION pos = GetFirstDocTemplatePosition();
	for(int i = 1; i < nMenuMaxCount && pos != NULL; i++)
	{
		CMultiDocTemplate *pMultiDocTemplate = STATIC_DOWNCAST(CMultiDocTemplate, GetNextDocTemplate(pos));
		ASSERT(pMultiDocTemplate != NULL);

		hMenuArray[i] = pMultiDocTemplate->m_hMenuShared;
	}
}

// �ݒ�𔽉f
void CPOPApp::ReflectSetting()
{
	// ���ׂẴh�L�������g�e���v���[�g���܂��
	POSITION doctemplate_pos = GetFirstDocTemplatePosition();
	while(doctemplate_pos != NULL)
	{
		CDocTemplate *pDocTemplate = GetNextDocTemplate(doctemplate_pos);

		// ���ׂẴh�L�������g���܂��
		POSITION doc_pos = pDocTemplate->GetFirstDocPosition();
		while(doc_pos != NULL)
		{
			// �h�L�������g���X�V
			CDocument *pDocument = pDocTemplate->GetNextDoc(doc_pos);
			pDocument->UpdateAllViews(NULL);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// �摜�̎�荞��

// �N���b�v�{�[�h����V�K�摜���쐬
void CPOPApp::NewImageFromClipboard(void)
{
	// �N���b�v�{�[�h���J��
	const BOOL clip_status = ::OpenClipboard(NULL);
	if(!clip_status)
	{
		return;
	}

	// �N���b�v�{�[�h����r�b�g�}�b�v���擾
	HBITMAP hBitmap = reinterpret_cast<HBITMAP>(::GetClipboardData(CF_BITMAP));
	ASSERT(hBitmap != NULL);

	// �^�C�g�����쐬
	CString title;
	title.Format(IDS_DOCTITLE_CLIPBOARD, m_nClipboardCount++);

	// �C���[�W����荞��
	ImportFromHandle(hBitmap, title);

	// �㏈��
	::CloseClipboard();
}

// �E�C���h�E����V�K�摜���쐬
void CPOPApp::NewImageFromWindow(HWND hWnd, const BOOL bAltogether, const DWORD dwMilliSeconds)
{
	// �E�G�C�g
	::Sleep(dwMilliSeconds);

	// ��荞�ރE�C���h�E���A�N�e�B�u�ɂ���
	if(!::IsWindow(hWnd))
	{
		return;
	}
	::SetForegroundWindow(hWnd);
	::UpdateWindow(hWnd);

	// �E�C���h�E�n���h������r�b�g�}�b�v���擾
	if(TRUE)
	{
		CBitmap bitmap;
		bitmap.Attach(GetWindowBitmap(hWnd, bAltogether));
		if((HBITMAP)bitmap == NULL)
		{
			return;
		}

		// �^�C�g�����쐬
		CString title;
		title.Format(IDS_DOCTITLE_DISPLAY, m_nDisplayCount++);

		// �C���[�W����荞��
		ImportFromHandle(bitmap, title);
	}
}


////////////////////////////////////////////////////////////////////////////////
// �v���O�C���֘A

// hModule�ŗ^����ꂽ�v���O�C�����C���X�g�[��
BOOL CPOPApp::InstallPlugin(HMODULE hModule, HKEY hKeyPlugin)
{
	PLUGIN_INFO plugin;
	if(!ReadPluginInfo(hModule, plugin, hKeyPlugin))
	{
		// �v���O�C���n���h�����J��
		::FreeLibrary(hModule);
		return FALSE;
	}

	// �C���X�g�[��
	const int filter_count = LoadPluginFilter(hModule);
	const int format_count = LoadPluginFormat(hModule);

	// �v���O�C���֐����܂܂�Ă��Ȃ������ꍇ
	if(filter_count == -1
	&& format_count == -1)
	{
		// �v���O�C���n���h�����J��
		::FreeLibrary(hModule);
		return FALSE;
	}

	// �v���O�C���n���h�����X�^�b�N�ɐς�
	m_plugin_handle_stack.push(hModule);

	m_plugin_info.push_back(plugin);
	m_plugin_count++;

	return TRUE;
}

// �u�J���v���j���[�Ŏg�p����t�@�C���t�B���^���擾
CString CPOPApp::GetFileFilterLoad(void) const
{
	CString filter;
	filter.Format(
		IDS_FORMAT_OPEN,                        // %s���ׂẲ摜̧��%c%s%c���ׂĂ�̧��(*.*)%c*.*%c%c
		m_file_filter_load,
		TCHAR_FILTER_DELIMITER,
		m_file_filter_load_all,
		TCHAR_FILTER_DELIMITER, TCHAR_FILTER_DELIMITER, TCHAR_FILTER_DELIMITER, TCHAR_FILTER_DELIMITER);

	return filter;
}

// �u�ۑ��v���j���[�Ŏg�p����t�@�C���t�B���^���擾
CString CPOPApp::GetFileFilterSave(void) const
{
	return m_file_filter_save;
}


////////////////////////////////////////////////////////////////////////////////
// private�֐�

// �r�b�g�}�b�v�n���h������摜����荞��
void CPOPApp::ImportFromHandle(HBITMAP hBitmap, LPCTSTR lpszTitle)
{
	// CPOPDoc �̃h�L�������g�e���v���[�g���擾
	POSITION doctemplate_pos   = GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = GetNextDocTemplate(doctemplate_pos);

	// �V�K�h�L�������g���쐬
	CDocument *pDocument = pDocTemplate->CreateNewDocument();
	CPOPDoc *pPOPDoc = STATIC_DOWNCAST(CPOPDoc, pDocument);
	ASSERT(pPOPDoc != NULL);

	// �V�K�쐬�ɐ��������ꍇ
	if(pPOPDoc->PasteAsNew(hBitmap))
	{
		// �^�C�g����ݒ�
		pPOPDoc->SetTitle(lpszTitle);

		// �V�K�t���[�����쐬
		CFrameWnd *pFrameWnd = pDocTemplate->CreateNewFrame(pPOPDoc, NULL);
		pDocTemplate->InitialUpdateFrame(pFrameWnd, pPOPDoc);
	}
	// ���s�����ꍇ
	else
	{
		// �h�L�������g�e���v���[�g���X�g����폜
		pDocTemplate->RemoveDocument(pPOPDoc);

		// �쐬�����h�L�������g��j��
		pPOPDoc->DeleteContents();
		if(pPOPDoc->m_bAutoDelete)
		{
			delete pPOPDoc;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// �v���O�C��

// hModule�ŗ^����ꂽ�v���O�C���̏����擾
BOOL CPOPApp::ReadPluginInfo(HMODULE hModule, PLUGIN_INFO &plugin, HKEY hKeyPlugin)
{
	GETPRODUCT  get_product  = reinterpret_cast<GETPRODUCT >(::GetProcAddress(hModule, funcname_get_product));
	SETREGISTRY set_registry = reinterpret_cast<SETREGISTRY>(::GetProcAddress(hModule, funcname_set_registry));
	SETUP       setup        = reinterpret_cast<SETUP      >(::GetProcAddress(hModule, funcname_setup));
	if(get_product == NULL || set_registry == NULL)
	{
		return FALSE;
	}

	// �v���_�N�g�����擾
	const UINT author_max      = SIZE_OF_ARRAY(plugin.author);
	const UINT description_max = SIZE_OF_ARRAY(plugin.description);
	char author[author_max];
	char description[description_max];
	const DWORD version = get_product(
		author     , author_max,
		description, description_max);

	// TCHAR������ɕϊ�
	GvxCHAR2TCHAR(author     , plugin.author     , author_max);
	GvxCHAR2TCHAR(description, plugin.description, description_max);

	// �o�[�W���������i�[
	plugin.version = version;

	// �t�@�C�������i�[
	TCHAR path[_MAX_PATH], fname[_MAX_FNAME], ext[_MAX_EXT];
	::GetModuleFileName(hModule, path, SIZE_OF_ARRAY(path));
	_tsplitpath(path, NULL, NULL, fname, ext);
	_tmakepath(plugin.filename, NULL, NULL, fname, ext);

	// ���W�X�g���̐ݒ�
	set_registry(hKeyPlugin);

	// �Z�b�g�A�b�v�֐����擾
	plugin.setup = reinterpret_cast<SETUP>(setup);

	return TRUE;
}

// �v���O�C����ǂݍ���
void CPOPApp::LoadPlugin()
{
	HMENU hMenuArray[2];
	GetHMenuArray(hMenuArray, SIZE_OF_ARRAY(hMenuArray));

	// �ϐ��̏�����
	if(TRUE)
	{
		m_plugin_count = 0;
		m_plugin_format_count = 0;
		m_plugin_filter_count = 0;
		m_plugin_format_detail_count = 0;
		m_plugin_filter_detail_count = 0;
	}

	// �t�H�[�}�b�g�v���O�C�����j���[���擾
	m_hFormatMenuMainframe = GvxFindMenu(hMenuArray[0], ID_SETTING_ENVIRONMENT, NULL);
	m_hFormatMenuPoptype   = GvxFindMenu(hMenuArray[1], ID_SETTING_ENVIRONMENT, NULL);
	ASSERT(m_hFormatMenuMainframe != NULL);
	ASSERT(m_hFormatMenuPoptype != NULL);

	// �t�B���^�v���O�C���p���j���[���擾
	m_hFilterMenu = GvxFindMenu(hMenuArray[1], ID_FILTER_REAPPLY, NULL);
	ASSERT(m_hFilterMenu != NULL);

	// �g�ݍ��݃t�H�[�}�b�g�����[�h
	if(TRUE)
	{
		const int format_count = LoadPluginFormat(AfxGetInstanceHandle());
		ASSERT(format_count != -1);
	}

	// �v���O�C�������[�h
	if(TRUE)
	{
		// �v���O�C���̐ݒ�ۑ��p���W�X�g��
		HKEY hKeyPlugin = NULL;
		::RegCreateKey(HKEY_CURRENT_USER, STRING_REG_SECTION_PLUGIN, &hKeyPlugin);

		// �v���O�C��������
		TCHAR szFindPath[_MAX_PATH];
		GvxPathAppendToModuleDir(
			AfxGetInstanceHandle(),
			szFindPath, SIZE_OF_ARRAY(szFindPath),
			TCHAR_PLUGIN_DIRECTORY,
			TCHAR_PLUGIN_WILDCARD,
			NULL);

		CFileFind find;
		BOOL bFound = find.FindFile(szFindPath);

		while(bFound)
		{
			bFound = find.FindNextFile();

			// ���������v���O�C�������[�h
			HMODULE hModule = ::LoadLibrary(find.GetFilePath());
			if(hModule != NULL)
			{
				InstallPlugin(hModule, hKeyPlugin);
			}
		}

		// ���W�X�g���L�[�����
		::RegCloseKey(hKeyPlugin);
	}

	// �u�J���v�_�C�A���O�{�b�N�X�̃t�B���^�C���f�b�N�X
	m_nFilterIndex = m_plugin_format_detail_count + 1;
}

// �v���O�C�����J��
void CPOPApp::FreePlugin()
{
	// �v���O�C���n���h�������
	while(!m_plugin_handle_stack.empty())
	{
		// �X�^�b�N�̐擪����v���O�C���n���h�������o���ĊJ��
		HMODULE hModule = m_plugin_handle_stack.top();
		::FreeLibrary(hModule);

		// �X�^�b�N��1���炷
		m_plugin_handle_stack.pop();
	}
	m_plugin_filter.clear();        m_plugin_filter_count = 0;
	m_plugin_filter_detail.clear(); m_plugin_filter_detail_count = 0;
	m_plugin_format.clear();        m_plugin_format_count = 0;
	m_plugin_format_detail.clear(); m_plugin_format_detail_count = 0;
	m_plugin_info.clear();          m_plugin_count = 0;
}

// �t�B���^�v���O�C����ǂݍ���
int CPOPApp::LoadPluginFilter(HMODULE hModule)
{
	// �v���O�C�����̊֐����擾
	FILTERFUNC_COUNT      get_count  = reinterpret_cast<FILTERFUNC_COUNT     >(::GetProcAddress(hModule, funcname_filter_count));
	FILTERFUNC_INFO       get_info   = reinterpret_cast<FILTERFUNC_INFO      >(::GetProcAddress(hModule, funcname_filter_info));
	FILTERFUNC_EXECUTABLE executable = reinterpret_cast<FILTERFUNC_EXECUTABLE>(::GetProcAddress(hModule, funcname_filter_executable));
	FILTERFUNC_EFFECT     effect     = reinterpret_cast<FILTERFUNC_EFFECT    >(::GetProcAddress(hModule, funcname_filter_effect));
	FILTERFUNC_OPTION     option     = reinterpret_cast<FILTERFUNC_OPTION    >(::GetProcAddress(hModule, funcname_filter_option));

	// �v���O�C���Ɋ܂܂�Ă���֐����m�F
	if(get_count == NULL || get_info == NULL || executable == NULL || effect == NULL || option == NULL)
	{
		return -1;
	}

	// �܂܂�Ă���t�B���^�̐������J��Ԃ�
	const int filter_count_available = MAX_FILTER_COUNT - m_plugin_filter_count;
	const int filter_count           = min(get_count(), filter_count_available);
	for(int i = 0; i < filter_count; i++)
	{
		FILTERPLUGIN_DETAIL filter_detail;
		filter_detail.plugin_no = m_plugin_filter_count;    // �ǂ̃v���O�C����
		filter_detail.filter_no = i;                        // ���Ԗڂ�

		// �v���O�C���̏����擾
		char genre[20];                         // �v���O�C���̃W������
		char name[40];                          // �v���O�C����
		filter_detail.function_flag = get_info(
			i,
			genre, SIZE_OF_ARRAY(genre),
			name , SIZE_OF_ARRAY(name));

		// TCHAR������ɕϊ�
		GvxCHAR2TCHAR(genre, filter_detail.genre, SIZE_OF_ARRAY(filter_detail.genre));
		GvxCHAR2TCHAR(name , filter_detail.name , SIZE_OF_ARRAY(filter_detail.name));

		// �W���������̃T�u���j���[���Ȃ�������V�������A�t�B���^���j���[�̍Ō�ɒǉ�
		HMENU &hSubMenu = m_plugin_filter_map[filter_detail.genre];
		if(hSubMenu == NULL)
		{
			hSubMenu = ::CreatePopupMenu();
			ASSERT(hSubMenu != NULL);
			::AppendMenu(
				m_hFilterMenu,
				MF_POPUP | MF_STRING,
				reinterpret_cast<UINT>(hSubMenu),
				filter_detail.genre);
		}
		// �T�u���j���[�̍Ō�Ƀt�B���^���̃��j���[��ǉ�
		::AppendMenu(
			hSubMenu,
			MF_STRING,
			ID_FILTER_MIN + m_plugin_filter_detail_count,
			filter_detail.name);

		// �t�B���^���X�g�ɒǉ�
		m_plugin_filter_detail.push_back(filter_detail);
		m_plugin_filter_detail_count++;
	}

	if(filter_count > 0)
	{
		FILTERPLUGIN plugin;
		plugin.count      = filter_count;
		plugin.executable = executable;
		plugin.effect     = effect;
		plugin.option     = option;

		// �v���O�C�����X�g�ɒǉ�
		m_plugin_filter.push_back(plugin);
		m_plugin_filter_count++;
	}

	return filter_count;
}

// �t�H�[�}�b�g�v���O�C����ǂݍ���
int CPOPApp::LoadPluginFormat(HMODULE hModule)
{
	// �v���O�C�����̊֐����擾
	FORMATFUNC_COUNT  get_count     = reinterpret_cast<FORMATFUNC_COUNT >(::GetProcAddress(hModule, funcname_format_count));
	FORMATFUNC_INFO   get_info      = reinterpret_cast<FORMATFUNC_INFO  >(::GetProcAddress(hModule, funcname_format_info));
	FORMATFUNC_BITS   get_bits      = reinterpret_cast<FORMATFUNC_BITS  >(::GetProcAddress(hModule, funcname_format_support_mode));
	FORMATFUNC_IMPORT format_import = reinterpret_cast<FORMATFUNC_IMPORT>(::GetProcAddress(hModule, funcname_format_import));
	FORMATFUNC_EXPORT format_export = reinterpret_cast<FORMATFUNC_EXPORT>(::GetProcAddress(hModule, funcname_format_export));
	FORMATFUNC_OPTION format_option = reinterpret_cast<FORMATFUNC_OPTION>(::GetProcAddress(hModule, funcname_format_option));

	// �v���O�C���Ɋ܂܂�Ă���֐����m�F
	if(get_count == NULL || get_info == NULL || get_bits == NULL || format_import == NULL || format_export == NULL || format_option == NULL)
	{
		return -1;
	}

	// �܂܂�Ă���t�H�[�}�b�g�̐������J��Ԃ�
	const int format_count_available = MAX_FORMAT_COUNT - m_plugin_format_count;
	const int format_count           = min(get_count(), format_count_available);
	for(int i = 0; i < format_count; i++)
	{
		FORMATPLUGIN_DETAIL detail;
		detail.plugin_no = m_plugin_format_count;     // �ǂ̃v���O�C����
		detail.format_no = i;                         // ���Ԗڂ�

		// �v���O�C���̏����擾
		char name[10];
		char ext[10];
		char filter[50];
		char expression[50];
		detail.included_functions = get_info(
			i,
			name      , SIZE_OF_ARRAY(name),
			ext       , SIZE_OF_ARRAY(ext),
			filter    , SIZE_OF_ARRAY(filter),
			expression, SIZE_OF_ARRAY(expression));

		// TCHAR������ɕϊ�
		GvxCHAR2TCHAR(name      , detail.name      , SIZE_OF_ARRAY(detail.name));
		GvxCHAR2TCHAR(ext       , detail.ext       , SIZE_OF_ARRAY(detail.ext));
		GvxCHAR2TCHAR(filter    , detail.filter    , SIZE_OF_ARRAY(detail.filter));
		GvxCHAR2TCHAR(expression, detail.expression, SIZE_OF_ARRAY(detail.expression));

		detail.support_mode = get_bits(i);

		// ���j���[�̍Ō�Ƀt�H�[�}�b�g���j���[�̕������ǉ�
		const UINT menu_id = ID_SETTING_FORMAT_MIN + m_plugin_format_detail_count;
		CString setting_string;
		setting_string.Format(IDS_FORMAT_SETTING, detail.name);
		::AppendMenu(m_hFormatMenuMainframe, MF_STRING, menu_id, setting_string);
		::AppendMenu(m_hFormatMenuPoptype  , MF_STRING, menu_id, setting_string);

		// �g���q��A��
		if(detail.included_functions & IF_IMPORT)
		{
			m_file_filter_load = m_file_filter_load
				+ detail.expression
				+ TCHAR_FILTER_DELIMITER
				+ detail.filter
				+ TCHAR_FILTER_DELIMITER;

			m_file_filter_load_all = m_file_filter_load_all
				+ detail.filter
				+ TCHAR_FILTER_EXT_DELIMITER;
		}
		if(detail.included_functions & IF_EXPORT)
		{
			m_file_filter_save = m_file_filter_save
				+ detail.expression
				+ TCHAR_FILTER_DELIMITER
				+ detail.filter
				+ TCHAR_FILTER_DELIMITER;
		}

		// �t�H�[�}�b�g���X�g�ɒǉ�
		m_plugin_format_detail.push_back(detail);
		m_plugin_format_detail_count++;
	}

	if(format_count > 0)
	{
		FORMATPLUGIN plugin;
		plugin.count  = format_count;
		plugin.import = format_import;
		plugin.export = format_export;
		plugin.option = format_option;

		// �v���O�C�����X�g�ɒǉ�
		m_plugin_format.push_back(plugin);
		m_plugin_format_count++;
	}

	// �܂܂�Ă���v���O�C���̐���Ԃ�
	return format_count;
}
