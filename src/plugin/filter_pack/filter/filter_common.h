/* filter_common.h �c�G�N�X�|�[�g����֐��Q */
#ifndef __FILTER_COMMON__
#define __FILTER_COMMON__


/******************************************************************************/
/* �\���̂̒�` */

/* �F */
typedef struct tagCOLOR
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} COLOR;

/* ���W�i�����j */
typedef struct tagPOINT_2D
{
	double x;
	double y;
} POINT_2D;

/* ���W�i�����j */
typedef struct tagPOINT_2D_INTEGER
{
	int x;
	int y;
} POINT_2D_INTEGER;


/******************************************************************************/
/* �֐��^�̐錾 */
typedef void    (*COLORPROC        )(COLOR            *color, void_ptr_t param);
typedef void    (*GEOMETRICPROC    )(POINT_2D         *point, void_ptr_t param);
typedef void    (*GEOMETRICPROC_INT)(POINT_2D_INTEGER *point, void_ptr_t param);
typedef uint8_t (*CUSTOMPROC       )(const uint8_t pixel_array[], const int pixelcount_x, const int pixelcount_y, void_ptr_t param);


/******************************************************************************/
/* �֐��̃v���g�^�C�v�錾 */
IMAGERESULT _pop_filter_transform_color            (const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, COLORPROC         colorproc    , void_ptr_t param, callback_t callback, long callback_data);
IMAGERESULT _pop_filter_transform_geometric        (const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC     geometricproc, void_ptr_t param, const INTERPOLATE_METHOD interpolate_method, callback_t callback, long callback_data);
IMAGERESULT _pop_filter_transform_geometric_integer(const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, GEOMETRICPROC_INT geometricproc, void_ptr_t param, callback_t callback, long callback_data);
IMAGERESULT _pop_filter_custom                     (const IMAGE_INFO *info_ptr, const IMAGE_DATA *in_data_ptr, IMAGE_DATA *out_data_ptr, CUSTOMPROC        customproc   , const int pixelcount_x, const int pixelcount_y, void_ptr_t param, callback_t callback, long callback_data);

#endif /* __FILTER_COMMON__ */
