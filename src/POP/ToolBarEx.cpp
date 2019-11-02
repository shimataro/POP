// ToolBarEx.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "POP.h"
#include "ToolBarEx.h"

#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx

BEGIN_MESSAGE_MAP(CToolBarEx, CToolBar)
	//{{AFX_MSG_MAP(CToolBarEx)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(TBN_QUERYINSERT, OnNotifyQueryInsert)
	ON_NOTIFY_REFLECT(TBN_QUERYDELETE, OnNotifyQueryDelete)
	ON_NOTIFY_REFLECT(TBN_BEGINADJUST, OnNotifyBeginAdjust)
	ON_NOTIFY_REFLECT(TBN_ENDADJUST, OnNotifyEndAdjust)
	ON_NOTIFY_REFLECT(TBN_RESET, OnNotifyReset)
	ON_NOTIFY_REFLECT(TBN_TOOLBARCHANGE, OnNotifyToolBarChange)
	ON_NOTIFY_REFLECT(TBN_GETBUTTONINFO, OnNotifyGetButtonInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPOPView �N���X�̍\�z/����

CToolBarEx::CToolBarEx()
{
	m_nItemCount = -1;
	m_nButtonCount = -1;

	m_pContextMenu = NULL;
	m_pWndOwner    = NULL;
	m_bAutoDestroy = FALSE;
}

CToolBarEx::~CToolBarEx()
{
	SetContextMenu(NULL, NULL);
}

// �c�[���o�[�����[�h
BOOL CToolBarEx::LoadToolBar(LPCTSTR lpszResourceName)
{
	if(!CToolBar::LoadToolBar(lpszResourceName))
	{
		return FALSE;
	}

	// �������
	if(TRUE)
	{
		CToolBarCtrl &ctrl = GetToolBarCtrl();

		// �A�C�e���̑������J�E���g
		m_nItemCount = ctrl.GetButtonCount();
		m_toolbarinfos.SetSize(m_nItemCount);

		// �e�{�^���̏��ƃe�L�X�g���擾
		const int item_count = m_nItemCount;
		for(int i = 0; i < item_count; i++)
		{
			// TBBUTTON�\���̂̎擾
			TBBUTTON tbButton;
			ctrl.GetButton(i, &tbButton);
			m_toolbarinfos[i].tbButton = tbButton;

			// �e�L�X�g��'\n'�̑O�̕���
			if(TRUE)
			{
				CString string;
				string.LoadString(tbButton.idCommand);

				AfxExtractSubString(
					m_toolbarinfos[i].strText,
					string,
					0);
			}
		}
	}

	// �Z�p���[�^���������{�^���̌����擾
	if(TRUE)
	{
		m_nButtonCount = 0;

		const int item_count = m_nItemCount;
		for(int i = 0; i < item_count; i++)
		{
			// �Z�p���[�^�łȂ��A�C�e���̐����J�E���g
			if(GetItemID(i) != ID_SEPARATOR)
			{
				m_nButtonCount++;
			}
		}
	}

	return TRUE;
}

// �R���e�L�X�g���j���[�̐ݒ�
void CToolBarEx::SetContextMenu(CMenu *pContextMenu, CWnd *pWndOwner, const BOOL bAutoDestroy /* = FALSE */)
{
	if(m_pContextMenu != NULL && m_bAutoDestroy)
	{
		m_pContextMenu->DestroyMenu();
		m_pContextMenu = NULL;
	}

	m_pContextMenu = pContextMenu;
	m_pWndOwner    = pWndOwner;
	m_bAutoDestroy = bAutoDestroy;
}

// �c�[���o�[�̃A�C�R����u������
void CToolBarEx::ReplaceIcons(const UINT nIDNormal, const UINT nIDDisabled, const UINT nIDHot)
{
	// �c�[���o�[�̃C���[�W���X�g���쐬
	if(TRUE)
	{
		CreateToolbarImageList(nIDNormal  , m_ilToolbar);
		CreateToolbarImageList(nIDDisabled, m_ilToolbarDisabled);
		CreateToolbarImageList(nIDHot     , m_ilToolbarHot);
	}

	// �c�[���o�[�ɃC���[�W���X�g�����蓖�Ă�
	if(TRUE)
	{
		CToolBarCtrl &ctrl = GetToolBarCtrl();
		ctrl.SetImageList        (&m_ilToolbar);
		ctrl.SetDisabledImageList(&m_ilToolbarDisabled);
		ctrl.SetHotImageList     (&m_ilToolbarHot);
	}
}

// �w��{�^���̍폜
BOOL CToolBarEx::DeleteButtons(const UINT nIDDelete[], const int count)
{
	CToolBarCtrl &ctrl = GetToolBarCtrl();

	for(int i = 0; i < count; i++)
	{
		const int index = CommandToIndex(nIDDelete[i]);
		if(index == -1)
		{
			return FALSE;
		}

		ctrl.DeleteButton(index);
	}

	return TRUE;
}

// �c�[���o�[�p�̃C���[�W���X�g���쐬
BOOL CToolBarEx::CreateToolbarImageList(const UINT nIDBitmap, CImageList &il)
{
	// �r�b�g�}�b�v�����[�h
	CBitmap bm;
	bm.LoadBitmap(nIDBitmap);

	// �C���[�W���X�g�̏����擾
	IMAGELISTINFO info;
	GetImageListInfo(bm, info);

	// �C���[�W���X�g���쐬
	ASSERT(m_nButtonCount > 0);
	il.Create(info.width / m_nButtonCount, info.height, info.flags | ILC_MASK, m_nButtonCount, 0);
	il.Add(&bm, info.mask);

	return TRUE;
}

// �C���[�W���X�g�̏����擾
BOOL CToolBarEx::GetImageListInfo(CBitmap &rBitmap, IMAGELISTINFO &info)
{
	// �r�b�g�}�b�v�����擾
	BITMAP bmInfo;
	rBitmap.GetBitmap(&bmInfo);

	// ���ƍ������i�[
	info.width  = bmInfo.bmWidth;
	info.height = bmInfo.bmHeight;

	// �C���[�W���X�g�̍쐬���Ɏg�p����t���O��ݒ�
	switch(bmInfo.bmBitsPixel)
	{
	case 4 : info.flags = ILC_COLOR4 ; break;
	case 8 : info.flags = ILC_COLOR8 ; break;
	case 16: info.flags = ILC_COLOR16; break;
	case 24: info.flags = ILC_COLOR24; break;
	case 32: info.flags = ILC_COLOR32; break;
	default: return FALSE;
	}

	// �s�N�Z���f�[�^���i�[����o�b�t�@
	CArray<RGBQUAD, RGBQUAD &> buffer;
	buffer.SetSize(info.width);

	// BITMAPINFO�\���̂ɒl��ݒ�
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize        = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth       = bmInfo.bmWidth;
	bmi.bmiHeader.biHeight      =-bmInfo.bmHeight;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 8 * sizeof(buffer[0]);
	bmi.bmiHeader.biCompression = 0;

	// 32bit�ɕϊ������s�N�Z���f�[�^���擾
	if(TRUE)
	{
		CDC *pDC = GetDC();
		::GetDIBits(pDC->m_hDC, rBitmap, 0, 1, &buffer[0], &bmi, DIB_RGB_COLORS);
		ReleaseDC(pDC);
	}

	// ����̐F���}�X�N�Ƃ���
	info.mask = RGB(buffer[0].rgbRed, buffer[0].rgbGreen, buffer[0].rgbBlue);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CToolBarEx ���b�Z�[�W �n���h��

// �R���e�L�X�g���j���[�̕\��
void CToolBarEx::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(m_pContextMenu == NULL)
	{
		return;
	}

	m_pContextMenu->TrackPopupMenu(
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		point.x, point.y,
		m_pWndOwner);
}


////////////////////////////////////////
// �c�[���o�[�J�X�^�}�C�Y�p�̒ʒm���b�Z�[�W����

// �c�[���{�^�����}���\���H
void CToolBarEx::OnNotifyQueryInsert(LPNMHDR /* lpNotifyStruct */, LRESULT *result)
{
	ASSERT(m_nItemCount != -1);

	*result = TRUE;
}

// �c�[���{�^�����폜�\���H
void CToolBarEx::OnNotifyQueryDelete(LPNMHDR /* lpNotifyStruct */, LRESULT *result)
{
	ASSERT(m_nItemCount != -1);

	*result = TRUE;
}

// �J�X�^�}�C�Y�J�n
void CToolBarEx::OnNotifyBeginAdjust(LPNMHDR /* lpNotifyStruct */, LRESULT * /*result */)
{
	ASSERT(m_nItemCount != -1);
}

// �J�X�^�}�C�Y�I��
void CToolBarEx::OnNotifyEndAdjust(LPNMHDR /* lpNotifyStruct */, LRESULT * /* result */)
{
	ASSERT(m_nItemCount != -1);
}

// �c�[���o�[�̃��Z�b�g�iLoadToolBar()���Ăяo�����Ƃ��̏�Ԃ֖߂��j
void CToolBarEx::OnNotifyReset(LPNMHDR /* lpNotifyStruct */, LRESULT * /*result */)
{
	ASSERT(m_nItemCount != -1);

	// �{�^�������ׂč폜
	if(TRUE)
	{
		CToolBarCtrl &ctrl = GetToolBarCtrl();

		const int nOldButtonCount = ctrl.GetButtonCount();
		for(int i = 0; i < nOldButtonCount; i++)
		{
			ctrl.DeleteButton(0);
		}
	}

	// �ǉ����Ȃ���
	if(TRUE)
	{
		CToolBarCtrl &ctrl = GetToolBarCtrl();

		const int nNewButtonCount = m_nItemCount;
		CArray<TBBUTTON, TBBUTTON &> array_tbbutton;
		array_tbbutton.SetSize(nNewButtonCount);

		for(int i = 0; i < nNewButtonCount; i++)
		{
			array_tbbutton[i] = m_toolbarinfos[i].tbButton;
		}
		ctrl.AddButtons(nNewButtonCount, &array_tbbutton[0]);
	}

	// �ύX����
	OnNotifyToolBarChange(NULL, NULL);
}

// �c�[���o�[�ύX����
void CToolBarEx::OnNotifyToolBarChange(LPNMHDR /* lpNotifyStruct */, LRESULT * /* result */)
{
	ASSERT(m_nItemCount != -1);

	GetParentFrame()->RecalcLayout();
}

// �c�[���o�[�̃{�^�����擾
void CToolBarEx::OnNotifyGetButtonInfo(LPNMHDR lpNotifyStruct, LRESULT *result)
{
	ASSERT(m_nItemCount != -1);

	LPTBNOTIFY tbStruct = reinterpret_cast<LPTBNOTIFY>(lpNotifyStruct);

	const int iItem = tbStruct->iItem;
	if(0 <= iItem && iItem < m_nItemCount)
	{
		TOOLBARINFO &info = m_toolbarinfos[iItem];

		// �e�L�X�g�����擾
		const int cchText = info.strText.GetLength();
		LPCTSTR   pszText = info.strText;

		// �{�^�����E�e�L�X�g��������
		tbStruct->tbButton = info.tbButton;
		tbStruct->cchText  = cchText;
		tbStruct->pszText  = const_cast<LPTSTR>(pszText);

		*result = TRUE;
	}
	else
	{
		*result = FALSE;
	}
}
