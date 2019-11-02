/* plugin.h */
#ifndef __PLUGIN__
#define __PLUGIN__

#include <windows.h>


/******************************************************************************/
// �֐����̒�`
#define funcname_get_product    "pop_get_product"
#define funcname_set_registry   "pop_set_registry"
#define funcname_setup          "pop_setup"


/******************************************************************************/
/* �֐��^�̒�` */
typedef DWORD (WINAPI *GETPRODUCT)(LPSTR author, UINT author_max, LPSTR description, UINT description_max);
typedef void  (WINAPI *SETREGISTRY)(HKEY key_root);
typedef BOOL  (WINAPI *SETUP)(HWND hWnd);


/******************************************************************************/
/* �\���̂̒�` */

/* �v���O�C�����\���� */
typedef struct tagPLUGIN_INFO
{
	DWORD version;                          /* �v���O�C���̃o�[�W�����i��ʃo�C�g���烁�W���[�ԍ��E�}�C�i�[�ԍ��E�r���h�ԍ��E���r�W�����j */
	TCHAR filename[_MAX_FNAME];             /* �t�@�C���� */
	TCHAR author[64];                       /* ��� */
	TCHAR description[256];                 /* �v���O�C���ɂ��Ă̋L�q */

	SETUP setup;                            /* �v���O�C���̃Z�b�g�A�b�v�֐� */
} PLUGIN_INFO;

#endif /* __FILTER__ */
