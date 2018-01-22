#include "stdafx.h"
#include "MiniDump.h"

typedef BOOL(WINAPI *MINIDUMPWRITEDUMP)( // �ݹ� �Լ��� ����
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter = NULL; 

// Unhandled Exception �߻� �� �Ѿ���� �ݹ�
LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS* exceptionInfo)
{
	HMODULE DllHandle	= NULL;

	// Windows 2000 �������� ���� DBGHELP�� �����ؼ� �������־�� ��
	// ���⼭ ���Ǵ� ���� DBGHELP.DLL �ȿ� ���Ե� �Լ��̸� �ݵ�� �ε��ؾ� ��
	DllHandle			= LoadLibrary(L"DBGHELP.DLL");

	if (DllHandle) {
		// ������ �޾� ���Ϸ� ����� ����
		// DBGHELP.DLL���� MiniDumpWriteDump�� �ҷ��� Dump��� ����
		// �̸� �̿��� ���� ������ ����� ��

		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");

		if (Dump) {
			TCHAR		DumpPath[MAX_PATH] = { 0, };
			SYSTEMTIME	SystemTime;

			// ���� �ð��� ������
			GetLocalTime(&SystemTime);

			// ���� �ð��� �������� �α� ���� ����
			_sntprintf(DumpPath, MAX_PATH, L"%d-%d-%d %d_%d_%d.dmp",
				SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond);

			// ���� ����
			HANDLE FileHandle = CreateFile(
				DumpPath,
				GENERIC_WRITE,
				FILE_SHARE_WRITE,
				NULL, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			// ���� ����� ����
			if (FileHandle != INVALID_HANDLE_VALUE) {
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

				// MiniDump ���� ���� ���� ����ü
				MiniDumpExceptionInfo.ThreadId			= GetCurrentThreadId();
				MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
				MiniDumpExceptionInfo.ClientPointers	= NULL;

				// ���� ���μ����� ���� ������ ����
				// ���⼭ Dump�� ���� DBGHELP.DLL���� �ҷ��� ���� �̿�
				// �̸� ���� ���� ������ ������
				BOOL Success = Dump(
					GetCurrentProcess(),
					GetCurrentProcessId(),
					FileHandle,					// �̸� ������ ���� ������ �ڵ� ��
					MiniDumpNormal,
					&MiniDumpExceptionInfo,		// ���� ����
					NULL,
					NULL);

				if (Success) {
					// ����
					CloseHandle(FileHandle);

					return EXCEPTION_EXECUTE_HANDLER;
				}
			}

			CloseHandle(FileHandle);
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

BOOL MiniDump::Begin(VOID)
{
	// �߸��� ���� â�� ��Ÿ���� �ʰ� ��
	SetErrorMode(SEM_FAILCRITICALERRORS);

	// Unhandled Exception �߻� �� ���� �� �ݹ� ����
	PreviousExceptionFilter = SetUnhandledExceptionFilter(UnHandledExceptionFilter);

	return TRUE;
}

BOOL MiniDump::End(VOID)
{
	SetUnhandledExceptionFilter(PreviousExceptionFilter);

	return TRUE;
}
