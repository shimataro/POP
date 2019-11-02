// Registry.cpp �c�C���v�������e�[�V�����t�@�C��
#include "Registry.h"
#include <tchar.h>


_WGC_BEGIN                              // namespace wgc {

// �R���X�g���N�^�ihKeyRoot���Ǘ�����j
registry::registry(HKEY hKeyRoot /* = HKEY_CURRENT_USER */)
{
	m_hKeyRoot = hKeyRoot;
	m_error    = ERROR_SUCCESS;
}

// �R���X�g���N�^2�ihKeyRoot�̃T�u�L�[���Ǘ�����j
registry::registry(HKEY hKeyRoot, LPCTSTR lpszSubName, BOOL bCreate /* = FALSE */, REGSAM samDesired /* = KEY_ALL_ACCESS */)
{
	m_hKeyRoot = hKeyRoot;
	if(bCreate)
	{
		m_hKeyRoot = CreateSubKey(lpszSubName, REG_OPTION_NON_VOLATILE);
	}
	else
	{
		m_hKeyRoot = OpenSubKey(lpszSubName, samDesired);
	}
}

// �f�X�g���N�^
registry::~registry(void)
{
/*
	if(m_hKeyRoot != NULL)
	{
		::RegCloseKey(m_hKeyRoot);
	}
*/
}

// �T�u�L�[���쐬
HKEY registry::CreateSubKey(LPCTSTR lpszSubName, const DWORD dwOptions /* = REG_OPTION_NON_VOLATILE */, const REGSAM samDesired /* = KEY_ALL_ACCESS */)
{
	HKEY hKeyResult;
	DWORD dwDisposition;

	m_error = ::RegCreateKeyEx(
		m_hKeyRoot,
		lpszSubName,
		NULL, NULL,
		dwOptions,
		samDesired,
		NULL,
		&hKeyResult,
		&dwDisposition);
	if(m_error != ERROR_SUCCESS)
	{
		return NULL;
	}
	return hKeyResult;
}

// �T�u�L�[���I�[�v��
HKEY registry::OpenSubKey(LPCTSTR lpszSubName, const REGSAM samDesired /* = KEY_ALL_ACCESS */)
{
	HKEY hKey;

	m_error = ::RegOpenKeyEx(m_hKeyRoot, lpszSubName, 0, samDesired, &hKey);
	if(m_error != ERROR_SUCCESS)
	{
		return NULL;
	}
	return hKey;
}

// �T�u�L�[���폜
LONG registry::DeleteSubKey(LPCTSTR lpszSubName)
{
	// �T�u�L�[���J��
	WUCRegistry regSubKey = OpenSubKey(lpszSubName, KEY_ALL_ACCESS);
	if(regSubKey == NULL)
	{
		// �J���Ȃ�������G���[�R�[�h��Ԃ�
		return m_error;
	}

	// �T�u�L�[�̃T�u�L�[���
	TCHAR szName[128];
	while(regSubKey.EnumerateSubKey(0, szName, sizeof(szName) / sizeof(szName[0])) != ERROR_NO_MORE_ITEMS)
		{
		// ���������L�[���폜
		regSubKey.DeleteSubKey(szName);
	}

	// �Ō�ɃT�u�L�[���폜
	m_error = ::RegDeleteKey(m_hKeyRoot, lpszSubName);
	return m_error;
}

// �T�u�L�[���
LONG registry::EnumerateSubKey(DWORD dwIndex, LPTSTR lpName, DWORD cbName)
{
	m_error = ::RegEnumKey(m_hKeyRoot, dwIndex, lpName, cbName);
	return m_error;
}


////////////////////////////////////////
// ��������

// �C�ӂ̃f�[�^����������
LONG registry::SetValue(LPCTSTR lpszValueName, DWORD dwReserved, DWORD dwType, CONST BYTE *lpData, DWORD cbData)
{
	m_error = ::RegSetValueEx(m_hKeyRoot, lpszValueName, dwReserved, dwType, lpData, cbData);
	return m_error;
}

// �o�C�i���l����������
LONG registry::SetValue(LPCTSTR lpszValueName, CONST BYTE *lpData, DWORD cbData)
{
	return SetValue(lpszValueName, 0, REG_BINARY, lpData, cbData);
}

// 32bit�l����������
LONG registry::SetValue(LPCTSTR lpszValueName, DWORD dwData)
{
	return SetValue(lpszValueName, 0, REG_DWORD, (CONST BYTE *)&dwData, sizeof(dwData));
}

// ���������������
LONG registry::SetValue(LPCTSTR lpszValueName, LPCTSTR lpszData)
{
	return SetValue(lpszValueName, 0, REG_SZ, (CONST BYTE *)lpszData, _tcsclen(lpszData)+1);
}


////////////////////////////////////////
// �ǂݍ���

// �C�ӂ̃f�[�^��ǂݍ���
LONG registry::GetValue(LPCTSTR lpszValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	m_error = ::RegQueryValueEx(m_hKeyRoot, lpszValueName, lpReserved, lpType, lpData, lpcbData);
	return m_error;
}

// �o�C�i���l��ǂݍ���
LONG registry::GetValue(LPCTSTR lpszValueName, LPBYTE lpData, DWORD cbData)
{
	return GetValue(lpszValueName, NULL, NULL, lpData, &cbData);
}

// 32bit�l��ǂݍ���
LONG registry::GetValue(LPCTSTR lpszValueName, LPDWORD lpdwData)
{
	DWORD cbData;
	return GetValue(lpszValueName, NULL, NULL, (LPBYTE)lpdwData, &cbData);
}

// �������ǂݍ���
LONG registry::GetValue(LPCTSTR lpszValueName, LPCTSTR lpszData, DWORD cbData)
{
	return GetValue(lpszValueName, NULL, NULL, (LPBYTE)lpszData, &cbData);
}

_WGC_END                                // }
