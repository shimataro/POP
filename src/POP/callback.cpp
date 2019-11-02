// callback.cpp �c�C���v�������e�[�V�����t�@�C��
#include "stdafx.h"
#include "POP.h"
#include "callback.h"


#define TCHAR_SPACE   _T(" ")


/////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// �X�e�[�^�X�o�[�ւ̃|�C���^���擾
	CStatusBar *GetStatusBar(void)
	{
		CFrameWnd  *pFrameWnd  = STATIC_DOWNCAST(CFrameWnd , AfxGetMainWnd());
		CStatusBar *pStatusBar = STATIC_DOWNCAST(CStatusBar, pFrameWnd->GetMessageBar());

		return pStatusBar;
	}
END_NAMESPACE()


////////////////////////////////////////////////////////////////////////////////
// �R�[���o�b�N������
long callback_init(const int id)
{
	// �}�E�X�J�[�\���������v�ɂ���
	AfxGetApp()->DoWaitCursor(1);

	// �X�e�[�^�X�o�[�ւ̃|�C���^���擾
	CStatusBar *pStatusBar = GetStatusBar();

	// �������̃L���v�V������\��
	CString strCaption;
	strCaption.LoadString(id);
	pStatusBar->SetWindowText(strCaption);

	// �L���v�V�����̕����擾
	int width = 0;
	if(TRUE)
	{
		CClientDC dc(pStatusBar);
		CFont *pOldFont = dc.SelectObject(pStatusBar->GetFont());

		const CSize caption_size = dc.GetTextExtent(strCaption + TCHAR_SPACE);
		width = caption_size.cx;

		dc.SelectObject(pOldFont);
	}

	// �X�e�[�^�X�o�[�̃N���C�A���g�̈�T�C�Y���擾
	RECT rect;
	pStatusBar->GetClientRect(&rect);

	// �v���O���X�o�[���쐬
	HWND hProgress = CreateWindow(
		PROGRESS_CLASS, NULL,                   // lpClassName, lpWindowName
		WS_CHILD | WS_VISIBLE,                  // dwStyle
		width         , rect.bottom * 1 / 8,    // x, y
		rect.right / 4, rect.bottom * 3 / 4,    // nWidth, nHeight
		pStatusBar->m_hWnd,                     // hWndParent
		NULL,                                   // hMenu
		AfxGetInstanceHandle(),                 // hInstance
		NULL);                                  // lpParam

	// �v���O���X�o�[�̃n���h����߂�l�Ƃ���
	return reinterpret_cast<long>(hProgress);
}

// �R�[���o�b�N�i�I���Ȃ�TRUE��Ԃ��j
int CALLBACK callback_default(const int num, const int denom, const long data)
{
	// �v���O���X�o�[�̃n���h�����擾
	HWND hProgress = reinterpret_cast<HWND>(data);
	ASSERT(hProgress != NULL);

	// �L���[���̃��b�Z�[�W������
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		switch(msg.message)
		{
		case WM_PAINT:
			// WM_PAINT���b�Z�[�W�Ȃ�΃f�B�X�p�b�`
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			break;

		case WM_KEYDOWN:
			// ESC�L�[�������ꂽ��I��
			if(msg.wParam == VK_ESCAPE)
			{
				return TRUE;
			}
		}
	}

	// �v���O���X�o�[��L�΂�
	SendMessage(hProgress, PBM_SETRANGE, 0  , MAKELPARAM(0, denom));
	SendMessage(hProgress, PBM_SETPOS  , num, 0);

	return FALSE;
}

// �������Ȃ��R�[���o�b�N�֐�
int CALLBACK callback_none(const int /* num */, const int /* denom */, const long /* data */)
{
	return 0;
}

// �R�[���o�b�N�I���i�v���O���X�o�[�̔j�����L���v�V�����̕����j
void callback_term(long data)
{
	// �v���O���X�o�[�̔j��
	if(TRUE)
	{
		// �v���O���X�o�[�̃n���h�����擾
		HWND hProgress = reinterpret_cast<HWND>(data);
		ASSERT(hProgress != NULL);

		DestroyWindow(hProgress);
	}

	// �L���v�V���������ɖ߂�
	if(TRUE)
	{
		CString strCaption;
		strCaption.LoadString(AFX_IDS_IDLEMESSAGE);

		CStatusBar *pStatusBar = GetStatusBar();
		pStatusBar->SetWindowText(strCaption);
	}

	// �}�E�X�J�[�\�������ɖ߂�
	AfxGetApp()->DoWaitCursor(-1);
}
