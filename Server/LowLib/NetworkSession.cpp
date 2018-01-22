#include "stdafx.h"
#include "NetworkSession.h"

DWORD WINAPI ReliableUdpThreadCallback(LPVOID parameter)
{
	NetworkSession *UdpNetwork = (NetworkSession*)parameter;
	UdpNetwork->ReliableUdpThreadCallback();

	return 0;
}

NetworkSession::NetworkSession(VOID)
{
	// Accept에 관련된 Overlapped 구조체
	ZeroMemory(&AcceptOverlapped, sizeof(AcceptOverlapped));

	// Read에 관련된 Overlapped 구조체
	ZeroMemory(&ReadOverlapped, sizeof(ReadOverlapped));

	// Write에 관련된 Overlapped 구조체
	ZeroMemory(&WriteOverlapped, sizeof(WriteOverlapped));

	// UDP 사용 시 상대 주소 정보
	ZeroMemory(&UdpRemoteInfo, sizeof(UdpRemoteInfo));

	Socket							= NULL;		// 소켓 핸들 값 저장
	ReliableUdpThreadHandle			= NULL;		// 같은 패킷을 받을 때까지 계속 전송해주는
												// 스레드(ReliableUpdThread) 핸들
	ReliableUdpThreadStartupEvent	= NULL;		// ReliableUdpThread의 시작을 알리는 이벤트
	ReliableUdpThreadDestroyEvent	= NULL;		// ReliableUdpThread의 종료를 알리는 이벤트
	ReliableUdpThreadWakeUpEvent	= NULL;		// ReliableUdpThread를 깨울 때 사용하는 이벤트
	ReliableUdpWriteCompleteEvent	= NULL;		// 상대가 패킷을 받아 더 이상 보낼 필요가 없을 떄
												// 사용하는 이벤트
	IsReliableUdpSending			= NULL;		// 현재 보내고 있는 Reliable Data가 있는지 확인

	AcceptOverlapped.IoType			= IO_ACCEPT;// Overlapped 구조체의 종류를 정의
	ReadOverlapped.IoType			= IO_READ;	// Overlapped 구조체의 종류를 정의
	WriteOverlapped.IoType			= IO_WRITE;	// Overlapped 구조체의 종류를 정의

	AcceptOverlapped.Object			= this;		// 현재 개체의 포인터를 가지고 있음
	ReadOverlapped.Object			= this;		// 현재 개체의 포인터를 가지고 있음
	WriteOverlapped.Object			= this;		// 현재 개체의 포인터를 가지고 있음
}


NetworkSession::~NetworkSession(VOID)
{
}

// 개체 시작
BOOL NetworkSession::Begin(VOID)
{
	ThreadSync	Sync;		// 다중 스레드 동기화

	if (Socket)				// 이미 실행된 소켓이 있으면 실패
		return FALSE;

	// 변수 초기화
	ZeroMemory(ReadBuffer, sizeof(ReadBuffer));
	// UDP를 사용할 때 데이터를 보낼 상대 정보를 저장하기 위한 변수
	ZeroMemory(&UdpRemoteInfo, sizeof(UdpRemoteInfo));

	// 앞으로 계속 사용될 소켓 핸들
	Socket							= NULL;

	// Reliable UDP에서 사용할 스레드 관련 변수
	ReliableUdpThreadHandle			= NULL;		// Reliable UDP 메인 스레드 핸들
	ReliableUdpThreadStartupEvent	= NULL;		// 스레드가 시작됨을 알려주는 이벤트
	ReliableUdpThreadDestroyEvent	= NULL;		// 스레드가 종료되었음을 알려주는 이벤트
	ReliableUdpThreadWakeUpEvent	= NULL;		
	ReliableUdpWriteCompleteEvent	= NULL;		// 스레드 내 한 개의 데이터 전송이 완료되었을 때
												// 발생하는 이벤트
	
	IsReliableUdpSending			= FALSE;	// 현재 데이터를 전송 중인지 확인하는 FLAG

	return TRUE;
}

