// chacode.h …キャラクタコードに関する処理
#ifndef __CHARCODE__
#define __CHARCODE__

#include <wchar.h>


////////////////////////////////////////////////////////////
// キャラクタコートの分類
typedef enum _charcode_categorize_t
{
	CC_SPACE,                               // ホワイトスペース
	CC_ALPHA_UPPER,                         // アルファベット大文字
	CC_ALPHA_LOWER,                         // アルファベット小文字
	CC_NUMBER,                              // 数字
	CC_UNDERSCORE,                          // アンダースコア（_）
	CC_BRACKET,                             // 括弧（()[]{}<>）
	CC_SPECIAL,                             // 特殊文字（#$"'%&~^\+-*/@`;:,.!?|）

	CC_WIDE_ALPHA_UPPER,                    // 全角アルファベット大文字
	CC_WIDE_ALPHA_LOWER,                    // 全角アルファベット小文字
	CC_WIDE_NUMBER,                         // 全角数字
	CC_WIDE_BRACKET,                        // 全角括弧

	// 日本語
	CC_JAPANESE_HIRAGANA,                   // ひらがな
	CC_JAPANESE_KATAKANA,                   // カタカナ
	CC_JAPANESE_KANJI,                      // 漢字
	CC_JAPANESE_SYMBOL,                     // 漢字記号

	// ハングル語
	CC_HANGEUL,                             // ハングル文字

	// 
	CC_CYRILLIC,                            // キリル文字
	CC_ARMENIAN,                            // アルメニア文字
	CC_HEBREW,                              // ヘブライ文字
	CC_ARABIC,                              // アラビア文字
	CC_ARABIC_NUMBER,                       // アラビア数字（普段使ってる数字とは違うらしい）
	CC_DEVANAGARI,                          // デバナーガリ文字
	CC_DEVANAGARI_NUMBER,                   // デバナーガリ数字

	CC_ETC,                                 // それ以外
} charcode_categorize_t;

void                  charcode_categorize_init(void);
charcode_categorize_t charcode_categorize(const wchar_t wchar);


////////////////////////////////////////////////////////////
// 改行位置の検索
typedef enum _charcode_linefeed_t
{
	CL_NONE,                                // 改行なし
	CL_CR,                                  // CR（Macintosh形式）
	CL_LF,                                  // LF（Unix形式）
	CL_CRLF,                                // CR-LF（Windows形式）
} charcode_linefeed_t;

int charcode_search_linefeed(const wchar_t *wstr, charcode_linefeed_t &type);

#endif // __CHARCODE__
