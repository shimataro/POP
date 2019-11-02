// POPDoc.cpp : CPOPDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "POP.h"

#include "POPDoc.h"
#include "POPView.h"

#include "operation.h"
#include "quant.h"

#include "QuantDlg.h"
#include "ResizeDlg.h"
#include "TrimmingDlg.h"

#include "winutil.h"                    // GvxPathAppendToModuleDir(), GvxCHAR2TCHAR()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPOPDoc

IMPLEMENT_DYNCREATE(CPOPDoc, CDocument)

BEGIN_MESSAGE_MAP(CPOPDoc, CDocument)
	//{{AFX_MSG_MAP(CPOPDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO_CLEAR, OnEditUndoClear)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO_CLEAR, OnUpdateEditUndoClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_OPERATION_ALPHA_APPEND, OnOperationAlphaAppend)
	ON_COMMAND(ID_OPERATION_ALPHA_INVERSE, OnOperationAlphaInverse)
	ON_COMMAND(ID_OPERATION_RESIZE, OnOperationResize)
	ON_COMMAND(ID_OPERATION_TRIMMING, OnOperationTrimming)
	ON_COMMAND(ID_OPERATION_QUANTIZE, OnOperationQuantize)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_ALPHA_APPEND, OnUpdateOperationAlphaAppend)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_QUANTIZE, OnUpdateOperationQuantize)
	ON_COMMAND(ID_FILTER_REAPPLY, OnFilterReapply)
	ON_COMMAND(ID_FILTER_RESUBMIT, OnFilterResubmit)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_OPERATION_COLORMODE_GRAYSCALE, ID_OPERATION_COLORMODE_RGB, OnOperationColormode)
	ON_COMMAND_RANGE(ID_OPERATION_ALPHA_0, ID_OPERATION_ALPHA_255, OnOperationAlphaFill)
	ON_COMMAND_RANGE(ID_OPERATION_TURN_FLIP, ID_OPERATION_TURN_270, OnOperationTurn)
	ON_COMMAND_RANGE(ID_OPERATION_SWAP_GRB, ID_OPERATION_SWAP_GBR, OnOperationSwap)
	ON_COMMAND_RANGE(ID_OPERATION_SELECT_RECT, ID_OPERATION_SELECT_AUTO, OnOperationSelect)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPERATION_COLORMODE_GRAYSCALE, ID_OPERATION_COLORMODE_RGB, OnUpdateOperationColormode)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPERATION_ALPHA_0, ID_OPERATION_ALPHA_INVERSE, OnUpdateOperationAlpha)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPERATION_SWAP_GRB, ID_OPERATION_SWAP_GBR, OnUpdateOperationSwap)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPERATION_SELECT_RECT, ID_OPERATION_SELECT_AUTO, OnUpdateOperationSelect)
	ON_COMMAND_RANGE(ID_FILTER_MIN, ID_FILTER_MAX, OnFilterCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILTER_REAPPLY, ID_FILTER_RESUBMIT, OnUpdateFilterRedo)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILTER_MIN, ID_FILTER_MAX, OnUpdateFilterCommand)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// �N���X��static�ϐ�
int CPOPDoc::m_select_type    = 0;
int CPOPDoc::m_last_filter_no = -1;


/////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// �V�K�C���[�W�p�p�����[�^
	NEWIMAGEPARAM g_new_param = {128, 128, 0x00ffffff};

	// �T�C�Y�ύX�p�p�����[�^
	RESIZEPARAM g_resize_param = {0, 0, IM_LINER};

	// �g���~���O�p�p�����[�^
	TRIMMINGPARAM g_trimming_param = {0, 0, TRUE, IM_LINER};
END_NAMESPACE()


/////////////////////////////////////////////////////////////////////////////
// CPOPDoc �N���X�̍\�z/����

CPOPDoc::CPOPDoc()
{
	m_rect.SetRectEmpty();                  // �I��̈����ɂ���
	m_format = 0;                           // �f�t�H���g�̌`���͂O�Ԗ�

	// IMAGE_INFO,IMAGE_PLANE�̏����ݒ�
	memset(&m_info, 0, sizeof(m_info));
	memset(&m_data, 0, sizeof(m_data));
}

CPOPDoc::~CPOPDoc()
{
}

// �V�K�쐬
BOOL CPOPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// �摜���쐬
	if(image_create_empty(&m_info, &m_data, &g_new_param) != IR_SUCCESS)
	{
		AfxMessageBox(AFX_IDP_FAILED_MEMORY_ALLOC, MB_ICONEXCLAMATION);
		return FALSE;
	}

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(m_info, m_data);
	return TRUE;
}

