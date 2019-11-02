// ChildFrm.cpp : CChildFrame �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "POP.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame �N���X�̍\�z/����

CChildFrame::CChildFrame()
{
	// TODO: �����o�������R�[�h�����̈ʒu�ɒǉ����Ă��������B
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs �̐ݒ���s���āAWindow �N���X�܂���
	//       �X�^�C����ύX���Ă��������B

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame �N���X�̐f�f

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame �N���X�̃��b�Z�[�W�n���h��

// �t���[���̃L���v�V������ݒ�
#include "POPDoc.h"
void CChildFrame::OnUpdateFrameTitle(BOOL bTitle)
{
	CMDIChildWnd::OnUpdateFrameTitle(bTitle);

	// �A�N�e�B�u�ȃh�L�������g���擾
	CPOPDoc *pPOPDoc = NULL;
	if(TRUE)
	{
		// �r���[���擾
		CView *pView = GetActiveView();
		ASSERT(pView != NULL);

		pPOPDoc = STATIC_DOWNCAST(CPOPDoc, pView->GetDocument());
		ASSERT(pPOPDoc != NULL);
	}

	// �摜�����擾
	const IMAGE_INFO &info = pPOPDoc->m_info;
	const UINT title_id = pPOPDoc->IsModified() ? IDS_FORMAT_TITLE_MODIFIED : IDS_FORMAT_TITLE;

	CString colormode;
	colormode.LoadString(IDS_FORMAT_TITLE_COLORMODE_GRAYSCALE + info.mode);

	// �L���v�V������������쐬
	CString caption;
	caption.Format(
		title_id,
		pPOPDoc->GetTitle(),
		info.width, info.height,
		info.depth, colormode,
		info.colors);

	// �^�C�g���o�[�ɏ���\��
	SetWindowText(caption);
}
