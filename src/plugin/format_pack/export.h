/* export.h �c�G�N�X�|�[�g�֐��̃v���g�^�C�v�錾 */
#ifndef __EXPORT__
#define __EXPORT__

#include <windows.h>


/******************************************************************************/
/* �G�N�X�|�[�g����֐� */
EXTERN_C DWORD WINAPI ppi_get_product(LPSTR author, UINT author_max, LPSTR description, UINT description_max);
EXTERN_C void  WINAPI pop_set_registry(HKEY hKey);

#endif /* __EXPORT__ */