// �摜���J��
BOOL CPOPDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	try
	{
		CFile file(lpszPathName, CFile::modeRead | CFile::shareDenyWrite);
		CCallBack callback;

		// ���ׂẲ摜�`���œǂݍ���ł݂�
		extern CPOPApp theApp;
		const int format_count = theApp.GetPluginFormatDetailCount();
		for(int i = 0; i < format_count; i++)
		{
			const FORMATPLUGIN_DETAIL &format_detail = theApp.GetPluginFormatDetail(i);

			// �ǂݍ��ݗp�֐������݂����ꍇ
			if(format_detail.included_functions & IF_IMPORT)
			{
				const FORMATPLUGIN &plugin = theApp.GetPluginFormat(format_detail.plugin_no);

				// �}�b�N�o�C�i�����l��
				for(LONG pos = 0; pos <= 128; pos += 128)
				{
					// �t�@�C���|�C���^���ړ�
					file.Seek(pos, CFile::begin);

					const IMAGERESULT result = plugin.import(
						format_detail.format_no,
						reinterpret_cast<HANDLE>(file.m_hFile),
						&m_info, &m_data,
						callback.GetCallBackProc(), callback.GetCallBackData());

					switch(result)
					{
					case IR_SUCCESS:
						m_format = i;
						goto SUCCESS;

					case IR_IMAGEDESTROYED:
						throw IDS_IR_SUCCESS + result;
					}
				}
			}
		}

		// �ǂݍ��݂Ɏ��s����
		throw IDS_UNABLETOIMPORT;

SUCCESS:
		m_info.colors = image_count_colors(&m_info, &m_data);

		// �A���h�D�o�b�t�@�֒ǉ�
		AddToUndoBuffer(m_info, m_data);

		// �h�L�������g�̕ύX���Ȃ�
		m_undo.SetModifiedFlag(FALSE);
		UpdateDocument();
		return TRUE;
	}
	catch(int id)
	{
		ReportSaveLoadException(lpszPathName, NULL, FALSE, id);
		return FALSE;
	}
	catch(CFileException *e)
	{
		ASSERT(e != NULL);
		ReportSaveLoadException(lpszPathName, e, FALSE, 0);
		e->Delete();
		return FALSE;
	}
}

// �摜��ۑ�
BOOL CPOPDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	extern CPOPApp theApp;
	
	try
	{
		const FORMATPLUGIN_DETAIL &format_detail = theApp.GetPluginFormatDetail(m_format);
		ASSERT(format_detail.included_functions & IF_EXPORT);

		// �t�@�C�����J��
		CFile file(lpszPathName, CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive);

		// ���݂̌`���ɍœK�ȃJ���[���[�h�����߂�
		const int new_mode = GetOptimumColormode(format_detail.support_mode);
		if(new_mode == -1)
		{
			throw IDS_NOEXPORTABLEFORMAT;
		}

		// �J���[���[�h��ύX
		if(new_mode != m_info.mode)
		{
			IMAGE_INFO new_info;
			IMAGE_DATA new_data;
			image_convert_colormode(&m_info, &m_data, &new_info, &new_data, new_mode, NULL, 0);

			// �A���h�D�o�b�t�@�֒ǉ�
			AddToUndoBuffer(new_info, new_data);
		}

		// �R�����g��ANSI�����ɕϊ�
		char comment_ansi[256];
		if(TRUE)
		{
			SETTING setting;
			theApp.GetSetting(setting);
			if(setting.basic.check.elements.implant_comment && !setting.basic.comment.IsEmpty())
			{
				GvxTCHAR2CHAR(setting.basic.comment, comment_ansi, SIZE_OF_ARRAY(comment_ansi));
				m_info.comment = comment_ansi;
			}
			else
			{
				m_info.comment = NULL;
			}
		}

		// �C���[�W�������o��
		if(TRUE)
		{
			const FORMATPLUGIN &plugin = theApp.GetPluginFormat(format_detail.plugin_no);

			CCallBack callback;
			const IMAGERESULT result = plugin.export(
				format_detail.format_no,
				reinterpret_cast<HANDLE>(file.m_hFile),
				&m_info, &m_data,
				callback.GetCallBackProc(), callback.GetCallBackData());

			if(result != IR_SUCCESS)
			{
				throw IDS_IR_SUCCESS + result;
			}
		}

		// �h�L�������g�̕ύX���Ȃ�
		m_undo.SetModifiedFlag(FALSE);
		UpdateDocument();

		return TRUE;
	}
	catch(int id)
	{
		ReportSaveLoadException(lpszPathName, NULL, TRUE, id);
		return FALSE;
	}
	catch(CFileException *e)
	{
		ASSERT(e != NULL);
		ReportSaveLoadException(lpszPathName, e, TRUE, 0);
		e->Delete();
		return FALSE;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CPOPDoc �N���X�̐f�f

#ifdef _DEBUG
void CPOPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPOPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPOPDoc �R�}���h

// �u�t�@�C���v���u�V�K�쐬�v
#include "FileNewDlg.h"
void CPOPApp::OnFileNew() 
{
	CFileNewDlg dlg(g_new_param);

	// �V�K�C���[�W�̃T�C�Y��₢���킹��
	if(dlg.DoModal() == IDOK)
	{
		CWinApp::OnFileNew();
	}
}

