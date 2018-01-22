#include "stdafx.h"
#include "ServerIocp.h"

// KeepAlive Thread Callback 함수
DWORD WINAPI KeepThreadCallback(LPVOID parameter)
{
	ServerIocp* Owner = (ServerIocp*)parameter;

	Owner->KeepThreadCallback();

	return 0;
}

VOID ServerIocp::OnIoRead(VOID * object, DWORD dataLength)
{
	// 여기 가상 함수에서 넘어온 object는 OnIoConnected와 마찬가지로
	// 접속을 담당한 개체가 넘어오게 함
	// 이것을 여기서 클라이언트를 관리할 ConnectedSession으로 형 변환을 거쳐 받게 됨
	// 형 변환은 reinterpret_cast를 사용
	ConnectedSession* connectedSession = reinterpret_cast<ConnectedSession*>(object);

	// 받은 프로토콜과 패킷 길이를 저장하는 변수
	DWORD protocol = 0, packetLength = 0;

	// 패킷을 저장하는 변수
	BYTE packet[MAX_BUFFER_LENGTH] = { 0, };

	// NetworkSEssion에서 PacketSession으로 데이터를 가져옴
	if (connectedSession->ReadPacketForIocp(dataLength)) {
		// PacketSession에서 패킷을 뽑아냄
		while(connectedSession->GetPacket(protocol, packet, packetLength)) {
			// 프로토콜에 따른 switch 문
			switch (protocol) {
			// 사용자 등록 프로토콜일 경우
			case PT_REG_USER:
				PROC_PT_REG_USER(connectedSession, protocol, packet, packetLength);
				break;
			// 사용자 검색 프로토콜일 경우
			case PT_QUERY_USER:
				PROC_PT_QUERY_USER(connectedSession, protocol, packet, packetLength);
				break;
			// 컴퓨터 등록 프로토콜일 경우
			case PT_REG_COMPUTER:
				PROC_PT_REG_COMPUTER(connectedSession, protocol, packet, packetLength);
				break;
			// 컴퓨터 검색 프로토콜일 경우
			case PT_QUERY_COMPUTER:
				PROC_PT_QUERY_COMPUTER(connectedSession, protocol, packet, packetLength);
				break;
			// 프로그램 등록 프로토콜일 경우
			case PT_REG_PROGRAM:
				PROC_PT_REG_PROGRAM(connectedSession, protocol, packet, packetLength);
				break;
			// 프로그램 검색 프로토콜일 경우
			case PT_QUERY_PROGRAM:
				PROC_PT_QUERY_PROGRAM(connectedSession, protocol, packet, packetLength);
				break;

			}
		}
	}
	if (!connectedSession->InitializeReadForIocp())
		connectedSession->Restart(listen->GetSocket());
}

VOID ServerIocp::OnIoWrote(VOID * Object, DWORD dataLength)
{

}

VOID ServerIocp::OnIoConnected(VOID * object)
{
	// 여기 가상 함수에서 넘어온 object는 접속을 담당한 개체가 넘어옴
	// 이것을 여기서 클라이언트를 관리할 ConnectedSession으로 형변환을 하여 받게 됨
	// 형 변환은 reinterpret_cast를 사용
	ConnectedSession* connectedSession = reinterpret_cast<ConnectedSession*>(object);

	// 접속한 개체의 IO를 IOCP를 통해서 받기 위해서 IOCP에 등록하는 과정을 거침
	// 여기서 GetSocket을 이용해서 소켓을 등록하고 해당 키는 개체의 포인터를 이용
	if (!Iocp::RegisterSocketToIocp(connectedSession->GetSocket(),
		reinterpret_cast<ULONG_PTR>(object)))
		return;

	// IOCP 초기 받기를 실행
	if (!connectedSession->InitializeReadForIocp()) {
		// 만일 실패했을 경우 개체를 재시작
		connectedSession->Restart(listen->GetSocket());
		return;
	}

	connectedSession->SetConnected(TRUE);
}

VOID ServerIocp::OnIoDisconnected(VOID * object)
{
	// 여기 가상함수에서 넘어온 object는 OnIoConnected와
	// 마찬가지로 접속을 담당한 개체가 넘어옴
	// 이것을 여기서 클라이언트를 관리할 ConnectedSession으로 형 변환을 해주어 받게 됨
	// 마찬가지로 reinterpret_cast 사용
	ConnectedSession* connectedSession = reinterpret_cast<ConnectedSession*>(object);

	// 접속을 종료하였기 때문에 개체를 재시작
	connectedSession->Restart(listen->GetSocket());

	connectedSession->SetConnected(FALSE);
}

