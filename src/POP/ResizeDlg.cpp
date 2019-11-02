// ResizeDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "POP.h"
#include "ResizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// �R���g���[���̏�ԕۑ��p�ϐ�
	BOOL bKeepAspect  = TRUE;
	BOOL bStretch     = TRUE;
	UINT nInterpolate = (UINT)(IM_LINER - IM_NEAREST);
	UINT nSize        = 0;
END_NAMESPACE()


/////////////////////////////////////////////////////////////////////////////
// CResizeDlg �_�C�A���O


CResizeDlg::CResizeDlg(CWnd* pParent /*=NULL*/)
	: CToolTipDialog(CResizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResizeDlg)
	m_nNewWidth = 0;
	m_nNewHeight = 0;
	m_nNewWidthRate = 0;
	m_nNewHeightRate = 0;
	m_nInterpolate = -1;
	m_nSize = -1;
	m_bKeepAspect = FALSE;
	//}}AFX_DATA_INIT
}


void CResizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CToolTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResizeDlg)
	DDX_Check(pDX, IDC_RESIZE_CHECK_KEEPASPECT, m_bKeepAspect);
	DDX_Text(pDX, IDC_RESIZE_EDIT_PIXEL_NEWWIDTH, m_nNewWidth);
	DDX_Text(pDX, IDC_RESIZE_EDIT_PIXEL_NEWHEIGHT, m_nNewHeight);
	DDX_Text(pDX, IDC_RESIZE_EDIT_PERCENTAGE_NEWWIDTH, m_nNewWidthRate);
	DDX_Text(pDX, IDC_RESIZE_EDIT_PERCENTAGE_NEWHEIGHT, m_nNewHeightRate);
	DDX_Radio(pDX, IDC_RESIZE_RADIO_INTERPOLATE_NEAREST, m_nInterpolate);
	DDX_Radio(pDX, IDC_RESIZE_RADIO_SIZE_PERCENTAGE, m_nSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResizeDlg, CToolTipDialog)
	//{{AFX_MSG_MAP(CResizeDlg)
	ON_BN_CLICKED(IDC_RESIZE_CHECK_KEEPASPECT, OnResizeCheckKeepaspect)
	ON_EN_CHANGE(IDC_RESIZE_EDIT_PERCENTAGE_NEWWIDTH, OnChangeResizePercentageNewwidth)
	ON_EN_CHANGE(IDC_RESIZE_EDIT_PERCENTAGE_NEWHEIGHT, OnChangeResizePercentageNewheight)
	ON_EN_CHANGE(IDC_RESIZE_EDIT_PIXEL_NEWWIDTH, OnChangeResizePixelNewwidth)
	ON_EN_CHANGE(IDC_RESIZE_EDIT_PIXEL_NEWHEIGHT, OnChangeResizePixelNewheight)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RESIZE_RADIO_SIZE_PERCENTAGE, IDC_RESIZE_RADIO_SIZE_PIXEL, OnResizeRadioSize)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizeDlg ���b�Z�[�W �n���h��

BOOL CResizeDlg::OnInitDialog() 
{
	CToolTipDialog::OnInitDialog();
	
	// �c�[���`�b�v��ݒ�
	if(TRUE)
	{
		const UINT arToolTipIDs[]=
		{
			IDC_RESIZE_CHECK_KEEPASPECT,
			IDC_RESIZE_RADIO_INTERPOLATE_NEAREST,
			IDC_RESIZE_RADIO_INTERPOLATE_LINER,
			IDC_RESIZE_RADIO_INTERPOLATE_SPLINE,
			IDC_RESIZE_RADIO_SIZE_PERCENTAGE,
			IDC_RESIZE_RADIO_SIZE_PIXEL,
			IDC_RESIZE_EDIT_PERCENTAGE_NEWWIDTH,
			IDC_RESIZE_EDIT_PERCENTAGE_NEWHEIGHT,
			IDC_RESIZE_EDIT_PIXEL_NEWWIDTH,
			IDC_RESIZE_EDIT_PIXEL_NEWHEIGHT,
		};
		SetToolTips(arToolTipIDs, SIZE_OF_ARRAY(arToolTipIDs));
	}

	// DDX�ϐ���������
	if(TRUE)
	{
		m_nNewWidthRate  = 100;
		m_nNewHeightRate = 100;
		m_nNewWidth  = nWidth;
		m_nNewHeight = nHeight;

		SetDlgItemInt(IDC_RESIZE_STATIC_NOW_WIDTH , nWidth);
		SetDlgItemInt(IDC_RESIZE_STATIC_NOW_HEIGHT, nHeight);
	}

	// �R���g���[���̏�Ԃ�ǂݍ���
	if(TRUE)
	{
		m_bKeepAspect  = bKeepAspect;
		m_nInterpolate = nInterpolate;
		m_nSize        = nSize;
	}

	// �t���J���[���[�h�łȂ���Ε�Ԃ̎�ނ�I���ł��Ȃ��悤�ɂ���
	if(TRUE)
	{
		GetDlgItem(IDC_RESIZE_RADIO_INTERPOLATE_NEAREST)->EnableWindow(m_bIsAbleToSmooth);
		GetDlgItem(IDC_RESIZE_RADIO_INTERPOLATE_LINER  )->EnableWindow(m_bIsAbleToSmooth);
		GetDlgItem(IDC_RESIZE_RADIO_INTERPOLATE_SPLINE )->EnableWindow(m_bIsAbleToSmooth);
	}

	// �c������擾
	if(TRUE)
	{
		const int nGcd = math_gcd(nWidth, nHeight);
		nWidthRate  = nWidth  / nGcd;
		nHeightRate = nHeight / nGcd;
	}

	// �t�H�[�J�X�����킹��E�C���h�E������
	switch(m_nSize)
	{
	case 0:                                 // �u�p�[�Z���e�[�W���w��v���I�����ꂽ�ꍇ
		OnResizeRadioSize(IDC_RESIZE_RADIO_SIZE_PERCENTAGE);
		GetDlgItem(IDC_RESIZE_EDIT_PERCENTAGE_NEWWIDTH)->SetFocus();
		break;

	case 1:                                 // �u�T�C�Y���w��v���I�����ꂽ�ꍇ
		OnResizeRadioSize(IDC_RESIZE_RADIO_SIZE_PIXEL);
		GetDlgItem(IDC_RESIZE_EDIT_PIXEL_NEWWIDTH)->SetFocus();
		break;

	default:                                // �����ɂ͗��Ȃ�
		__assume(0);
	}

	UpdateData(FALSE);
	return FALSE;
}

// �u�c������Œ�v
void CResizeDlg::OnResizeCheckKeepaspect() 
{
	UpdateData(TRUE);

	// �c���䂪�Œ�Ȃ�A�����𕝂ɂ��킹��
	if(m_bKeepAspect)
	{
		m_nNewHeightRate = m_nNewWidthRate;
		m_nNewHeight     = nHeight * m_nNewHeightRate / 100;
		if(m_nNewHeight == 0)
		{
			m_nNewHeight = 1;
		}
	}
	UpdateData(FALSE);
}

// �u�T�C�Y�̎w��@�v
void CResizeDlg::OnResizeRadioSize(UINT nID) 
{
	typedef struct tagSIZEID
	{
		UINT radio_ID;
		UINT width_ID;
		UINT height_ID;
	} SIZEID;
	const SIZEID arIDs[] =
	{
		{IDC_RESIZE_RADIO_SIZE_PERCENTAGE, IDC_RESIZE_EDIT_PERCENTAGE_NEWWIDTH, IDC_RESIZE_EDIT_PERCENTAGE_NEWHEIGHT},
		{IDC_RESIZE_RADIO_SIZE_PIXEL     , IDC_RESIZE_EDIT_PIXEL_NEWWIDTH     , IDC_RESIZE_EDIT_PIXEL_NEWHEIGHT},
	};

	// �e�w��I�v�V�����̑I���^�s��Ԃ�ݒ�
	for(UINT i=0; i<SIZE_OF_ARRAY(arIDs); i++)
	{
		const BOOL bEnable = (nID == arIDs[i].radio_ID);
		GetDlgItem(arIDs[i].width_ID )->EnableWindow(bEnable);
		GetDlgItem(arIDs[i].height_ID)->EnableWindow(bEnable);
	}
}

void CResizeDlg::OnChangeResizePercentageNewwidth() 
{
	UpdateData(TRUE);

	// �����v�Z
	m_nNewWidth = nWidth * m_nNewWidthRate/100;

	// �c���䂪�Œ�Ȃ�A�������v�Z
	if(m_bKeepAspect)
	{
		m_nNewHeightRate = m_nNewWidthRate;
		m_nNewHeight     = nHeight * m_nNewHeightRate / 100;
		if(m_nNewHeight == 0)
		{
			m_nNewHeight = 1;
		}
	}
	UpdateData(FALSE);
}

void CResizeDlg::OnChangeResizePercentageNewheight() 
{
	UpdateData(TRUE);

	// �������v�Z
	m_nNewHeight = nHeight * m_nNewHeightRate / 100;

	// �c���䂪�Œ�Ȃ�A�����v�Z
	if(m_bKeepAspect)
	{
		m_nNewWidthRate = m_nNewHeightRate;
		m_nNewWidth     = nWidth * m_nNewWidthRate / 100;
		if(m_nNewWidth == 0)
		{
			m_nNewWidth = 1;
		}
	}
	UpdateData(FALSE);
}

void CResizeDlg::OnChangeResizePixelNewwidth() 
{
	UpdateData(TRUE);

	// ���̃p�[�Z���e�[�W���v�Z
	m_nNewWidthRate = m_nNewWidth * 100 / nWidth;

	// �c���䂪�Œ�Ȃ�A�������v�Z
	if(m_bKeepAspect)
	{
		m_nNewHeightRate = m_nNewWidthRate;
		m_nNewHeight     = m_nNewWidth * nHeightRate / nWidthRate;
		if(m_nNewHeight == 0)
		{
			m_nNewHeight = 1;
		}
	}
	UpdateData(FALSE);
}

void CResizeDlg::OnChangeResizePixelNewheight() 
{
	UpdateData(TRUE);

	// �����̃p�[�Z���e�[�W���v�Z
	m_nNewHeightRate = m_nNewHeight * 100 / nHeight;

	// �c���䂪�Œ�Ȃ�A�����v�Z
	if(m_bKeepAspect)
	{
		m_nNewWidthRate = m_nNewHeightRate;
		m_nNewWidth     = m_nNewHeight * nWidthRate / nHeightRate;
		if(m_nNewWidth == 0)
		{
			m_nNewWidth = 1;
		}
	}
	UpdateData(FALSE);
}

// OK�{�^��
void CResizeDlg::OnOK() 
{
	if(UpdateData(TRUE))
	{
		// �R���g���[���̏�Ԃ�ۑ�
		if(TRUE)
		{
			bKeepAspect  = m_bKeepAspect;
			nInterpolate = m_nInterpolate;
			nSize        = m_nSize;
		}

		// �p�����[�^���Z�b�g
		if(TRUE)
		{
			m_param.new_width          = m_nNewWidth;
			m_param.new_height         = m_nNewHeight;
			m_param.interpolate_method = (INTERPOLATE_METHOD)(IM_NEAREST + m_nInterpolate);
		}

		CToolTipDialog::OnOK();
	}
}
