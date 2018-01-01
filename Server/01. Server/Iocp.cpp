#include "stdafx.h"
#include "Iocp.h"

DWORD WINAPI WorkerThreadCallback(LPVOID parameter)
{
	Iocp* iocp = (Iocp*)parameter;
	iocp->WorkerThreadCallback();

	return 0;
}

Iocp::Iocp(VOID)
{
	IocpHandle			= NULL;		// IOCP�� �����ϴ� �ڵ� ��
	WorkerThreadCount	= 0;		// ����� WorkerThread ����

	StartupEventHandle	= NULL;		// ������ �����ϴ� �̺�Ʈ
}

Iocp::~Iocp(VOID)
{
}

// IOCP�� ����ϱ� ���� �⺻���� �غ�
BOOL Iocp::Begin(VOID)
{
	// IocpHandle �� �ʱ�ȭ
	IocpHandle = NULL;

	// ���� �ý����� ������ ������
	// ������ �ý��� �������� CPU �������� ���
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	// CPU ������ 2�踸ŭ�� WorkerThread ����
	WorkerThreadCount = SystemInfo.dwNumberOfProcessors * 2;
	// IocpHandle�� ����
	// ���� IOCP �ڵ��� �����̹Ƿ� ��� �Ķ���� ���� 0 �Ǵ� NULL
	// CreateIoCompletionPort : �⺻�� �Ǵ� ���� �ڵ� ���� �� ��� �Ķ���͸� 0�̳� NULL�� ����
	// 1���� CPU�� 2���� �����尡 ���� by MSDN -> ������ ������ ���� �ٲ㵵 ��
	// 1~2���� ����ڰ� �ִ��� ���� �۾��ؾ��ϴ� ��� ������ ������ ���� �ø��ų�
	// ���� ������ �������� ������ �޾ƾ��ϴ� ������ ��� �������� ������ ���� �ø�
	IocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// ������ �����ϸ� ����
	if (!IocpHandle)
		return FALSE;

	// ������ �����ϴ� �̺�Ʈ�� ����
	StartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (StartupEventHandle == NULL) {
		End();
		return FALSE;
	}

	// CPU ������ 2�踸ŭ�� WorkerThread�� �����ؼ� WorkerThreadVector�� ����
	for (DWORD i = 0; i < WorkerThreadCount; ++i) {
		HANDLE WorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
		WorkerThreadVector.push_back(WorkerThread);
		// �ϳ��� �����帶�� ������ �Ϸ�� ������ ���
		WaitForSingleObject(StartupEventHandle, INFINITE);
	}

	return TRUE;
}

BOOL Iocp::End(VOID)
{
	// IOCP�� �̿��ؼ� �� WorkerThread���� ���Ḧ ��û
	for (DWORD i = 0; i < WorkerThreadCount; ++i) {
		// PostQueuedCompletionStatus : IOCP�� ���������� ��ȣ�� ������ �Լ�
		// 1��° �Ķ���ʹ� ��ȣ�� ���� IOCP �ڵ��̰�
		// �ڿ� 3���� �Ķ���ʹ� ��ȣ�� ��Ƽ� ���� ������
		// �� End �Լ�����ó�� 0, 0, NULL�� �־ ������
		// WorkerThread������ ���� 0, 0, NULL�� ����� �ް� ��
		PostQueuedCompletionStatus(IocpHandle, 0, 0, NULL);
	}

	// �� WorkerThread�� ����Ǳ⸦ ��ٷ� �ڵ��� �ݾ���
	for (DWORD i = 0; i < WorkerThreadCount; ++i) {
		// �� WorkerThread�� ������ ����� ������ ���
		WaitForSingleObject(WorkerThreadVector[i], INFINITE);
		CloseHandle(WorkerThreadVector[i]);
	}
	
	// IOCP �ڵ鵵 ����
	if (IocpHandle)
		CloseHandle(IocpHandle);

	// WorkerThread�� �����ϴ� ���͸� ������
	WorkerThreadVector.clear();

	// ������ �����ϴ� �̺�Ʈ �ڵ� ����
	if (StartupEventHandle)
		CloseHandle(StartupEventHandle);

	return TRUE;
}

BOOL Iocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	
	// ���� �ڵ��̳� Ű�� �߸��Ǿ��� ��쿡�� ������� ����
	if (!socket || !completionKey)
		return FALSE;

	// Begin���� �����ߴ� IocpHandle�� �߰��� ���� �ڵ� �� Ű�� ���
	IocpHandle = CreateIoCompletionPort((HANDLE)socket, IocpHandle, completionKey, 0);

	if (!IocpHandle)
		return FALSE;

	return TRUE;
}

