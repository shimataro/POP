/* export.h …エクスポート関数のプロトタイプ宣言 */
#ifndef __EXPORT__
#define __EXPORT__

#include <windows.h>


/******************************************************************************/
/* エクスポートする関数 */
EXTERN_C DWORD WINAPI ppi_get_product(LPSTR author, UINT author_max, LPSTR description, UINT description_max);
EXTERN_C void  WINAPI pop_set_registry(HKEY hKey);

#endif /* __EXPORT__ */
