// export.cpp …インプリメンテーションファイル
#include "stdafx.h"
#include "export.h"


// バージョン番号
#define VERSION_MAJOR       1           // メジャー番号（1バイト）
#define VERSION_MINOR       0           // マイナー番号（1バイト）
#define VERSION_BUILD       0           // ビルド番号（2バイト）


// プラグインの情報
EXTERN_C
DWORD WINAPI pop_get_product(LPSTR author, UINT author_max, LPSTR description, UINT description_max)
{
	// ストリングリソースから読み込む
	HINSTANCE hInstance = GetInstanceHandle();
	::LoadStringA(hInstance, IDS_PRODUCT_AUTHOR     , author     , author_max);
	::LoadStringA(hInstance, IDS_PRODUCT_DESCRIPTION, description, description_max);

	// バージョン情報を返す
	return MAKELONG(VERSION_BUILD, MAKEWORD(VERSION_MINOR, VERSION_MAJOR));
}

// レジストリキーの設定
EXTERN_C
void WINAPI pop_set_registry(HKEY /* hKey */)
{
}
