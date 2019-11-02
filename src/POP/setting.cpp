// setting.cpp
#include "stdafx.h"
#include "setting.h"


////////////////////////////////////////////////////////////////////////////////
// マクロ定義

// 設定
#define TCHAR_REG_SECTION_SETTING                       _T("Settings")
#define TCHAR_REG_ENTRY_CATEGORY                        _T("Category")
#define TCHAR_REG_ENTRY_BASIC_FLAGS                     _T("Basic Flags")
#define TCHAR_REG_ENTRY_BASIC_COMMENT                   _T("Basic Comment")
#define TCHAR_REG_ENTRY_BASIC_MRUCOUNT                  _T("Basic MRU count")
#define TCHAR_REG_ENTRY_DISPLAY_GAMMA_FLAGS             _T("Display Gamma Flags")
#define TCHAR_REG_ENTRY_DISPLAY_GAMMA_DECODE            _T("Display Gamma Decode")
#define TCHAR_REG_ENTRY_DISPLAY_GAMMA_DISPLAY           _T("Display Gamma Display")
#define TCHAR_REG_ENTRY_DISPLAY_BACKGRONUD_GRID_SIZE    _T("Display Background Grid Size")
#define TCHAR_REG_ENTRY_DISPLAY_BACKGRONUD_COLOR1       _T("Display Background Color1")
#define TCHAR_REG_ENTRY_DISPLAY_BACKGRONUD_COLOR2       _T("Display Background Color2")
#define TCHAR_REG_ENTRY_DISPLAY_TOOLBAR_XP              _T("Display Toolbar XP")
#define TCHAR_REG_ENTRY_DISPLAY_LANGUAGE_RESOURCE       _T("Language Resource")

// デフォルト値
#define DEFAULT_BASIC_FLAGS                     0x00000000
#define DEFAULT_BASIC_COMMENT                   _T("Created by Photo Operator")
#define DEFAULT_BASIC_MRUCOUNT                  _AFX_MRU_COUNT
#define DEFAULT_DISPLAY_GAMMA_FLAGS             0x00000001
#define DEFAULT_DISPLAY_GAMMA_DECODE            GAMMA_DEFAULT_DECODE
#define DEFAULT_DISPLAY_GAMMA_DISPLAY           GAMMA_DEFAULT_DISPLAY
#define DEFAULT_DISPLAY_BACKGRONUD_GRID_SIZE    2
#define DEFAULT_DISPLAY_BACKGRONUD_COLOR1       RGB(0x80, 0x80, 0x80)
#define DEFAULT_DISPLAY_BACKGRONUD_COLOR2       RGB(0xff, 0xff, 0xff)
#define DEFAULT_DISPLAY_TOOLBAR_XP              TRUE
#define DEFAULT_DISPLAY_LANGUAGE_RESOURCE       NULL

#define GAMMA_TIMES     (100000.0)


////////////////////////////////////////////////////////////////////////////////
// 関数のインプリメント