// 종료
BOOL NetworkSession::End(VOID)
{
	ThreadSync	Sync;		// 다중 스레드 동기화

	if (!Socket)			// 이미 소켓이 없으면 실패
		return FALSE;

	Socket = NULL;

	if (ReliableUdpThreadHandle) {					// ReliableUpdThread가 실행 중일 경우
		// 일단 스레드를 종료하는 이벤트를 발생
		SetEvent(ReliableUdpThreadDestroyEvent);

		// 스레드가 종료될 때까지 무한대로 대기
		// 여기서는 INFINITE를 이용해서 무한대로 기다리게 되지만 약 3~5초 대기 후 응답이 없을 경우
		// TerminateThread를 이용해서 강제 종료하는 것도 방법
		WaitForSingleObject(ReliableUdpThreadHandle, INFINITE);

		CloseHandle(ReliableUdpThreadHandle);		// 종료가 되면 핸들을 닫아줌
	}

	// 관련 이벤트를 모두 종료해줌
	if (ReliableUdpThreadDestroyEvent)
		CloseHandle(ReliableUdpThreadDestroyEvent);

	if (ReliableUdpThreadStartupEvent)
		CloseHandle(ReliableUdpThreadStartupEvent);

	if (ReliableUdpThreadWakeUpEvent)
		CloseHandle(ReliableUdpThreadWakeUpEvent);

	if (ReliableUdpWriteCompleteEvent)
		CloseHandle(ReliableUdpWriteCompleteEvent);

	// Reliable UDP로 보낼 데이터를 저자하는 WriteQueue를 비움
	ReliableWriteQueue.End();

	return TRUE;
}

// TCP 사용 시 맨 처음 호출하는 함수
// TCP_NODELAY : TCP에서 Nagle 알고리즘 사용 여부 결정 옵션
// Nagle : 양방향 네트워크에 의해서 만들어 질 때 작은 데이터를 순간적으로 묶어 한꺼번에 전송
// 패킷 분할 단위가 불확실해 나중에 따로 신경 써야할 것이 많음
BOOL NetworkSession::TcpBind(VOID)
{
	ThreadSync	Sync;		// 다중 스레드 동기화

	// Socket이 NULL이 아닐 경우 이미 소켓이 생성되어 Bind 되어있는 상태이므로 다시 TcpBind 함수를 실행 안함
	if (Socket)
		return FALSE;

	// TCP 형태의 소켓 생성
	Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (Socket == INVALID_SOCKET)
		return FALSE;

	// TCP_NODELAY 옵션
	// BOOL Nodelay = TRUE;
	//setsockopt(Socket, IPPROTO_TCP, TCP_NODELAY, (const char FAR*)&NoDelay, sizeof(NoDelay));

	return TRUE;
}

