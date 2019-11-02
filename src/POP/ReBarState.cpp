// ReBarState.cpp �c�C���v�������e�[�V�����t�@�C��
#include "stdafx.h"
#include "ReBarState.h"
#include <afxtempl.h>


// ���W�X�g���̃Z�N�V�����^�G���g����
#define TCHAR_REG_SECTION_REBAR         _T("ReBar Placement#%d")
#define TCHAR_REG_ENTRY_IDMAP           _T("ID-%d")
#define TCHAR_REG_ENTRY_REBARINFO       _T("ReBar#%d")
#define TCHAR_REG_ENTRY_SUFFIX_SIZE     _T(".cx")
#define TCHAR_REG_ENTRY_SUFFIX_STYLE    _T(".fStyle")


// ���W�X�g���ɕۑ��������o�[�̏�Ԃ𕜌�
void LoadReBarState(CReBar &bar)
{
	CReBarCtrl &ctrl = bar.GetReBarCtrl();
	const int count = ctrl.GetBandCount();

	CMap<int, int &, HWND, HWND &> id_map;
	CList<HWND, HWND &> hWndList;

	// ID-HWND �Ή��}�b�v���쐬
	for(int i = 0; i < count; i++)
	{
		// �q�E�C���h�E���擾
		REBARBANDINFO rbbi = {sizeof(REBARBANDINFO), RBBIM_CHILD};
		ctrl.GetBandInfo(i, &rbbi);

		// ID�ƃE�C���h�E�n���h����Ή��t����
		HWND hWnd = rbbi.hwndChild;
		int  id   = ::GetDlgCtrlID(hWnd);
		id_map[id] = hWnd;

		hWndList.AddTail(hWnd);

		// �o���h���͂���
		rbbi.hwndChild = NULL;
		ctrl.SetBandInfo(i, &rbbi);
	}

	// �o���h�̔z�u��ݒ�
	for(int j = 0; j < count; j++)
	{
		CWinApp *pApp = AfxGetApp();
		ASSERT(pApp != NULL);

		REBARBANDINFO rbbi = {sizeof(REBARBANDINFO), 0};

		// �Z�N�V�����ƃG���g���[���擾
		CString section, entry;
		section.Format(TCHAR_REG_SECTION_REBAR, bar.GetDlgCtrlID());
		entry.Format(TCHAR_REG_ENTRY_IDMAP, j);

		// j�Ԗڂ̃E�C���h�EID�ɑΉ�����E�C���h�E�n���h�����擾
		POSITION pos = NULL;
		int id = pApp->GetProfileInt(section, entry, -1);
		HWND hWnd = NULL;

		// ID�ɑΉ�����E�C���h�E�����������ꍇ
		if(id_map.Lookup(id, hWnd))
		{
			entry.Format(TCHAR_REG_ENTRY_REBARINFO, id);

			// cx�����[�h
			const int cx = pApp->GetProfileInt(section, entry + TCHAR_REG_ENTRY_SUFFIX_SIZE, -1);
			if(cx != -1)
			{
				rbbi.fMask |= RBBIM_SIZE;
				rbbi.cx     = cx;
			}

			// fStyle�����[�h
			const int fStyle = pApp->GetProfileInt(section, entry + TCHAR_REG_ENTRY_SUFFIX_STYLE, -1);
			if(cx != -1 && fStyle != -1)
			{
				rbbi.fMask |= RBBIM_STYLE;
				rbbi.fStyle = fStyle;
			}

			pos = hWndList.Find(hWnd);
		}
		// ID�ɑΉ�����E�C���h�E��������Ȃ������ꍇ
		else
		{
			// CMap::GetStartPosition()�́A�ǂ̏��Ԃŏo�Ă��邩�킩��Ȃ�����g���Ȃ�
			pos  = hWndList.GetHeadPosition();
			hWnd = hWndList.GetAt(pos);
			id   = ::GetDlgCtrlID(hWnd);
		}

		// �q�E�C���h�E��ݒ�
		rbbi.fMask    |= RBBIM_CHILD;
		rbbi.hwndChild = hWnd;
		ctrl.SetBandInfo(j, &rbbi);

		// ReBar�ɒǉ�����ID���폜
		ASSERT(pos != NULL);
		id_map.RemoveKey(id);
		hWndList.RemoveAt(pos);
	}

	// �}�b�v�ƃ��X�g�͋�ɂȂ��Ă���͂�
	ASSERT(id_map.IsEmpty());
	ASSERT(hWndList.IsEmpty());
}

// ���W�X�g���Ƀ��o�[�̏�Ԃ�ۑ�
void SaveReBarState(const CReBar &bar)
{
	CReBarCtrl &ctrl = bar.GetReBarCtrl();

	const int count = ctrl.GetBandCount();
	for(int i = 0; i < count; i++)
	{
		CWinApp *pApp = AfxGetApp();
		ASSERT(pApp != NULL);

		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask  = RBBIM_SIZE | RBBIM_STYLE | RBBIM_CHILD;

		ctrl.GetBandInfo(i, &rbbi);

		const int id = ::GetDlgCtrlID(rbbi.hwndChild);

		// �Z�N�V�������擾
		CString section;
		section.Format(TCHAR_REG_SECTION_REBAR, bar.GetDlgCtrlID());

		// �C���f�b�N�X��ID�̑Ή���o�^
		if(TRUE)
		{
			CString entry;
			entry.Format(TCHAR_REG_ENTRY_IDMAP, i);
			pApp->WriteProfileInt(section, entry, id);
		}

		// �w��ID�̃o���h����o�^
		if(TRUE)
		{
			CString entry;
			entry.Format(TCHAR_REG_ENTRY_REBARINFO, id);
			pApp->WriteProfileInt(section, entry + TCHAR_REG_ENTRY_SUFFIX_SIZE , rbbi.cx);
			pApp->WriteProfileInt(section, entry + TCHAR_REG_ENTRY_SUFFIX_STYLE, rbbi.fStyle);
		}
	}
}
