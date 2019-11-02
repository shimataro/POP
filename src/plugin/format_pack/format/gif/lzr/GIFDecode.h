/*--------------------------------------------------------------------------------------*/
/*	非LZW理論GIFデコーダー本体 ヘッダファイル	Ver 1.20								*/
/*	for VisualStudio6.0 only															*/
/*	※展開用パラメータ格納構造体と関数のプロトタイプ宣言。								*/
/*	Copyright (C) 2000 DJ☆Uchi [H.Uchida]												*/
/*--------------------------------------------------------------------------------------*/
#ifndef __GIF_LZR__
#define __GIF_LZR__

#include <windows.h>
#include <stdio.h>


/*--------------------------------------------------------------------------------------*/
/*	GifDecode()																			*/
/*	GIF画像（最初の一枚のみ）を展開する。												*/
/*	※入力はGIFデータ、出力はindex（トップダウン型アラインメントなしDIB）				*/
/*	※今回のプラグインの要。LZW理論を使用していません。									*/
/*--------------------------------------------------------------------------------------*/
DWORD GifDecode(
	BYTE buffer[], const BYTE gif[],
	const DWORD width, const DWORD height, const WORD depth,
	const DWORD left, const DWORD top, const DWORD image_width, const DWORD image_height,
	const BOOL interlace, const DWORD size);


#endif // __GIF_LZR__
