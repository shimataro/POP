// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__BF5410AC_332D_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_STDAFX_H__BF5410AC_332D_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

// 対応バージョン
#define WINVER        0x0400            // Windows 95 / NT or later
#define _WIN32_IE     0x0400            // Internet Explorer 4 or later

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdisp.h>        // MFC のオートメーション クラス
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT


// 本プロジェクトのインクルードファイル
#include <htmlhelp.h>                   // ::HtmlHelp()
#include <shlwapi.h>                    // ::PathRenameExtension()

// コモンヘッダファイルをインクルード
#include "..\common\image.h"            // 画像処理基本ヘッダファイル
#include "..\common\bitmap.h"           // ビットマップ操作関数
#include "..\common\interpolate.h"      // 補間アルゴリズム
#include "..\common\mathematics.h"      // 速度優先数学関数
#include "..\common\mem.h"              // メモリ管理関数
#include "..\common\template_utility.h" // テンプレートユーティリティー関数
#include "..\common\plugin\plugin.h"
#include "..\common\plugin\filter.h"
#include "..\common\plugin\format.h"


// 警告の無効化
#pragma warning(disable:4786)           // 「識別子が○○文字に切り捨てられました」


// for文の先頭で定義された変数のスコープをfor文内に限定（最新のC++言語仕様）
#define for     if(0); else for


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__BF5410AC_332D_11D5_8205_08004605D14C__INCLUDED_)