// �u�t�@�C���v���u�J���v
void CPOPApp::OnFileOpen() 
{
	// �t�@�C�������i�[����o�b�t�@
	TCHAR buffer[1 << 10];
	buffer[0] = TCHAR_CTRL_NIL;

	// �t�B���^�̃f���~�^��'|'����'\0'�ɕϊ�
	CString filter = GetFileFilterLoad();
	filter.Replace(TCHAR_FILTER_DELIMITER, TCHAR_CTRL_NIL);

	// OPENFILENAME�\���̂̐ݒ�
	OPENFILENAME ofn = {sizeof(OPENFILENAME)};
	ofn.hwndOwner    = AfxGetMainWnd()->m_hWnd;
	ofn.lpstrFilter  = filter;
	ofn.nFilterIndex = m_nFilterIndex;
	ofn.lpstrFile    = buffer;
	ofn.nMaxFile     = SIZE_OF_ARRAY(buffer);
	ofn.Flags        = OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	// �_�C�A���O��\��
	if(::GetOpenFileName(&ofn))
	{
		// �t�@�C�����̃I�t�Z�b�g���擾
		int pos = ofn.nFileOffset;

		while(buffer[pos] != TCHAR_CTRL_NIL)
		{
			// �w�肳�ꂽ�t�@�C�����J��
			LPCTSTR lpszFileName = &buffer[pos];
			OpenDocumentFile(lpszFileName);

			// ���̃t�@�C�����̈ʒu
			pos += _tcslen(lpszFileName) + 1;
		}
		m_nFilterIndex = ofn.nFilterIndex;
	}
}

// �u�t�@�C���v���u�ۑ��v
void CPOPDoc::OnFileSave() 
{
	extern CPOPApp theApp;

	// �t�@�C�����̑��݂��`�F�b�N
	const CString &pathname = GetPathName();
	if(pathname.IsEmpty())
	{
		// ���݂��Ȃ���Ζ��O�����ĕۑ�
		OnFileSaveAs();
		return;
	}

	// �ۑ��p�֐��̑��݂��`�F�b�N
	const FORMATPLUGIN_DETAIL &format_detail = theApp.GetPluginFormatDetail(m_format);
	if((format_detail.included_functions & IF_EXPORT) != IF_EXPORT)
	{
		// ���݂��Ȃ���Ζ��O�����ĕۑ�
		ReportSaveLoadException(pathname, NULL, TRUE, IDS_UNABLETOEXPORT);
		OnFileSaveAs();
		return;
	}
	OnSaveDocument(pathname);
	return;
}

// �u�t�@�C���v���u���O�����ĕۑ��v
void CPOPDoc::OnFileSaveAs() 
{
	extern CPOPApp theApp;

	// �t�B���^�C���f�b�N�X�����߂�
	DWORD index = 0;
	for(int i = 0; i <= m_format; i++)
	{
		const FORMATPLUGIN_DETAIL &format_detail = theApp.GetPluginFormatDetail(i);
		if(format_detail.included_functions & IF_EXPORT)
		{
			index++;
		}
	}

	// �t�@�C�������i�[����o�b�t�@
	TCHAR buffer[1 << 10];
	buffer[0] = TCHAR_CTRL_NIL;

	// �t�B���^�̃f���~�^��'|'����'\0'�ɕϊ�
	CString filter = theApp.GetFileFilterSave();
	filter.Replace(TCHAR_FILTER_DELIMITER, TCHAR_CTRL_NIL);

	// OPENFILENAME�\���̂̐ݒ�
	OPENFILENAME ofn = {sizeof(OPENFILENAME)};
	ofn.hwndOwner    = AfxGetMainWnd()->m_hWnd;
	ofn.lpstrFilter  = filter;
	ofn.nFilterIndex = index;
	ofn.lpstrFile    = buffer;
	ofn.nMaxFile     = SIZE_OF_ARRAY(buffer);
	ofn.Flags        = OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_OVERWRITEPROMPT | OFN_EXPLORER;

	// �_�C�A���O��\��
	if(!::GetSaveFileName(&ofn))
	{
		return;
	}

	// �Ή�����t�H�[�}�b�g�����߂�
	DWORD save_plugin_count = 0;

	const int plugin_count = theApp.GetPluginFormatDetailCount();
	for(int i = 0; i < plugin_count; i++)
	{
		const FORMATPLUGIN_DETAIL &format_detail = theApp.GetPluginFormatDetail(i);
		if((format_detail.included_functions & IF_EXPORT) != IF_EXPORT)
		{
			continue;
		}

		save_plugin_count++;
		if(save_plugin_count != ofn.nFilterIndex)
		{
			continue;
		}

		// �Ή��t�H�[�}�b�g����������
		m_format = i;
		::PathRenameExtension(buffer, format_detail.ext);
		break;
	}

	ASSERT(save_plugin_count == ofn.nFilterIndex);

	// �h�L�������g��ۑ�
	if(OnSaveDocument(buffer))
	{
		SetPathName(buffer);                    // �t�@�C������ύX
		SetModifiedFlag(FALSE);                 // �_�[�e�B�t���O���N���A
	}
}

