// SettingDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "pop.h"
#include "SettingDlg.h"
#include "MainFrm.h"

#include <map>

#include "winutil.h"                    // GvxPathAppendToModuleDir(), GvxGetFileVersion()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg �_�C�A���O


CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CToolTipDialog(CSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingDlg)
	m_bBasicImplantComment = FALSE;
	m_strBasicComment = _T("");
	m_bBasicSaveWindowPlacement = FALSE;
	m_nBasicMRUCount = 0;
	m_bDisplayActivateGammaFile = FALSE;
	m_bDisplayCustomizeGammaDecode = FALSE;
	m_dDisplayGammaDecode = 0.0;
	m_bDisplayCustomizeGammaDisplay = FALSE;
	m_dDisplayGammaDisplay = 0.0;
	m_bDisplayToolbarXP = FALSE;
	m_nDisplayBackgroundGridSize = -1;
	//}}AFX_DATA_INIT
}


void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CToolTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingDlg)
	DDX_Control(pDX, IDC_SETTING_BASIC_PLUGIN_LIST_INSTALLED, m_listBasicPluginInstalled);
	DDX_Control(pDX, IDC_SETTING_TREE_CATEGORY, m_treeCategory);
	DDX_Check(pDX, IDC_SETTING_BASIC_CHECK_IMPLANT_COMMENT, m_bBasicImplantComment);
	DDX_Text(pDX, IDC_SETTING_BASIC_EDIT_COMMENT, m_strBasicComment);
	DDX_Check(pDX, IDC_SETTING_BASIC_CHECK_SAVE_WINDOW_PLACEMENT, m_bBasicSaveWindowPlacement);
	DDX_Text(pDX, IDC_SETTING_BASIC_MRUCOUNT, m_nBasicMRUCount);
	DDV_MinMaxUInt(pDX, m_nBasicMRUCount, 0, 16);
	DDX_Control(pDX, IDC_SETTING_CATEGORY_DISPLAY_LANGUAGE_LIST, m_listDisplayLanguage);
	DDX_Check(pDX, IDC_SETTING_DISPLAY_CHECK_ACTIVATE_GAMMA_FILE, m_bDisplayActivateGammaFile);
	DDX_Check(pDX, IDC_SETTING_DISPLAY_CHECK_CUSTOMIZE_GAMMA_DECODE, m_bDisplayCustomizeGammaDecode);
	DDX_Text(pDX, IDC_SETTING_DISPLAY_GAMMA_DECODE, m_dDisplayGammaDecode);
	DDV_MinMaxDouble(pDX, m_dDisplayGammaDecode, 1.e-002, 9.99);
	DDX_Check(pDX, IDC_SETTING_DISPLAY_CHECK_CUSTOMIZE_GAMMA_DISPLAY, m_bDisplayCustomizeGammaDisplay);
	DDX_Text(pDX, IDC_SETTING_DISPLAY_GAMMA_DISPLAY, m_dDisplayGammaDisplay);
	DDV_MinMaxDouble(pDX, m_dDisplayGammaDisplay, 1.e-002, 9.99);
	DDX_CBIndex(pDX, IDC_SETTING_DISPLAY_TRANSPARENT_COMBO_SIZE, m_nDisplayBackgroundGridSize);
	DDX_Control(pDX, IDC_SETTING_DISPLAY_TRANSPARENT_STATIC_COLOR_SELECT1, m_staticDisplayBackgroundColor1);
	DDX_Control(pDX, IDC_SETTING_DISPLAY_TRANSPARENT_STATIC_COLOR_SELECT2, m_staticDisplayBackgroundColor2);
	DDX_Check(pDX, IDC_SETTING_DISPLAY_TOOBAR_XP, m_bDisplayToolbarXP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingDlg, CToolTipDialog)
	//{{AFX_MSG_MAP(CSettingDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SETTING_TREE_CATEGORY, OnSelchangedTreeCategory)
	ON_BN_CLICKED(IDC_SETTING_BASIC_CHECK_IMPLANT_COMMENT, OnBasicCheckImplantComment)
	ON_NOTIFY(NM_DBLCLK, IDC_SETTING_BASIC_PLUGIN_LIST_INSTALLED, OnDblclkSettingBasicPluginListInstalled)
	ON_BN_CLICKED(IDC_SETTING_DISPLAY_CHECK_CUSTOMIZE_GAMMA_DECODE, OnDisplayCheckCustomizeGammaDecode)
	ON_BN_CLICKED(IDC_SETTING_DISPLAY_CHECK_CUSTOMIZE_GAMMA_DISPLAY, OnDisplayCheckCustomizeGammaDisplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////
//

// �o�[�W�������Ɏg������
#define TCHAR_STR_FORMAT_VERSION_PLUGIN     _T("%d.%02d.%03d")
#define TCHAR_STR_FORMAT_VERSION_RESOURCE   _T("%d.%d.%d.%d")

// 1�J�e�S�����̍ő�R���g���[����
#define MAX_CONTROLS  16

typedef struct tagSETTING_TREE
{
	UINT id;                                // ID
	UINT id_parent;                         // �eID�i�Ȃ��ꍇ��0�j
	UINT id_control_array[MAX_CONTROLS];    // �R���g���[��ID�̔z��
} SETTING_TREE;

const SETTING_TREE setting_tree[] =
{
	// ��{�J�e�S��
	{
		IDS_SETTING_CATEGORY_BASIC,
		0,
		{
			IDC_SETTING_BASIC_CHECK_IMPLANT_COMMENT,
			IDC_SETTING_BASIC_EDIT_COMMENT,
			IDC_SETTING_BASIC_CHECK_SAVE_WINDOW_PLACEMENT,
			IDC_SETTING_BASIC_STATIC_MRUCOUNT,
			IDC_SETTING_BASIC_MRUCOUNT,
			IDC_SETTING_BASIC_SPIN_MRUCOUNT,
			0,
		},
	},
	// ��{���v���O�C��
	{
		IDS_SETTING_CATEGORY_BASIC_PLUGIN,
		IDS_SETTING_CATEGORY_BASIC,
		{
			IDC_SETTING_BASIC_PLUGIN_STATIC,
			IDC_SETTING_BASIC_PLUGIN_LIST_INSTALLED,
			0,
		},
	},
	// �\���J�e�S��
	{
		IDS_SETTING_CATEGORY_DISPLAY,
		0,
		{
			IDC_SETTING_DISPLAY_CHECK_ACTIVATE_GAMMA_FILE,
			IDC_SETTING_DISPLAY_CHECK_CUSTOMIZE_GAMMA_DECODE,
			IDC_SETTING_DISPLAY_GAMMA_DECODE,
			IDC_SETTING_DISPLAY_CHECK_CUSTOMIZE_GAMMA_DISPLAY,
			IDC_SETTING_DISPLAY_GAMMA_DISPLAY,
			IDC_SETTING_DISPLAY_GAMMA_STATIC_GROUP,
			IDC_SETTING_DISPLAY_TRANSPARENT_STATIC_COLOR,
			IDC_SETTING_DISPLAY_TRANSPARENT_STATIC_COLOR_SELECT1,
			IDC_SETTING_DISPLAY_TRANSPARENT_STATIC_COLOR_SELECT2,
			IDC_SETTING_DISPLAY_TRANSPARENT_STATIC_SIZE,
			IDC_SETTING_DISPLAY_TRANSPARENT_COMBO_SIZE,
			IDC_SETTING_DISPLAY_TRANSPARENT_STATIC_GROUP,
			IDC_SETTING_DISPLAY_TOOBAR_XP,
			IDC_SETTING_DISPLAY_TOOBAR_STATIC_GROUP,
			0,
		},
	},
	// �\��������
	{
		IDS_SETTING_CATEGORY_DISPLAY_LANGUAGE,
		IDS_SETTING_CATEGORY_DISPLAY,
		{
			IDC_SETTING_CATEGORY_DISPLAY_LANGUAGE_LIST,
			0,
		},
	},
};


////////////////////////////////////////////////////////////////////////////////
// �J�e�S�����̃R���g���[���̕\��/��\��

void CSettingDlg::ShowCategoryControls(const UINT nCategoryIndex, const BOOL bShow /* = TRUE */)
{
	// �O���[�v�{�b�N�X�ɃL���v�V������\��
	if(bShow)
	{
		CString caption;
		caption.LoadString(setting_tree[nCategoryIndex].id);

		CWnd *pWndGroup = GetDlgItem(IDC_SETTING_STATIC_GROUP);
		pWndGroup->SetWindowText(caption);
	}

	// �w�肳�ꂽ�J�e�S���̃R���g���[���̏�Ԃ�ύX
	if(TRUE)
	{
		const int nCmdShow = bShow ? SW_SHOW : SW_HIDE;
		for(int i = 0; i < SIZE_OF_ARRAY(setting_tree[nCategoryIndex].id_control_array); i++)
		{
			// �R���g���[��ID���擾
			const int id = setting_tree[nCategoryIndex].id_control_array[i];
			if(id == 0)
			{
				// ID��0�Ȃ�I��
				break;
			}

			// �R���g���[���I�u�W�F�N�g���擾
			CWnd *pWnd = GetDlgItem(id);
			if(pWnd == NULL)
			{
				// �I�u�W�F�N�g��NULL�Ȃ珈�����΂�
				continue;
			}

			// �R���g���[����\������
			pWnd->ShowWindow(nCmdShow);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// �e�J�e�S���̏�����

// �u��{�v�J�e�S���̏�����
void CSettingDlg::InitCategoryBasic(void)
{
	// �X�s���{�^���͈̔͂�ݒ�
	CSpinButtonCtrl spin;
	spin.Attach(GetDlgItem(IDC_SETTING_BASIC_SPIN_MRUCOUNT)->m_hWnd);
	spin.SetRange(0, _AFX_MRU_MAX_COUNT);
	spin.Detach();

	OnBasicCheckImplantComment();
}

// �u��{�v���u�v���O�C���v�J�e�S���̏�����
void CSettingDlg::InitCategoryBasicPlugin(void)
{
	// �e�J�����̃^�C�g����ݒ�
	if(TRUE)
	{
		const UINT title_array[] =
		{
			IDS_PLUGIN_NAME,
			IDS_PLUGIN_VERSION,
			IDS_PLUGIN_AUTHOR,
			IDS_PLUGIN_DESCRIPTION,
		};

		ListCtrlSetHeader(
			m_listBasicPluginInstalled,
			m_fontBasicPluginTitle,
			title_array, SIZE_OF_ARRAY(title_array));
	}

	BasicPluginUpdateList();
}

// �u�\���v�J�e�S���̏�����
void CSettingDlg::InitCategoryDisplay(void)
{
	OnDisplayCheckCustomizeGammaDecode();
	OnDisplayCheckCustomizeGammaDisplay();
	m_staticDisplayBackgroundColor1.SetColor(m_color_background1);
	m_staticDisplayBackgroundColor2.SetColor(m_color_background2);
}

// �u�\���v���u����v�J�e�S���̏�����
void CSettingDlg::InitCategoryDisplayLanguage(void)
{
	// �e�J�����̃^�C�g����ݒ�
	if(TRUE)
	{
		const UINT title_array[] =
		{
			IDS_LANGUAGE_MODULE,
			IDS_LANGUAGE_LANGUAGE,
			IDS_LANGUAGE_VERSION,
			IDS_LANGUAGE_AUTHOR,
		};

		ListCtrlSetHeader(
			m_listDisplayLanguage,
			m_fontDisplayLanguageTitle,
			title_array, SIZE_OF_ARRAY(title_array));
	}

	DisplayLanguageUpdateList();
	DisplayLanguageSelectItem(m_resource_name);
}


////////////////////////////////////////////////////////////////////////////////
// ���X�g�R���g���[���̐ݒ�

void CSettingDlg::ListCtrlSetHeader(CListCtrl &rListCtrl, CFont &rFont, const UINT title_array[], const int count)
{
	// �w�b�_�̃t�H���g���{�[���h�ɂ���
	if(TRUE)
	{
		CHeaderCtrl *pHeader = rListCtrl.GetHeaderCtrl();
		ASSERT(pHeader != NULL);

		LOGFONT lf;
		pHeader->GetFont()->GetLogFont(&lf);
		lf.lfWeight = FW_BOLD;

		rFont.CreateFontIndirect(&lf);
		pHeader->SetFont(&rFont);
	}

	// �e�J�����̃^�C�g���ƕ���ݒ�
	if(TRUE)
	{
		// �J�����̕��́A���X�g�R���g���[���̕���count��������
		CRect rect;
		rListCtrl.GetClientRect(&rect);
		const int width = rect.Width() / count;

		for(int i = 0; i < count; i++)
		{
			// �J�����̃^�C�g����ݒ�
			CString title;
			title.LoadString(title_array[i]);
			rListCtrl.InsertColumn(i, title);

			// �J�����̕���ݒ�
			rListCtrl.SetColumnWidth(i, width);
		}
	}
}

BOOL CSettingDlg::ListCtrlSetItem(CListCtrl &rListCtrl, const int pos, const int sub_pos, LPCTSTR text)
{
	// pos�s�Asub_pos���ڂ̃A�C�e���̃e�L�X�g��ύX
	return rListCtrl.SetItem(
		pos, sub_pos,
		LVIF_TEXT, text,
		0, 0, 0, 0);
}


////////////////////////////////////////////////////////////////////////////////
// �u��{�v���u�v���O�C���v
void CSettingDlg::BasicPluginUpdateList(void)
{
	extern CPOPApp theApp;

	const int plugin_count = theApp.GetPluginCount();
	for(int i = 0; i < plugin_count; i++)
	{
		// �v���O�C�������擾
		const PLUGIN_INFO &plugin_info = theApp.GetPluginInfo(i);

		// �A�C�e����}��
		const int pos = m_listBasicPluginInstalled.InsertItem(
			0,
			i,
			NULL, 0, 0, 0, 0);

		// �o�[�W������������쐬
		CString version;
		if(TRUE)
		{
			const WORD hi = HIWORD(plugin_info.version);
			const WORD lo = LOWORD(plugin_info.version);

			const BYTE major = HIBYTE(hi);
			const BYTE minor = LOBYTE(hi) % 100;
			const WORD build = lo % 1000;

			version.Format(TCHAR_STR_FORMAT_VERSION_PLUGIN, major, minor, build);
		}

		// �J������ݒ�
		if(TRUE)
		{
			LPCTSTR item_array[] =
			{
				plugin_info.filename,
				version,
				plugin_info.author,
				plugin_info.description
			};

			for(int j = 0; j < SIZE_OF_ARRAY(item_array); j++)
			{
				ListCtrlSetItem(m_listBasicPluginInstalled, pos, j, item_array[j]);
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// �u�\���v���u����v

void CSettingDlg::DisplayLanguageUpdateList(void)
{
	extern CPOPApp theApp;

	int item_no = 0;

	// ����p�b�N������
	TCHAR szFindPath[_MAX_PATH];
	GvxPathAppendToModuleDir(
		AfxGetInstanceHandle(),
		szFindPath, SIZE_OF_ARRAY(szFindPath),
		TCHAR_LANGUAGE_DIRECTORY,
		TCHAR_LANGUAGE_WILDCARD,
		NULL);

	CFileFind find;
	BOOL bFound = find.FindFile(szFindPath);

	while(bFound)
	{
		bFound = find.FindNextFile();

		// ������������p�b�N�����[�h
		const CString strLanguagePath = find.GetFilePath();
		HMODULE hModule = ::LoadLibrary(strLanguagePath);
		if(hModule == NULL)
		{
			continue;
		}

		// i�s�ڂ̈ʒu�ɐV�K�A�C�e����}��
		const int pos = DisplayLanguageInsertItem(item_no++);

		// �t�@�C���o�[�W�������擾
		VS_FIXEDFILEINFO vffi;
		TCHAR szLanguageName[128], szCompanyName[128];
		GvxGetFileVersion(
			strLanguagePath, &vffi,
			szLanguageName, SIZE_OF_ARRAY(szLanguageName),
			VERSION_SUB_COMPANYNAME, szCompanyName, SIZE_OF_ARRAY(szCompanyName),
			NULL);

		// �o�[�W������������쐬
		CString version;
		version.Format(
			TCHAR_STR_FORMAT_VERSION_RESOURCE,
			HIWORD(vffi.dwProductVersionMS), LOWORD(vffi.dwProductVersionMS),
			HIWORD(vffi.dwProductVersionLS), LOWORD(vffi.dwProductVersionLS));

		ListCtrlSetItem(m_listDisplayLanguage, pos, 0, find.GetFileName());
		ListCtrlSetItem(m_listDisplayLanguage, pos, 1, szLanguageName);
		ListCtrlSetItem(m_listDisplayLanguage, pos, 2, version);
		ListCtrlSetItem(m_listDisplayLanguage, pos, 3, szCompanyName);

		::FreeLibrary(hModule);
	}
}

int CSettingDlg::DisplayLanguageInsertItem(const int pos)
{
	// pos�s�ڂ̈ʒu�ɐV�K�A�C�e����}��
	return m_listDisplayLanguage.InsertItem(
		0,
		pos,
		NULL, 0, 0, 0, 0);
}

int CSettingDlg::DisplayLanguageSelectItem(const CString &rString)
{
	LVFINDINFO lvf;
	lvf.flags = LVFI_STRING;
	lvf.psz = rString;

	// �e�L�X�g������
	const int item = m_listDisplayLanguage.FindItem(&lvf);
	if(item != -1)
	{
		// ���������炻�̍s��I��
		m_listDisplayLanguage.SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_listDisplayLanguage.EnsureVisible(item, FALSE);
	}
	return item;
}

BOOL CSettingDlg::DisplayLanguageGetSelectedResourceName(CString &rString)
{
	// �I������Ă��鍀�ڂ��擾
	int item = m_listDisplayLanguage.GetNextItem(-1, LVNI_SELECTED);
	if(item == -1)
	{
		return FALSE;
	}
	rString = m_listDisplayLanguage.GetItemText(item, 0);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CSettingDlg ���b�Z�[�W �n���h��

BOOL CSettingDlg::OnInitDialog() 
{
	CToolTipDialog::OnInitDialog();

	// �e�J�e�S���̏�����
	UpdateData(FALSE);
	InitCategoryBasic();
	InitCategoryBasicPlugin();
	InitCategoryDisplay();
	InitCategoryDisplayLanguage();

	// ID-�c���[�A�C�e���Ή��}�b�v
	std::map<UINT, HTREEITEM> tree_map;
	tree_map[0] = m_treeCategory.GetRootItem();

	// �c���[�r���[�R���g���[���ɃJ�e�S����ǉ�
	const int count = SIZE_OF_ARRAY(setting_tree);
	for(int i = 0; i < count; i++)
	{
		// �J�e�S���̕�������擾
		CString category;
		category.LoadString(setting_tree[i].id);

		// �}������J�e�S���̕K�v�f�[�^�����낦��
		TVINSERTSTRUCT insertstruct;
		insertstruct.hInsertAfter	 = TVI_LAST;
		insertstruct.hParent         = tree_map[setting_tree[i].id_parent];
		insertstruct.item.mask       = TVIF_PARAM | TVIF_TEXT;
		insertstruct.item.lParam     = i;
		insertstruct.item.pszText    = category.GetBuffer(0);
		insertstruct.item.cchTextMax = category.GetLength();

		// �J�e�S����ǉ����A�e�A�C�e����W�J
		tree_map[setting_tree[i].id] = m_treeCategory.InsertItem(&insertstruct);
		m_treeCategory.Expand(insertstruct.hParent, TVE_EXPAND);

		// �c�[���`�b�v��ݒ�
		SetToolTips(setting_tree[i].id_control_array, SIZE_OF_ARRAY(setting_tree[i].id_control_array));

		// �R���g���[�����B��
		ShowCategoryControls(i, FALSE);
	}
	// �c���[�r���[�̃c�[���`�b�v��ǉ�
	SetToolTip(IDC_SETTING_TREE_CATEGORY);

	// �O��̍Ō�ɑI�΂ꂽ�A�C�e����I��
	if(m_setting_category < count)
	{
		const UINT category_id = setting_tree[m_setting_category].id;
		m_treeCategory.SelectItem(tree_map[category_id]);
	}

	// �f�t�H���g�̃t�H�[�J�X���g��
	UpdateData(FALSE);
	return TRUE;
}

void CSettingDlg::OnSelchangedTreeCategory(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	const UINT nOldIndex = pNMTreeView->itemOld.lParam;
	const UINT nNewIndex = pNMTreeView->itemNew.lParam;

	ShowCategoryControls(nOldIndex, FALSE);
	ShowCategoryControls(nNewIndex, TRUE);

	*pResult = 0;
}

// ��{���R�����g���摜�ɖ��ߍ���
void CSettingDlg::OnBasicCheckImplantComment() 
{
	UpdateData(TRUE);

	CWnd *pWndEditComment = GetDlgItem(IDC_SETTING_BASIC_EDIT_COMMENT);
	ASSERT(pWndEditComment != NULL);

	// �u�R�����g���摜�ɖ��ߍ��ށv���`�F�b�N����Ă�����R�����g���͗���L���ɂ���
	pWndEditComment->EnableWindow(m_bBasicImplantComment);
}

void CSettingDlg::OnDblclkSettingBasicPluginListInstalled(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// �}�E�X�J�[�\���̍��W���擾
	POINT point;
	::GetCursorPos(&point);
	m_listBasicPluginInstalled.ScreenToClient(&point);

	// ���X�g�R���g���[�����̑Ή��A�C�e�����擾
	LVHITTESTINFO hti;
	hti.pt = point;
	m_listBasicPluginInstalled.HitTest(&hti);

	// �A�C�e�����_�u���N���b�N���ꂽ�ꍇ
	if(hti.iItem != -1)
	{
		const UINT iItem = hti.iItem;

		// �v���O�C�������擾
		extern CPOPApp theApp;
		const PLUGIN_INFO &plugin_info = theApp.GetPluginInfo(iItem);

		// �v���O�C���̃Z�b�g�A�b�v
		if(plugin_info.setup != NULL)
		{
			plugin_info.setup(m_hWnd);
		}
		else
		{
			::MessageBeep(MB_OK);
		}
	}
	
	*pResult = 0;
}

// �\�����f�R�[�f�B���O�K���}���J�X�^�}�C�Y����
void CSettingDlg::OnDisplayCheckCustomizeGammaDecode() 
{
	UpdateData(TRUE);

	CWnd *pWndEditGammaDecode = GetDlgItem(IDC_SETTING_DISPLAY_GAMMA_DECODE);
	ASSERT(pWndEditGammaDecode != NULL);

	// �u�f�R�[�f�B���O�K���}���J�X�^�}�C�Y����v���`�F�b�N����Ă�����f�R�[�f�B���O�K���}�l���͗���L���ɂ���
	pWndEditGammaDecode->EnableWindow(m_bDisplayCustomizeGammaDecode);
}

// �\�����f�B�X�v���C�K���}���J�X�^�}�C�Y����
void CSettingDlg::OnDisplayCheckCustomizeGammaDisplay() 
{
	UpdateData(TRUE);

	CWnd *pWndEditGammaDisplay = GetDlgItem(IDC_SETTING_DISPLAY_GAMMA_DISPLAY);
	ASSERT(pWndEditGammaDisplay != NULL);

	// �u�f�B�X�v���C�K���}���J�X�^�}�C�Y����v���`�F�b�N����Ă�����f�B�X�v���C�K���}�l���͗���L���ɂ���
	pWndEditGammaDisplay->EnableWindow(m_bDisplayCustomizeGammaDisplay);
}

void CSettingDlg::OnOK() 
{
	if(UpdateData(TRUE))
	{
		// �Ō�ɑI�����ꂽ�J�e�S���̃C���f�b�N�X���擾
		m_setting_category = m_treeCategory.GetItemData(m_treeCategory.GetSelectedItem());

		m_color_background1 = m_staticDisplayBackgroundColor1.GetColor();
		m_color_background2 = m_staticDisplayBackgroundColor2.GetColor();

		DisplayLanguageGetSelectedResourceName(m_resource_name);

		CToolTipDialog::OnOK();
	}
}
