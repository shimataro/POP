/* image.h �cPhoto Operator�̒��j */
#ifndef __IMAGE__
#define __IMAGE__

#include <windows.h>


/******************************************************************************/
/* �񋓌^�̒�` */
typedef enum tagIMAGERESULT             /* �������� */
{
	IR_SUCCESS = 0,                         /* ���� */
	IR_CANCELED,                            /* �������L�����Z�����ꂽ */
	IR_INCORRECTHEADER,                     /* �w�b�_���������Ȃ� */
	IR_IMAGEDESTROYED,                      /* �C���[�W�����Ă���i�w�b�_�͐������j */
	IR_NOMEMORY,                            /* �������s�� */
	IR_READERROR,                           /* �ǂݍ��݃G���[ */
	IR_WRITEERROR,                          /* �������݃G���[ */
	IR_ILLEGALPARAM,                        /* �s���ȃp�����[�^ */
	IR_APIERROR,                            /* API�Ăяo���G���[ */
	IR_UNKNOWN,                             /* �s���ȃG���[ */
} IMAGERESULT;


/******************************************************************************/
/* �^�̒�` */

/* �����t��8�r�b�g���� */
typedef       signed char  int8_t;
typedef       signed char *int8_ptr_t;
typedef const signed char  const_int8_t;
typedef const signed char *const_int8_ptr_t;

/* �����t��16�r�b�g���� */
typedef       signed short  int16_t;
typedef       signed short *int16_ptr_t;
typedef const signed short  const_int16_t;
typedef const signed short *const_int16_ptr_t;

/* �����t��32�r�b�g���� */
typedef       signed long  int32_t;
typedef       signed long *int32_ptr_t;
typedef const signed long  const_int32_t;
typedef const signed long *const_int32_ptr_t;

/* �����t��64�r�b�g���� */
typedef       signed __int64  int64_t;
typedef       signed __int64 *int64_ptr_t;
typedef const signed __int64  const_int64_t;
typedef const signed __int64 *const_int64_ptr_t;

/* �����Ȃ�8�r�b�g���� */
typedef       unsigned char  uint8_t;
typedef       unsigned char *uint8_ptr_t;
typedef const unsigned char  const_uint8_t;
typedef const unsigned char *const_uint8_ptr_t;

/* �����Ȃ�16�r�b�g���� */
typedef       unsigned short  uint16_t;
typedef       unsigned short *uint16_ptr_t;
typedef const unsigned short  const_uint16_t;
typedef const unsigned short *const_uint16_ptr_t;

/* �����Ȃ�32�r�b�g���� */
typedef       unsigned long  uint32_t;
typedef       unsigned long *uint32_ptr_t;
typedef const unsigned long  const_uint32_t;
typedef const unsigned long *const_uint32_ptr_t;

/* �����Ȃ�32�r�b�g���� */
typedef       unsigned __int64  uint64_t;
typedef       unsigned __int64 *uint64_ptr_t;
typedef const unsigned __int64  const_uint64_t;
typedef const unsigned __int64 *const_uint64_ptr_t;

/* unsigned int */
typedef       unsigned int  uint_t;
typedef       unsigned int *uint_ptr_t;
typedef const unsigned int  const_uint_t;
typedef const unsigned int *const_uint_ptr_t;

/* void�|�C���^ */
typedef       void *void_ptr_t;
typedef const void *const_void_ptr_t;

typedef int (CALLBACK *callback_t)(const int num, const int denom, const long data);


/******************************************************************************/
/* 1��f�̋P�x�l���i�[����^ */

/* �O���C�X�P�[���i8�r�b�g�j */
typedef union _pixel_gray8_t
{
	uint16_t value;
	struct elements
	{
		uint8_t gray;
		uint8_t alpha;
	} el;
} pixel_gray8_t;
typedef       pixel_gray8_t *pixel_gray8_ptr_t;
typedef const pixel_gray8_t  const_pixel_gray8_t;
typedef const pixel_gray8_t *const_pixel_gray8_ptr_t;

/* �O���C�X�P�[���i16�r�b�g�j */
typedef union _pixel_gray16_t
{
	uint32_t value;
	struct elements
	{
		uint16_t gray;
		uint16_t alpha;
	} el;
} pixel_gray16_t;
typedef       pixel_gray16_t *pixel_gray16_ptr_t;
typedef const pixel_gray16_t  const_pixel_gray16_t;
typedef const pixel_gray16_t *const_pixel_gray16_ptr_t;

/* �J���[�p���b�g�i8�r�b�g�j */
typedef struct _palette8_t
{
	uint8_t b;                              /* �� */
	uint8_t g;                              /* �� */
	uint8_t r;                              /* �� */
	uint8_t a;                              /* �s�����x */
} palette8_t;
typedef       palette8_t *palette8_ptr_t;
typedef const palette8_t  const_palette8_t;
typedef const palette8_t *const_palette8_ptr_t;

/* �J���[�p���b�g�i16�r�b�g�j */
typedef struct _palette16_t
{
	uint16_t b;                             /* �� */
	uint16_t g;                             /* �� */
	uint16_t r;                             /* �� */
	uint16_t a;                             /* �s�����x */
} palette16_t;
typedef       palette16_t *palette16_ptr_t;
typedef const palette16_t  const_palette16_t;
typedef const palette16_t *const_palette16_ptr_t;

