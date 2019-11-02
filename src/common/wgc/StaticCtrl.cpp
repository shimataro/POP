// StaticCtrl.cpp …インプリメンテーションファイル
#include "StaticCtrl.h"


_WGC_BEGIN                              // namespace wgc {

////////////////////////////////////////////////////////////////////////////////
// 構築

// コンストラクタ
static_ctrl::static_ctrl(void)
{
}

// デストラクタ
static_ctrl::~static_ctrl(void)
{
}


////////////////////////////////////////////////////////////////////////////////
// アイコン

// 割り当てられたアイコンを取得
HICON static_ctrl::GetIcon(void) const
{
	LRESULT result = SendMessage(STM_GETICON);
	return reinterpret_cast<HICON>(result);
}

// アイコンを割り当て
HICON static_ctrl::SetIcon(HICON hIcon)
{
	LRESULT result = SendMessage(
		STM_SETICON,
		reinterpret_cast<WPARAM>(hIcon));

	return reinterpret_cast<HICON>(result);
}


////////////////////////////////////////////////////////////////////////////////
// ビットマップ

// 割り当てられたビットマップを取得
HBITMAP static_ctrl::GetBitmap(void) const
{
	return reinterpret_cast<HBITMAP>(GetImage(IMAGE_BITMAP));
}

// ビットマップを割り当て
HBITMAP static_ctrl::SetBitmap(HBITMAP hBitmap)
{
	return reinterpret_cast<HBITMAP>(SetImage(IMAGE_BITMAP, hBitmap));
}


////////////////////////////////////////////////////////////////////////////////
// カーソル

// 割り当てられたカーソルを取得
HCURSOR static_ctrl::GetCursor(void) const
{
	return reinterpret_cast<HCURSOR>(GetImage(IMAGE_CURSOR));
}

// カーソルを割り当て
HCURSOR static_ctrl::SetCursor(HCURSOR hCursor)
{
	return reinterpret_cast<HCURSOR>(SetImage(IMAGE_CURSOR, hCursor));
}


#if(WINVER >= 0x0400)

////////////////////////////////////////////////////////////////////////////////
// 拡張メタファイル
HENHMETAFILE static_ctrl::GetEnhMetaFile(void) const
{
	return reinterpret_cast<HENHMETAFILE>(GetImage(IMAGE_ENHMETAFILE));
}

HENHMETAFILE static_ctrl::SetEnhMetaFile(HENHMETAFILE hEnhMetaFile)
{
	return reinterpret_cast<HENHMETAFILE>(SetImage(IMAGE_ENHMETAFILE, hEnhMetaFile));
}

#endif


////////////////////////////////////////////////////////////////////////////////
// イメージ

// 割り当てられたイメージを取得
HANDLE static_ctrl::GetImage(int fImageType) const
{
	LRESULT result = SendMessage(STM_GETIMAGE, fImageType);
	return reinterpret_cast<HANDLE>(result);
}

// イメージ割り当て
HANDLE static_ctrl::SetImage(int fImageType, HANDLE hImage)
{
	LRESULT result = SendMessage(STM_SETIMAGE, fImageType, (LPARAM)hImage);
	return reinterpret_cast<HANDLE>(result);
}

_WGC_END                                // }
