#pragma once
#include "EventSelect.h"

#include "PacketSession.h"
#include "CircularQueue.h"

class ClientSession :
	public EventSelect
{
private:
	// 통신을 담당하는 Session 개체
	PacketSession			Session;
	// 받은 패킷을 저장하고 있는 큐
	CircularQueue			ReadPacketQueue;
	// 현재 UDP로 사용 중인지 확인하는 변수
	BOOL					IsUdp;
public:
	BOOL BeginTcp(LPSTR remoteAddress, USHORT remotePort);
	BOOL BeginUdp(USHORT remotePort);

	BOOL End(VOID);

	BOOL WritePacket(DWORD protocol, const BYTE* packet, DWORD packetLength);
	BOOL ReadPacket(DWORD& protocol, BYTE* packet, DWORD& packetLength);
protected:
	virtual VOID OnIoRead(VOID);
public:
	ClientSession(VOID);
	virtual ~ClientSession(VOID);
};

