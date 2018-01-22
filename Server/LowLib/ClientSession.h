#pragma once
#include "EventSelect.h"

#include "PacketSession.h"
#include "CircularQueue.h"

class ClientSession :
	public EventSelect
{
private:
	// ����� ����ϴ� Session ��ü
	PacketSession			Session;
	// ���� ��Ŷ�� �����ϰ� �ִ� ť
	CircularQueue			ReadPacketQueue;
	// ���� UDP�� ��� ������ Ȯ���ϴ� ����
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

