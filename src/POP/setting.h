// setting.h
#ifndef __SETTING__
#define __SETTING__


////////////////////////////////////////////////////////////////////////////////
// 環境設定のパラメータ

typedef struct tagSETTING
{
	// 基本設定
	struct BASIC
	{
		union CHECK
		{
			unsigned long checkflags;               // チェックフラグ
			struct ELEMENTS
			{
				unsigned implant_comment       : 1;     // 画像ファイルにコメントを埋め込むか
				unsigned save_window_placement : 1;     // ウインドウの位置とサイズを保存するか
			} elements;
		} check;
		CString comment;                        // 埋め込むコメント
		int     mru_count;                      // MRUリストの数
	} basic;

	// 表示設定
	struct DISPLAY
	{
		union CHECK
		{
			unsigned long checkflags;               // チェックフラグ
			struct ELEMENTS
			{
				unsigned activate_gamma_file     : 1;   // ファイルガンマを有効にする
				unsigned customize_gamma_decode  : 1;   // デコーディングガンマをカスタマイズする
				unsigned customize_gamma_display : 1;   // ディスプレイガンマをカスタマイズする
			} elements;
		} check;
		double gamma_decode;                    // デコーディングガンマの値
		double gamma_display;                   // ディスプレイガンマの値

		int background_grid_size;               // 格子のサイズ(bit)
		COLORREF background_color1;             // 格子の色1
		COLORREF background_color2;             // 格子の色2

		BOOL toolbar_XP;                        // XP風ツールバーアイコン使用

		// 表示→言語
		struct LANGUAGE
		{
			CString resource_name;                  // 言語パックのリソース名
		} language;
	} display;
} SETTING;


////////////////////////////////////////////////////////////////////////////////
// 関数のプロトタイプ宣言
void setting_load(      int &category,       SETTING &setting);
void setting_save(const int  category, const SETTING &setting);

#endif // __SETTING__
