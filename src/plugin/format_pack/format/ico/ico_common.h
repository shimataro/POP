// ico_common.h
#ifndef __ICO_COMMON__
#define __ICO_COMMON__

#include <windef.h>


// http://www.bd.wakwak.com/%7Eopapa/VC/Specification/Icon.htm
typedef struct tagICONDIRENTRY
{
	BYTE  bWidth;                           // �A�C�R���̕�
	BYTE  bHeight;                          // �A�C�R���̍���
	BYTE  bColorCount;                      // �F��
	BYTE  bReserved;                        // must be 0x00
	WORD  wPlanes;                          // �v���[����
	WORD  wBitCount;                        // �r�b�g�J�E���g
	DWORD dwBytesInRes;                     // ���\�[�X�̃T�C�Y
	DWORD dwImageOffset;                    // ICONDIR�̐擪����BITMAPINFOHEADER�̐擪�܂ł̃I�t�Z�b�g
} ICONDIRENTRY;

typedef struct tagICONDIR
{
	WORD         idReserved;                // must be 0x0000
	WORD         idType;                    // must be 0x0001
	WORD         idCount;                   // �A�C�R���̐�
	ICONDIRENTRY idEntries[1];              // idCount��������
} ICONDIR;

#endif // __ICO_COMMON__