// �u�t�@�C���v���u���ׂĕۑ��v
void CPOPDoc::OnFileSaveAll() 
{
	// �h�L�������g�e���v���[�g���擾
	CDocTemplate *pDocTemplate = GetDocTemplate();

	// ���ׂẴh�L�������g�ɑ΂��ČJ��Ԃ�
	POSITION pos = pDocTemplate->GetFirstDocPosition();
	while(pos != NULL)
	{
		// �h�L�������g�N���X�ւ̃|�C���^���擾
		CPOPDoc *pPOPDoc = STATIC_DOWNCAST(CPOPDoc, pDocTemplate->GetNextDoc(pos));
		ASSERT(pPOPDoc != NULL);

		// �h�L�������g��ۑ�
		pPOPDoc->OnFileSave();
	}
}

void CPOPDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// �_�[�e�B�[�t���O�������Ă�����I����
	pCmdUI->Enable(IsModified());
}

// �u�t�@�C���v���u��荞�݁v���u�N���b�v�{�[�h����v
void CPOPApp::OnFileImportClipboard() 
{
	NewImageFromClipboard();
}

void CPOPApp::OnUpdateFileImportClipboard(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_BITMAP));
}

// �u�t�@�C���v���u��荞�݁v���u��ʂ���v
#include "FileImportDisplayDlg.h"
#include "ImportWindowDlg.h"
void CPOPApp::OnFileImportDisplay() 
{
	////////////////////////////////////////////////////////////
	// ��荞�ޗ̈�i�f�X�N�g�b�v�^�E�C���h�E�S�́^�E�C���h�E�̓����j�����肷��
	CFileImportDisplayDlg dlgImport;
	dlgImport.m_nArea   = 1;
	dlgImport.m_nTiming = 0;
	if(dlgImport.DoModal() != IDOK)
	{
		return;
	}

	////////////////////////////////////////////////////////////
	// ��荞�ރE�C���h�E�����肷��

	// �f�X�N�g�b�v�S��
	if(dlgImport.m_nArea == 0)
	{
		HWND hWndCapture = ::GetDesktopWindow();
		BOOL bAltogether = TRUE;
		NewImageFromWindow(hWndCapture, bAltogether, dlgImport.m_nTiming * 1000);
		return;
	}
	// �E�C���h�E�S��or����
	else
	{
		// ��荞�ރE�C���h�E��I��
		CImportWindowDlg *pDlgSelect = new CImportWindowDlg;
		pDlgSelect->m_bAltogether = (dlgImport.m_nArea == 2) ? FALSE : TRUE;
		pDlgSelect->m_dwMilliSeconds = dlgImport.m_nTiming * 1000;

		// ���[�h���X�_�C�A���O���쐬
		pDlgSelect->Create(IDD_IMPORT_WINDOW);
		pDlgSelect->ShowWindow(SW_SHOW);
		return;
	}
}


////////////////////////////////////////
// �⏕�֐�

// �h�L�������g���N���[�Y�����O�ɌĂяo�����
BOOL CPOPDoc::SaveModified() 
{
	// �h�L�������g���ύX����Ă��Ȃ���΃N���[�Y��
	if(!IsModified())
	{
		return TRUE;
	}

	// �v�����v�g��\���i�����ւ̕ύX��ۑ����܂���?�j
	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, GetTitle());
	switch(AfxMessageBox(prompt, MB_YESNOCANCEL))
	{
	case IDYES:                             // �u�͂��v
		OnFileSaveAs();                         // ���O��t���ĕۑ�
		return !IsModified();

	case IDNO:                              // �u�������v
		return TRUE;                            // �N���[�Y��

	case IDCANCEL:                          // �L�����Z��
		return FALSE;                           // �N���[�Y�s��
	}

	return TRUE;
}

// �A���h�D�o�b�t�@�փA���h�D����ǉ�
bool CPOPDoc::AddToUndoBuffer(const IMAGE_INFO &info, const IMAGE_DATA &data)
{
	// �A���h�D����ǉ�
	if(TRUE)
	{
		UNDOINFO undoinfo;
		undoinfo.info = info;
		undoinfo.data = data;

		if(!m_undo.AddUndoInfo(undoinfo))
		{
			// �摜�f�[�^���J��
			if(!binary_equal(m_data, undoinfo.data))
			{
				image_free(&undoinfo.data);
			}
			return false;
		}

		// �ǉ��ɐ���������A�摜�����X�V
		m_info = undoinfo.info;
		m_data = undoinfo.data;
	}

	// �h�L�������g���X�V
	UpdateDocument();

	return true;
}

// �h�L�������g���ύX���ꂽ
void CPOPDoc::UpdateDocument()
{
	// �_�[�e�B�[�t���O�̐ݒ�
	SetModifiedFlag(m_undo.IsModified());

	// �r���[���X�V���A�E�C���h�E�����T�C�Y
	UpdateAllViews(NULL);
}

