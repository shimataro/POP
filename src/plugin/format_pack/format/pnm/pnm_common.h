// PNM_common.h �c
#ifndef __PNM_COMMON__
#define __PNM_COMMON__


////////////////////////////////////////////////////////////////////////////////
// �萔�̒�`
#define PNM_SIGNATURE_SIZE    2
#define PNM_SIGNATURE_ASCII_BITMAP      "P1"
#define PNM_SIGNATURE_ASCII_GRAYSCALE   "P2"
#define PNM_SIGNATURE_ASCII_COLOR       "P3"
#define PNM_SIGNATURE_BINARY_BITMAP     "P4"
#define PNM_SIGNATURE_BINARY_GRAYSCALE  "P5"
#define PNM_SIGNATURE_BINARY_COLOR      "P6"


////////////////////////////////////////////////////////////////////////////////
// PNM�`���Ǝ��̃f�[�^
typedef struct tagPNMEXTRA
{
	int max_color_components;               // �ő�P�x�l
} PNMEXTRA;


////////////////////////////////////////////////////////////////////////////////
// �s�N�Z�����[�h�i�A�X�L�[�^�o�C�i���j
typedef enum PNM_PIXELMODE
{
	PP_ASCII,
	PP_BINARY,
} PNM_PIXELMODE;

// �J���[���[�h�i�r�b�g�}�b�v�^�O���C�X�P�[���^�t���J���[�j
typedef enum PNM_COLORMODE
{
	PC_BITMAP,
	PC_GRAYSCALE,
	PC_COLOR,
} PNM_COLORMODE;

// PNG���[�h
typedef struct tagPNMMODE
{
	char *name;
	PNM_PIXELMODE pixelmode;
	PNM_COLORMODE colormode;
	uint_t bits;
	int mode;
} PNMMODE;


const PNMMODE pnm_mode[] =
{
//	{PNM_SIGNATURE_ASCII_BITMAP    , PP_ASCII , PC_BITMAP   , 8, COLORMODE_INDEX},     // �A�X�L�[�^�r�b�g�}�b�v
	{PNM_SIGNATURE_ASCII_GRAYSCALE , PP_ASCII , PC_GRAYSCALE, 8, COLORMODE_GRAYSCALE}, // �A�X�L�[�^�O���C�X�P�[��
	{PNM_SIGNATURE_ASCII_COLOR     , PP_ASCII , PC_COLOR    , 8, COLORMODE_RGB},       // �A�X�L�[�^�t���J���[
//	{PNM_SIGNATURE_BINARY_BITMAP   , PP_BINARY, PC_BITMAP   , 8, COLORMODE_INDEX},     // �o�C�i���^�r�b�g�}�b�v
	{PNM_SIGNATURE_BINARY_GRAYSCALE, PP_BINARY, PC_GRAYSCALE, 8, COLORMODE_GRAYSCALE}, // �o�C�i���^�O���C�X�P�[��
	{PNM_SIGNATURE_BINARY_COLOR    , PP_BINARY, PC_COLOR    , 8, COLORMODE_RGB},       // �o�C�i���^�t���J���[
};

#endif // __PNM_COMMON__