// GetQueuedCompletionStatus �Լ��� �̿��ؼ� ���� �Ķ������ ��� ���� �Ǵ��ϰ� ó��
VOID Iocp::WorkerThreadCallback(VOID)
{
	// ����, ���� ���θ� �Ǵ��ϴ� ����
	BOOL Successed					= FALSE;
	DWORD NumberOfByteTransfered	= 0;			// �� ����Ʈ�� IO�� �߻��ߴ��� Ȯ��
	VOID* CompletionKey				= NULL;			// Ű ��
	OVERLAPPED* Overlapped			= NULL;			// Overlapped ��
	OVERLAPPED_EX* OverlappedEx		= NULL;
	VOID* Object					= NULL;			// ���� �ڵ鿡 ��ġ�Ǵ� ��ü ������ ��

	while (TRUE) {
		// ���� WorkerThread�� ������ �� �ִٰ� �˷��ִ� �̺�Ʈ
		// Begin �Լ������� �� �̺�Ʈ�� �߻��ؾ߸� ���������� ���� ��ƾ���� �Ѿ
		SetEvent(StartupEventHandle);

		// PostQueuedCompletionStatus�� IOCP�� ��ϵ� ���� �� ������ Read, Write ���� �Լ���
		// ���¸� �޾ƿ��� �Լ�
		//
		// ��� IO �߻� �� �׻� Overlapped�� �Ѿ���� �Ǵµ�
		// �װ��� ��� �۾��� IO������ Ȯ���ϱ� ���ؼ� ������ Overlapped�� �����ؼ� ����ߴ� ��
		//
		// OverlappedEx->Object�� ��� � �����ڰ� ��ȣ�� �����Դ����� Ȯ���ϱ� ����
		Successed = GetQueuedCompletionStatus(
					IocpHandle,					// IOCP �ڵ� ��
					&NumberOfByteTransfered,	// IO�� �̿�� ������ ũ��
					(LPDWORD)&CompletionKey,	// �����̳� ������ �ڵ� ��
					&Overlapped,				// Read, Write � ���� Overlapped ��
					INFINITE);					// ��ȣ�� �߻��� ������ ���������� ��ٷ���

		// Ű�� NULL�� ��� ����. �̰��� End���� PostQueuedCompletionStatus��
		// Ű�� NULL�� �־��� ������ ����
		// �Ǵ� �ڵ��� ������ �����Ͱ� �߸� �Ѿ�� ��� �ɷ��ִ� ������ ��
		if (!CompletionKey)
			return;

		// Overlapped �����͸� �޾� OverlappedEx�� �����͸� Ȯ��
		OverlappedEx = (OVERLAPPED_EX*)Overlapped;
		// �� �κ��� NetworkSession���� WSARecv, WSASend ���� �� �� �Ѱ��� OVERLAPPED_EX��
		// �״�� ����. NetworkSession�� Begin �Լ��� ���ø� OverlappedEx->Object��
		// this�� �Է����� ��� ��ü���� Ȯ���� �� �ְ� �� ���� �� �� ����
		Object = OverlappedEx->Object;

		// GetQueuedCompletionStatus�� ����� �����̰� �������� IO�� ���� ũ�Ⱑ 0�� ���
		if (!Successed || (Successed && !NumberOfByteTransfered)) {
			// Ŭ���̾�Ʈ�� AcceptEx�� ���ؼ� Accept�� ����
			if (OverlappedEx->IoType == IO_ACCEPT)
				OnIoConnected(Object);
			// Ŭ���̾�Ʈ�� ������ ����� ���
			else
				OnIoDisconnected(Object);
			
			// �ٽ� ������ �ʱ�ȭ
			continue;
		}

		// �׸��� AcceptEx���� IO_TYPE�� IO_ACCEPT�� ������ OverlappedEx�� WSARecv����
		// IO_TYPE�� IO_READ�� ������ ���� WSASend���� IO_TYPE�� IO_WRITE�� ������
		// OverlappedEx�� �־��ִ� �͵� ���� ����
		switch (OverlappedEx->IoType)
		{
			// ������ ���� (WSARecv, WSARecvFrom)
		case IO_READ:
			OnIoRead(Object, NumberOfByteTransfered);
			break;
			// ������ �۽� (WSASend, WSASendTo)
		case IO_WRITE:
			OnIoWrote(Object, NumberOfByteTransfered);
			break;
		}
	}
}
