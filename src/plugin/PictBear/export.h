/* export.h �c�G�N�X�|�[�g�p�֐��̃v���g�^�C�v�錾 */
#ifndef __EXPORT__
#define __EXPORT__

#include <windows.h>


/******************************************************************************/
/* �G�N�X�|�[�g����֐� */
EXTERN_C DWORD WINAPI pop_get_product(LPSTR author, UINT author_max, LPSTR description, UINT description_max);
EXTERN_C void  WINAPI pop_set_registry(HKEY hKey);
EXTERN_C BOOL  WINAPI pop_setup(HWND hWnd);

/* DLL���A�^�b�`�^�f�^�b�`���ꂽ */
void attach_process(void);
void detach_process(void);

#endif /* __EXPORT__ */
