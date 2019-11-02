// export.cpp �c�C���v�������e�[�V�����t�@�C��
#include "stdafx.h"
#include "export.h"


// �o�[�W�����ԍ�
#define VERSION_MAJOR       1           // ���W���[�ԍ��i1�o�C�g�j
#define VERSION_MINOR       0           // �}�C�i�[�ԍ��i1�o�C�g�j
#define VERSION_BUILD       0           // �r���h�ԍ��i2�o�C�g�j


// �v���O�C���̏��
EXTERN_C
DWORD WINAPI pop_get_product(LPSTR author, UINT author_max, LPSTR description, UINT description_max)
{
	// �X�g�����O���\�[�X����ǂݍ���
	HINSTANCE hInstance = GetInstanceHandle();
	::LoadStringA(hInstance, IDS_PRODUCT_AUTHOR     , author     , author_max);
	::LoadStringA(hInstance, IDS_PRODUCT_DESCRIPTION, description, description_max);

	// �o�[�W��������Ԃ�
	return MAKELONG(VERSION_BUILD, MAKEWORD(VERSION_MINOR, VERSION_MAJOR));
}

// ���W�X�g���L�[�̐ݒ�
EXTERN_C
void WINAPI pop_set_registry(HKEY /* hKey */)
{
}
