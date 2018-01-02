#include "stdafx.h"
#include "Registry.h"


Registry::Registry()
	:rootKey(NULL), isOpened(FALSE)
{
}


Registry::~Registry()
{
}

BOOL Registry::Open(HKEY rootKey, LPCTSTR subKey)
{
	// 포인터형 파라미터는 NULL 여부 확인이 중요
	if (!subKey)
		return FALSE;

	// 이미 열려있으면 실패
	if (isOpened)
		return FALSE;

	// 지정된 키를 열어 rootKey에 저장
	if (RegOpenKey(rootKey, subKey, &this->rootKey) != ERROR_SUCCESS)
		return FALSE;

	// 앞으로 사용할 키를 열었는지 판단하는 변수
	// 값을 TRUE로 세팅
	isOpened = TRUE;

	return TRUE;
}

BOOL Registry::Close(VOID)
{
	if (RegCloseKey(this->rootKey) != ERROR_SUCCESS)
		return FALSE;

	// 키를 닫으면서 FALSE로 세팅
	isOpened = FALSE;

	return TRUE;
}

BOOL Registry::CreateKey(HKEY rootKey, LPCTSTR subKey)
{
	// rootKey를 기준으로 생성할 subKey를 입력받아 키를 생성
	if (!subKey)
		return FALSE;

	// 이미 열려있으면 실패
	if (isOpened)
		return FALSE;

	// 레지스트리 키를 생성하는 API 함수 호출
	if (RegCreateKey(rootKey, subKey, &this->rootKey) != ERROR_SUCCESS)
		return FALSE;

	// OpenKey 외에 생성하면서 자동으로 열린 형태로 세팅
	isOpened = TRUE;

	return TRUE;
}

BOOL Registry::DeleteKey(HKEY rootKey, LPCTSTR subKey)
{
	if (!subKey)
		return FALSE;

	// 레지스트리 키를 삭제하는 API 함수를 호출
	if (RegDeleteKey(rootKey, subKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// 키와 달리 Create할 필요 없이 SetValue를 할 때 값이 없을 경우 설정
BOOL Registry::SetValue(LPCTSTR valueName, LPCTSTR value)
{
	// TCHAR형의 문자열 데이터를 레지스트리에 입력할 때 사용
	if (!valueName || !value)
		return FALSE;

	if (!isOpened)
		return FALSE;

	// REG_SZ형으로 데이터를 입력
	if (RegSetValueEx(this->rootKey, valueName, 0, REG_SZ,
		(BYTE*)value, (DWORD)_tcslen(value) * sizeof(TCHAR)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL Registry::SetValue(LPCTSTR valueName, DWORD value)
{
	// TCHAR형의 문자열 데이터를 레지스트리에 입력할 때 사용
	if (!valueName)
		return FALSE;

	if (!isOpened)
		return FALSE;

	// REG_WORD형으로 데이터 입력
	if (RegSetValueEx(this->rootKey, valueName, 0, REG_SZ,
		(BYTE*)&value, sizeof(DWORD)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// 별로 쓸 일이 없는 MultiSZ형의 문자열을 사용할 때 쓰는 함수
BOOL Registry::SetValueForMultiSize(LPCTSTR valueName, LPCTSTR value, DWORD byteLength)
{
	if (!valueName || !value)
		return FALSE;

	if (!isOpened)
		return FALSE;

	// REG_MULTI_SZ형으로 데이터를 입력
	if (RegSetValueEx(this->rootKey, valueName, 0, REG_MULTI_SZ,
		(BYTE*)value, byteLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL Registry::GetValue(LPCTSTR valueName, LPCTSTR value, LPDWORD bufferLength)
{
	DWORD valueType = 0;

	if (!valueName || !value || !bufferLength)
		return FALSE;

	if (!isOpened)
		return FALSE;

	// 문자열 데이터를 bufferLength 저장된 길이만큼 읽어옴
	if (RegQueryValueEx(this->rootKey, valueName, 0, &valueType, 
						(BYTE*)value, bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL Registry::GetValue(LPCTSTR valueName, LPDWORD value)
{
	DWORD valueType = 0;
	DWORD bufferLength = sizeof(DWORD);

	if (!valueName || !value)
		return FALSE;

	if (!isOpened)
		return FALSE;

	if (RegQueryValueEx(this->rootKey, valueName, 0, &valueType,
		(BYTE*)value, &bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
	

BOOL Registry::DeleteValue(LPCTSTR valueName)
{
	if (!valueName)
		return FALSE;

	if (!isOpened)
		return FALSE;

	if (RegDeleteValue(this->rootKey, valueName) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
