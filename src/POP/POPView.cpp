// POPView.cpp : CPOPView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "POP.h"

#include "MainFrm.h"
#include "POPDoc.h"
#include "POPView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPOPView

IMPLEMENT_DYNCREATE(CPOPView, CScrollView)

BEGIN_MESSAGE_MAP(CPOPView, CScrollView)
	//{{AFX_MSG_MAP(CPOPView)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_XBUTTONDOWN, OnXButtonDown)
	ON_MESSAGE(WM_XBUTTONUP  , OnXButtonUp)
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPOPView �N���X�̍\�z/����

CPOPView::CPOPView()
{
	//{{AFX_DATA_INIT(CPOPView)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�̏�����������ǉ����܂��B
	//}}AFX_DATA_INIT
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B

	// �r���[�̍ő�T�C�Y���Ƃ肠����-1�iOnInitialUpdate()�Őݒ肷��j
	m_sizeViewMax.cx = -1;
	m_sizeViewMax.cy = -1;

	// �}�E�X�C�x���g���g���b�L���O���Ă��Ȃ�
	m_bTrackingMouseEvent = FALSE;

	// �{�^����������Ă��Ȃ�
	m_bXButton1Down = FALSE;
	m_bXButton2Down = FALSE;

	// �K���}�e�[�u�����쐬
	CreateGammaTable(GAMMA_DEFAULT_FILE, GAMMA_DEFAULT_DECODE, GAMMA_DEFAULT_DISPLAY, m_gamma_table);

	::ZeroMemory(&m_bitmapinfo, sizeof(m_bitmapinfo));
	m_buffer = NULL;
}

CPOPView::~CPOPView()
{
}

void CPOPView::DoDataExchange(CDataExchange* pDX)
{
	CScrollView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPOPView)
		// ����: ClassWizard �͂��̈ʒu�� DDX �� DDV �̌Ăяo����ǉ����܂��B
	//}}AFX_DATA_MAP
}

BOOL CPOPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CScrollView::PreCreateWindow(cs);
}

// �r���[�������Ƃ��ɍŏ��ɌĂ΂��
void CPOPView::OnInitialUpdate()
{
	// �X�e�[�^�X�o�[�ւ̃|�C���^���擾
	if(TRUE)
	{
		CMainFrame *pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
		ASSERT(pMainFrame != NULL);

		CStatusBar *pStatusBar = STATIC_DOWNCAST(CStatusBar, pMainFrame->GetMessageBar());
		ASSERT(pStatusBar != NULL);

		m_pStatusBar = pStatusBar;
	}

	// �X�e�[�^�X�o�[�̃y�C���C���f�b�N�X���擾
	if(TRUE)
	{
		m_nIndexColor = m_pStatusBar->CommandToIndex(ID_INDICATOR_COLOR);
		m_nIndexPoint = m_pStatusBar->CommandToIndex(ID_INDICATOR_POINT);
		ASSERT(m_nIndexColor != -1);
		ASSERT(m_nIndexPoint != -1);
	}

	// �r���[�̍ő�T�C�Y��������Ԃ̃r���[�T�C�Y
	if(TRUE)
	{
		CRect rect;
		GetClientRect(rect);
		m_sizeViewMax = rect.Size();
	}

	CScrollView::OnInitialUpdate();
}