// レジストリに保存した設定を復元
void setting_load(int &category, SETTING &setting)
{
	CWinApp *pApp = AfxGetApp();
	ASSERT(pApp != NULL);

	// 最後に選ばれたカテゴリ
	if(TRUE)
	{
		category = pApp->GetProfileInt(TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_CATEGORY, 0);
	}

	// 基本設定
	if(TRUE)
	{
		// チェックフラグ
		setting.basic.check.checkflags = pApp->GetProfileInt(
			TCHAR_REG_SECTION_SETTING,
			TCHAR_REG_ENTRY_BASIC_FLAGS,
			DEFAULT_BASIC_FLAGS);

		// コメント（DEFAULT_COMMENT）
		setting.basic.comment = pApp->GetProfileString(
			TCHAR_REG_SECTION_SETTING,
			TCHAR_REG_ENTRY_BASIC_COMMENT,
			DEFAULT_BASIC_COMMENT);

		// MRUリストの数（DEFAULT_MRUCOUNT）
		const int mru_count = pApp->GetProfileInt(
			TCHAR_REG_SECTION_SETTING,
			TCHAR_REG_ENTRY_BASIC_MRUCOUNT,
			DEFAULT_BASIC_MRUCOUNT);

		setting.basic.mru_count = saturate(mru_count, 0, _AFX_MRU_MAX_COUNT);
	}

	// 表示設定
	if(TRUE)
	{
		// ガンマ
		if(TRUE)
		{
			// チェックフラグ
			setting.display.check.checkflags = pApp->GetProfileInt(
				TCHAR_REG_SECTION_SETTING,
				TCHAR_REG_ENTRY_DISPLAY_GAMMA_FLAGS,
				DEFAULT_DISPLAY_GAMMA_FLAGS);

			// デコーディングガンマ、ディスプレイガンマの値
			const int gamma_decode = pApp->GetProfileInt(
				TCHAR_REG_SECTION_SETTING,
				TCHAR_REG_ENTRY_DISPLAY_GAMMA_DECODE,
				static_cast<int>(DEFAULT_DISPLAY_GAMMA_DECODE * GAMMA_TIMES));

			const int gamma_display = pApp->GetProfileInt(
				TCHAR_REG_SECTION_SETTING,
				TCHAR_REG_ENTRY_DISPLAY_GAMMA_DISPLAY,
				static_cast<int>(DEFAULT_DISPLAY_GAMMA_DISPLAY * GAMMA_TIMES));

			setting.display.gamma_decode  = saturate(gamma_decode  / GAMMA_TIMES, 0.01, 9.99);
			setting.display.gamma_display = saturate(gamma_display / GAMMA_TIMES, 0.01, 9.99);
		}

		// 背景
		if(TRUE)
		{
			// グリッドの大きさ（普通）
			setting.display.background_grid_size = pApp->GetProfileInt(
				TCHAR_REG_SECTION_SETTING,
				TCHAR_REG_ENTRY_DISPLAY_BACKGRONUD_GRID_SIZE,
				DEFAULT_DISPLAY_BACKGRONUD_GRID_SIZE);

			// グリッドの色1（灰色）
			setting.display.background_color1 = pApp->GetProfileInt(
				TCHAR_REG_SECTION_SETTING,
				TCHAR_REG_ENTRY_DISPLAY_BACKGRONUD_COLOR1,
				DEFAULT_DISPLAY_BACKGRONUD_COLOR1);

			// グリッドの色2（白色）
			setting.display.background_color2 = pApp->GetProfileInt(
				TCHAR_REG_SECTION_SETTING,
				TCHAR_REG_ENTRY_DISPLAY_BACKGRONUD_COLOR2,
				DEFAULT_DISPLAY_BACKGRONUD_COLOR2);
		}

		// ツールバー
		if(TRUE)
		{
			// 外観をXP風にする（TRUE）
			setting.display.toolbar_XP = pApp->GetProfileInt(
				TCHAR_REG_SECTION_SETTING,
				TCHAR_REG_ENTRY_DISPLAY_TOOLBAR_XP,
				DEFAULT_DISPLAY_TOOLBAR_XP);
		}

		// 表示→言語
		if(TRUE)
		{
			setting.display.language.resource_name = pApp->GetProfileString(
				TCHAR_REG_SECTION_SETTING,
				TCHAR_REG_ENTRY_DISPLAY_LANGUAGE_RESOURCE,
				DEFAULT_DISPLAY_LANGUAGE_RESOURCE);
		}
	}
}

// レジストリに設定を保存
void setting_save(const int category, const SETTING &setting)
{
	CWinApp *pApp = AfxGetApp();
	ASSERT(pApp != NULL);

	// 最後に選ばれたカテゴリ
	if(TRUE)
	{
		pApp->WriteProfileInt(TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_CATEGORY, category);
	}

	// 基本設定
	if(TRUE)
	{
		pApp->WriteProfileInt   (TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_BASIC_FLAGS   , setting.basic.check.checkflags);
		pApp->WriteProfileString(TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_BASIC_COMMENT , setting.basic.comment);
		pApp->WriteProfileInt   (TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_BASIC_MRUCOUNT, setting.basic.mru_count);
	}

	// 表示設定
	if(TRUE)
	{
		pApp->WriteProfileInt(TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_DISPLAY_GAMMA_FLAGS         , setting.display.check.checkflags);
		pApp->WriteProfileInt(TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_DISPLAY_GAMMA_DECODE        , static_cast<int>(setting.display.gamma_decode  * GAMMA_TIMES));
		pApp->WriteProfileInt(TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_DISPLAY_GAMMA_DISPLAY       , static_cast<int>(setting.display.gamma_display * GAMMA_TIMES));
		pApp->WriteProfileInt(TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_DISPLAY_BACKGRONUD_GRID_SIZE, setting.display.background_grid_size);
		pApp->WriteProfileInt(TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_DISPLAY_BACKGRONUD_COLOR1   , setting.display.background_color1);
		pApp->WriteProfileInt(TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_DISPLAY_BACKGRONUD_COLOR2   , setting.display.background_color2);
		pApp->WriteProfileInt(TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_DISPLAY_TOOLBAR_XP          , setting.display.toolbar_XP);

		// 表示→言語
		if(TRUE)
		{
			pApp->WriteProfileString(TCHAR_REG_SECTION_SETTING, TCHAR_REG_ENTRY_DISPLAY_LANGUAGE_RESOURCE, setting.display.language.resource_name);
		}
	}
}