/* RGB�i8�r�b�g�j */
typedef union _pixel_rgb8_t
{
	uint32_t value;
	struct elements
	{
		uint8_t b;                              /* �� */
		uint8_t g;                              /* �� */
		uint8_t r;                              /* �� */
		uint8_t a;                              /* �s�����x */
	} el;
} pixel_rgb8_t;
typedef       pixel_rgb8_t *pixel_rgb8_ptr_t;
typedef const pixel_rgb8_t  const_pixel_rgb8_t;
typedef const pixel_rgb8_t *const_pixel_rgb8_ptr_t;

/* RGB�i16�r�b�g�j */
typedef union _pixel_rgb16_t
{
	uint64_t value;
	struct elements
	{
		uint16_t b;                             /* �� */
		uint16_t g;                             /* �� */
		uint16_t r;                             /* �� */
		uint16_t a;                             /* �s�����x */
	} el;
} pixel_rgb16_t;
typedef       pixel_rgb16_t *pixel_rgb16_ptr_t;
typedef const pixel_rgb16_t  const_pixel_rgb16_t;
typedef const pixel_rgb16_t *const_pixel_rgb16_ptr_t;


/******************************************************************************/
/* �萔�̒�` */

/* �T�|�[�g���Ă���r�b�g�� */
#define SUPPORT_GRAYSCALE (0x0001)      /* �O���C�X�P�[��256�K�� */
#define SUPPORT_INDEX     (0x0002)      /* �C���f�b�N�X�J���[256�F */
#define SUPPORT_RGB       (0x0004)      /* RGB24bit */

/* �J���[���[�h */
#define COLORMODE_GRAYSCALE  0          /* �O���C�X�P�[�� */
#define COLORMODE_INDEX      1          /* �C���f�b�N�X�J���[ */
#define COLORMODE_RGB        2          /* RGB�J���[ */


/* ���l */
#define GAMMA_MIN              (0.01)   /* �ŏ��l */
#define GAMMA_MAX              (1.00)   /* �ő�l */
#define GAMMA_DEFAULT_FILE     (1 / 2.2)/* �f�t�H���g�̃t�@�C���K���} */
#define GAMMA_DEFAULT_DECODE   (1.0)    /* �f�t�H���g�̃f�R�[�f�B���O�K���} */
#define GAMMA_DEFAULT_DISPLAY  (2.2)    /* �f�t�H���g�̃f�B�X�v���C�K���} */


/******************************************************************************/
/* �\���̂̒�` */
typedef struct tagIMAGE_INFO            /* �摜��� */
{
	uint16_t width, height;                 /* ���A���� */
	uint16_t depth;                         /* �r�b�g�[�x */
	uint16_t mode;                          /* �J���[���[�h�iCOLORMODE_???�j */
	BOOL     alpha;                         /* �A���t�@�`���l�����܂�ł��邩 */
	uint32_t colors;                        /* ���ۂɎg���Ă���F�� */
	uint16_t density_x, density_y;          /* �����A�����������x */
	double   gamma_file;                    /* �t�@�C���K���} */
	char    *comment;                       /* �摜�t�@�C�����̃e�L�X�g���i�ۑ����ɂ̂ݎg�p�j */
} IMAGE_INFO;

typedef struct tagIMAGE_DATA            /* �摜�f�[�^ */
{
	uint32_t image_size;                    /* ��f�� */
	uint32_t extra_size;                    /* �ǉ��f�[�^�T�C�Y */
	HGLOBAL image;                          /* �摜�f�[�^ */
	HGLOBAL extra;                          /* �ǉ��f�[�^�i�J���[�p���b�g�Ȃǁj */
} IMAGE_DATA;


/******************************************************************************/
/* �}�N���֐��̒�` */

/* �z��̃T�C�Y */
#define SIZE_OF_ARRAY(array)    (sizeof(array) / sizeof(array[0]))

/* �摜�Ɋւ���v�Z */
#define CALC_OFFSET(x, y, hsize)                    ((x) + (y) * (hsize))
#define CALC_BOUND(hsize, bound)                    (((hsize) + (bound) - 1) / (bound) * (bound))
#define CALC_HSIZE(width, bit)                      (((width) * (bit) + 7) / 8)
#define CALC_IMAGE_SIZE(width, height, bit, bound)  (CALC_BOUND(CALC_HSIZE(width, bit), bound) * (height))
#define CALC_IMAGE_PIXELS(width, height)            ((width) * (height))

#define CALC_PACK_OFFSET(pos, log2_depth)   ((pos) >> (~(log2_depth) & 0x03))
#define CALC_PACK_SHIFTS(pos, log2_depth)   ((~(pos) << (log2_depth)) & 0x07)

#define PACK_EXTRACT(pack, depth, shifts)   (((pack) >> (shifts)) & (0xff >> (8 - depth)))
#define PACK_STORE(pack, data, shifts)      (pack |= (data) << (shifts))

/* �n�b�V�� */
#define MAX_COLOR_HASH  0x0fff
#define HASH_COLOR_RGB(r, g, b)     (((r) << 4) & 0x0f00) | ((g) & 0x00f0) | (((b) >> 4) & 0x000f)
#define HASH_COLOR_REF(rgb)         (((rgb) >> 12) & 0x0f00) | (((rgb) >> 8) & 0x00f0) | (((rgb) >> 4) & 0x000f)

/* �O���C�X�P�[���l */
/* r * 0.3 + g * 0.59 + b * 0.11 */
#define GRAYSCALE(r, g, b)  (((r) * 77 + (g) * 151 + (b) * 28) >> 8)


/* ���O��Ԃ̒�` */
#ifdef __cplusplus

#define BEGIN_NAMESPACE(name)       namespace name {
#define BEGIN_NAMESPACE_NONAME()    namespace {
#define END_NAMESPACE()             }

#endif /* __cplusplus */

#endif