BOOL NetworkSession::UdpBind(USHORT port)
{
	ThreadSync	Sync;							// 다중 스레드 동기화

	if (Socket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo;				// 사용할 UDP 주소, 포트를 설정
	RemoteAddressInfo.sin_family = AF_INET;
	// 지정된 포트로 데이터를 받기 위해 지정
	RemoteAddressInfo.sin_port = htons(port);	
	RemoteAddressInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// UDP용 소켓을 생성. SOCK_DRAM과 IPPROTO_UDP로 정의해서 생성한 것을 확인
	Socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (Socket == INVALID_SOCKET)
		return FALSE;
	
	// 설정된 주소를 Bind
	if (bind(Socket, (struct sockaddr*) &RemoteAddressInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		End();
		return FALSE;
	}

	// ReliableUdpThread에서 사용할 핸들, 이벤트들을 생성
	// ReliableUdpThreadDestroyEvent
	// ReliableUdpThread를 종료할 때 발생시키는 이벤트
	ReliableUdpThreadDestroyEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (ReliableUdpThreadDestroyEvent == NULL) {
		End();
		return FALSE;
	}

	// ReliableUdpThreadStartupEvent
	// ReliableUdpThread가 시작되었을 때 발생시키는 이벤트
	ReliableUdpThreadStartupEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (ReliableUdpThreadStartupEvent == NULL) {
		End();
		return FALSE;
	}

	// ReliableUdpThreadWakeUpEvent
	// ReliableUdpThread를 깨울 때 사용하는 이벤트
	ReliableUdpThreadWakeUpEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (ReliableUdpThreadWakeUpEvent == NULL) {
		End();
		return FALSE;
	}

	// ReliableUdpWriteCompleteEvent
	// ReliableUdpThread에서 데이터 하나를 보내고 완료했을 때 발생하는 이벤트
	ReliableUdpWriteCompleteEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (ReliableUdpWriteCompleteEvent == NULL) {
		End();
		return FALSE;
	}

	// ReliableUdpThread에서 사용할 큐를 초기화
	if (!ReliableWriteQueue.Begin()) {
		End();
		return FALSE;
	}

	// ReliableUdpThread를 생성
	DWORD ReliableUdpThreadID = 0;
	// 2번째는 DWORD WINAPI 스레드 함수 포인터, 3번째는 LPVOID형 스레드 함수 매개변수
	// this를 넣음으로써 클래스 포인터를 초기화한 후
	// 클래스내 ReliableUdpThreadCallback 함수를 사용
	ReliableUdpThreadHandle = CreateThread(NULL, 0, ::ReliableUdpThreadCallback,
											this, 0, &ReliableUdpThreadID);

	// 생성이 될 때까지 무한 대기
	WaitForSingleObject(ReliableUdpThreadStartupEvent, INFINITE);
	
	return TRUE;
}

// ReliableUDPThread 내부
VOID NetworkSession::ReliableUdpThreadCallback(VOID)
{
	DWORD EventID = 0;										// 시작 종료를 체크하는 이벤트 ID 값
	// ReliableUdpThread의 시작과 종료를 담당하는 이벤트
	HANDLE ThreadEvent[2] = { ReliableUdpThreadDestroyEvent, 
								ReliableUdpThreadWakeUpEvent };

	CHAR RemoteAddress[32]			= { 0, };				// 데이터를 보낼 주소
	USHORT RemotePort				= 0;					// 데이터를 보낼 포트
	BYTE Data[MAX_BUFFER_LENGTH]	= { 0, };				// 보낼 데이터
	DWORD DataLength				= 0;					// 보낼 데이터 길이
	VOID* Object = NULL;									// 오브젝트 미사용
	
	while(TRUE) {
		SetEvent(ReliableUdpThreadStartupEvent);			// ReliableUdpThread 시작을 알리는 이벤트
															// UdpBind의 wait를 종료시킴
		EventID = WaitForMultipleObjects(2, ThreadEvent,	// 이벤트 발생까지 무한 대기
										FALSE, INFINITE);

		switch (EventID) {
		// ReliableUdpThreadDestroyEvent 발생 시 종료
		case WAIT_OBJECT_0:		// 종료
			return;
		case WAIT_OBJECT_0 + 1:
		NEXT_DATA:
			// write일 경우 1개의 보낼 데이터를 Pop해주고
			if (ReliableWriteQueue.Pop(&Object, Data, DataLength, RemoteAddress, RemotePort)) {
				// 데이터가 있을 경우
				// 실제 Write를 해주고 WaitForSingleObject를 실행
				// 받았을 때 그 SetEvent를 해주면 풀림
			RETRY:
				// 실제로 데이터를 전송하는 함수
				if (!WriteTo2(RemoteAddress, RemotePort, Data, DataLength)) {
					return;
				}

				// 응답이 오기를 0.01초 동안 대기
				DWORD Result = WaitForSingleObject(ReliableUdpWriteCompleteEvent, 10);

				// 받았을 경우 다음 데이터 처리
				if (Result == WAIT_OBJECT_0)
					goto NEXT_DATA;
				// 오지 않았을 경우 데이터 재전송
				else
					goto RETRY;
			}
			else {
				// WriteTo 함수에서 현재 보내는 데이터 확인 FLAG 변수
				IsReliableUdpSending = FALSE;				// 큐에 더이상 보낼 데이터가 없을 경우
			}
			break;
		}
	}

}

BOOL NetworkSession::Listen(USHORT port, INT backLog)
{
	ThreadSync	Sync;		// 다중 스레드 동기화

	if (port <= 0 || backLog <= 0)	// 만약 포트가 0보다 작거나 backLog가 0보다 작으면 실패
		return FALSE;

	if (!Socket)
		return FALSE;

	// Listen을 할 정보를 저장하는 변수
	SOCKADDR_IN	ListenSocketInfo;
	ListenSocketInfo.sin_family				= AF_INET;
	ListenSocketInfo.sin_port				= htons(port);			// Listen할 포트
	ListenSocketInfo.sin_addr.S_un.S_addr	= htonl(INADDR_ANY);	// Listen할 주소는 전체 주소로 함

	// 위에서 정의한 주소 정보로 Bind
	if (bind(Socket, (struct sockaddr*)&ListenSocketInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		End();
		return FALSE;
	}

	// Listen 수행
	// backLog : 연결 대기큐의 개수
	// SOMAXCONN으로 설정하면 최대 값으로 설정
	if (listen(Socket, backLog) == SOCKET_ERROR) {
		End();
		return FALSE;
	}

	// LINGER : 소켓을 Close할 때 영향을 미치는 함수
	// 현재 보내고 있거나 받는 데이터가 있을 경우 그 데이터를 모두 보낸 후 소켓을 Close하게 하는 옵션
	// setsockopt 함수로 설정하고 l_onoff 값을 1로 초기화해 동작
	// 필요한 옵션이나 보내고 받는 데이터가 많은 경우 종료에 많은 시간이 걸릴 수 있으니 판단 잘해야 함
	LINGER Linger;
	Linger.l_onoff	= 1;
	Linger.l_linger = 0;

	if(setsockopt(Socket, SOL_SOCKET, SO_LINGER, (char*)&Linger, sizeof(LINGER)) == SOCKET_ERROR) {
		End();
		return FALSE;
	}

	return TRUE;
}

// 접속 함수
BOOL NetworkSession::Connect(LPSTR address, USHORT port)
// 여기서는 TCHAR 형태로 주소를 받지 않음
{
	ThreadSync	Sync;					// 다중 스레드 동기화
	if (!address || port <= 0)
		return FALSE;

	if (!Socket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo;		// 접속할 주소의 정보를 저장하는 구조체

	RemoteAddressInfo.sin_family			= AF_INET;
	RemoteAddressInfo.sin_port				= htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr	= inet_addr(address);

	// 위에서 설정된 주소로 접속을 시도
	// 여기서는 WSAConnect를 이용해서 접속했지만 일반 버클리 소켓 함수 사용 시
	// connect 함수를 이요해서 접속 가능
	if (WSAConnect(Socket, (LPSOCKADDR)&RemoteAddressInfo,
					sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
		// 실패이지만 대기상태라고 결과가 나왔을 경우엔 성공으로 인식
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			End();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL NetworkSession::Accept(SOCKET listenSocket)
{
	ThreadSync	Sync;					// 다중 스레드 동기화

	if (!listenSocket)					// Listen 먼저 실행되지 않으면 실행 불가
		return FALSE;

	if (Socket)
		return FALSE;

	// Accept에 사용할 소켓을 미리 생성
	Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (Socket == INVALID_SOCKET) {
		End();
		return FALSE;
	}

	// BOOL Nodelay = TRUE;
	//setsockopt(Socket, IPPROTO_TCP, TCP_NODELAY, (const char FAR*)&NoDelay, sizeof(NoDelay));
	
	// AcceptEx를 사용
	// 일반적으로 while문을 사용한 WSAAccept는 하나의 Accept가 완료되기 전에
	// 다른 연결에 대한 Accept를 할 수가 없음
	// 그래서 갑자기 많은 접속이 몰릴 경우 접속이 실패되는 경우가 발생
	// AcceptEx를 통해 AcceptEx당 하나의 소켓을 미리 만들어두고
	// ACcept 요청이 있을 때 만들어둔 소켓을 활용하는 방식
	// 이를 통해 여러 개의 AcceptEx를 호출해 놓으면 갑자기 많이 접속이 몰릴 경우를 대비 가능
	if (!AcceptEx(listenSocket,				// AcceptEx 함수를 사용하려면
											// mswsock.lib 라이브러리 참조
		Socket,
		ReadBuffer,							// 버퍼는 정의해 주나,
		0,									// 크기를 0으로 잡아서 받지 않음
		sizeof(sockaddr_in) + 16,			// 16 바이트 더 커야함
		sizeof(sockaddr_in) + 16,			// 16 바이트 더 커야함
		NULL,
		&AcceptOverlapped.Overlapped)) {	// AcceptOverlapped
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK) {
			End();
			return FALSE;
		}
	}

	return TRUE;
}

// IOCP 사용할 때 사용하는 초기 수신
BOOL NetworkSession::InitializeReadForIocp(VOID)
{
	ThreadSync	Sync;					// 다중 스레드 동기화
	
	if (!Socket)
		return FALSE;

	WSABUF			Wsabuf;								// WSABUF buf, len으로 구성
	DWORD			ReadBytes	= 0;
	DWORD			ReadFlag	= 0;

	Wsabuf.buf					= (CHAR*)ReadBuffer;	// 멤버로 선언된 ReadBuffer 포인터를 넘겨줌
	Wsabuf.len					= MAX_BUFFER_LENGTH;	// 최대 버퍼 길이

	// WSARecv를 호출
	INT ReturnValue = WSARecv(Socket,
		&Wsabuf,
		1,									// 버퍼 개수를 지정. WSABUF를 이용한 원형 버퍼를 사용할 경우
											// 1이상의 숫자를 입력
		&ReadBytes,
		&ReadFlag,
		&ReadOverlapped.Overlapped,			// ReadOverlapped를 사용
		NULL);

	// SOCKET_ERROR이지만 WSA_IO_PENDING 이거나 WSAEWOULDBLOCK이면 정상적으로 진행
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	return TRUE;
}

BOOL NetworkSession::InitializeReadForFromIocp(VOID)
{
	ThreadSync	Sync;					// 다중 스레드 동기화

	if (!Socket)
		return FALSE;

	WSABUF			Wsabuf;											// WSABUF buf, len으로 구성
	DWORD			ReadBytes = 0;
	DWORD			ReadFlag = 0;
	INT				RemoteAddressInfoSize = sizeof(UdpRemoteInfo);	// 데이터 받은 주소를 저장

	Wsabuf.buf = (CHAR*)ReadBuffer;	// 멤버로 선언된 ReadBuffer 포인터를 넘겨줌
	Wsabuf.len = MAX_BUFFER_LENGTH;	// 최대 버퍼 길이

	// WSARecvFrom을 호출
	INT ReturnValue = WSARecvFrom(Socket,
		&Wsabuf,
		1,					
		&ReadBytes,
		&ReadFlag,
		(SOCKADDR*)& UdpRemoteInfo,			// WSARecv와는 다른 파라미터
											// 데이터를 받은 주소 정보가 들어감
		&RemoteAddressInfoSize,				// 주소 정보 길이
		&ReadOverlapped.Overlapped,
		NULL
		);

	// SOCKET_ERROR이지만 WSA_IO_PENDING 이거나 WSAEWOULDBLOCK이면 정상적으로 진행
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	return TRUE;
}

// 받은 데이터 가져오기
BOOL NetworkSession::ReadForIocp(BYTE * data, DWORD & dataLength)
{
	ThreadSync	Sync;					// 다중 스레드 동기화

	if (!Socket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	// WSARecv를 통해서 받아온 데이터가 들어있는 ReadBuffer에서 데이터를 복제
	CopyMemory(data, ReadBuffer, dataLength);

	return TRUE;
}

// IOCP에서 실제로 받은 데이터 확인
BOOL NetworkSession::ReadFromForIocp(LPSTR remoteAddress, USHORT & remotePort, BYTE * data, DWORD & dataLength)
{
	ThreadSync	Sync;					// 다중 스레드 동기화

	if (!Socket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	// WSARecvFrom을 통해서 받아온 데이터가 들어있는 ReadBuffer에서 데이터를 복제
	CopyMemory(data, ReadBuffer, dataLength);

	strcpy(remoteAddress, inet_ntoa(UdpRemoteInfo.sin_addr));		// 데이터를 보낸 주소 확인
	remotePort = ntohs(UdpRemoteInfo.sin_port);						// 데이터를 보낸 포트 확인

	USHORT Ack = 0;
	// 받은 데이터의 앞부분 2바이트를 확인한 후
	CopyMemory(&Ack, ReadBuffer, sizeof(USHORT));

	if (Ack == 9999) {
		// 앞의 2바이트가 9999일 경우 데이터를 잘 받은 응답 패킷으로 인식
		return FALSE;
	}
	else {
		Ack = 9999;			// 새로운 데이터일 경우
		// 데이터를 보낸 주소로 2바이트를 전송
		WriteTo2(remoteAddress, remotePort, (BYTE*)&Ack, sizeof(USHORT));
	}

	return TRUE;
}

// EventSelect 방식을 사용할 때의 Read
// Iocp와의 차이는 즉시 받은 데이터를 읽기 가능 여뷰

// EventSelect 사용 시 데이터 도착 신호 발생 시,
// WSARecv를 호출하면 받은 데이터를 즉시 읽어올 수 있음

// 이에 비해 IOCP는 미리 WSARecv를 호출해 놓고 있지 않으면 
// 데이터를 받을 수 없으며 IOCP에서 신호가 왔을 때 WSARecv로 받는 것이 아닌
// 미리 호출해 놓았던 WSARecv의 버퍼에서 데이터를 확인할 수 있음
BOOL NetworkSession::ReadForEventSelect(BYTE * data, DWORD & dataLength)
{
	ThreadSync	Sync;					// 다중 스레드 동기화

	if (!Socket)
		return FALSE;
	
	if (!data)
		return FALSE;

	if (!Socket)
		return FALSE;

	WSABUF			Wsabuf;								// WSABUF buf, len으로 구성
	DWORD			ReadBytes	= 0;
	DWORD			ReadFlag	= 0;

	Wsabuf.buf					= (CHAR*)ReadBuffer;	// 멤버로 선언된 ReadBuffer 포인터를 넘겨줌
	Wsabuf.len					= MAX_BUFFER_LENGTH;	// 최대 버퍼 길이
														// WSARecv를 호출
	INT ReturnValue = WSARecv(Socket,
		&Wsabuf,
		1,									// 버퍼 개수를 지정. WSABUF를 이용한 원형 버퍼를 사용할 경우
											// 1이상의 숫자를 입력
		&ReadBytes,
		&ReadFlag,
		&ReadOverlapped.Overlapped,			// ReadOverlapped를 사용
		NULL);

	// SOCKET_ERROR이지만 WSA_IO_PENDING 이거나 WSAEWOULDBLOCK이면 정상적으로 진행
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	// 받은 내용을 복제
	CopyMemory(data, ReadBuffer, ReadBytes);
	// 받은 데이터 길이도 초기화
	dataLength = ReadBytes;

	return TRUE;
}

// EventSelect 방식을 사용할 때의 ReadFrom
BOOL NetworkSession::ReadFromForEventSelect(LPSTR remoteAddress, USHORT & remotePort, BYTE * data, DWORD & dataLength)
{
	ThreadSync	Sync;					// 다중 스레드 동기화

	if (!Socket)
		return FALSE;

	if (!data)
		return FALSE;

	if (!Socket)
		return FALSE;

	WSABUF			Wsabuf;											// WSABUF buf, len으로 구성
	DWORD			ReadBytes = 0;
	DWORD			ReadFlag = 0;
	INT				RemoteAddressInfoSize = sizeof(UdpRemoteInfo);	// 주소 데이터 길이

	Wsabuf.buf = (CHAR*)ReadBuffer;	// 멤버로 선언된 ReadBuffer 포인터를 넘겨줌
	Wsabuf.len = MAX_BUFFER_LENGTH;	// 최대 버퍼 길이
									// WSARecv를 호출
	INT ReturnValue = WSARecvFrom(Socket,
		&Wsabuf,
		1,									// 버퍼 개수를 지정. WSABUF를 이용한 원형 버퍼를 사용할 경우
											// 1이상의 숫자를 입력
		&ReadBytes,
		&ReadFlag,
		(SOCKADDR*)&UdpRemoteInfo,
		&RemoteAddressInfoSize,
		&ReadOverlapped.Overlapped,			// ReadOverlapped를 사용
		NULL);

	// SOCKET_ERROR이지만 WSA_IO_PENDING 이거나 WSAEWOULDBLOCK이면 정상적으로 진행
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	// IOCP와 달리 바로 결과가 나옴
	
	CopyMemory(data, ReadBuffer, ReadBytes);		// 데이터를 복사
	dataLength = ReadBytes;							// 파라미터로 데이터 길이를 받아옴

	// 주소 정보를 복사
	strcpy(remoteAddress, inet_ntoa(UdpRemoteInfo.sin_addr));
	remotePort = ntohs(UdpRemoteInfo.sin_port);

	// IOCP 쪽과 같은 ReliableUDP 코드

	USHORT Ack = 0;
	CopyMemory(&Ack, ReadBuffer, sizeof(USHORT));

	if (Ack == 9999) {
		// 앞의 2바이트가 9999일 경우 데이터를 잘 받은 응답 패킷으로 인식
		SetEvent(ReliableUdpWriteCompleteEvent);
		return FALSE;
	}
	else {
		Ack = 9999;			// 새로운 데이터일 경우
							// 데이터를 보낸 주소로 2바이트를 전송
		WriteTo2(remoteAddress, remotePort, (BYTE*)&Ack, sizeof(USHORT));
	}

	return TRUE;
}

// IOCP용과 EventSelect용으로 따로 구분하지 않음
// Write를 할때 넣었던 데이터 포인터를 보내기 완료가 될 때까지 살려두어야 함
// 그렇지 않으면 도중에 문제 발생 여부가 있음
// 대부분 이 부분을 처리하기 위해 Write한 데이터를 큐에 넣어두고
// 완료되었을 때 큐에서 데이터를 삭제하는 형태로 제작
BOOL NetworkSession::Write(BYTE * data, DWORD dataLength)
{
	ThreadSync	Sync;					// 다중 스레드 동기화

	if (!Socket)
		return FALSE;

	if (!data)
		return FALSE;

	if (!Socket)
		return FALSE;

	WSABUF			Wsabuf;								// write에서도 WSABUF를 사용
	DWORD			WriteBytes	= 0;
	DWORD			WriteFlag	= 0;

	Wsabuf.buf = (CHAR*)data;
	Wsabuf.len = dataLength;

	INT ReturnValue = WSASend(Socket,
		&Wsabuf,
		1,
		&WriteBytes,
		WriteFlag,
		&WriteOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	return TRUE;
}

// 사용자들이 호출하는 WriteTo 함수
BOOL NetworkSession::WriteTo(LPCSTR remoteAddress, USHORT remotePort, BYTE * data, DWORD dataLength)
{
	ThreadSync	Sync;		// 다중 스레드 동기화

	if (!Socket)
		return FALSE;

	if (!remoteAddress || remotePort <= 0 || !data || dataLength <= 0)
		return FALSE;

	// 큐에 데이터를 입력
	if (!ReliableWriteQueue.Push(this, data, dataLength, remoteAddress, remotePort))
		return FALSE;

	// 만약 현재 보내는 데이터가 없을 경우
	if (!IsReliableUdpSending) {
		// 보내는 데이터가 있다고 플래그를 바꿔주고 ReliableUDPThread를 꺠움
		IsReliableUdpSending = TRUE;
		SetEvent(ReliableUdpThreadWakeUpEvent);
	}

	return TRUE;
}

BOOL NetworkSession::WriteTo2(LPCSTR remoteAddress, USHORT remotePort, BYTE * data, DWORD dataLength)
{
	ThreadSync	Sync;					// 다중 스레드 동기화

	if (!Socket)
		return FALSE;

	if (!remoteAddress || remotePort <= 0 || !data || dataLength <= 0)
		return FALSE;

	WSABUF			Wsabuf;											// WSABUF buf, len으로 구성
	DWORD			WriteBytes = 0;
	DWORD			WriteFlag = 0;

	SOCKADDR_IN		RemoteAddressInfo;
	INT				RemoteAddressInfoSize = sizeof(UdpRemoteInfo);	// 주소 데이터 길이

	Wsabuf.buf = (CHAR*)data;
	Wsabuf.len = dataLength;

	// 보낼 데이터 주소를 입력
	RemoteAddressInfo.sin_family = AF_INET;
	RemoteAddressInfo.sin_addr.S_un.S_addr = inet_addr(remoteAddress);
	RemoteAddressInfo.sin_port = htons(remotePort);

	// WSASendTo 함수를 이용해서 전송
	INT ReturnValue = WSASendTo(Socket,
		&Wsabuf,
		1,
		&WriteBytes,
		WriteFlag,
		(SOCKADDR*)&RemoteAddressInfo,				// 보낼 주소 정보
		RemoteAddressInfoSize,						// 보낼 주소 정보 길이
		&WriteOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	return TRUE;
}

SOCKET NetworkSession::GetSocket(VOID)
{
	ThreadSync Sync;

	return Socket;
}
