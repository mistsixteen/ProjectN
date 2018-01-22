#include "stdafx.h"
#include "ClientSession.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "EventSelect.h"
#include "ClientSession.h"

// TCP로 상요할 때 사용하는 함수
// 접속할 상대 주소와 포트를 파라미터로 입력
BOOL ClientSession::BeginTcp(LPSTR remoteAddress, USHORT remotePort)
{
	// 정상적인 파라미터인지 체크
	if (!remoteAddress || remotePort <= 0)
		return FALSE;

	// 사용할 접속 개체를 초기화
	// 실제로는 PacketSession Begin 함수를 호출
	if (!Session.Begin()) {
		End();
		return FALSE;
	}

	// 접속 개체를 TCP로 선언
	if(!Session.TcpBind()){
		End();
		return FALSE;
	}

	// EventSelect 방식으로 해당 소켓의 이벤트를 받아오는 것을 등록
	if (!EventSelect::Begin(Session.GetSocket())) {
		End();
		return FALSE;
	}

	// 접속할 주소에 접속
	if (!Session.Connect(remoteAddress, remotePort)) {
		End();
		return FALSE;
	}

	return TRUE;
}

// UDP로 사용할 때 초기화하는 함수
// TCP와 달리 사용할 포트만 입력
BOOL ClientSession::BeginUdp(USHORT remotePort)
{
	// 파라미터가 정상인지 체크
	if (remotePort <= 0) {
		return FALSE;
	}

	// 사용할 개체를 초기화
	if (!Session.Begin()) {
		End();
		return FALSE;
	}

	// UDP로 사용할 것을 선언
	if (!Session.UdpBind(remotePort)) {
		End();
		return FALSE;
	}

	// EventSelect에 등록
	if (!EventSelect::Begin(Session.GetSocket())) {
		End();
		return FALSE;
	}

	// UDP로 사용할 것을 저장
	IsUdp = TRUE;

	return TRUE;
}

BOOL ClientSession::End(VOID)
{
	// 연결 개체를 종료
	Session.End();

	// EventSelect 개체를 종료
	EventSelect::End();

	return TRUE;
}

// TCP 데이터 전송 함수
// 파라미터로 사용할 프로토콜, 패킷, 패킷 길이를 넣게 됨
BOOL ClientSession::WritePacket(DWORD protocol, const BYTE * packet, DWORD packetLength)
{
	// 데이터 전송 함수를 호출
	if (!Session.WritePacket(protocol, packet, packetLength)) {
		return FALSE;
	}

	// 바로 WriteComplete를 해줌
	// 왜냐하면 EventSelect의 경우 FD_WRITE가 꼭 보내기 성공 이후 발생하지
	// 않을 수도 있기 때문
	if (!Session.WriteComplete())
		return FALSE;

	return TRUE;
}

// 패킷 하나를 받아옴
BOOL ClientSession::ReadPacket(DWORD & protocol, BYTE * packet, DWORD & packetLength)
{
	VOID* Object = NULL;

	// 큐에서 패킷 하나를 받아옴
	return ReadPacketQueue.Pop(&Object, protocol, packet, packetLength);
}

// 데이터가 발생했을 때 호출되는 가상 함수
VOID ClientSession::OnIoRead(VOID)
{
	// 패킷 저장 버퍼
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	// 패킷의 길이를 저장하는 변수
	DWORD PacketLength = 0;
	// 프로토콜 저장 변수
	DWORD Protocol = 0;

	// UDP일 경우와 TCP일 경우를 구분
	if (IsUdp) {
		// UDP일 경우에는 보낸 주소와 포트 정보를 받아와야 하기 떄문에 따로 변수로 지정
		CHAR RemoteAddress[32]	= { 0, };
		USHORT RemotePort		= 0;

		// NetworkSession에서 PacketSession으로 데이터를 복제
		if (Session.ReadFromPacketForEventSelect(RemoteAddress, RemotePort)) {
			// 패킷을 가져옴
			while (Session.GetPacket(RemoteAddress, RemotePort, Protocol, Packet, PacketLength)) {
				// 가져온 패킷을 큐에 저장
				ReadPacketQueue.Push(this, Protocol, Packet, PacketLength,
									RemoteAddress, RemotePort);
			}
		}
	}
	else {
		// NetworkSession에서 PacketSession으로 데이터 복제
		if (Session.ReadPacketForEventSelect()) {
			// 패킷을 뽑은 후
			while (Session.GetPacket(Protocol, Packet, PacketLength))
				// 큐에 저장
				ReadPacketQueue.Push(this, Protocol, Packet, PacketLength);
		}
	}
}

ClientSession::ClientSession(VOID)
{
}


ClientSession::~ClientSession(VOID)
{
}