// �T�|�[�g���Ă���J���[���[�h�̂Ȃ��ōœK�Ȃ��̂����߂�
int CPOPDoc::GetOptimumColormode(uint_t support_mode)
{
	const int support_mask[] = {SUPPORT_GRAYSCALE, SUPPORT_INDEX, SUPPORT_RGB};

	// ���x�̍����ق�������
	if(TRUE)
	{
		for(int new_mode = m_info.mode; new_mode <= COLORMODE_RGB; new_mode++)
		{
			if(support_mode & support_mask[new_mode])
			{
				return new_mode;
			}
		}
	}

	// ���x�̒Ⴂ�ق�������
	if(TRUE)
	{
		for(int new_mode = m_info.mode; new_mode >= COLORMODE_GRAYSCALE; new_mode--)
		{
			if(support_mode & support_mask[new_mode])
			{
				return new_mode;
			}
		}
	}

	return -1;
}

// �N���b�v�{�[�h���̃r�b�g�}�b�v��V�K�h�L�������g�Ƃ���
BOOL CPOPDoc::PasteAsNew(HBITMAP hBitmap)
{
	IMAGE_INFO new_info;
	IMAGE_DATA new_data;
	if(HBITMAP_GetImage(hBitmap, &new_info, &new_data) != IR_SUCCESS)
	{
		return FALSE;
	}

	new_info.colors = image_count_colors(&new_info, &new_data);

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(new_info, new_data);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// �A���h�D�^���h�D

// �A���h�D
void CPOPDoc::OnEditUndo() 
{
	if(TRUE)
	{
		UNDOINFO info;
		m_undo.Undo(info);

		m_info = info.info;
		m_data = info.data;
	}

	UpdateDocument();
}

// ���h�D
void CPOPDoc::OnEditRedo() 
{
	if(TRUE)
	{
		UNDOINFO info;
		m_undo.Redo(info);

		m_info = info.info;
		m_data = info.data;
	}

	UpdateDocument();
}

// �A���h�D�������̃N���A
void CPOPDoc::OnEditUndoClear() 
{
	const int return_id = AfxMessageBox(IDS_UNDOCLEAR, MB_OKCANCEL | MB_ICONQUESTION);
	if(return_id == IDOK)
	{
		m_undo.ClearMemory();
	}
}

// �J�b�g
void CPOPDoc::OnEditCut() 
{
	// �J�b�g���R�s�[�{�N���A
	OnEditCopy();
	OnEditClear();
}

// �R�s�[
void CPOPDoc::OnEditCopy() 
{
	// ���݂̉摜����r�b�g�}�b�v�n���h�����쐬
	HBITMAP hBitmap = HBITMAP_CreateFromImage(&m_info, &m_data);
	if(hBitmap == NULL)
	{
		return;
	}

	// �Ƃ肠�����摜�S�����N���b�v�{�[�h�փR�s�[
	::OpenClipboard(NULL);
	::EmptyClipboard();
	::SetClipboardData(CF_BITMAP, hBitmap);
	::CloseClipboard();
}

// �y�[�X�g
void CPOPDoc::OnEditPaste() 
{
}

// �N���A
void CPOPDoc::OnEditClear() 
{
}

// ���ׂđI��
void CPOPDoc::OnEditSelectAll() 
{
	// �I��̈���X�V
	m_rect.SetRect(0, 0, m_info.width, m_info.height);
	UpdateDocument();
}

// �A���h�D��UPDATE_COMMAND_UI���b�Z�[�W�n���h��
void CPOPDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_undo.IsAbleToUndo());
}

// ���h�D��UPDATE_COMMAND_UI���b�Z�[�W�n���h��
void CPOPDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_undo.IsAbleToRedo());
}

// �A���h�D�������̃N���A��UPDATE_COMMAND_UI���b�Z�[�W�n���h��
void CPOPDoc::OnUpdateEditUndoClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_undo.IsAbleToUndo() || m_undo.IsAbleToRedo());
}

// �J�b�g��UPDATE_COMMAND_UI���b�Z�[�W�n���h��
void CPOPDoc::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_rect.IsRectEmpty());
}

// �R�s�[��UPDATE_COMMAND_UI���b�Z�[�W�n���h��
void CPOPDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_rect.IsRectEmpty());
}

// �y�[�X�g��UPDATE_COMMAND_UI���b�Z�[�W�n���h��
void CPOPDoc::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_BITMAP));
}

// �N���A��UPDATE_COMMAND_UI���b�Z�[�W�n���h��
void CPOPDoc::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_rect.IsRectEmpty());
}

/////////////////////////////////////////////////////////////////////////////
// ����

