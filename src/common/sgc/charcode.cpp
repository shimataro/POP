// charcode.cpp �c�C���v�������e�[�V�����t�@�C��
#include "charcode.h"


#define ALPHA_UPPER_BEGIN       (L'A')
#define ALPHA_UPPER_END         (L'Z' + 1)
#define ALPHA_LOWER_BEGIN       (L'a')
#define ALPHA_LOWER_END         (L'z' + 1)
#define NUMBER_BEGIN            (L'0')
#define NUMBER_END              (L'9' + 1)
#define WIDE_ALPHA_UPPER_BEGIN  (L'�`')
#define WIDE_ALPHA_UPPER_END    (L'�y' + 1)
#define WIDE_ALPHA_LOWER_BEGIN  (L'��')
#define WIDE_ALPHA_LOWER_END    (L'��' + 1)
#define WIDE_NUMBER_BEGIN       (L'�O' )
#define WIDE_NUMBER_END         (L'�X' + 1)
/*
#define WIDE_SYMBOL_BEGIN       (0x3000)
#define WIDE_SYMBOL_END         (0x3038)
*/

// ���{��
#define JAPANESE_HIRAGANA_BEGIN     (0x3041)
#define JAPANESE_HIRAGANA_END       (0x309f)
#define JAPANESE_KATAKANA1_BEGIN    (0x309b)
#define JAPANESE_KATAKANA1_END      (0x309d)
#define JAPANESE_KATAKANA2_BEGIN    (0x30a1)
#define JAPANESE_KATAKANA2_END      (0x30ff)
#define JAPANESE_KANJI1_BEGIN       (0x3400)
#define JAPANESE_KANJI1_END         (0xa000)
#define JAPANESE_KANJI2_BEGIN       (0xf900)
#define JAPANESE_KANJI2_END         (0xfa2e)
#define JAPANESE_SYMBOL1_BEGIN      (0x3220)
#define JAPANESE_SYMBOL1_END        (0x3380)
#define JAPANESE_SYMBOL2_BEGIN      (0x33e0)
#define JAPANESE_SYMBOL2_END        (0x4e00)

// �n���O����
#define HANGEUL1_BEGIN  (0x1100)
#define HANGEUL1_END    (0x11fa)
#define HANGEUL2_BEGIN  (0x3131)
#define HANGEUL2_END    (0x318f)
#define HANGEUL3_BEGIN  (0xac00)
#define HANGEUL3_END    (0xd7a4)

// �L������
#define CYRILLIC_BEGIN  (0x0401)
#define CYRILLIC_END    (0x04fa)

// �A�����j�A��
#define ARMENIAN_BEGIN  (0x0531)
#define ARMENIAN_END    (0x058a)

// �w�u���C��i�����ƋL�����킯���ق��������H�j
#define HEBREW_BEGIN    (0x0591)
#define HEBREW_END      (0x05f5)

// �A���r�A��
#define ARABIC_BEGIN        (0x060c)
#define ARABIC_END          (0x06ee)
#define ARABIC_NUMBER_BEGIN (0x06f0)
#define ARABIC_NUMBER_END   (0x06f9)

// �f�o�i�[�K����
#define DEVANAGARI_BEGIN        (0x0901)
#define DEVANAGARI_END          (0x0966)
#define DEVANAGARI_NUMBER_BEGIN (0x0966)
#define DEVANAGARI_NUMBER_END   (0x0971)


#define STR_SPACE           L" \t\r\n"
#define STR_UNDERSCORE      L"_"
#define STR_BRACKET         L"()[]{}<>"
#define STR_SPECIAL         L"#$%&~^+-*/@`;:,.!?|\"\'\\"

#define STR_WIDE_BRACKET    L"�i�j[]�k�l�y�z�o�p�����s�t�u�v�w�x"

#define STR_JAPANESE_SYMBOL L"�A�B�S�X�T�U�V�Y"


namespace
{
	charcode_categorize_t g_table[WCHAR_MAX + 1];     // ���ރe�[�u��

	// �e�[�u���t�B���֐��i�ꉞ inline ���Ă��Ă����j
	// �͈͎w��ɂ��t�B��
	inline void _fill_range(const int begin, const int end, const charcode_categorize_t val)
	{ charcode_categorize_t *table_ptr = g_table; for(int i = begin; i < end; i++) { table_ptr[i] = val; } }

	// ������ɂ��t�B��
	inline void _fill_str(const wchar_t *str, const charcode_categorize_t val)
	{ charcode_categorize_t *table_ptr = g_table; while(*str != L'\0') { table_ptr[*str++] = val; } }
}


