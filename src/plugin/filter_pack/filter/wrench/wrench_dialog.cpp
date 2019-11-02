// wrench_dialog.cpp �c�C���v�������e�[�V�����t�@�C��
#include "../../stdafx.h"
#include "wrench_dialog.h"


// �R���X�g���N�^
wrench_dialog::wrench_dialog(const WRENCHPARAM &param, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, HWND hWndParent /* = NULL */)
	: preview_dialog(IDD, info_ptr, data_ptr, hWndParent)
{
	// �p�����[�^��ۑ�
	m_param = param;
}

// �f�X�g���N�^
wrench_dialog::~wrench_dialog(void)
{
}

// �R���g���[�����I�u�W�F�N�g�Ɋ֘A�t��
void wrench_dialog::AssociateDialogControls(void)
{
	m_ctrlCenterX.SubclassDlgItem(IDC_OPTION_WRENCH_SCROLL_X, this);
	m_ctrlCenterY.SubclassDlgItem(IDC_OPTION_WRENCH_SCROLL_Y, this);
	m_ctrlStaticCenterX.SubclassDlgItem(IDC_OPTION_WRENCH_STATIC_X, this);
	m_ctrlStaticCenterY.SubclassDlgItem(IDC_OPTION_WRENCH_STATIC_Y, this);
	m_ctrlPreview.SubclassDlgItem(IDC_OPTION_WRENCH_STATIC_PREVIEW, this);
/*
	AssociateControlItem(IDC_OPTION_WRENCH_SCROLL_X, m_ctrlCenterX);
	AssociateControlItem(IDC_OPTION_WRENCH_SCROLL_Y, m_ctrlCenterY);
	AssociateControlItem(IDC_OPTION_WRENCH_STATIC_X, m_ctrlStaticCenterX);
	AssociateControlItem(IDC_OPTION_WRENCH_STATIC_Y, m_ctrlStaticCenterY);
	AssociateControlItem(IDC_OPTION_WRENCH_STATIC_PREVIEW, m_ctrlPreview);
*/
}

// �_�C�A���O�p�����[�^���X�V
void wrench_dialog::UpdateData(const BOOL bUpdate /* = TRUE */)
{
	m_bUpdating = TRUE;
	if(bUpdate)
	{
		// �_�C�A���O���ϐ�
		m_param.center_x_percent = m_ctrlCenterX.GetPos();
		m_param.center_y_percent = m_ctrlCenterY.GetPos();
	}
	else
	{
		// �ϐ����_�C�A���O
		m_ctrlCenterX.SetPos(m_param.center_x_percent);
		m_ctrlCenterY.SetPos(m_param.center_y_percent);
	}
	SetDlgItemInt(IDC_OPTION_WRENCH_STATIC_X, m_param.center_x_percent, FALSE);
	SetDlgItemInt(IDC_OPTION_WRENCH_STATIC_Y, m_param.center_y_percent, FALSE);

	m_bUpdating = FALSE;
}

// �v���r���[���X�V
void wrench_dialog::UpdatePreview(void)
{
	// ���݂̐ݒ�ł˂���t�B���^��������
	UpdateData(TRUE);
	effect_wrench(&m_info_preview, &m_data_preview_before, &m_data_preview_after, &m_param, NULL, 0);

	preview_dialog::UpdatePreview();
}

// �_�C�A���O������
BOOL wrench_dialog::OnInitDialog(HWND hWndFocus, LPARAM dwInitParam)
{
	preview_dialog::OnInitDialog(hWndFocus, dwInitParam);

	// �R���g���[���̏�Ԃ�ݒ�
	m_ctrlCenterX.SetRange(0, 100);
	m_ctrlCenterY.SetRange(0, 100);
	m_ctrlPreview.SetBitmap(m_hDIBSection); // �s�N�`���[�R���g���[���̃r�b�g�}�b�v��DIB�Z�N�V�����ɐݒ�

	UpdateData(FALSE);

	// �c�[���`�b�v��ݒ�
	const UINT arIDs[] =
	{
		IDC_OPTION_WRENCH_SCROLL_X,
		IDC_OPTION_WRENCH_SCROLL_Y,
		IDC_OPTION_WRENCH_SCROLL_ANGLE,
		IDC_OPTION_WRENCH_SCROLL_RADIUS,
		IDC_OPTION_WRENCH_STATIC_PREVIEW,
	};

	SetToolTips(arIDs, SIZE_OF_ARRAY(arIDs));

	// OK�{�^���Ƀt�H�[�J�X�����킹��
	::SetFocus(GetDlgItem(IDOK));
	return FALSE;
}

