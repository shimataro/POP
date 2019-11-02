// string_search.cpp
#include "string_search.h"


////////////////////////////////////////////////////////////////////////////////
// 単体テスト用コード
#ifdef __UNITTEST_STRING_SEARCH__

#include <iostream>


int main(void)
{
	sgc::string_search<char> object = "nothing";

	// 普通に検索
	std::cout << object.find("I saw something but there was nothing.", 38) << std::endl;

	// 最初に出現
	std::cout << object.find("nothing...", 10) << std::endl;

	// 最後に出現
	std::cout << object.find("...nothing", 10) << std::endl;

	// 検索文字列そのまま
	std::cout << object.find("nothing", 7) << std::endl;

	// 全く含まれない
	std::cout << object.find("This is a pen.", 14) << std::endl;

	// 検索文字列より短い
	std::cout << object.find("thing", 5) << std::endl;

	return 0;
}

#endif // __UNITTEST_STRING_SEARCH__
