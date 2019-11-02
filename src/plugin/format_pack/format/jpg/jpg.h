/* jpg.h �c�G�N�X�|�[�g�֐��̃v���g�^�C�v�錾 */
#ifndef __JPG__
#define __JPG__

#include <windows.h>
#include <stdio.h>


/******************************************************************************/
/*  */
IMAGERESULT jpg_import(HANDLE hFile,       IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
IMAGERESULT jpg_export(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, LPCSTR lpszPathName,                    callback_t callback, long callback_data);
BOOL        jpg_option(HWND hWnd);

#endif /* __JPG__ */