// �h�L�������g���ύX���ꂽ�iDIB�Z�N�V�����̍X�V�A�E�C���h�E�̃��T�C�Y�j
void CPOPView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// �摜�����擾
	const IMAGE_INFO &info = GetDocument()->m_info;
	const IMAGE_DATA &data = GetDocument()->m_data;

	// ���݂�DIB�Z�N�V�����ƃT�C�Y���قȂ��Ă�����ADIB�Z�N�V��������蒼��
	if(m_bitmapinfo.bmiHeader.biWidth  != info.width
	|| m_bitmapinfo.bmiHeader.biHeight != info.height)
	{
		// ���݂�DIB�Z�N�V������j��
		m_dibsection.DeleteObject();

		// ��蒼��
		HBITMAP hDIBSection = DIBSECTION_Create32bits(
			info.width, info.height,
			&m_bitmapinfo, reinterpret_cast<void **>(&m_buffer));

		m_dibsection.Attach(hDIBSection);
	}

	// �ݒ��ǂݏo��
	SETTING setting;
	if(TRUE)
	{
		extern CPOPApp theApp;
		theApp.GetSetting(setting);
	}

	// �K���}�e�[�u���̍X�V
	if(TRUE)
	{
		CreateGammaTable(
			setting.display.check.elements.activate_gamma_file     ? info.gamma_file               : GAMMA_DEFAULT_FILE,
			setting.display.check.elements.customize_gamma_decode  ? setting.display.gamma_decode  : GAMMA_DEFAULT_DECODE,
			setting.display.check.elements.customize_gamma_display ? setting.display.gamma_display : GAMMA_DEFAULT_DISPLAY,
			m_gamma_table);
	}

	// DIB�Z�N�V�����̃f�[�^���쐬
	if(TRUE)
	{
		const int      grid_size = setting.display.background_grid_size;
		const COLORREF bgc1      = setting.display.background_color1;
		const COLORREF bgc2      = setting.display.background_color2;
		BITMAP_SetDataFromImage(
			&info, &data,
			m_gamma_table,
			(grid_size == 0) ? BITMAP_GRID_SINGLE_COLOR : grid_size + 1,
			bgc1, bgc2,
			m_buffer);
	}

	// �q�t���[���̃^�C�g���o�[�ɃC���[�W����\��
	GetParentFrame()->DelayUpdateFrameTitle();

	// �E�C���h�E�̃��T�C�Y
	ResizeWindow();

	CScrollView::OnUpdate(pSender, lHint, pHint);
}

/////////////////////////////////////////////////////////////////////////////
// CPOPView �N���X�̕`��

void CPOPView::OnDraw(CDC* pDC) 
{
	// pDC�ƌ݊��ȃ������f�o�C�X�R���e�L�X�g���쐬
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);

	// DIB�Z�N�V�������I�u�W�F�N�g�Ƃ��đI��
	CBitmap *pOldBitmap = dcMemory.SelectObject(&m_dibsection);

	// �N���b�s���O�̈���擾
	CRect rectClip;
	pDC->GetClipBox(&rectClip);

	// �N���C�A���g�̈�̓����Ƀu���b�g
	pDC->BitBlt(
		rectClip.left, rectClip.top,
		min(m_bitmapinfo.bmiHeader.biWidth , rectClip.Width()),
		min(m_bitmapinfo.bmiHeader.biHeight, rectClip.Height()),
		&dcMemory,
		rectClip.left, rectClip.top,
		SRCCOPY);

	// �I�����ꂽ�̈��`��
	if(TRUE)
	{
		CRect rectSelect = GetDocument()->m_rect;
		if(!rectSelect.IsRectEmpty())
		{
			rectSelect.DeflateRect(1, 1);

			// �������W�֕ϊ�
			CRectTracker tracker(rectSelect, CRectTracker::dottedLine | CRectTracker::resizeInside | CRectTracker::hatchInside);
			ConvertToDevicePoint(tracker.m_rect);

			tracker.Draw(pDC);
		}
	}

	// �I���I�u�W�F�N�g�����ɖ߂�
	dcMemory.SelectObject(pOldBitmap);
	pOldBitmap = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CPOPView �N���X�̈��

BOOL CPOPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);                   // �ő�y�[�W��ݒ�
	return DoPreparePrinting(pInfo);
}

void CPOPView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
	CScrollView::OnPrepareDC(pDC, pInfo);
}

void CPOPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
}

void CPOPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
}

void CPOPView::OnPrint(CDC* pDC, CPrintInfo* /* pInfo */)
{
	ASSERT_VALID(pDC);

	const int width  = m_bitmapinfo.bmiHeader.biWidth;
	const int height = m_bitmapinfo.bmiHeader.biHeight;

	// �y�[�W�̕��A����
	const int nPageWidth  = pDC->GetDeviceCaps(HORZRES);
	const int nPageHeight = pDC->GetDeviceCaps(VERTRES);

	// ������镝�A�����i�y�[�W�ɍ����悤�Ɍ��C���[�W���g��^�k���j
	int nPrintWidth, nPrintHeight;
	if(nPageWidth > nPageHeight)
	{
		nPrintWidth  = width * nPageHeight / height;
		nPrintHeight = nPageHeight;
	}
	else
	{
		nPrintWidth  = nPageWidth;
		nPrintHeight = height * nPageWidth / width;
	}

	const int nOldMap  = pDC->SetMapMode(MM_TEXT);
	const int nOldMode = pDC->SetStretchBltMode(COLORONCOLOR);

	::StretchDIBits(
		pDC->m_hDC,
		0, 0,
		nPrintWidth, nPrintHeight,
		0, 0,
		m_bitmapinfo.bmiHeader.biWidth, m_bitmapinfo.bmiHeader.biHeight,
		m_buffer,
		&m_bitmapinfo,
		DIB_RGB_COLORS,
		SRCCOPY);

	pDC->SetStretchBltMode(nOldMode);
	pDC->SetMapMode(nOldMap);
}

