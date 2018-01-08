/**
* ��Ŷ�� ���� -> ������ ����(DWORD), ��������(DWORD), UN(��Ŷ ���� ��ȣ)(DWORD), ������(4084 ����Ʈ) �� 4096 ����Ʈ
* UN : ��ŷ �����̳� UDP�� Reliable ��⿡�� ���� ��Ŷ�� �ɷ��ִ� ����
* �����͸��� �����ϴ� NetworkSession Ŭ������ ��ӹ޾� ��Ŷ ����� �߰�
*/
#pragma once
#include "NetworkSession.h"
#include "Crypt.h"

typedef struct _READ_PACKET_INFO
{
	CHAR	RemoteAddress[14];		// �����͸� ���� �ּ�
	USHORT	RemotePort;				// �����͸� ���� ��Ʈ
	DWORD	PacketNumber;			// ���� ��Ŷ ��ȣ
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

