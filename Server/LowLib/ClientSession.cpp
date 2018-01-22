#include "stdafx.h"
#include "ClientSession.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "EventSelect.h"
#include "ClientSession.h"

// TCP�� ����� �� ����ϴ� �Լ�
// ������ ��� �ּҿ� ��Ʈ�� �Ķ���ͷ� �Է�
BOOL ClientSession::BeginTcp(LPSTR remoteAddress, USHORT remotePort)
{
	// �������� �Ķ�������� üũ
	if (!remoteAddress || remotePort <= 0)
		return FALSE;

	// ����� ���� ��ü�� �ʱ�ȭ
	// �����δ� PacketSession Begin �Լ��� ȣ��
	if (!Session.Begin()) {
		End();
		return FALSE;
	}

	// ���� ��ü�� TCP�� ����
	if(!Session.TcpBind()){
		End();
		return FALSE;
	}

	// EventSelect ������� �ش� ������ �̺�Ʈ�� �޾ƿ��� ���� ���
	if (!EventSelect::Begin(Session.GetSocket())) {
		End();
		return FALSE;
	}

	// ������ �ּҿ� ����
	if (!Session.Connect(remoteAddress, remotePort)) {
		End();
		return FALSE;
	}

	return TRUE;
}

// UDP�� ����� �� �ʱ�ȭ�ϴ� �Լ�
// TCP�� �޸� ����� ��Ʈ�� �Է�
BOOL ClientSession::BeginUdp(USHORT remotePort)
{
	// �Ķ���Ͱ� �������� üũ
	if (remotePort <= 0) {
		return FALSE;
	}

	// ����� ��ü�� �ʱ�ȭ
	if (!Session.Begin()) {
		End();
		return FALSE;
	}

	// UDP�� ����� ���� ����
	if (!Session.UdpBind(remotePort)) {
		End();
		return FALSE;
	}

	// EventSelect�� ���
	if (!EventSelect::Begin(Session.GetSocket())) {
		End();
		return FALSE;
	}

	// UDP�� ����� ���� ����
	IsUdp = TRUE;

	return TRUE;
}

BOOL ClientSession::End(VOID)
{
	// ���� ��ü�� ����
	Session.End();

	// EventSelect ��ü�� ����
	EventSelect::End();

	return TRUE;
}

// TCP ������ ���� �Լ�
// �Ķ���ͷ� ����� ��������, ��Ŷ, ��Ŷ ���̸� �ְ� ��
BOOL ClientSession::WritePacket(DWORD protocol, const BYTE * packet, DWORD packetLength)
{
	// ������ ���� �Լ��� ȣ��
	if (!Session.WritePacket(protocol, packet, packetLength)) {
		return FALSE;
	}

	// �ٷ� WriteComplete�� ����
	// �ֳ��ϸ� EventSelect�� ��� FD_WRITE�� �� ������ ���� ���� �߻�����
	// ���� ���� �ֱ� ����
	if (!Session.WriteComplete())
		return FALSE;

	return TRUE;
}

// ��Ŷ �ϳ��� �޾ƿ�
BOOL ClientSession::ReadPacket(DWORD & protocol, BYTE * packet, DWORD & packetLength)
{
	VOID* Object = NULL;

	// ť���� ��Ŷ �ϳ��� �޾ƿ�
	return ReadPacketQueue.Pop(&Object, protocol, packet, packetLength);
}

// �����Ͱ� �߻����� �� ȣ��Ǵ� ���� �Լ�
VOID ClientSession::OnIoRead(VOID)
{
	// ��Ŷ ���� ����
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	// ��Ŷ�� ���̸� �����ϴ� ����
	DWORD PacketLength = 0;
	// �������� ���� ����
	DWORD Protocol = 0;

	// UDP�� ���� TCP�� ��츦 ����
	if (IsUdp) {
		// UDP�� ��쿡�� ���� �ּҿ� ��Ʈ ������ �޾ƿ;� �ϱ� ������ ���� ������ ����
		CHAR RemoteAddress[32]	= { 0, };
		USHORT RemotePort		= 0;

		// NetworkSession���� PacketSession���� �����͸� ����
		if (Session.ReadFromPacketForEventSelect(RemoteAddress, RemotePort)) {
			// ��Ŷ�� ������
			while (Session.GetPacket(RemoteAddress, RemotePort, Protocol, Packet, PacketLength)) {
				// ������ ��Ŷ�� ť�� ����
				ReadPacketQueue.Push(this, Protocol, Packet, PacketLength,
									RemoteAddress, RemotePort);
			}
		}
	}
	else {
		// NetworkSession���� PacketSession���� ������ ����
		if (Session.ReadPacketForEventSelect()) {
			// ��Ŷ�� ���� ��
			while (Session.GetPacket(Protocol, Packet, PacketLength))
				// ť�� ����
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
