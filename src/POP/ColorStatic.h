#if !defined(AFX_COLORSTATIC_H__44741B52_49EF_4409_8701_8810C94A7B20__INCLUDED_)
#define AFX_COLORSTATIC_H__44741B52_49EF_4409_8701_8810C94A7B20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorStatic.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CColorStatic ウィンドウ

class CColorStatic : public CStatic
{
// コンストラクション
public:
	CColorStatic(const COLORREF color = RGB(0, 0, 0), const DWORD dwFlags = CC_FULLOPEN);
	CColorStatic(const int red, const int green, const int blue, const DWORD dwFlags = CC_FULLOPEN);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	COLORREF GetColor(void) const;
	COLORREF SetColor(const int red, const int green, const int blue);
	COLORREF SetColor(const COLORREF color);
	virtual ~CColorStatic();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CColorStatic)
	afx_msg void OnPaint();
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_color;
	DWORD    m_dwFlags;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_COLORSTATIC_H__44741B52_49EF_4409_8701_8810C94A7B20__INCLUDED_)
