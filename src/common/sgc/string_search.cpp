// string_search.cpp
#include "string_search.h"


////////////////////////////////////////////////////////////////////////////////
// �P�̃e�X�g�p�R�[�h
#ifdef __UNITTEST_STRING_SEARCH__

#include <iostream>


int main(void)
{
	sgc::string_search<char> object = "nothing";

	// ���ʂɌ���
	std::cout << object.find("I saw something but there was nothing.", 38) << std::endl;

	// �ŏ��ɏo��
	std::cout << object.find("nothing...", 10) << std::endl;

	// �Ō�ɏo��
	std::cout << object.find("...nothing", 10) << std::endl;

	// ���������񂻂̂܂�
	std::cout << object.find("nothing", 7) << std::endl;

	// �S���܂܂�Ȃ�
	std::cout << object.find("This is a pen.", 14) << std::endl;

	// ������������Z��
	std::cout << object.find("thing", 5) << std::endl;

	return 0;
}

#endif // __UNITTEST_STRING_SEARCH__
