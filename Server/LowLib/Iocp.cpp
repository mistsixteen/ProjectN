#include "stdafx.h"
#include "Iocp.h"

DWORD WINAPI WorkerThreadCallback(LPVOID parameter)
{
	Iocp* iocp = (Iocp*)parameter;
	iocp->WorkerThreadCallback();

	return 0;
}

Iocp::Iocp(VOID)
{
	IocpHandle			= NULL;		// IOCP를 관리하는 핸들 값
	WorkerThreadCount	= 0;		// 사용할 WorkerThread 개수

	StartupEventHandle	= NULL;		// 시작을 관리하는 이벤트
}

Iocp::~Iocp(VOID)
{
}

// IOCP를 사용하기 위한 기본적인 준비
BOOL Iocp::Begin(VOID)
{
	// IocpHandle 값 초기화
	IocpHandle = NULL;

	// 서버 시스템의 정보를 가져옴
	// 가져온 시스템 정보에서 CPU 정보만을 사용
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	// CPU 개수의 2배만큼의 WorkerThread 생성
	WorkerThreadCount = SystemInfo.dwNumberOfProcessors * 2;
	// IocpHandle을 생성
	// 메인 IOCP 핸들의 생성이므로 모든 파라미터 값이 0 또는 NULL
	// CreateIoCompletionPort : 기본이 되는 메인 핸들 생성 시 모든 파라미터를 0이나 NULL로 설정
	// 1개의 CPU에 2개의 스레드가 최적 by MSDN -> 서버의 종류에 따라 바꿔도 됨
	// 1~2명의 사용자가 최대한 빨리 작업해야하는 경우 스레드 개수를 조금 늘리거나
	// 많은 유저가 느리더라도 응답을 받아야하는 서버의 경우 스레드의 개수를 조금 늘림
	IocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// 생성에 실패하면 종료
	if (!IocpHandle)
		return FALSE;

	// 시작을 관리하는 이벤트도 생성
	StartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (StartupEventHandle == NULL) {
		End();
		return FALSE;
	}

	// CPU 개수의 2배만큼의 WorkerThread를 생성해서 WorkerThreadVector로 관리
	for (DWORD i = 0; i < WorkerThreadCount; ++i) {
		HANDLE WorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
		WorkerThreadVector.push_back(WorkerThread);
		// 하나의 스레드마다 생성이 완료될 때까지 대기
		WaitForSingleObject(StartupEventHandle, INFINITE);
	}

	return TRUE;
}

BOOL Iocp::End(VOID)
{
	// IOCP를 이용해서 각 WorkerThread마다 종료를 요청
	for (DWORD i = 0; i < WorkerThreadCount; ++i) {
		// PostQueuedCompletionStatus : IOCP에 강제적으로 신호를 보내는 함수
		// 1번째 파라미터는 신호를 보낼 IOCP 핸들이고
		// 뒤에 3개의 파라미터는 신호에 담아서 보낼 데이터
		// 즉 End 함수에서처럼 0, 0, NULL을 넣어서 보내면
		// WorkerThread에서도 역시 0, 0, NULL로 결과를 받게 됨
		PostQueuedCompletionStatus(IocpHandle, 0, 0, NULL);
	}

	// 각 WorkerThread가 종료되기를 기다려 핸들을 닫아줌
	for (DWORD i = 0; i < WorkerThreadCount; ++i) {
		// 한 WorkerThread가 완전히 종료될 때까지 대기
		WaitForSingleObject(WorkerThreadVector[i], INFINITE);
		CloseHandle(WorkerThreadVector[i]);
	}
	
	// IOCP 핸들도 종료
	if (IocpHandle)
		CloseHandle(IocpHandle);

	// WorkerThread를 관리하는 벡터를 지워줌
	WorkerThreadVector.clear();

	// 시작을 관리하는 이벤트 핸들 종료
	if (StartupEventHandle)
		CloseHandle(StartupEventHandle);

	return TRUE;
}

BOOL Iocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	
	// 소켓 핸들이나 키가 잘못되었을 경우에는 등록하지 않음
	if (!socket || !completionKey)
		return FALSE;

	// Begin에서 생성했던 IocpHandle에 추가로 소켓 핸들 및 키를 등록
	IocpHandle = CreateIoCompletionPort((HANDLE)socket, IocpHandle, completionKey, 0);

	if (!IocpHandle)
		return FALSE;

	return TRUE;
}

