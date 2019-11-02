// ImportWindowDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "pop.h"
#include "ImportWindowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportWindowDlg �_�C�A���O


CImportWindowDlg::CImportWindowDlg(CWnd* pParent /*=NULL*/)
	: CToolTipDialog(CImportWindowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportWindowDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
	m_hWndCapture = NULL;
	m_bCaptured = FALSE;
	m_hCursorDefault = ::GetCursor();
	m_hCursorDrag    = ::LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_CROSSHAIR));
}


void CImportWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CToolTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportWindowDlg)
	DDX_Control(pDX, IDC_IMPORT_WINDOW_STATIC_CROSSHAIR, m_staticCrosshair);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportWindowDlg, CToolTipDialog)
	//{{AFX_MSG_MAP(CImportWindowDlg)
	ON_BN_CLICKED(IDC_IMPORT_WINDOW_STATIC_CROSSHAIR, OnStaticCrosshair)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////
// �⏕�֐�

// �t���[���𔽓]
BOOL CImportWindowDlg::InvertFrame(HWND hWnd)
{
	const int width = 4;

	if(!::IsWindow(hWnd))
	{
		return FALSE;
	}

	// �E�C���h�E�̑傫�����擾
	RECT rect;
	::GetWindowRect(hWnd, &rect);

	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.left = 0;
	rect.top = 0;

	// �N���b�s���O�̈�����߂�
	HDC hDC = ::GetWindowDC(hWnd);
	::ExcludeClipRect(	
		hDC,
		rect.left   + width,
		rect.top    + width,
		rect.right  - width,
		rect.bottom - width);

	// ���߂��N���b�s���O�̈�𔽓]
	::InvertRect(hDC, &rect);

	::ReleaseDC(hWnd, hDC);
	return TRUE;
}

void CImportWindowDlg::Restore()
{
	// �܂�m_hWndCapture�̃t���[�������]���Ă���̂Ō��ɖ߂�
	InvertFrame(m_hWndCapture);

	// �}�E�X�L���v�`��������
	ReleaseCapture();
	m_bCaptured = FALSE;

	// �}�[�N��\��
	m_staticCrosshair.ShowWindow(SW_SHOW);

	// �J�[�\�������ɖ߂�
	::SetCursor(m_hCursorDefault);
}

/////////////////////////////////////////////////////////////////////////////
// CImportWindowDlg ���b�Z�[�W �n���h��

BOOL CImportWindowDlg::OnInitDialog() 
{
	CToolTipDialog::OnInitDialog();

	// �c�[���`�b�v��ݒ�
	if(TRUE)
	{
		const UINT arIDs[] =
		{
			IDC_IMPORT_WINDOW_STATIC_CROSSHAIR,
		};
		SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));
	}

	// �}�[�N�ɃJ�[�\����ݒ�
	m_staticCrosshair.SetCursor(m_hCursorDrag);

	return TRUE;                            // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ�
}

// �}�[�N���N���b�N���ꂽ
void CImportWindowDlg::OnStaticCrosshair() 
{
	// �h���b�O�p�J�[�\���ɕύX
	::SetCursor(m_hCursorDrag);

	// �}�[�N�������i�}�[�N���h���b�O����Ă���悤�Ɍ�����j
	m_staticCrosshair.ShowWindow(SW_HIDE);

	// �L���v�`���J�n
	SetCapture();
	m_bCaptured = TRUE;

	// ���̎��_�ł͂܂��E�C���h�E��I�����Ȃ�
	m_hWndCapture = NULL;
}

// �}�E�X�𓮂�����
void CImportWindowDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bCaptured)
	{
		// �X�N���[�����W�ł̃}�E�X�ʒu���擾
		POINT ptScreen;
		::GetCursorPos(&ptScreen);

		HWND hWnd = ::WindowFromPoint(ptScreen);
		if(hWnd != NULL && hWnd != m_hWndCapture)
		{
			// �}�E�X�̂���E�C���h�E�t���[���𔽓]
			InvertFrame(m_hWndCapture);
			InvertFrame(hWnd);
			m_hWndCapture = hWnd;
		}
	}

	CToolTipDialog::OnMouseMove(nFlags, point);
}

// ���{�^���𗣂���
void CImportWindowDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bCaptured)
	{
		Restore();

		if(m_hWndCapture != NULL)
		{
			// �E�C���h�E�n���h������C���[�W��V�K�쐬
			extern CPOPApp theApp;
			theApp.NewImageFromWindow(m_hWndCapture, m_bAltogether, m_dwMilliSeconds);
		}
		DestroyWindow();
	}
}

// �E�{�^���𗣂���
void CImportWindowDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bCaptured)
	{
		// �L�����Z������
		Restore();
		m_hWndCapture = NULL;
	}

	CToolTipDialog::OnRButtonDown(nFlags, point);
}

// �L�����Z��
void CImportWindowDlg::OnCancel() 
{
	DestroyWindow();
}

void CImportWindowDlg::PostNcDestroy() 
{
	CToolTipDialog::PostNcDestroy();
	delete this;
}
