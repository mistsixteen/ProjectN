#include "stdafx.h"
#include "MiniDump.h"

typedef BOOL(WINAPI *MINIDUMPWRITEDUMP)( // 콜백 함수의 원형
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter = NULL; 

// Unhandled Exception 발생 시 넘어오는 콜백
LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS* exceptionInfo)
{
	HMODULE DllHandle	= NULL;

	// Windows 2000 이전에는 따로 DBGHELP를 배포해서 설정해주어야 함
	// 여기서 사용되는 것은 DBGHELP.DLL 안에 포함된 함수이며 반드시 로드해야 함
	DllHandle			= LoadLibrary(L"DBGHELP.DLL");

	if (DllHandle) {
		// 덤프를 받아 파일로 만드는 과정
		// DBGHELP.DLL에서 MiniDumpWriteDump를 불러와 Dump라고 정의
		// 이를 이용해 덤프 파일을 만들게 됨

		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");

		if (Dump) {
			TCHAR		DumpPath[MAX_PATH] = { 0, };
			SYSTEMTIME	SystemTime;

			// 현재 시간을 가져옴
			GetLocalTime(&SystemTime);

			// 현재 시간을 기준으로 로그 파일 생성
			_sntprintf(DumpPath, MAX_PATH, L"%d-%d-%d %d_%d_%d.dmp",
				SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond);

			// 파일 생성
			HANDLE FileHandle = CreateFile(
				DumpPath,
				GENERIC_WRITE,
				FILE_SHARE_WRITE,
				NULL, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			// 파일 만들기 성공
			if (FileHandle != INVALID_HANDLE_VALUE) {
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

				// MiniDump 예외 정보 저장 구조체
				MiniDumpExceptionInfo.ThreadId			= GetCurrentThreadId();
				MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
				MiniDumpExceptionInfo.ClientPointers	= NULL;

				// 현재 프로세스에 대한 덤프를 실행
				// 여기서 Dump는 위의 DBGHELP.DLL에서 불러온 것을 이용
				// 이를 통해 덤프 파일이 생성됨
				BOOL Success = Dump(
					GetCurrentProcess(),
					GetCurrentProcessId(),
					FileHandle,					// 미리 생성해 놓은 파일의 핸들 값
					MiniDumpNormal,
					&MiniDumpExceptionInfo,		// 예외 정보
					NULL,
					NULL);

				if (Success) {
					// 성공
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
	// 잘못된 연산 창이 나타나지 않게 함
	SetErrorMode(SEM_FAILCRITICALERRORS);

	// Unhandled Exception 발생 시 진행 될 콜백 정의
	PreviousExceptionFilter = SetUnhandledExceptionFilter(UnHandledExceptionFilter);

	return TRUE;
}

BOOL MiniDump::End(VOID)
{
	SetUnhandledExceptionFilter(PreviousExceptionFilter);

	return TRUE;
}
