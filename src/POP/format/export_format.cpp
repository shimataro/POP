// export_format.cpp �c�C���v�������e�[�V�����t�@�C��
#include "stdafx.h"
#include "export_format.h"
#include "../POP.h"

#include "pgf/pgf.h"


typedef IMAGERESULT (*IMAGE_IMPORT_F)(HANDLE hFile,       IMAGE_INFO *info_ptr,       IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
typedef IMAGERESULT (*IMAGE_EXPORT_F)(HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data);
typedef BOOL        (*OPTION   )(HWND hWnd);


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	typedef struct tagFORMATFUNC
	{
		int id_format_name;                     // 
		int id_format_ext;                      // 
		int id_format_filter;                   // 
		int id_format_expression;               // 
		int support_mode;
		DWORD impremented_functions;
		IMAGE_IMPORT_F format_import;
		IMAGE_EXPORT_F format_export;
		OPTION         format_option;
	} FORMATFUNC;

	const FORMATFUNC g_formatfunc_array[] =
	{
		{IDS_FORMAT_NAME_PGF, IDS_FORMAT_EXT_PGF, IDS_FORMAT_FILTER_PGF, IDS_FORMAT_EXPRESSION_PGF, SUPPORT_GRAYSCALE | SUPPORT_INDEX | SUPPORT_RGB, IF_IMPORT | IF_EXPORT | IF_OPTION, pgf_import, pgf_export, pgf_option},
/*
		{IDS_FORMAT_NAME_BMP, IDS_FORMAT_EXT_BMP, IDS_FORMAT_FILTER_BMP, IDS_FORMAT_EXPRESSION_BMP,                     SUPPORT_INDEX | SUPPORT_RGB, IF_IMPORT | IF_EXPORT            , bmp_import, bmp_export, NULL},
		{IDS_FORMAT_NAME_GIF, IDS_FORMAT_EXT_GIF, IDS_FORMAT_FILTER_GIF, IDS_FORMAT_EXPRESSION_GIF,                     SUPPORT_INDEX              , IF_IMPORT                        , gif_import, NULL    , NULL},
		{IDS_FORMAT_NAME_ICO, IDS_FORMAT_EXT_ICO, IDS_FORMAT_FILTER_ICO, IDS_FORMAT_EXPRESSION_ICO,                     SUPPORT_INDEX              , IF_IMPORT                        , ico_import, NULL    , NULL},
		{IDS_FORMAT_NAME_JPG, IDS_FORMAT_EXT_JPG, IDS_FORMAT_FILTER_JPG, IDS_FORMAT_EXPRESSION_JPG, SUPPORT_GRAYSCALE |                 SUPPORT_RGB, IF_IMPORT                        , jpg_import, NULL    , NULL},
		{IDS_FORMAT_NAME_PNG, IDS_FORMAT_EXT_PNG, IDS_FORMAT_FILTER_PNG, IDS_FORMAT_EXPRESSION_PNG, SUPPORT_GRAYSCALE | SUPPORT_INDEX | SUPPORT_RGB, IF_IMPORT | IF_EXPORT | IF_OPTION, png_import, png_export, png_option},
//		{IDS_FORMAT_NAME_PNM, IDS_FORMAT_EXT_PNM, IDS_FORMAT_FILTER_PNM, IDS_FORMAT_EXPRESSION_PNM, SUPPORT_GRAYSCALE |                 SUPPORT_RGB, IF_IMPORT | IF_EXPORT | IF_OPTION, pnm_import, pnm_export, pnm_option},
		{IDS_FORMAT_NAME_RAS, IDS_FORMAT_EXT_RAS, IDS_FORMAT_FILTER_RAS, IDS_FORMAT_EXPRESSION_RAS,                     SUPPORT_INDEX | SUPPORT_RGB, IF_IMPORT | IF_EXPORT            , ras_import, ras_export, NULL},
*/
	};

	// �f�t�H���g�R�[���o�b�N�֐�
	int CALLBACK callback_none(const int /* num */, const int /* denom */, const long /* data */)
	{return FALSE;}
END_NAMESPACE()


// �Ή��t�H�[�}�b�g�̌�
EXTERN_C
int WINAPI pop_format_count(void)
{
	return SIZE_OF_ARRAY(g_formatfunc_array);
}

// �Ή��t�H�[�}�b�g�̏��
EXTERN_C
DWORD WINAPI pop_format_info(const int format_no, LPSTR name, UINT name_max, LPSTR ext, UINT ext_max, LPSTR filter, UINT filter_max, LPSTR expression, UINT expression_max)
{
	assert(format_no < SIZE_OF_ARRAY(g_formatfunc_array));

	// �X�g�����O���\�[�X���當�����ǂݍ���
	if(TRUE)
	{
		HINSTANCE hInstance = AfxGetResourceHandle();

		::LoadStringA(hInstance, g_formatfunc_array[format_no].id_format_name      , name      , name_max);
		::LoadStringA(hInstance, g_formatfunc_array[format_no].id_format_ext       , ext       , ext_max);
		::LoadStringA(hInstance, g_formatfunc_array[format_no].id_format_filter    , filter    , filter_max);
		::LoadStringA(hInstance, g_formatfunc_array[format_no].id_format_expression, expression, expression_max);
	}

	return g_formatfunc_array[format_no].impremented_functions;
}

// �T�|�[�g���Ă���J���[���[�h
EXTERN_C
uint_t WINAPI pop_format_support_mode(const int format_no)
{
	assert(format_no < SIZE_OF_ARRAY(g_formatfunc_array));

	return g_formatfunc_array[format_no].support_mode;
}

// �C���[�W�����
EXTERN_C
IMAGERESULT WINAPI pop_format_import(const int format_no, HANDLE hFile, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	assert(format_no < SIZE_OF_ARRAY(g_formatfunc_array));

	// �C���|�[�g�֐��̑��݂��`�F�b�N
	IMAGE_IMPORT_F import = g_formatfunc_array[format_no].format_import;
	if(import == NULL)
	{
		return IR_UNKNOWN;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback == NULL)
	{
		callback = callback_none;
	}

	::ZeroMemory(data_ptr, sizeof(IMAGE_DATA));
	return import(hFile, info_ptr, data_ptr, callback, callback_data);
}

// �C���[�W���o��
EXTERN_C
IMAGERESULT WINAPI pop_format_export(const int format_no, HANDLE hFile, const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, callback_t callback, long callback_data)
{
	assert(format_no < SIZE_OF_ARRAY(g_formatfunc_array));

	IMAGE_EXPORT_F export = g_formatfunc_array[format_no].format_export;
	if(export == NULL)
	{
		return IR_UNKNOWN;
	}

	// �R�[���o�b�N�֐��̕␳
	if(callback == NULL)
	{
		callback = callback_none;
	}

	return export(hFile, info_ptr, data_ptr, callback, callback_data);
}

// �I�v�V����
EXTERN_C
BOOL WINAPI pop_format_option(const int format_no, HWND hWnd)
{
	assert(format_no < SIZE_OF_ARRAY(g_formatfunc_array));

	OPTION option = g_formatfunc_array[format_no].format_option;
	if(option == NULL)
	{
		return TRUE;
	}

	return option(hWnd);
}
