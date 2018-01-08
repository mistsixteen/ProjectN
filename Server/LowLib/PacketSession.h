/**
* 패킷의 구조 -> 데이터 길이(DWORD), 프로토콜(DWORD), UN(패킷 고유 번호)(DWORD), 데이터(4084 바이트) 총 4096 바이트
* UN : 해킹 검증이나 UDP의 Reliable 모듈에서 같은 패킷을 걸러주는 역할
* 데이터만을 관리하는 NetworkSession 클래스를 상속받아 패킷 기능을 추가
*/
#pragma once
#include "NetworkSession.h"
#include "Crypt.h"

typedef struct _READ_PACKET_INFO
{
	CHAR	RemoteAddress[14];		// 데이터를 받은 주소
	USHORT	RemotePort;				// 데이터를 받은 포트
	DWORD	PacketNumber;			// 받은 패킷 번호
} READ_PACKET_INFO;

class PacketSession :
	public NetworkSession
{
private:
	BYTE							PacketBuffer[MAX_BUFFER_LENGTH * 3];
	INT								RemainLength;
	DWORD							CurrentPacketNumber;

	DWORD							LastReadPacketNumber;

	CircularQueue					WriteQueue;

	vector<READ_PACKET_INFO>		LastReadPacketInfoVectorForUdp;
public:
	BOOL Begin(VOID);
	BOOL End(VOID);

	BOOL WriteComplete(VOID);

	BOOL GetPacket(DWORD& protocol, BYTE* packet, DWORD& packetLength);
	BOOL GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD& protocol,
					BYTE* packet, DWORD& packetLength);

	BOOL ReadPacketForIocp(DWORD readLength);
	BOOL WritePacket(DWORD protocol, const BYTE* packet, DWORD packetLength);

	BOOL ResetUdp(VOID);
public:
	PacketSession(VOID);
	virtual ~PacketSession(VOID);
};

