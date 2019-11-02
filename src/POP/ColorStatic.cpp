// ColorStatic.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "pop.h"
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic(COLORREF color /* = RGB(0, 0, 0) */, DWORD dwFlags /* = CC_FULLOPEN */)
{
	m_color   = color;
	m_dwFlags = dwFlags;
}

CColorStatic::CColorStatic(int red, int green, int blue, DWORD dwFlags /* = CC_FULLOPEN */)
{
	m_color   = RGB(red, green, blue);
	m_dwFlags = dwFlags;
}

CColorStatic::~CColorStatic()
{
}


// 現在の色を取得
COLORREF CColorStatic::GetColor(void) const
{
	return m_color;
}

// 新しい色をRGBコンポーネントで指定
COLORREF CColorStatic::SetColor(const int red, const int green, const int blue)
{
	return SetColor(RGB(red, green, blue));
}

// 新しい色を COLORREF で設定
COLORREF CColorStatic::SetColor(const COLORREF color)
{
	const COLORREF color_old = m_color;

	// 色を変えて再描画
	m_color = color;
	Invalidate(FALSE);

	// 古い色を返す
	return color_old;
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic メッセージ ハンドラ

void CColorStatic::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	// カラーボックスを塗りつぶす
	if(TRUE)
	{
		CRect rect;
		GetClientRect(rect);
		dc.FillSolidRect(rect, m_color);
	}
}

// クリックされたら色選択ダイアログを表示
void CColorStatic::OnClicked() 
{
	CColorDialog theDlg(m_color, m_dwFlags);
	if(theDlg.DoModal() == IDOK)
	{
		SetColor(theDlg.GetColor());
	}
}
