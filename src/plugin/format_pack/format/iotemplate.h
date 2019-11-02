// iotemplate.h 
#ifndef __IOTEMPLATE__
#define __IOTEMPLATE__

#include <windows.h>


// データ読み込み関数
template<typename _T, typename _E>
void ReadData(HANDLE hFile, _T *data, const DWORD count, const _E except_value)
{
	DWORD dwReads = 0;

	BOOL result = ::ReadFile(
		hFile,
		data, sizeof(_T) * count,
		&dwReads, NULL);

	if(!result || dwReads < sizeof(_T) * count)
	{
		throw except_value;
	}
}

// データ読み込み関数
template<typename _T, typename _E>
DWORD ReadDataMax(HANDLE hFile, _T *data, const DWORD count_max, const _E except_value)
{
	DWORD dwReads = 0;

	BOOL result = ::ReadFile(
		hFile,
		data, sizeof(_T) * count_max,
		&dwReads, NULL);

	if(!result)
	{
		throw except_value;
	}
	return dwReads;
}

// データ書き込み関数
template<typename _T, typename _E>
void WriteData(HANDLE hFile, const _T *data, const DWORD count, const _E except_value)
{
	DWORD dwWrites = 0;

	BOOL result = ::WriteFile(
		hFile,
		data, sizeof(_T) * count,
		&dwWrites, NULL);

	if(!result || dwWrites < sizeof(_T) * count)
	{
		throw except_value;
	}
}

#endif // __IOTEMPLATE__
