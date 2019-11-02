// file.h
#ifndef __WGC_FILE__
#define __WGC_FILE__

#include "wgc.h"


_WGC_BEGIN                              // namespace wgc {

class file
{
public:
	file(HANDLE hFile = INVALID_HANDLE_VALUE);
	~file(void);

	BOOL Open(
		LPCTSTR               lpszFileName,
		const DWORD           dwDesiredAccess       = GENERIC_READ | GENERIC_WRITE,
		const DWORD           dwShareMode           = FILE_SHARE_READ,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes  = NULL,
		const DWORD           dwCreationDisposition = OPEN_ALWAYS,
		const DWORD           dwFlagsAndAttributes  = FILE_ATTRIBUTE_NORMAL,
		HANDLE                hTemplateFile         = NULL);

	BOOL Close(void);

	DWORD Write(LPCVOID lpBuffer, const DWORD nNumber);

	// 情報
	DWORD GetSize(LPDWORD lpFileSizeHigh = NULL) const;

	operator HANDLE(void) const;

private:
	HANDLE m_hFile;
	BOOL   m_bOwner;
};


// ファイルマッピングオブジェクト
class mapping_object
{
public:
	// ファイルマッピングデータ
	template<typename _Ty>
	class mapping_data
	{
	public:
		mapping_data(void) : m_lpData(NULL) {}
		~mapping_data(void) { UnmapView(); }

		BOOL MapView(
			mapping_object &rMappingObject,
			const DWORD dwDesiredAccess      = FILE_MAP_READ,
			const DWORD dwFileOffsetHigh     = 0,
			const DWORD dwFileOffsetLow      = 0,
			const DWORD dwNumberOfBytesToMap = 0)
		{
			LPVOID lpData = ::MapViewOfFile(
				rMappingObject,
				dwDesiredAccess,
				dwFileOffsetHigh,
				dwFileOffsetLow,
				dwNumberOfBytesToMap);

			if(lpData == NULL)
			{
				return FALSE;
			}
			m_lpData = reinterpret_cast<_Ty *>(lpData);
			return TRUE;
		}

		BOOL UnmapView(void)
		{
			LPVOID lpData = m_lpData;
			m_lpData = NULL;
			return ::UnmapViewOfFile(lpData);
		}

		_Ty *GetData(void) { return m_lpData; }
		const _Ty *GetData(void) const { return m_lpData; }

	private:
		_Ty *m_lpData;
	};

	mapping_object(void);
	~mapping_object(void);

	BOOL Create(
		file                  &rFile,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		const DWORD           flProtect = PAGE_READONLY,
		const DWORD           dwMaximumSizeHigh = 0,
		const DWORD           dwMaximumSizeLow  = 0,
		LPCTSTR               lpName = NULL);

	BOOL Close(void);

	operator HANDLE(void) const;

private:
	HANDLE m_hMap;
};

_WGC_END                                // }

#endif // __WGC_FILE__
