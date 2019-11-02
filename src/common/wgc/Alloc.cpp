// Alloc.cpp…operator new/delete のオーバーロード
#include <windows.h>                    // ::GlobalAlloc()
#include <new>                          // std::bad_alloc


////////////////////////////////////////////////////////////////////////////////
// operator new/delete を独自の実装にオーバーロードする場合、
// このファイルをプロジェクトに追加する（追加するだけでOK）
// オーバーロードすると↓みたいないいことがある。
//  1. 高速化
//    （CRTのoperator new/deleteでは、メモリ確保のほかにもいろんなことをやってる。デバッグ用？）
//  2. メモリ確保に失敗した場合、throw std::bad_alloc の保証
//    （Visual C++ 対策）
//  3. サイズ0のメモリを確保しようとした場合の動作の保証
//    （サイズ0のメモリ領域へのポインタを返す）
// ただし、高速化されるといってもnewされるたびにメモリを確保してたら遅いから、
// そのうち大きなメモリブロックを用意して、newされるごとに必要な分だけ切り出したりするかも。
// でも実装が面倒だから今のところ予定はなし


// 単一オブジェクトのnew
void *operator new(size_t cb)
{
	void *ptr = ::GlobalAlloc(GMEM_FIXED, cb);
	if(ptr == NULL)
	{
		// 確保に失敗したら std::bad_alloc をスロー
		throw std::bad_alloc();
	}
	return ptr;
}

// 配列のnew
void *operator new[](size_t cb)
{
	// operator new と同じ
	return operator new(cb);
}

// 単一オブジェクトのdelete
void operator delete(void *ptr)
{
	::GlobalFree(ptr);
}

// 配列のdelete
void operator delete[](void *ptr)
{
	// operator delete と同じ
	operator delete(ptr);
}