// ���ރe�[�u����������
void charcode_categorize_init(void)
{
	// �e�[�u���� CC_ETC �ŏ�����
	_fill_range(WCHAR_MIN, WCHAR_MAX + 1, CC_ETC);

	// �z���C�g�X�y�[�X
	_fill_str(STR_SPACE, CC_SPACE);

	// �A���t�@�x�b�g
	_fill_range(ALPHA_UPPER_BEGIN, ALPHA_UPPER_END, CC_ALPHA_UPPER);
	_fill_range(ALPHA_LOWER_BEGIN, ALPHA_LOWER_END, CC_ALPHA_LOWER);

	// ����
	_fill_range(NUMBER_BEGIN, NUMBER_END, CC_NUMBER);

	// �A���_�[�X�R�A�i_�j
	_fill_str(STR_UNDERSCORE, CC_UNDERSCORE);

	// ����
	_fill_str(STR_BRACKET, CC_BRACKET);

	// ���ꕶ���i#$"'%&()[]{}<>~^\+-*/@`;:,.!?|�j
	_fill_str(STR_SPECIAL, CC_SPECIAL);


	// �S�p�A���t�@�x�b�g
	_fill_range(WIDE_ALPHA_UPPER_BEGIN, WIDE_ALPHA_UPPER_END, CC_WIDE_ALPHA_UPPER);
	_fill_range(WIDE_ALPHA_LOWER_BEGIN, WIDE_ALPHA_LOWER_END, CC_WIDE_ALPHA_LOWER);

	// �S�p����
	_fill_range(WIDE_NUMBER_BEGIN, WIDE_NUMBER_END, CC_WIDE_NUMBER);

	// �S�p����
	_fill_str(STR_WIDE_BRACKET, CC_WIDE_BRACKET);


	// ���{��
	_fill_range(JAPANESE_HIRAGANA_BEGIN , JAPANESE_HIRAGANA_END , CC_JAPANESE_HIRAGANA);
	_fill_range(JAPANESE_KATAKANA1_BEGIN, JAPANESE_KATAKANA1_END, CC_JAPANESE_KATAKANA);
	_fill_range(JAPANESE_KATAKANA2_BEGIN, JAPANESE_KATAKANA2_END, CC_JAPANESE_KATAKANA);
	_fill_range(JAPANESE_KANJI1_BEGIN   , JAPANESE_KANJI1_END   , CC_JAPANESE_KANJI);
	_fill_range(JAPANESE_KANJI2_BEGIN   , JAPANESE_KANJI2_END   , CC_JAPANESE_KANJI);
	_fill_range(JAPANESE_SYMBOL1_BEGIN  , JAPANESE_SYMBOL1_END  , CC_JAPANESE_SYMBOL);
	_fill_range(JAPANESE_SYMBOL2_BEGIN  , JAPANESE_SYMBOL2_END  , CC_JAPANESE_SYMBOL);
	_fill_str(STR_JAPANESE_SYMBOL, CC_JAPANESE_SYMBOL);


	// �n���O����
	_fill_range(HANGEUL1_BEGIN, HANGEUL1_END, CC_HANGEUL);
	_fill_range(HANGEUL2_BEGIN, HANGEUL2_END, CC_HANGEUL);
	_fill_range(HANGEUL3_BEGIN, HANGEUL3_END, CC_HANGEUL);

	// �L������
	_fill_range(CYRILLIC_BEGIN, CYRILLIC_END, CC_CYRILLIC);

	// �A�����j�A��
	_fill_range(ARMENIAN_BEGIN, ARMENIAN_END, CC_ARMENIAN);

	// �w�u���C��
	_fill_range(HEBREW_BEGIN, HEBREW_END, CC_HEBREW);

	// �A���r�A��
	_fill_range(ARABIC_BEGIN       , ARABIC_END       , CC_ARABIC);
	_fill_range(ARABIC_NUMBER_BEGIN, ARABIC_NUMBER_END, CC_ARABIC_NUMBER);

	// �f�o�i�[�K����
	_fill_range(DEVANAGARI_BEGIN       , DEVANAGARI_END       , CC_DEVANAGARI);
	_fill_range(DEVANAGARI_NUMBER_BEGIN, DEVANAGARI_NUMBER_END, CC_DEVANAGARI_NUMBER);
}

// �����𕪗�
charcode_categorize_t charcode_categorize(const wchar_t wchar)
{
	return g_table[wchar];
}


// ���s�ʒu�̌���
int charcode_search_linefeed(const wchar_t *wstr, charcode_linefeed_t &type)
{
	const wchar_t NUL = L'\0';
	const wchar_t CR  = L'\r';
	const wchar_t LF  = L'\n';

	for(int i = 0; wstr[i] != NUL; i++)
	{
		// CR����������
		if(wstr[i] == CR)
		{
			// CR-LF
			if(wstr[i + 1] == LF)
			{
				type = CL_CRLF;
			}
			// CR
			else
			{
				type = CL_CR;
			}
			return i;
		}
		// LF����������
		if(wstr[i] == LF)
		{
			type = CL_LF;
			return i;
		}
	}

	// ���s��������Ȃ�����
	type = CL_NONE;
	return -1;
}
