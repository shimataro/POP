// StaticCtrl.h …スタティックコントロールのサブクラス化
#ifndef __WGC_STATICCTRL__
#define __WGC_STATICCTRL__

#include "Control.h"
#include <commctrl.h>


_WGC_BEGIN                              // namespace wgc {

class static_ctrl : public control
{
public:
	// コンストラクタ／デストラクタ
	explicit static_ctrl(void);
	virtual ~static_ctrl(void);

	// アイコン
	HICON GetIcon(void) const;              // 割り当てられたアイコンを取得
	HICON SetIcon(HICON hIcon);             // アイコンを割り当て

	// ビットマップ
	HBITMAP GetBitmap(void) const;          // 割り当てられたビットマップを取得
	HBITMAP SetBitmap(HBITMAP hBitmap);     // ビットマップを割り当て

	// カーソル
	HCURSOR GetCursor(void) const;          // 割り当てられたカーソルを取得
	HCURSOR SetCursor(HCURSOR hCursor);     // カーソルを割り当て

#if(WINVER >= 0x0400)

	// 拡張メタファイル
	HENHMETAFILE GetEnhMetaFile(void) const;
	HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hEnhMetaFile);

#endif

private:
	// イメージ
	HANDLE GetImage(int fImageType) const;          // 割り当てられたイメージを取得
	HANDLE SetImage(int fImageType, HANDLE hImage);	// イメージを割り当て
};

_WGC_END                                // }

#endif // __WGC_STATICCTRL__
