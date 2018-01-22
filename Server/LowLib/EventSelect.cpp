#include "stdafx.h"
#include "EventSelect.h"

DWORD WINAPI SelectThreadCallback(LPVOID parameter)
{
	EventSelect* eventSelect = (EventSelect*)parameter;
	eventSelect->SelectThreadCallback();

	return 0;
}

EventSelect::EventSelect(VOID)
{
	SelectEventHandle	= NULL;		// 소켓 이벤트 관리
	StartupEventHandle	= NULL;		// 스레드 이벤를 관리하는 스레드 시작을 체크
	DestroyEventHandle	= NULL;		// 스레드 종료 시 사용
	SelectThreadHandle	= NULL;		// 소켓 이벤트를 관리하는 스레드 핸들 값

	Socket				= NULL;		// 사용한 소켓 핸들
}

EventSelect::~EventSelect(VOID)
{
}

// 시작할 때 소켓 핸들 값을 받아오는데 이것은 NetworkSession의 GetSocket을 통해 확인 가능
// 소켓 이벤트를 관리할 스레드 생성이 주 작업
// WSAEventSelect 보다 편리한 WSAEventAsyncSelect 함수가 있으나
// 소켓 이벤트를 윈도우즈 창의 이벤트 핸들러를 이용하기 때문에
// 쓸데없이 윈도우즈 창을 하나 생성해야하는 리소스 낭비가 생김
BOOL EventSelect::Begin(SOCKET socket)
{
	// 소켓 파라미터가 잘못되었을 경우 실행하지 않음
	if (!socket)
		return FALSE;

	// 멤버 변수 소켓이 이미 NULL이 아닐 경우 중복 실행으로 간주하고 실행하지 않음
	if (Socket)
		return FALSE;

	// 멤버 변수에 입력
	Socket = socket;

	SelectEventHandle = WSACreateEvent();		// 소켓 이벤트를 관리할 이벤트를 생성
	if (SelectEventHandle == WSA_INVALID_EVENT) {
		End();
		return FALSE;
	}

	DestroyEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	// 스레드 종료 이벤트를 생성
	if (DestroyEventHandle == NULL) {
		End();
		return FALSE;
	}

	StartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	// 스레드 시작 이벤트를 생성
	if (StartupEventHandle == NULL) {
		End();
		return FALSE;
	}

	// Connect, Read, Write, Close 이벤트를 관리하는 것으로 확인
	// WSAEventSelect 내 3번째 매개변수 : 어떤 이벤트를 검출할지 등록
	DWORD Result = WSAEventSelect(socket, SelectEventHandle, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	if (Result == SOCKET_ERROR) {
		End();
		return FALSE;
	}

	// 소켓 이벤트를 관리할 스레드를 생성
	DWORD SelectThreadID = 0;
	SelectThreadHandle = CreateThread(NULL, 0, ::SelectThreadCallback, this, 0, &SelectThreadID);
	if (!SelectThreadHandle) {
		End();
		return FALSE;
	}

	// 스레드가 생성 완료될 때까지 대기
	// 스레드가 Wait될 때까지 시간을 벌어줌
	// 스레드가 생성되면서 StartupEventHandle 이벤트 발생 시 함수 종료
	WaitForSingleObject(StartupEventHandle, INFINITE);

	return TRUE;
}

// 개체 종료
BOOL EventSelect::End(VOID)
{
	// 소켓 멤버가 이미 NULL일 경우 실행하지 않음
	if (!Socket)
		return FALSE;

	if (SelectThreadHandle) {		//  스레드가 실행 중일 때 종료시켜줌
		// 스레드를 종료하는 이벤트를 발생
		SetEvent(DestroyEventHandle);

		// 스레드의 완전 종료를 기다림
		WaitForSingleObject(SelectThreadHandle, INFINITE);

		// 종료되었을 경우 스레드 핸들을 초기화
		CloseHandle(SelectThreadHandle);
	}

	// 이벤트 종료
	if (SelectEventHandle)
		CloseHandle(SelectEventHandle);

	if (DestroyEventHandle)
		CloseHandle(DestroyEventHandle);

	if (StartupEventHandle)
		CloseHandle(StartupEventHandle);

	return TRUE;
}

// 소켓 이벤트를 관리하는 스레드
VOID EventSelect::SelectThreadCallback(VOID)
{
	// 네트워크 이벤트를 관리하는 함수
	WSANETWORKEVENTS	NetworkEvents;
	DWORD				EventID			= 0;
	DWORD				REsult			= 0;
	// 스레드에 직접적으로 영향을 미치는 이벤트는 종료 & 네트워크 이벤트
	HANDLE				ThreadEvents[2] = {DestroyEventHandle, SelectEventHandle};

	while (TRUE){
		// 스레드가 정상적으로 시작되었다는 것을 Begin 함수에 알려줌
		SetEvent(StartupEventHandle);

		// 여기서는 스레드의 시작과 종료를 관리

		// 어떠한 이벤트가 발생할 때까지 계속 대기
		EventID = ::WaitForMultipleObjects(2, ThreadEvents, FALSE, INFINITE);
		switch (EventID)
		{
		// 스레드 종료 이벤트 발생
		case WAIT_OBJECT_0:
			return;
		case WAIT_OBJECT_0 + 1:
			// 소켓 이벤트 발생 시 받아옴
			// SelectEventhandle에서 네트워크 이벤트를 뽑아옴
			// WSAEnumNetworkEvents 내 4번째 파라미터 : FD 이벤트들의 값과 AND 연산을 통해 이벤트 확인
			EventID = WSAEnumNetworkEvents(Socket, SelectEventHandle, &NetworkEvents);

			// 만약 Event ID가 SOCKET_ERROR일 경우 문제가 발생한 것이므로 스레드를 종료
			// 에러 내용은 WSAGetLastError 함수를 통해 확인 가능
			if (EventID == SOCKET_ERROR)
				return;
			else {
				// Connect 이벤트 발생
				if (NetworkEvents.lNetworkEvents & FD_CONNECT) {
					// 가상 함수 호출
					OnIoConnected();

					if (NetworkEvents.iErrorCode[FD_CONNECT_BIT])
						return;
				}
				// 미사용
				// Write가 완료된 시점 자체가 정말 완료된 시점이 아닐 수도 있음
				else if (NetworkEvents.lNetworkEvents & FD_WRITE) {
					// OnIoWrote();
				}
				// Read 이벤트 발생
				else if (NetworkEvents.lNetworkEvents & FD_READ) {
					// 가상 함수 호출
					OnIoRead();
				}
				// Disconnect 이벤트 발생
				else if (NetworkEvents.lNetworkEvents & FD_CLOSE) {
					// 가상 함수 호출
					OnIoDisconnected();
					// 종료
					return;
				}
			}
			break;
		default:
			return;
		}
	}
}
