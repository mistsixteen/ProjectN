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