/////////////////////////////////////////////////////////////////////////////
// CPOPView �N���X�̐f�f

#ifdef _DEBUG
void CPOPView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPOPView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CPOPDoc* CPOPView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPOPDoc)));
	return (CPOPDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// �⏕�֐�

// �X�N���[���r���[�̃T�C�Y��ύX
void CPOPView::ResizeWindow(void)
{
	const SIZE sizeNew = {m_bitmapinfo.bmiHeader.biWidth, m_bitmapinfo.bmiHeader.biHeight};

	SetScrollSizes(MM_TEXT, sizeNew);

	// �q�t���[�����ő剻����Ă��Ȃ���΃t���[���̃T�C�Y�ύX
	if(!GetParentFrame()->IsZoomed())
	{
		BOOL bShrinkOnly = FALSE;
		if(sizeNew.cx > m_sizeViewMax.cx)
		{
			bShrinkOnly = TRUE;
		}
		if(sizeNew.cy > m_sizeViewMax.cy)
		{
			bShrinkOnly = TRUE;
		}
		ResizeParentToFit(bShrinkOnly);
	}
}

// �X�e�[�^�X�o�[�Ƀ}�E�X�|�C���^�ʒu�̐F������\��
void CPOPView::UpdateStatusPaneColor(const POINT &point)
{
	const CRect rect(0, 0, m_bitmapinfo.bmiHeader.biWidth, m_bitmapinfo.bmiHeader.biHeight);
	CString status_color;

	if(rect.PtInRect(point))
	{
		// �_�̐F������\��
		const IMAGE_INFO &info = GetDocument()->m_info;
		const IMAGE_DATA &data = GetDocument()->m_data;
		uint_t r = 0, g = 0, b = 0;

		switch(info.mode)
		{
		case COLORMODE_GRAYSCALE:
			if(TRUE)
			{
				const_uint32_t offset = CALC_OFFSET(point.x, point.y, info.width);

				// �������̈�����b�N
				const_pixel_gray8_ptr_t pixel_gray8_ptr;
				auto_lock lock_obj(&data, pixel_gray8_ptr);

				r = g = b = pixel_gray8_ptr[offset].el.gray;
			}
			break;

		case COLORMODE_INDEX:
			if(TRUE)
			{
				const_uint_t   depth  = info.depth;
				const_uint32_t hsize  = CALC_HSIZE(info.width, depth);

				// log2(depth)���v�Z
				const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
				const_uint_t log2_depth = log2_depth_array[depth - 1];

				// �������̈�����b�N
				const_uint8_ptr_t    index_ptr = NULL;
				const_palette8_ptr_t palette8_ptr = NULL;
				auto_lock lock_obj(&data, index_ptr, palette8_ptr);

				// �I�t�Z�b�g�ƃV�t�g�ʂ����߂�
				const_uint32_t offset = CALC_PACK_OFFSET(point.x, log2_depth);
				const int      shifts = CALC_PACK_SHIFTS(point.x, log2_depth);

				// �C���f�b�N�X�l���擾
				const_uint8_t index = PACK_EXTRACT(index_ptr[offset + hsize * point.y], depth, shifts);

				r = palette8_ptr[index].r;
				g = palette8_ptr[index].g;
				b = palette8_ptr[index].b;
			}
			break;

		case COLORMODE_RGB:
			if(TRUE)
			{
				const_uint32_t offset = CALC_OFFSET(point.x, point.y, info.width);

				// �������̈�����b�N
				const_pixel_rgb8_ptr_t pixel_rgb8_ptr;
				auto_lock lock_obj(&data, pixel_rgb8_ptr);

				r = pixel_rgb8_ptr[offset].el.r;
				g = pixel_rgb8_ptr[offset].el.g;
				b = pixel_rgb8_ptr[offset].el.b;
			}
			break;
		}

		// �F���𕶎���
		status_color.Format(IDS_INDICATOR_COLOR, r, g, b);
	}
	else
	{
		// �F����������
		status_color.LoadString(ID_INDICATOR_COLOR);
	}

	m_pStatusBar->SetPaneText(m_nIndexColor, status_color);
}

// �X�e�[�^�X�o�[�Ƀ}�E�X�|�C���^�̍��W��\��
void CPOPView::UpdateStatusPanePoint(const POINT &point)
{
	const CRect rect(0, 0, m_bitmapinfo.bmiHeader.biWidth, m_bitmapinfo.bmiHeader.biHeight);
	CString status;

	if(rect.PtInRect(point))
	{
		// �}�E�X���W�𕶎���
		status.Format(IDS_INDICATOR_POINT, point.x, point.y);
	}
	else
	{
		// �}�E�X���W��������
		status.LoadString(ID_INDICATOR_POINT);
	}

	m_pStatusBar->SetPaneText(m_nIndexPoint, status);
}

// �_��_�����W�֕ϊ�
void CPOPView::ConvertToLogicalPoint(POINT &point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
}

// ��`��_�����W�֕ϊ�
void CPOPView::ConvertToLogicalPoint(RECT &rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&rect);
}

