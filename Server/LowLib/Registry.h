#pragma once

class Registry
{
private:
	HKEY rootKey;
	BOOL isOpened;
public:
	BOOL Open(HKEY rootKey, LPCTSTR subKey);
	BOOL Close(VOID);

	BOOL CreateKey(HKEY rootKey, LPCTSTR subKey);
	BOOL DeleteKey(HKEY rootKey, LPCTSTR subKey);

	BOOL SetValue(LPCTSTR valueName, LPCTSTR value);
	BOOL SetValue(LPCTSTR valueName, DWORD value);
	BOOL SetValueForMultiSize(LPCTSTR valueName, LPCTSTR value, DWORD byteLength);

	BOOL GetValue(LPCTSTR valueName, LPCTSTR value, LPDWORD bufferLength);
	BOOL GetValue(LPCTSTR valueName, LPDWORD value);

	BOOL DeleteValue(LPCTSTR valueName);
public:
	Registry();
	virtual ~Registry();
};

