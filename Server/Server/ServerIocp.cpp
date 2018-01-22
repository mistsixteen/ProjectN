#include "stdafx.h"
#include "ServerIocp.h"

// KeepAlive Thread Callback �Լ�
DWORD WINAPI KeepThreadCallback(LPVOID parameter)
{
	ServerIocp* Owner = (ServerIocp*)parameter;

	Owner->KeepThreadCallback();

	return 0;
}

VOID ServerIocp::OnIoRead(VOID * object, DWORD dataLength)
{
	// ���� ���� �Լ����� �Ѿ�� object�� OnIoConnected�� ����������
	// ������ ����� ��ü�� �Ѿ���� ��
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ ConnectedSession���� �� ��ȯ�� ���� �ް� ��
	// �� ��ȯ�� reinterpret_cast�� ���
	ConnectedSession* connectedSession = reinterpret_cast<ConnectedSession*>(object);

	// ���� �������ݰ� ��Ŷ ���̸� �����ϴ� ����
	DWORD protocol = 0, packetLength = 0;

	// ��Ŷ�� �����ϴ� ����
	BYTE packet[MAX_BUFFER_LENGTH] = { 0, };

	// NetworkSEssion���� PacketSession���� �����͸� ������
	if (connectedSession->ReadPacketForIocp(dataLength)) {
		// PacketSession���� ��Ŷ�� �̾Ƴ�
		while(connectedSession->GetPacket(protocol, packet, packetLength)) {
			// �������ݿ� ���� switch ��
			switch (protocol) {
			// ����� ��� ���������� ���
			case PT_REG_USER:
				PROC_PT_REG_USER(connectedSession, protocol, packet, packetLength);
				break;
			// ����� �˻� ���������� ���
			case PT_QUERY_USER:
				PROC_PT_QUERY_USER(connectedSession, protocol, packet, packetLength);
				break;
			// ��ǻ�� ��� ���������� ���
			case PT_REG_COMPUTER:
				PROC_PT_REG_COMPUTER(connectedSession, protocol, packet, packetLength);
				break;
			// ��ǻ�� �˻� ���������� ���
			case PT_QUERY_COMPUTER:
				PROC_PT_QUERY_COMPUTER(connectedSession, protocol, packet, packetLength);
				break;
			// ���α׷� ��� ���������� ���
			case PT_REG_PROGRAM:
				PROC_PT_REG_PROGRAM(connectedSession, protocol, packet, packetLength);
				break;
			// ���α׷� �˻� ���������� ���
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
	// ���� ���� �Լ����� �Ѿ�� object�� ������ ����� ��ü�� �Ѿ��
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ ConnectedSession���� ����ȯ�� �Ͽ� �ް� ��
	// �� ��ȯ�� reinterpret_cast�� ���
	ConnectedSession* connectedSession = reinterpret_cast<ConnectedSession*>(object);

	// ������ ��ü�� IO�� IOCP�� ���ؼ� �ޱ� ���ؼ� IOCP�� ����ϴ� ������ ��ħ
	// ���⼭ GetSocket�� �̿��ؼ� ������ ����ϰ� �ش� Ű�� ��ü�� �����͸� �̿�
	if (!Iocp::RegisterSocketToIocp(connectedSession->GetSocket(),
		reinterpret_cast<ULONG_PTR>(object)))
		return;

	// IOCP �ʱ� �ޱ⸦ ����
	if (!connectedSession->InitializeReadForIocp()) {
		// ���� �������� ��� ��ü�� �����
		connectedSession->Restart(listen->GetSocket());
		return;
	}

	connectedSession->SetConnected(TRUE);
}

VOID ServerIocp::OnIoDisconnected(VOID * object)
{
	// ���� �����Լ����� �Ѿ�� object�� OnIoConnected��
	// ���������� ������ ����� ��ü�� �Ѿ��
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ ConnectedSession���� �� ��ȯ�� ���־� �ް� ��
	// ���������� reinterpret_cast ���
	ConnectedSession* connectedSession = reinterpret_cast<ConnectedSession*>(object);

	// ������ �����Ͽ��� ������ ��ü�� �����
	connectedSession->Restart(listen->GetSocket());

	connectedSession->SetConnected(FALSE);
}

BOOL ServerIocp::Begin(VOID)
{
	// ��ӹ��� IOCP ��ü ����
	if (!Iocp::Begin()) return FALSE;

	// Listen�� ����� ��ü ����
	listen = new NetworkSession();

	// ��ü ����
	if (!listen->Begin()){
		// ���� �� End �Լ��� ȣ���ϰ� ����
		ServerIocp::End();
		return FALSE;
	}

	// TCP ��� ����
	if (!listen->TcpBind()) {
		// ���� �� End �Լ��� ȣ���ϰ� ����
		ServerIocp::End();
		return FALSE;
	}

	// ��Ʈ 1820�� Listen�ϰ� �ִ� 100������ ����
	if(!listen->Listen(DEFAULT_PORT, MAX_USER)) {
		// ���� �� End �Լ��� ȣ���ϰ� ����
		ServerIocp::End();
		return FALSE;
	}

	// IOCP�� Listen ������ ���. �׸��� Ű�δ� �ش� ��ü�� �����͸� ����
	if (!Iocp::RegisterSocketToIocp(listen->GetSocket(), reinterpret_cast<ULONG_PTR>(listen))) {
		// ���� �� End �Լ��� ȣ���ϰ� ����
		ServerIocp::End();
		return FALSE;
	}

	// ConnectedSessionManager�� ����
	// ���� �Լ� ������ ConnectedSession�� �����ϰ� Accept ���·� ����� �ڵ尡 �߰��Ǿ� ����
	if (!connectedSessionManager.Begin(listen->GetSocket())) {
		ServerIocp::End();
		return FALSE;
	}

	// KeepAlive ���� �̺�Ʈ ����
	keepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!keepThreadDestroyEvent) {
		// ���� ���� �� ����
		ServerIocp::End();
		return FALSE;
	}

	// KeepAlive Thread�� ����
	keepThread = CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!keepThread) {
		// ���� ���� �� ����
		ServerIocp::End();
		return FALSE;
	}

	return TRUE;
}

BOOL ServerIocp::End(VOID)
{
	// ServerIocp ���� ��

	// UserMap ����
	for (auto iter = UserMap.begin(); iter != UserMap.end(); ++iter) {
		delete iter->second;
		iter->second = NULL;
	}
	UserMap.clear();

	// ComputerMap ����
	for (auto iter = ComputerMap.begin(); iter != ComputerMap.end(); ++iter) {
		delete iter->second;
		iter->second = NULL;
	}
	ComputerMap.clear();

	// ProgramMap ����
	for (auto iter = ProgramMap.begin(); iter != ProgramMap.end(); ++iter) {
		delete iter->second;
		iter->second = NULL;
	}
	ProgramMap.clear();

	// keepThread�� NULL�� �ƴϸ� KeepAlive �����带 ����
	if (keepThread) {
		// ���� �̺�Ʈ �߻�
		SetEvent(keepThreadDestroyEvent);

		// Thread�� ����� ������ ���
		WaitForSingleObject(keepThread, INFINITE);

		// �ڵ��� �ݾ���
		CloseHandle(keepThread);
		keepThread = NULL;
	}

	// KeepAlive ���� �̺�Ʈ�� NULL�� �ƴϸ�
	if (keepThreadDestroyEvent) {
		// �̺�Ʈ�� �ݾ���
		CloseHandle(keepThreadDestroyEvent);
		keepThreadDestroyEvent = NULL;
	}

	// Iocp�� ����
	Iocp::End();

	// ConnectedSessionManager�� ����
	connectedSessionManager.End();

	// �����ߴ� Listen�� ��ü�� �����ϰ� ����
	if (listen) {
		listen->End();
		delete listen;
	}

	return TRUE;
}

VOID ServerIocp::KeepThreadCallback(VOID)
{
	// KeepAlive ��ȣ
	DWORD keepAlive = 0xFFFF;

	while (TRUE) {
		// 30�ʵ��� ���� �̺�Ʈ�� �߻����� ������ �Ѿ
		DWORD result = WaitForSingleObject(keepThreadDestroyEvent, 30000);

		// ���� �̺�Ʈ �߻� �� Thread ����
		if (result == WAIT_OBJECT_0) return;

		// ������ �ִ� ��� Session�� ��Ŷ�� ����
		connectedSessionManager.WriteAll(0x3000000, (BYTE*)&keepAlive, sizeof(DWORD));
	}
}

ServerIocp::ServerIocp(VOID)
{
}


ServerIocp::~ServerIocp(VOID)
{
}