// GetQueuedCompletionStatus 함수를 이용해서 받은 파라미터의 모든 것을 판단하고 처리
VOID Iocp::WorkerThreadCallback(VOID)
{
	// 성공, 실패 여부를 판단하는 변수
	BOOL Successed					= FALSE;
	DWORD NumberOfByteTransfered	= 0;			// 몇 바이트의 IO가 발생했는지 확인
	VOID* CompletionKey				= NULL;			// 키 값
	OVERLAPPED* Overlapped			= NULL;			// Overlapped 값
	OVERLAPPED_EX* OverlappedEx		= NULL;
	VOID* Object					= NULL;			// 소켓 핸들에 매치되는 개체 포인터 값

	while (TRUE) {
		// 이제 WorkerThread를 시작할 수 있다고 알려주는 이벤트
		// Begin 함수에서는 이 이벤트가 발생해야만 정상적으로 다음 루틴으로 넘어감
		SetEvent(StartupEventHandle);

		// PostQueuedCompletionStatus나 IOCP에 등록된 소켓 및 파일의 Read, Write 관련 함수의
		// 상태를 받아오는 함수
		//
		// 어떠한 IO 발생 시 항상 Overlapped가 넘어오게 되는데
		// 그것이 어떠한 작업의 IO인지를 확인하기 위해서 각각의 Overlapped를 생성해서 사용했던 것
		//
		// OverlappedEx->Object의 경우 어떤 접속자가 신호를 보내왔는지를 확인하기 위함
		Successed = GetQueuedCompletionStatus(
					IocpHandle,					// IOCP 핸들 값
					&NumberOfByteTransfered,	// IO에 이용된 데이터 크기
					(LPDWORD)&CompletionKey,	// 소켓이나 파일의 핸들 값
					&Overlapped,				// Read, Write 등에 사용된 Overlapped 값
					INFINITE);					// 신호가 발생될 때까지 무제한으로 기다려줌

		// 키가 NULL일 경우 종료. 이것은 End에서 PostQueuedCompletionStatus의
		// 키를 NULL로 넣어준 것으로 동작
		// 또는 코딩의 문제로 데이터가 잘못 넘어올 경우 걸러주는 역할을 함
		if (!CompletionKey)
			return;

		// Overlapped 포인터를 받아 OverlappedEx의 포인터를 확인
		OverlappedEx = (OVERLAPPED_EX*)Overlapped;
		// 이 부분은 NetworkSession에서 WSARecv, WSASend 등을 할 떄 넘겨준 OVERLAPPED_EX를
		// 그대로 받음. NetworkSession의 Begin 함수를 보시면 OverlappedEx->Object에
		// this를 입력해줘 어느 개체인지 확인할 수 있게 한 것을 볼 수 있음
		Object = OverlappedEx->Object;

		// GetQueuedCompletionStatus의 결과가 실패이거 성공지만 IO에 사용된 크기가 0일 경우
		if (!Successed || (Successed && !NumberOfByteTransfered)) {
			// 클라이언트가 AcceptEx를 통해서 Accept한 경우와
			if (OverlappedEx->IoType == IO_ACCEPT)
				OnIoConnected(Object);
			// 클라이언트가 접속이 종료된 경우
			else
				OnIoDisconnected(Object);
			
			// 다시 루프를 초기화
			continue;
		}

		// 그리고 AcceptEx에는 IO_TYPE를 IO_ACCEPT로 설정한 OverlappedEx를 WSARecv에는
		// IO_TYPE를 IO_READ로 설정한 것을 WSASend에는 IO_TYPE을 IO_WRITE로 설정한
		// OverlappedEx를 넣어주는 것도 볼수 있음
		switch (OverlappedEx->IoType)
		{
			// 데이터 수신 (WSARecv, WSARecvFrom)
		case IO_READ:
			OnIoRead(Object, NumberOfByteTransfered);
			break;
			// 데이터 송신 (WSASend, WSASendTo)
		case IO_WRITE:
			OnIoWrote(Object, NumberOfByteTransfered);
			break;
		}
	}
}