void wrench_dialog::OnHScroll(const UINT nSBCode, const UINT nPos, HWND hWndScrollBar)
{
	preview_dialog::OnHScroll(nSBCode, nPos, hWndScrollBar);

	const int nPageSize = 10;

	// �X�N���[�������擾
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask  = SIF_RANGE | SIF_POS;
	::GetScrollInfo(hWndScrollBar, SB_CTL, &si);

	// ���݈ʒu���X�V
	int position = si.nPos;
	switch(nSBCode)
	{
	case SB_LEFT:  position = si.nMin; break;
	case SB_RIGHT: position = si.nMax; break;

	case SB_PAGELEFT:  position -= nPageSize; break;
	case SB_PAGERIGHT: position += nPageSize; break;

	case SB_LINELEFT:  position--; break;
	case SB_LINERIGHT: position++; break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		position = nPos;
		break;

	case SB_ENDSCROLL: break;
	}

	// �X�N���[���ʒu��ύX
	::SetScrollPos(hWndScrollBar, SB_CTL, position, TRUE);

	// �v���r���[���X�V
	UpdatePreview();
}


#if 0
// FilterWrenchDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "FilterWrenchDlg.h"
#include "mathematics.h"
#include "export.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterWrenchDlg �_�C�A���O


CFilterWrenchDlg::CFilterWrenchDlg(CWnd* pParent /*=NULL*/)
	: CToolTipDialog(CFilterWrenchDlg::IDD, pParent)
{
	m_pParam=NULL;

	//{{AFX_DATA_INIT(CFilterWrenchDlg)
	m_strRadius = _T("");
	m_strX = _T("");
	m_strY = _T("");
	m_nAngle = 0;
	m_nRadius = 0;
	m_nX = 0;
	m_nY = 0;
	m_valAngle = _T("");
	m_valRadius = _T("");
	m_valX = _T("");
	m_valY = _T("");
	//}}AFX_DATA_INIT
}


void CFilterWrenchDlg::DoDataExchange(CDataExchange* pDX)
{
	CToolTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterWrenchDlg)
	DDX_Text(pDX, IDC_FILTER_WRENCH_STATIC_RADIUS, m_strRadius);
	DDX_Text(pDX, IDC_FILTER_WRENCH_STATIC_X, m_strX);
	DDX_Text(pDX, IDC_FILTER_WRENCH_STATIC_Y, m_strY);
	DDX_Scroll(pDX, IDC_FILTER_WRENCH_SCROLL_ANGLE, m_nAngle);
	DDX_Scroll(pDX, IDC_FILTER_WRENCH_SCROLL_RADIUS, m_nRadius);
	DDX_Scroll(pDX, IDC_FILTER_WRENCH_SCROLL_X, m_nX);
	DDX_Scroll(pDX, IDC_FILTER_WRENCH_SCROLL_Y, m_nY);
	DDX_Text(pDX, IDC_FILTER_WRENCH_VALUE_ANGLE, m_valAngle);
	DDX_Text(pDX, IDC_FILTER_WRENCH_VALUE_RADIUS, m_valRadius);
	DDX_Text(pDX, IDC_FILTER_WRENCH_VALUE_X, m_valX);
	DDX_Text(pDX, IDC_FILTER_WRENCH_VALUE_Y, m_valY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilterWrenchDlg, CToolTipDialog)
	//{{AFX_MSG_MAP(CFilterWrenchDlg)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterWrenchDlg ���b�Z�[�W �n���h��

