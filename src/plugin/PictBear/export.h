/* export.h …エクスポート用関数のプロトタイプ宣言 */
#ifndef __EXPORT__
#define __EXPORT__

#include <windows.h>


/******************************************************************************/
/* エクスポートする関数 */
EXTERN_C DWORD WINAPI pop_get_product(LPSTR author, UINT author_max, LPSTR description, UINT description_max);
EXTERN_C void  WINAPI pop_set_registry(HKEY hKey);
EXTERN_C BOOL  WINAPI pop_setup(HWND hWnd);

/* DLLがアタッチ／デタッチされた */
void attach_process(void);
void detach_process(void);

#endif /* __EXPORT__ */