BOOL ServerIocp::Begin(VOID)
{
	// 상속받은 IOCP 개체 시작
	if (!Iocp::Begin()) return FALSE;

	// Listen에 사용할 개체 생성
	listen = new NetworkSession();

	// 개체 시작
	if (!listen->Begin()){
		// 실패 시 End 함수를 호출하고 종료
		ServerIocp::End();
		return FALSE;
	}

	// TCP 사용 선언
	if (!listen->TcpBind()) {
		// 실패 시 End 함수를 호출하고 종료
		ServerIocp::End();
		return FALSE;
	}

	// 포트 1820로 Listen하고 최대 100명으로 설정
	if(!listen->Listen(DEFAULT_PORT, MAX_USER)) {
		// 실패 시 End 함수를 호출하고 종료
		ServerIocp::End();
		return FALSE;
	}

	// IOCP에 Listen 소켓을 등록. 그리고 키로는 해당 개체의 포인터를 설정
	if (!Iocp::RegisterSocketToIocp(listen->GetSocket(), reinterpret_cast<ULONG_PTR>(listen))) {
		// 실패 시 End 함수를 호출하고 종료
		ServerIocp::End();
		return FALSE;
	}

	// ConnectedSessionManager를 시작
	// 시작 함수 내에는 ConnectedSession을 생성하고 Accept 상태로 만드는 코드가 추가되어 있음
	if (!connectedSessionManager.Begin(listen->GetSocket())) {
		ServerIocp::End();
		return FALSE;
	}

	// KeepAlive 종료 이벤트 생성
	keepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!keepThreadDestroyEvent) {
		// 생성 실패 시 종료
		ServerIocp::End();
		return FALSE;
	}

	// KeepAlive Thread를 생성
	keepThread = CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!keepThread) {
		// 생성 실패 시 종료
		ServerIocp::End();
		return FALSE;
	}

	return TRUE;
}

BOOL ServerIocp::End(VOID)
{
	// ServerIocp 종료 시

	// UserMap 삭제
	for (auto iter = UserMap.begin(); iter != UserMap.end(); ++iter) {
		delete iter->second;
		iter->second = NULL;
	}
	UserMap.clear();

	// ComputerMap 삭제
	for (auto iter = ComputerMap.begin(); iter != ComputerMap.end(); ++iter) {
		delete iter->second;
		iter->second = NULL;
	}
	ComputerMap.clear();

	// ProgramMap 삭제
	for (auto iter = ProgramMap.begin(); iter != ProgramMap.end(); ++iter) {
		delete iter->second;
		iter->second = NULL;
	}
	ProgramMap.clear();

	// keepThread가 NULL이 아니면 KeepAlive 스레드를 종료
	if (keepThread) {
		// 종료 이벤트 발생
		SetEvent(keepThreadDestroyEvent);

		// Thread가 종료될 때까지 대기
		WaitForSingleObject(keepThread, INFINITE);

		// 핸들을 닫아줌
		CloseHandle(keepThread);
		keepThread = NULL;
	}

	// KeepAlive 종료 이벤트가 NULL이 아니면
	if (keepThreadDestroyEvent) {
		// 이벤트를 닫아줌
		CloseHandle(keepThreadDestroyEvent);
		keepThreadDestroyEvent = NULL;
	}

	// Iocp를 종료
	Iocp::End();

	// ConnectedSessionManager를 종료
	connectedSessionManager.End();

	// 생성했던 Listen용 개체를 종료하고 삭제
	if (listen) {
		listen->End();
		delete listen;
	}

	return TRUE;
}

VOID ServerIocp::KeepThreadCallback(VOID)
{
	// KeepAlive 신호
	DWORD keepAlive = 0xFFFF;

	while (TRUE) {
		// 30초동안 종료 이벤트가 발생하지 않으면 넘어감
		DWORD result = WaitForSingleObject(keepThreadDestroyEvent, 30000);

		// 종료 이벤트 발생 시 Thread 종료
		if (result == WAIT_OBJECT_0) return;

		// 접속해 있는 모든 Session에 패킷을 전송
		connectedSessionManager.WriteAll(0x3000000, (BYTE*)&keepAlive, sizeof(DWORD));
	}
}

ServerIocp::ServerIocp(VOID)
{
}


ServerIocp::~ServerIocp(VOID)
{
}
