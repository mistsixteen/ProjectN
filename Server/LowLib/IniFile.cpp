#include "stdafx.h"
#include "IniFile.h"


IniFile::IniFile()
{
	ZeroMemory(this->fileName, sizeof(this->fileName));
}

IniFile::~IniFile()
{
}

// 파라미터는 반드시 TCHAR 형
BOOL IniFile::Open(LPCTSTR fileName)
{
	if (!fileName)
		return FALSE;

	// 미리 정의된 PATH_SIZE를 통해 복제
	_tcsncpy(this->fileName, fileName, MAX_PATH);

	return TRUE;
}

BOOL IniFile::Close(VOID)
{
	return TRUE;
}

BOOL IniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, LPCTSTR value)
{
	if (!keyName || !valueName || !value)
		return FALSE;

	WritePrivateProfileString(keyName, valueName, value, this->fileName);

	return TRUE;
}

BOOL IniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, DWORD value)
{
	if (!keyName || !valueName)
		return FALSE;

	TCHAR Value[16] = { 0, };
	_itot(value, Value, 10);

	WritePrivateProfileString(keyName, valueName, Value, this->fileName);

	return TRUE;
}

BOOL IniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT value)
{
	if (!keyName || !valueName)
		return FALSE;

	TCHAR Value[16] = { 0, };
	_stprintf(Value, L"%f", value);

	WritePrivateProfileString(keyName, valueName, Value, this->fileName);

	return TRUE;
}

BOOL IniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, LPTSTR value, LPDWORD bufferLength)
{
	if (!keyName || !valueName || !value || !bufferLength)
		return FALSE;

	*bufferLength = GetPrivateProfileString(keyName, valueName, L"", value, *bufferLength, this->fileName);

	return TRUE;
}

BOOL IniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, LPDWORD value)
{
	if (!keyName || !valueName || !value)
		return FALSE;

	*value = GetPrivateProfileInt(keyName, valueName, 0, this->fileName);

	return TRUE;
}

BOOL IniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT * value)
{
	if (!keyName || !valueName || !value)
		return FALSE;

	TCHAR Value[16] = { 0, };

	GetPrivateProfileString(keyName, valueName, L"", Value, 16, this->fileName);

	*value = (FLOAT)_tstof(Value);

	return TRUE;
}
