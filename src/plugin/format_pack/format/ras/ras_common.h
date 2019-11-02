// ras_common.h �c
#ifndef __RAS_COMMON__
#define __RAS_COMMON__


// �萔�̒�`
#define RAS_MAGIC     0x59a66a95        // �}�W�b�N�i���o�[
#define sizeofRASTERFILEHEADER  32      // �w�b�_�̃T�C�Y
#define BOUND_RAS     2                 // Raster�t�@�C���̃��C���o�E���_��

// �J���[�}�b�v�̃^�C�v
#define RMT_NONE	    0                 // �J���[�}�b�v�Ȃ�
#define RMT_EQUAL_RGB	1                 // red[maplength/3],green[],blue[]
#define RMT_RAW         2                 // 

// RAS�C���[�W�̃^�C�v
#define RT_OLD            0             // Raw pixrect image in 68000 byte order
#define RT_STANDARD       1             // Raw pixrect image in 68000 byte order
#define RT_BYTE_ENCODED   2             // Run-length compression of bytes
#define RT_FORMAT_RGB     3             // XRGB or RGB instead of XBGR or BGR
#define RT_FORMAT_TIFF    4             // tiff <-> standard rasterfile
#define RT_FORMAT_IFF     5             // iff (TAAC format) <-> standard rasterfile
#define RT_EXPERIMENTAL   0xffff        // Reserved for testing


// �w�b�_
typedef struct tagRASTERFILEHEADER
{
	uint32_t rfMagic;                       // �}�W�b�N�i���o�[
	uint32_t rfWidth;                       // �摜�̕�
	uint32_t rfHeight;                      // �摜�̍���
	uint32_t rfBits;                        // �摜�̃r�b�g���i1,8,24�j
	uint32_t rfSize;                        // �摜�̃T�C�Y
	uint32_t rfType;                        // �摜�̃^�C�v
	uint32_t rfMaptype;                     // �J���[�}�b�v�̃^�C�v
	uint32_t rfMapsize;                     // �J���[�}�b�v�̃T�C�Y
} RASTERFILEHEADER;

#endif // __RAS_COMMON__
