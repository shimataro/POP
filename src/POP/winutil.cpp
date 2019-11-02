// winutil.cpp
#include "stdafx.h"
#include "winutil.h"

#include <stdio.h>
#include <stdarg.h>
#include <vector>

#include <shlwapi.h>                    // ::PathRemoveFileSpec(), ::PathAppend()

#pragma comment(lib, "version.lib")
#pragma comment(lib, "shlwapi.lib")

#define VERSION_ROOT        _T("\\")
#define VERSION_TRANSLATE   _T("\\VarFileInfo\\Translation")
#define VERSION_FORMAT      _T("\\StringFileInfo\\%04x%04x\\%s")


// ���s���W���[���̃o�[�W�������擾
BOOL GvxGetFileVersion(LPCTSTR lpszPathName, VS_FIXEDFILEINFO *lpvffi, LPTSTR lpszLanguage, const int nLanguageSize, ...)
{
	LPTSTR lptstrPathName = const_cast<LPTSTR>(lpszPathName);

	// �o�[�W�������̃T�C�Y���擾
	DWORD dwHandle = 0;
	const DWORD dwVersionSize = ::GetFileVersionInfoSize(lptstrPathName, &dwHandle);
	if(dwVersionSize == 0)
	{
		return FALSE;
	}

	// �o�[�W���������擾
	std::vector<BYTE> buffer(dwVersionSize);
	::GetFileVersionInfo(lptstrPathName, dwHandle, dwVersionSize, &buffer[0]);

	// �\���̂Ƀo�[�W���������i�[
	{
		LPVOID lpBuffer = NULL;
		UINT   uLen = 0;
		::VerQueryValue(&buffer[0], VERSION_ROOT, &lpBuffer, &uLen);
		::CopyMemory(lpvffi, lpBuffer, sizeof(VS_FIXEDFILEINFO));
	}

	// ���ꖼ���i�[
	if(lpszLanguage != NULL)
	{
		LPDWORD lpdwTranslate = NULL;
		UINT    uLen = 0;
		::VerQueryValue(&buffer[0], VERSION_TRANSLATE, reinterpret_cast<LPVOID *>(&lpdwTranslate), &uLen);
		::VerLanguageName(LOWORD(*lpdwTranslate), lpszLanguage, nLanguageSize);

		// �ό����̏���
		va_list va = NULL;
		va_start(va, nLanguageSize);

		// StringName���擾
		LPCTSTR lpszStringName = NULL;
		while((lpszStringName = va_arg(va, LPCTSTR)) != NULL)
		{
			// �T�u�u���b�N���쐬
			TCHAR szSubBlock[128];
			wsprintf(szSubBlock, VERSION_FORMAT, LOWORD(*lpdwTranslate), HIWORD(*lpdwTranslate), lpszStringName);

			// �l���擾
			LPTSTR lptstrValue = NULL;
			::VerQueryValue(&buffer[0], szSubBlock, reinterpret_cast<LPVOID *>(&lptstrValue), &uLen);

			// �l���o�b�t�@�Ɋi�[
			LPTSTR     lpszValue  = va_arg(va, LPTSTR);
			const UINT nValueSize = va_arg(va, UINT);

			_tcsncpy(lpszValue, lptstrValue, nValueSize);
			lpszValue[nValueSize - 1] = _T('\0');
		}
		va_end(va);
	}

	return TRUE;
}


// �w�肳�ꂽ���j���[ID���q�Ɏ����j���[�n���h���Əꏊ���擾
HMENU GvxFindMenu(HMENU hMenu, UINT nFindID, int *pIndex)
{
	int tmp;
	if(pIndex == NULL)
	{
		pIndex = &tmp;
	}

	// ���j���[������
	const int item_count = GetMenuItemCount(hMenu);
	for(int i = 0; i < item_count; i++)
	{
		UINT nSubID = GetMenuItemID(hMenu, i);

		// �|�b�v�A�b�v���j���[�̏ꍇ
		if(nSubID == -1)
		{ 
			// �ċA����
			HMENU hFindResult = GvxFindMenu(GetSubMenu(hMenu, i), nFindID, pIndex);
			if(hFindResult != NULL)
			{
				return hFindResult;
			}
		}
		// ���j���[�A�C�e��������ꍇ
		else
		{
			// �T���Ă���ID������������I��
			if(nSubID == nFindID)
			{
				*pIndex = i;
				return hMenu;
			}
		}
	}

	// ������Ȃ�������NULL��Ԃ�
	return NULL;
}


// TCHAR������ANSI������
void GvxTCHAR2CHAR(LPCTSTR lpszTcharString, LPSTR lpszAnsiString, int nBufferMax)
{
#ifdef _UNICODE
	::WideCharToMultiByte(
		CP_ACP,
		0,
		lpszTcharString, -1,
		lpszAnsiString, nBufferMax,
		NULL, NULL);
#else
	strncpy(lpszAnsiString, lpszTcharString, nBufferMax - 1);
	lpszAnsiString[nBufferMax - 1] = '\0';
#endif
}

// ANSI������TCHAR������
void GvxCHAR2TCHAR(LPCSTR lpszAnsiString, LPTSTR lpszTcharString, int nBufferMax)
{
#ifdef _UNICODE
	::MultiByteToWideChar(
		CP_ACP,
		0,
		lpszAnsiString, -1,
		lpszTcharString, nBufferMax);
#else
	strncpy(lpszTcharString, lpszAnsiString, nBufferMax - 1);
	lpszTcharString[nBufferMax - 1] = '\0';
#endif
}


// ���W���[���̂���f�B���N�g�����Ƀp�X��ǉ�
void GvxPathAppendToModuleDir(HMODULE hModule, LPTSTR lpPathName, DWORD dwSize, ...)
{
	// ���W���[���̃p�X������t�@�C�������폜
	::GetModuleFileName(hModule, lpPathName, dwSize);
	::PathRemoveFileSpec(lpPathName);

	LPCTSTR lpszAppendName = NULL;

	va_list va = NULL;
	va_start(va, dwSize);
	while((lpszAppendName = va_arg(va, LPCTSTR)) != NULL)
	{
		::PathAppend(lpPathName, lpszAppendName);
	}
	va_end(va);
}
