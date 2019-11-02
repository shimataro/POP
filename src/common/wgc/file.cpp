// file.cpp
#include "file.h"


_WGC_BEGIN

////////////////////////////////////////////////////////////
// file
file::file(HANDLE hFile /* = INVALID_HANDLE_VALUE */)
	: m_hFile(hFile), m_bOwner(FALSE)
{
}

file::~file(void)
{
	if(m_bOwner)
	{
		Close();
	}
}

BOOL file::Open(
	LPCTSTR               lpszFileName,
	const DWORD           dwDesiredAccess       /* = GENERIC_READ | GENERIC_WRITE */,
	const DWORD           dwShareMode           /* = FILE_SHARE_READ */,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes  /* = NULL */,
	const DWORD           dwCreationDisposition /* = OPEN_ALWAYS */,
	const DWORD           dwFlagsAndAttributes  /* = FILE_ATTRIBUTE_NORMAL */,
	HANDLE                hTemplateFile         /* = NULL */)
{
	m_hFile = ::CreateFile(
		lpszFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);

	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	m_bOwner = TRUE;
	return TRUE;
}

BOOL file::Close(void)
{
	HANDLE hFile = m_hFile;
	m_hFile  = INVALID_HANDLE_VALUE;
	m_bOwner = FALSE;

	return ::CloseHandle(hFile);
}

// èëÇ´çûÇ›
DWORD file::Write(LPCVOID lpBuffer, const DWORD nNumber)
{
	DWORD nNumberOfWritten;
	if(!::WriteFile(m_hFile, lpBuffer, nNumber, &nNumberOfWritten, NULL))
	{
		return static_cast<DWORD>(-1);
	}
	return nNumberOfWritten;
}

// èÓïÒ
DWORD file::GetSize(LPDWORD lpFileSizeHigh /* = NULL */) const
{
	return ::GetFileSize(m_hFile, lpFileSizeHigh);
}

file::operator HANDLE(void) const
{
	return m_hFile;
}


////////////////////////////////////////////////////////////
// mapping_object
mapping_object::mapping_object(void)
	: m_hMap(NULL)
{
}

mapping_object::~mapping_object(void)
{
	Close();
}

BOOL mapping_object::Create(
	file                  &rFile,
	LPSECURITY_ATTRIBUTES lpFileMappingAttributes /* = NULL */,
	const DWORD           flProtect               /* = PAGE_READONLY */,
	const DWORD           dwMaximumSizeHigh       /* = 0 */,
	const DWORD           dwMaximumSizeLow        /* = 0 */,
	LPCTSTR               lpName                  /* = NULL */)
{
	m_hMap = ::CreateFileMapping(
		rFile,
		lpFileMappingAttributes,
		flProtect,
		dwMaximumSizeHigh,
		dwMaximumSizeLow,
		lpName);

	if(m_hMap == NULL)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL mapping_object::Close(void)
{
	HANDLE hMap = m_hMap;
	m_hMap = NULL;

	return ::CloseHandle(hMap);
}

mapping_object::operator HANDLE(void) const
{
	return m_hMap;
}

_WGC_END
