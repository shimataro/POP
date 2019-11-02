/* pnm.h �c�G�N�X�|�[�g�֐��̃v���g�^�C�v�錾 */
#ifndef __PNM__
#define __PNM__

#include <windows.h>
#include <stdio.h>


/******************************************************************************/
/*  */
IMAGERESULT pnm_import(      IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, HANDLE hFile, callback_t callback, long callback_data);
IMAGERESULT pnm_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName,                    callback_t callback, long callback_data);
BOOL        pnm_option(HWND hWnd);

#endif /* __PNM__ */