// �T�C�Y��_�����W�֕ϊ�
void CPOPView::ConvertToLogicalPoint(SIZE &size)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&size);
}

// �_�𕨗����W�֕ϊ�
void CPOPView::ConvertToDevicePoint(POINT &point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.LPtoDP(&point);
}

// ��`�𕨗����W�֕ϊ�
void CPOPView::ConvertToDevicePoint(RECT &rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.LPtoDP(&rect);
}

// �T�C�Y�𕨗����W�֕ϊ�
void CPOPView::ConvertToDevicePoint(SIZE &size)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.LPtoDP(&size);
}


/////////////////////////////////////////////////////////////////////////////
// CPOPView �N���X�̃��b�Z�[�W �n���h��

// �r���[�̃T�C�Y���ύX���ꂽ�i�X�N���[���V���t�g�N���b�N���̃X�N���[���ʂ�ύX�j
void CPOPView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	SetScrollSizes(
		MM_TEXT,
		CSize(m_bitmapinfo.bmiHeader.biWidth, m_bitmapinfo.bmiHeader.biHeight),
		CSize(cx / 2, cy / 2));
}

// �w�i��h��Ԃ�
BOOL CPOPView::OnEraseBkgnd(CDC* pDC) 
{
	CBrush br;                              // �u���V���쐬
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(128, 128, 128));
	FillOutsideRect(pDC, &br);              // �N���C�A���g�̈�̊O����h��Ԃ�

	return TRUE;
}

