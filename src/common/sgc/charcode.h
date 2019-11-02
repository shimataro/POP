// chacode.h �c�L�����N�^�R�[�h�Ɋւ��鏈��
#ifndef __CHARCODE__
#define __CHARCODE__

#include <wchar.h>


////////////////////////////////////////////////////////////
// �L�����N�^�R�[�g�̕���
typedef enum _charcode_categorize_t
{
	CC_SPACE,                               // �z���C�g�X�y�[�X
	CC_ALPHA_UPPER,                         // �A���t�@�x�b�g�啶��
	CC_ALPHA_LOWER,                         // �A���t�@�x�b�g������
	CC_NUMBER,                              // ����
	CC_UNDERSCORE,                          // �A���_�[�X�R�A�i_�j
	CC_BRACKET,                             // ���ʁi()[]{}<>�j
	CC_SPECIAL,                             // ���ꕶ���i#$"'%&~^\+-*/@`;:,.!?|�j

	CC_WIDE_ALPHA_UPPER,                    // �S�p�A���t�@�x�b�g�啶��
	CC_WIDE_ALPHA_LOWER,                    // �S�p�A���t�@�x�b�g������
	CC_WIDE_NUMBER,                         // �S�p����
	CC_WIDE_BRACKET,                        // �S�p����

	// ���{��
	CC_JAPANESE_HIRAGANA,                   // �Ђ炪��
	CC_JAPANESE_KATAKANA,                   // �J�^�J�i
	CC_JAPANESE_KANJI,                      // ����
	CC_JAPANESE_SYMBOL,                     // �����L��

	// �n���O����
	CC_HANGEUL,                             // �n���O������

	// 
	CC_CYRILLIC,                            // �L��������
	CC_ARMENIAN,                            // �A�����j�A����
	CC_HEBREW,                              // �w�u���C����
	CC_ARABIC,                              // �A���r�A����
	CC_ARABIC_NUMBER,                       // �A���r�A�����i���i�g���Ă鐔���Ƃ͈Ⴄ�炵���j
	CC_DEVANAGARI,                          // �f�o�i�[�K������
	CC_DEVANAGARI_NUMBER,                   // �f�o�i�[�K������

	CC_ETC,                                 // ����ȊO
} charcode_categorize_t;

void                  charcode_categorize_init(void);
charcode_categorize_t charcode_categorize(const wchar_t wchar);


////////////////////////////////////////////////////////////
// ���s�ʒu�̌���
typedef enum _charcode_linefeed_t
{
	CL_NONE,                                // ���s�Ȃ�
	CL_CR,                                  // CR�iMacintosh�`���j
	CL_LF,                                  // LF�iUnix�`���j
	CL_CRLF,                                // CR-LF�iWindows�`���j
} charcode_linefeed_t;

int charcode_search_linefeed(const wchar_t *wstr, charcode_linefeed_t &type);

#endif // __CHARCODE__
