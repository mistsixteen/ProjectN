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
	// �������� �Ķ���ʹ� NULL ���� Ȯ���� �߿�
	if (!subKey)
		return FALSE;

	// �̹� ���������� ����
	if (isOpened)
		return FALSE;

	// ������ Ű�� ���� rootKey�� ����
	if (RegOpenKey(rootKey, subKey, &this->rootKey) != ERROR_SUCCESS)
		return FALSE;

	// ������ ����� Ű�� �������� �Ǵ��ϴ� ����
	// ���� TRUE�� ����
	isOpened = TRUE;

	return TRUE;
}

BOOL Registry::Close(VOID)
{
	if (RegCloseKey(this->rootKey) != ERROR_SUCCESS)
		return FALSE;

	// Ű�� �����鼭 FALSE�� ����
	isOpened = FALSE;

	return TRUE;
}

BOOL Registry::CreateKey(HKEY rootKey, LPCTSTR subKey)
{
	// rootKey�� �������� ������ subKey�� �Է¹޾� Ű�� ����
	if (!subKey)
		return FALSE;

	// �̹� ���������� ����
	if (isOpened)
		return FALSE;

	// ������Ʈ�� Ű�� �����ϴ� API �Լ� ȣ��
	if (RegCreateKey(rootKey, subKey, &this->rootKey) != ERROR_SUCCESS)
		return FALSE;

	// OpenKey �ܿ� �����ϸ鼭 �ڵ����� ���� ���·� ����
	isOpened = TRUE;

	return TRUE;
}

BOOL Registry::DeleteKey(HKEY rootKey, LPCTSTR subKey)
{
	if (!subKey)
		return FALSE;

	// ������Ʈ�� Ű�� �����ϴ� API �Լ��� ȣ��
	if (RegDeleteKey(rootKey, subKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// Ű�� �޸� Create�� �ʿ� ���� SetValue�� �� �� ���� ���� ��� ����
BOOL Registry::SetValue(LPCTSTR valueName, LPCTSTR value)
{
	// TCHAR���� ���ڿ� �����͸� ������Ʈ���� �Է��� �� ���
	if (!valueName || !value)
		return FALSE;

	if (!isOpened)
		return FALSE;

	// REG_SZ������ �����͸� �Է�
	if (RegSetValueEx(this->rootKey, valueName, 0, REG_SZ,
		(BYTE*)value, (DWORD)_tcslen(value) * sizeof(TCHAR)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL Registry::SetValue(LPCTSTR valueName, DWORD value)
{
	// TCHAR���� ���ڿ� �����͸� ������Ʈ���� �Է��� �� ���
	if (!valueName)
		return FALSE;

	if (!isOpened)
		return FALSE;

	// REG_WORD������ ������ �Է�
	if (RegSetValueEx(this->rootKey, valueName, 0, REG_SZ,
		(BYTE*)&value, sizeof(DWORD)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// ���� �� ���� ���� MultiSZ���� ���ڿ��� ����� �� ���� �Լ�
BOOL Registry::SetValueForMultiSize(LPCTSTR valueName, LPCTSTR value, DWORD byteLength)
{
	if (!valueName || !value)
		return FALSE;

	if (!isOpened)
		return FALSE;

	// REG_MULTI_SZ������ �����͸� �Է�
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

	// ���ڿ� �����͸� bufferLength ����� ���̸�ŭ �о��
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
