#pragma once

class Log
{
public:
	static BOOL WriteLog(LPTSTR data, ...)
	{
		SYSTEMTIME		systemTime;
		TCHAR			currentDate[32]						= { 0, };
		TCHAR			currentFileName[MAX_PATH]			= { 0, };
		FILE*			filePtr								= NULL;
		TCHAR			DebugLog[MAX_BUFFER_LENGTH]			= { 0, };

		va_list			ap;
		TCHAR			Log[MAX_BUFFER_LENGTH]				= { 0, };

		va_start(ap, data);
		_vstprintf(Log, data, ap);
		va_end(ap);

		GetLocalTime(&systemTime);
		_sntprintf(currentDate, 32, L"%d-%d-%d %d:%d:%d",
			systemTime.wYear,
			systemTime.wMonth,
			systemTime.wDay,
			systemTime.wHour,
			systemTime.wMinute,
			systemTime.wSecond);

		_sntprintf(currentFileName, MAX_PATH, L"LOG_%d-%d-%d %d.log",
			systemTime.wYear,
			systemTime.wMonth,
			systemTime.wDay,
			systemTime.wHour);

		filePtr = _tfopen(currentFileName, L"a");
		if (!filePtr)
			return FALSE;

		_ftprintf(filePtr, L"[%s] %s\n", currentDate, Log);
		_sntprintf(DebugLog, MAX_BUFFER_LENGTH, L"[%s] %s\n", currentDate, Log);

		fflush(filePtr);

		fclose(filePtr);

		OutputDebugString(DebugLog);
		_tprintf(L"%s", DebugLog);

		return TRUE;
	}
};