// �J�[�\���̌`��ݒ�
BOOL CPOPView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CRectTracker tracker;
	tracker.m_rect = GetDocument()->m_rect;
	ConvertToDevicePoint(tracker.m_rect);

	// �g���b�J�ɓ]��
	if(!tracker.m_rect.IsRectEmpty() && tracker.SetCursor(this, nHitTest))
	{
		return TRUE;
	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

// �}�E�X�J�[�\�����������i�X�e�[�^�X�o�[�ɏ���\���j
void CPOPView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// �}�E�X���W���X�N���[���ʒu���l�������_�����W�ɕϊ�
	POINT ptLogical = point;
	ConvertToLogicalPoint(ptLogical);

	// �X�e�[�^�X�o�[�̐F�E���W�����X�V
	UpdateStatusPaneColor(ptLogical);
	UpdateStatusPanePoint(ptLogical);

	if(!m_bTrackingMouseEvent)
	{
		m_bTrackingMouseEvent = TRUE;

		// WM_MOUSELEAVE���b�Z�[�W���L���v�`��
		TRACKMOUSEEVENT tme;
		tme.cbSize    = sizeof(tme);
		tme.dwFlags   = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		_TrackMouseEvent(&tme);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

// �}�E�X���r���[�O�ɏo��
LRESULT CPOPView::OnMouseLeave(WPARAM /* wParam */, LPARAM /* lParam */)
{
	// �X�e�[�^�X�o�[�̐F�E���W�\����������
	UpdateStatusPaneColor(CPoint(-1, -1));
	UpdateStatusPanePoint(CPoint(-1, -1));

	m_bTrackingMouseEvent = FALSE;
	return 0;
}

// 5�{�^���}�E�X�̃T�C�h�{�^������
LRESULT CPOPView::OnXButtonDown(WPARAM wParam, LPARAM lParam)
{
	const UINT nFlags = LOWORD(wParam);
	const CPoint point(LOWORD(lParam), HIWORD(lParam));

	switch(HIWORD(wParam))
	{
	case XBUTTON1:                          // �T�C�h�{�^����
		OnXButton1Down(nFlags, point);
		break;

	case XBUTTON2:                          // �T�C�h�{�^����
		OnXButton2Down(nFlags, point);
		break;
	}

	return 0;
}

// 5�{�^���}�E�X�̃T�C�h�{�^���J��
LRESULT CPOPView::OnXButtonUp(WPARAM wParam, LPARAM lParam)
{
	const UINT nFlags = LOWORD(wParam);
	const CPoint point(LOWORD(lParam), HIWORD(lParam));

	switch(HIWORD(wParam))
	{
	case XBUTTON1:                          // �T�C�h�{�^����
		OnXButton1Up(nFlags, point);
		break;

	case XBUTTON2:                          // �T�C�h�{�^����
		OnXButton2Up(nFlags, point);
		break;
	}

	return 0;
}

void CPOPView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect &rect = GetDocument()->m_rect;

	CRectTracker tracker;
	tracker.m_rect = rect;
	ConvertToDevicePoint(tracker.m_rect);

	// �g���b�J���ɃJ�[�\�����Ȃ��ꍇ
	if(tracker.HitTest(point) == -1)
	{
		// ���o�[�o���h�`��
		tracker.TrackRubberBand(this, point, TRUE);
	}
	else
	{
		// �g���b�L���O
		tracker.Track(this, point, TRUE);
	}

	// �I��̈��_�����W�֕ϊ�
	rect = tracker.m_rect;
	rect.NormalizeRect();
	ConvertToLogicalPoint(rect);

	Invalidate(FALSE);

	CScrollView::OnLButtonDown(nFlags, point);
}

// 5�{�^���}�E�X�̃T�C�h�{�^���i��j���������ꂽ
void CPOPView::OnXButton1Down(UINT /* nFlags */, CPoint /* point */)
{
	m_bXButton1Down = TRUE;
	SetCapture();
}

// 5�{�^���}�E�X�̃T�C�h�{�^���i��j���J�����ꂽ
void CPOPView::OnXButton1Up(UINT /* nFlags */, CPoint point)
{
	if(m_bXButton1Down)
	{
		m_bXButton1Down = FALSE;
		ReleaseCapture();

		// �N���C�A���g�̈�̋�`���擾
		CRect rect;
		GetClientRect(rect);

		// �}�E�X���N���C�A���g�̈�ɓ����Ă�����A���h�D
		if(rect.PtInRect(point))
		{
			// �A���h�D
			AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKELONG(ID_EDIT_UNDO, 1));
		}
	}
}

// 5�{�^���}�E�X�̃T�C�h�{�^���i���j���������ꂽ
void CPOPView::OnXButton2Down(UINT /* nFlags */, CPoint /* point */)
{
	m_bXButton2Down = TRUE;
	SetCapture();
}

// 5�{�^���}�E�X�̃T�C�h�{�^���i���j���J�����ꂽ
void CPOPView::OnXButton2Up(UINT /* nFlags */, CPoint point)
{
	if(m_bXButton2Down)
	{
		m_bXButton2Down = FALSE;
		ReleaseCapture();

		// �N���C�A���g�̈�̋�`���擾
		CRect rect;
		GetClientRect(rect);

		// �}�E�X���N���C�A���g�̈�ɓ����Ă����烊�h�D
		if(rect.PtInRect(point))
		{
			// ���h�D
			AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKELONG(ID_EDIT_REDO, 1));
		}
	}
}
