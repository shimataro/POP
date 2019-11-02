/* filter.h */
#ifndef __FILTER__
#define __FILTER__

#include <windows.h>
#include "../image.h"


/******************************************************************************/
/* ���W���[�����̒�` */
#define funcname_filter_count       "pop_filter_count"
#define funcname_filter_info        "pop_filter_info"
#define funcname_filter_executable  "pop_filter_executable"
#define funcname_filter_option      "pop_filter_option"
#define funcname_filter_effect      "pop_filter_effect"


/******************************************************************************/
/* �֐��^�̒�` */
typedef int         (WINAPI *FILTERFUNC_COUNT)     (void);
typedef DWORD       (WINAPI *FILTERFUNC_INFO)      (const int filter_no, LPSTR genre, UINT genre_max, LPSTR name, UINT name_max);
typedef BOOL        (WINAPI *FILTERFUNC_EXECUTABLE)(const int filter_no, const IMAGE_INFO *info);
typedef IMAGERESULT (WINAPI *FILTERFUNC_EFFECT)    (const int filter_no, const IMAGE_INFO *info, const IMAGE_DATA *data_in, IMAGE_DATA *data_out, callback_t callback, long callback_data);
typedef BOOL        (WINAPI *FILTERFUNC_OPTION)    (const int filter_no, const IMAGE_INFO *info, const IMAGE_DATA *data, HWND hWnd);


/******************************************************************************/
/* �\���̂̒�` */

/* �t�B���^�v���O�C���\���� */
typedef struct tagFILTERPLUGIN
{
	int count;

	FILTERFUNC_EXECUTABLE executable;
	FILTERFUNC_EFFECT effect;
	FILTERFUNC_OPTION option;
} FILTERPLUGIN;

/* �X�̃t�B���^���\���� */
typedef struct tagFILTERPLUGIN_DETAIL
{
	/* �t�B���^��� */
	TCHAR genre[20];                        /* �v���O�C���̃W������ */
	TCHAR name[40];                         /* �v���O�C���� */

	/* �܂܂�Ă���֐� */
	DWORD function_flag;

	/* �ǂ̃v���O�C���̉��Ԗڂ��H */
	int plugin_no;
	int filter_no;
} FILTERPLUGIN_DETAIL;

#endif /* __FILTER__ */