BOOL CFilterWrenchDlg::OnInitDialog() 
{
	CToolTipDialog::OnInitDialog();
	
  // �p�����[�^�����邩�ǂ�������
  ASSERT(m_pParam!=NULL);

  // �p�����[�^���Z�b�g
  {
    nWidth =m_pParam->nWidth;
    nHeight=m_pParam->nHeight;
    m_nAngle=m_pParam->param.angle*RAD2DEG;
  }

  // �f�[�^��ݒ�
  m_nX=nWidth/2;
  m_nY=nHeight/2;
  m_nRadius=min(nWidth,nHeight)/2;

  // �X�^�e�B�b�N�e�L�X�g��ݒ�
  m_strX.Format(IDC_FILTER_WRENCH_STATIC_X,nWidth);
  m_strY.Format(IDC_FILTER_WRENCH_STATIC_Y,nHeight);
  m_strRadius.Format(IDC_FILTER_WRENCH_STATIC_RADIUS,max(nWidth,nHeight));

  m_valX.Format(IDS_FORMAT_DIGIT,m_nX);
  m_valY.Format(IDS_FORMAT_DIGIT,m_nY);
  m_valRadius.Format(IDS_FORMAT_DIGIT,m_nRadius);
  m_valAngle.Format(IDS_FORMAT_DIGIT,m_nAngle);

  // �X�N���[���T�C�Y������
  {
    CScrollBar ScrollBar;
    ScrollBar.Attach(GetDlgItem(IDC_FILTER_WRENCH_SCROLL_X)->m_hWnd);
    ScrollBar.SetScrollRange(0,nWidth);
    ScrollBar.Detach();
  }
  {
    CScrollBar ScrollBar;
    ScrollBar.Attach(GetDlgItem(IDC_FILTER_WRENCH_SCROLL_Y)->m_hWnd);
    ScrollBar.SetScrollRange(0,nHeight);
    ScrollBar.Detach();
  }
  {
    CScrollBar ScrollBar;
    ScrollBar.Attach(GetDlgItem(IDC_FILTER_WRENCH_SCROLL_RADIUS)->m_hWnd);
    ScrollBar.SetScrollRange(1,(nWidth>nHeight)?nWidth:nHeight);
    ScrollBar.Detach();
  }
  {
    CScrollBar ScrollBar;
    ScrollBar.Attach(GetDlgItem(IDC_FILTER_WRENCH_SCROLL_ANGLE)->m_hWnd);
    ScrollBar.SetScrollRange(-360,360);
    ScrollBar.Detach();
  }

  // �c�[���`�b�v��ݒ�
  const UINT nIDs[]=
  {
    IDC_FILTER_WRENCH_SCROLL_X,
    IDC_FILTER_WRENCH_SCROLL_Y,
    IDC_FILTER_WRENCH_SCROLL_RADIUS,
    IDC_FILTER_WRENCH_SCROLL_ANGLE,
  };
  SetToolTips(nIDs,sizeof(nIDs)/sizeof(nIDs[0]));

  UpdateData(FALSE);

  return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CFilterWrenchDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  const int nScrollPage=10;

  // �ő�ʒu�A�ŏ��ʒu
  int nMinPos,nMaxPos;
  pScrollBar->GetScrollRange(&nMinPos,&nMaxPos);

  // �X�N���[���o�[�̈ʒu
  int nNowPos=pScrollBar->GetScrollPos();

  // �X�N���[���R�[�h�ɂ���ĕ���
  switch(nSBCode)
  {
  case SB_LEFT:
    nNowPos=nMinPos;
    break;
  case SB_RIGHT:
    nNowPos=nMaxPos;
    break;
  case SB_LINELEFT:
    nNowPos--;
    break;
  case SB_LINERIGHT:
    nNowPos++;
    break;
  case SB_PAGELEFT:
    nNowPos-=nScrollPage;
    break;
  case SB_PAGERIGHT:
    nNowPos+=nScrollPage;
    break;
  case SB_THUMBPOSITION:
  case SB_THUMBTRACK:
    nNowPos=(signed int)nPos;
    break;
  case SB_ENDSCROLL:
    break;
  }
  pScrollBar->SetScrollPos(nNowPos);      // �X�N���[���ʒu���X�V

  UpdateData(TRUE);                       // �_�C�A���O����f�[�^������Ă���

  CString *pStr;
  if(pScrollBar==GetDlgItem(IDC_FILTER_WRENCH_SCROLL_X))
  {
    pStr=&m_valX;
  }
  else if(pScrollBar==GetDlgItem(IDC_FILTER_WRENCH_SCROLL_Y))
  {
    pStr=&m_valY;
  }
  else if(pScrollBar==GetDlgItem(IDC_FILTER_WRENCH_SCROLL_RADIUS))
  {
    pStr=&m_valRadius;
  }
  else if(pScrollBar==GetDlgItem(IDC_FILTER_WRENCH_SCROLL_ANGLE))
  {
    pStr=&m_valAngle;
  }
  else
  {
    ASSERT(FALSE);
  }
  pStr->Format(IDS_FORMAT_DIGIT,nNowPos);

  UpdateData(FALSE);                      // �_�C�A���O�փf�[�^��n��

  CToolTipDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFilterWrenchDlg::OnOK() 
{
  {
    m_pParam->param.nCenterX=m_nX;
    m_pParam->param.nCenterY=m_nY;
    m_pParam->param.radius  =m_nRadius;
    m_pParam->param.angle   =m_nAngle*DEG2RAD;
  }
	
	CToolTipDialog::OnOK();
}

#endif