// �u����v���u�J���[���[�h�v
void CPOPDoc::OnOperationColormode(UINT nID)
{
	const int new_mode = COLORMODE_GRAYSCALE + (nID - ID_OPERATION_COLORMODE_GRAYSCALE);
	if(m_info.mode == new_mode)
	{
		return;
	}

	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	// ����
	CCallBack callback;
	const IMAGERESULT result = image_convert_colormode(
		&m_info, &m_data,
		&new_info, &new_data,
		new_mode,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(new_info, new_data);
}

// �u����v���u�A���t�@�v���u�A���t�@��ǉ��v
void CPOPDoc::OnOperationAlphaAppend() 
{
	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	// ����
	CCallBack callback;
	const IMAGERESULT result = image_alpha_append(
		&m_info, &m_data,
		&new_info, &new_data,
		!m_info.alpha,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(new_info, new_data);
}

// �u����v���u�A���t�@�v���u�������^�s�������v
void CPOPDoc::OnOperationAlphaFill(UINT nID)
{
	const_uint8_t alpha_val = (nID == ID_OPERATION_ALPHA_0) ? 0 : 255;
	IMAGE_DATA new_data;

	// ����
	CCallBack callback;
	const IMAGERESULT result = image_alpha_fill(
		&m_info, &m_data, &new_data,
		alpha_val,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(m_info, new_data);
}

// �u����v���u�A���t�@�v���u�A���t�@�l�𔽓]�v
void CPOPDoc::OnOperationAlphaInverse() 
{
	IMAGE_DATA new_data;

	// ����
	CCallBack callback;
	const IMAGERESULT result = image_alpha_inverse(
		&m_info, &m_data, &new_data,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(m_info, new_data);
}

// �u����v���u��]/���]�v
void CPOPDoc::OnOperationTurn(UINT nID)
{
	typedef IMAGERESULT (*TURNFUNC)(const IMAGE_INFO *in_info, const IMAGE_DATA *in_data, IMAGE_INFO *out_info, IMAGE_DATA *out_data, callback_t callback_default, long data);
	TURNFUNC func[] =
	{
		image_flip,
		image_mirror,
		image_turn_90,
		image_turn_180,
		image_turn_270,
	};

	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	// ����
	CCallBack callback;
	IMAGERESULT result = func[nID - ID_OPERATION_TURN_FLIP](
		&m_info, &m_data,
		&new_info, &new_data,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(new_info, new_data);
}

// �u����v���u�F�̓���ւ��v
void CPOPDoc::OnOperationSwap(UINT nID)
{
	typedef struct tagSWAP_COLOR_INFO
	{
		int red;
		int green;
		int blue;
	} SWAP_COLOR_INFO;
	const SWAP_COLOR_INFO swap_color_info_array[] =
	{
		{1, 0, 2},
		{2, 1, 0},
		{0, 2, 1},
		{2, 0, 1},
		{1, 2, 0},
	};

	const SWAP_COLOR_INFO &swap_color_info = swap_color_info_array[nID - ID_OPERATION_SWAP_GRB];
	IMAGE_DATA new_data;

	// ����
	CCallBack callback;
	IMAGERESULT result = image_swap_colors(
		&m_info, &m_data, &new_data,
		swap_color_info.red, swap_color_info.green, swap_color_info.blue,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// ������̉摜�̐F���𐔂���
	IMAGE_INFO new_info = m_info;
	new_info.colors = image_count_colors(&m_info, &new_data);

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(new_info, new_data);
}

// �u����v���u�I���v
void CPOPDoc::OnOperationSelect(UINT nID)
{
	m_select_type = nID - ID_OPERATION_SELECT_RECT;
}

// �u����v���u�T�C�Y�ύX�v
void CPOPDoc::OnOperationResize() 
{
	CResizeDlg dlg;
	dlg.nWidth            = m_info.width;
	dlg.nHeight           = m_info.height;
	dlg.m_bIsAbleToSmooth =(m_info.mode==COLORMODE_GRAYSCALE || m_info.mode==COLORMODE_RGB);

	// OK�{�^���������ꂽ�ꍇ
	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	// �p�����[�^�����o��
	g_resize_param = dlg.m_param;

	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	// ����
	CCallBack callback;
	const IMAGERESULT result = image_resize(
		&m_info, &m_data,
		&new_info, &new_data,
		&g_resize_param,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(new_info, new_data);
}

// �u����v���u�g���~���O�v
void CPOPDoc::OnOperationTrimming() 
{
	CTrimmingDlg dlg;
	dlg.m_nWidth  = m_info.width;
	dlg.m_nHeight = m_info.height;

	// OK�{�^���������ꂽ�ꍇ
	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	// �p�����[�^�����o��
	g_trimming_param = dlg.m_param;

	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	// ����
	CCallBack callback;
	const IMAGERESULT result = image_trimming(
		&m_info, &m_data,
		&new_info, &new_data,
		&g_trimming_param,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(new_info, new_data);
}

// �u����v���u���F�v
void CPOPDoc::OnOperationQuantize() 
{
	CQuantDlg dlg;
	dlg.m_radioDither = 0;
	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	IMAGE_INFO new_info;
	IMAGE_DATA new_data;

	const QUANT_PALETTE_METHOD palette_method = static_cast<QUANT_PALETTE_METHOD>(dlg.m_radioMethod);
	const QUANT_DITHER_METHOD  dither_method  = static_cast<QUANT_DITHER_METHOD >(dlg.m_radioDither);

	// ����
	CCallBack callback;
	const IMAGERESULT result = quantize(
		&m_info, &m_data,
		&new_info, &new_data,
		dlg.m_nQuantColors, palette_method, dither_method,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		return;
	}

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(new_info, new_data);
}

void CPOPDoc::OnUpdateOperationColormode(CCmdUI* pCmdUI) 
{
	// ���W�I�{�^���`�F�b�N�}�[�N������
	pCmdUI->SetRadio(m_info.mode == COLORMODE_GRAYSCALE + (pCmdUI->m_nID - ID_OPERATION_COLORMODE_GRAYSCALE));
}

void CPOPDoc::OnUpdateOperationAlphaAppend(CCmdUI* pCmdUI) 
{
	// �A���t�@�`���l�������݂��Ă�����`�F�b�N�}�[�N������
	pCmdUI->SetCheck(m_info.alpha);
}

void CPOPDoc::OnUpdateOperationAlpha(CCmdUI* pCmdUI)
{
	// �摜�ɃA���t�@�`���l�����ǉ�����Ă�����I����
	pCmdUI->Enable(m_info.alpha);
}

void CPOPDoc::OnUpdateOperationSwap(CCmdUI* pCmdUI)
{
	// �O���C�X�P�[���摜�͓���ւ��s��
	pCmdUI->Enable(m_info.mode != COLORMODE_GRAYSCALE);
}

// 
void CPOPDoc::OnUpdateOperationSelect(CCmdUI *pCmdUI)
{
	const UINT id = m_select_type + ID_OPERATION_SELECT_RECT;
	pCmdUI->SetRadio(pCmdUI->m_nID == id);
}

void CPOPDoc::OnUpdateOperationQuantize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_info.mode == COLORMODE_RGB);
}


/////////////////////////////////////////////////////////////////////////////
// �t�B���^����

BOOL CPOPDoc::DoFilter(const UINT filter_no, const BOOL bSetup /* = TRUE */)
{
	// �Y������v���O�C���֐������o��
	extern CPOPApp theApp;
	const FILTERPLUGIN_DETAIL &filter_detail = theApp.GetPluginFilterDetail(filter_no);
	const FILTERPLUGIN &plugin = theApp.GetPluginFilter(filter_detail.plugin_no);

	// �I�v�V�����֐������s���ċU���Ԃ��Ă�����A�t�B���^�����𒆒f
	if(bSetup && !plugin.option(filter_detail.filter_no, &m_info, &m_data, AfxGetMainWnd()->m_hWnd))
	{
		return FALSE;
	}

	// �V�����C���[�W�̃��������m��
	IMAGE_DATA new_data;
	if(!image_alloc(&m_info, &new_data))
	{
		AfxMessageBox(AFX_IDP_FAILED_MEMORY_ALLOC, MB_ICONEXCLAMATION);
		return FALSE;
	}

	// �t�B���^����
	CCallBack callback;
	const IMAGERESULT result = plugin.effect(
		filter_detail.filter_no,
		&m_info, &m_data, &new_data,
		callback.GetCallBackProc(), callback.GetCallBackData());
	if(result != IR_SUCCESS)
	{
		// �������J��
		image_free(&new_data);
		return FALSE;
	}

	// �u�t�B���^�̍Ď��s�v�p�̃t�B���^�ԍ���ۑ�
	m_last_filter_no = filter_no;

	// ������̉摜�̐F���𐔂���
	IMAGE_INFO new_info = m_info;
	new_info.colors = image_count_colors(&m_info, &new_data);

	// �A���h�D�o�b�t�@�֒ǉ�
	AddToUndoBuffer(new_info, new_data);
	return TRUE;
}

// �u�t�B���^�̍ēK�p�v�i�I�v�V�����Ȃ��Ŏ��s�j
void CPOPDoc::OnFilterReapply() 
{
	DoFilter(m_last_filter_no, FALSE);
}

// �u�t�B���^�̍Ď��s�v�i�I�v�V�������Ŏ��s�j
void CPOPDoc::OnFilterResubmit() 
{
	DoFilter(m_last_filter_no);
}

void CPOPDoc::OnUpdateFilterRedo(CCmdUI* pCmdUI) 
{
	if(m_last_filter_no < 0)
	{
		// ���O�̃t�B���^���o�^����Ă��Ȃ���ΑI��s��
		pCmdUI->Enable(FALSE);
	}
	else
	{
		// �Y������v���O�C���֐������o��
		extern CPOPApp theApp;
		const FILTERPLUGIN_DETAIL &filter_detail = theApp.GetPluginFilterDetail(m_last_filter_no);
		const FILTERPLUGIN &plugin = theApp.GetPluginFilter(filter_detail.plugin_no);

		// ���O�̃t�B���^�����s�\��������I����
		pCmdUI->Enable(plugin.executable(filter_detail.filter_no, &m_info));
	}
}

// �t�B���^
void CPOPDoc::OnFilterCommand(UINT nID)
{
	DoFilter(nID - ID_FILTER_MIN);
}

void CPOPDoc::OnUpdateFilterCommand(CCmdUI* pCmdUI)
{
	// �Y������v���O�C���֐������o��
	extern CPOPApp theApp;
	const FILTERPLUGIN_DETAIL &filter_detail = theApp.GetPluginFilterDetail(pCmdUI->m_nID - ID_FILTER_MIN);
	const FILTERPLUGIN &plugin = theApp.GetPluginFilter(filter_detail.plugin_no);

	pCmdUI->Enable(plugin.executable(filter_detail.filter_no, &m_info));
}


/////////////////////////////////////////////////////////////////////////////
// ���ݒ�

// �u�ݒ�v���u���ݒ�v
#include "SettingDlg.h"
void CPOPApp::OnSettingEnvironment() 
{
	CSettingDlg theDlg;
	theDlg.m_setting_category = m_setting_category;

	// ��{�J�e�S���̐ݒ�
	theDlg.m_bBasicImplantComment      = m_setting.basic.check.elements.implant_comment;
	theDlg.m_strBasicComment           = m_setting.basic.comment;
	theDlg.m_bBasicSaveWindowPlacement = m_setting.basic.check.elements.save_window_placement;
	theDlg.m_nBasicMRUCount            = m_setting.basic.mru_count;

	// �\���J�e�S���̐ݒ�
	theDlg.m_bDisplayActivateGammaFile     = m_setting.display.check.elements.activate_gamma_file;
	theDlg.m_bDisplayCustomizeGammaDecode  = m_setting.display.check.elements.customize_gamma_decode;
	theDlg.m_dDisplayGammaDecode           = m_setting.display.gamma_decode;
	theDlg.m_bDisplayCustomizeGammaDisplay = m_setting.display.check.elements.customize_gamma_display;
	theDlg.m_dDisplayGammaDisplay          = m_setting.display.gamma_display;
	theDlg.m_nDisplayBackgroundGridSize    = m_setting.display.background_grid_size;
	theDlg.m_color_background1             = m_setting.display.background_color1;
	theDlg.m_color_background2             = m_setting.display.background_color2;
	theDlg.m_bDisplayToolbarXP             = m_setting.display.toolbar_XP;

	// �\��������̐ݒ�
	theDlg.m_resource_name = m_setting.display.language.resource_name;

	if(theDlg.DoModal() == IDOK)
	{
		m_setting_category = theDlg.m_setting_category;

		// ��{�J�e�S���̐ݒ��ۑ�
		m_setting.basic.check.elements.implant_comment       = theDlg.m_bBasicImplantComment;
		m_setting.basic.comment                              = theDlg.m_strBasicComment;
		m_setting.basic.check.elements.save_window_placement = theDlg.m_bBasicSaveWindowPlacement;
		m_setting.basic.mru_count                            = theDlg.m_nBasicMRUCount;

		// �\���J�e�S���̐ݒ��ۑ�
		m_setting.display.check.elements.activate_gamma_file     = theDlg.m_bDisplayActivateGammaFile;
		m_setting.display.check.elements.customize_gamma_decode  = theDlg.m_bDisplayCustomizeGammaDecode;
		m_setting.display.gamma_decode                           = theDlg.m_dDisplayGammaDecode;
		m_setting.display.check.elements.customize_gamma_display = theDlg.m_bDisplayCustomizeGammaDisplay;
		m_setting.display.gamma_display                          = theDlg.m_dDisplayGammaDisplay;
		m_setting.display.background_grid_size                   = theDlg.m_nDisplayBackgroundGridSize;
		m_setting.display.background_color1                      = theDlg.m_color_background1;
		m_setting.display.background_color2                      = theDlg.m_color_background2;
		m_setting.display.toolbar_XP                             = theDlg.m_bDisplayToolbarXP;

		// �\��������̐ݒ��ۑ�
		m_setting.display.language.resource_name = theDlg.m_resource_name;

		// �ݒ�𔽉f
		ReflectSetting();
	}
}

// �u�ݒ�v���u�t�H�[�}�b�g���Ƃ̐ݒ�v
void CPOPApp::OnSettingFormat(UINT nID)
{
	const FORMATPLUGIN_DETAIL &format_detail = GetPluginFormatDetail(nID - ID_SETTING_FORMAT_MIN);
	const FORMATPLUGIN &plugin = GetPluginFormat(format_detail.plugin_no);
	ASSERT(format_detail.included_functions & IF_OPTION);

	plugin.option(format_detail.format_no, AfxGetMainWnd()->m_hWnd);
}

void CPOPApp::OnUpdateSettingFormat(CCmdUI* pCmdUI)
{
	const FORMATPLUGIN_DETAIL &format_detail = GetPluginFormatDetail(pCmdUI->m_nID - ID_SETTING_FORMAT_MIN);
	pCmdUI->Enable(format_detail.included_functions & IF_OPTION);
}
