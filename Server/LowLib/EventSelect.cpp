#include "stdafx.h"
#include "EventSelect.h"

DWORD WINAPI SelectThreadCallback(LPVOID parameter)
{
	EventSelect* eventSelect = (EventSelect*)parameter;
	eventSelect->SelectThreadCallback();

	return 0;
}

EventSelect::EventSelect(VOID)
{
	SelectEventHandle	= NULL;		// ���� �̺�Ʈ ����
	StartupEventHandle	= NULL;		// ������ �̺��� �����ϴ� ������ ������ üũ
	DestroyEventHandle	= NULL;		// ������ ���� �� ���
	SelectThreadHandle	= NULL;		// ���� �̺�Ʈ�� �����ϴ� ������ �ڵ� ��

	Socket				= NULL;		// ����� ���� �ڵ�
}

EventSelect::~EventSelect(VOID)
{
}

// ������ �� ���� �ڵ� ���� �޾ƿ��µ� �̰��� NetworkSession�� GetSocket�� ���� Ȯ�� ����
// ���� �̺�Ʈ�� ������ ������ ������ �� �۾�
// WSAEventSelect ���� ���� WSAEventAsyncSelect �Լ��� ������
// ���� �̺�Ʈ�� �������� â�� �̺�Ʈ �ڵ鷯�� �̿��ϱ� ������
// �������� �������� â�� �ϳ� �����ؾ��ϴ� ���ҽ� ���� ����
BOOL EventSelect::Begin(SOCKET socket)
{
	// ���� �Ķ���Ͱ� �߸��Ǿ��� ��� �������� ����
	if (!socket)
		return FALSE;

	// ��� ���� ������ �̹� NULL�� �ƴ� ��� �ߺ� �������� �����ϰ� �������� ����
	if (Socket)
		return FALSE;

	// ��� ������ �Է�
	Socket = socket;

	SelectEventHandle = WSACreateEvent();		// ���� �̺�Ʈ�� ������ �̺�Ʈ�� ����
	if (SelectEventHandle == WSA_INVALID_EVENT) {
		End();
		return FALSE;
	}

	DestroyEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	// ������ ���� �̺�Ʈ�� ����
	if (DestroyEventHandle == NULL) {
		End();
		return FALSE;
	}

	StartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	// ������ ���� �̺�Ʈ�� ����
	if (StartupEventHandle == NULL) {
		End();
		return FALSE;
	}

	// Connect, Read, Write, Close �̺�Ʈ�� �����ϴ� ������ Ȯ��
	// WSAEventSelect �� 3��° �Ű����� : � �̺�Ʈ�� �������� ���
	DWORD Result = WSAEventSelect(socket, SelectEventHandle, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	if (Result == SOCKET_ERROR) {
		End();
		return FALSE;
	}

	// ���� �̺�Ʈ�� ������ �����带 ����
	DWORD SelectThreadID = 0;
	SelectThreadHandle = CreateThread(NULL, 0, ::SelectThreadCallback, this, 0, &SelectThreadID);
	if (!SelectThreadHandle) {
		End();
		return FALSE;
	}

	// �����尡 ���� �Ϸ�� ������ ���
	// �����尡 Wait�� ������ �ð��� ������
	// �����尡 �����Ǹ鼭 StartupEventHandle �̺�Ʈ �߻� �� �Լ� ����
	WaitForSingleObject(StartupEventHandle, INFINITE);

	return TRUE;
}

// ��ü ����
BOOL EventSelect::End(VOID)
{
	// ���� ����� �̹� NULL�� ��� �������� ����
	if (!Socket)
		return FALSE;

	if (SelectThreadHandle) {		//  �����尡 ���� ���� �� ���������
		// �����带 �����ϴ� �̺�Ʈ�� �߻�
		SetEvent(DestroyEventHandle);

		// �������� ���� ���Ḧ ��ٸ�
		WaitForSingleObject(SelectThreadHandle, INFINITE);

		// ����Ǿ��� ��� ������ �ڵ��� �ʱ�ȭ
		CloseHandle(SelectThreadHandle);
	}

	// �̺�Ʈ ����
	if (SelectEventHandle)
		CloseHandle(SelectEventHandle);

	if (DestroyEventHandle)
		CloseHandle(DestroyEventHandle);

	if (StartupEventHandle)
		CloseHandle(StartupEventHandle);

	return TRUE;
}

// ���� �̺�Ʈ�� �����ϴ� ������
VOID EventSelect::SelectThreadCallback(VOID)
{
	// ��Ʈ��ũ �̺�Ʈ�� �����ϴ� �Լ�
	WSANETWORKEVENTS	NetworkEvents;
	DWORD				EventID			= 0;
	DWORD				REsult			= 0;
	// �����忡 ���������� ������ ��ġ�� �̺�Ʈ�� ���� & ��Ʈ��ũ �̺�Ʈ
	HANDLE				ThreadEvents[2] = {DestroyEventHandle, SelectEventHandle};

	while (TRUE){
		// �����尡 ���������� ���۵Ǿ��ٴ� ���� Begin �Լ��� �˷���
		SetEvent(StartupEventHandle);

		// ���⼭�� �������� ���۰� ���Ḧ ����

		// ��� �̺�Ʈ�� �߻��� ������ ��� ���
		EventID = ::WaitForMultipleObjects(2, ThreadEvents, FALSE, INFINITE);
		switch (EventID)
		{
		// ������ ���� �̺�Ʈ �߻�
		case WAIT_OBJECT_0:
			return;
		case WAIT_OBJECT_0 + 1:
			// ���� �̺�Ʈ �߻� �� �޾ƿ�
			// SelectEventhandle���� ��Ʈ��ũ �̺�Ʈ�� �̾ƿ�
			// WSAEnumNetworkEvents �� 4��° �Ķ���� : FD �̺�Ʈ���� ���� AND ������ ���� �̺�Ʈ Ȯ��
			EventID = WSAEnumNetworkEvents(Socket, SelectEventHandle, &NetworkEvents);

			// ���� Event ID�� SOCKET_ERROR�� ��� ������ �߻��� ���̹Ƿ� �����带 ����
			// ���� ������ WSAGetLastError �Լ��� ���� Ȯ�� ����
			if (EventID == SOCKET_ERROR)
				return;
			else {
				// Connect �̺�Ʈ �߻�
				if (NetworkEvents.lNetworkEvents & FD_CONNECT) {
					// ���� �Լ� ȣ��
					OnIoConnected();

					if (NetworkEvents.iErrorCode[FD_CONNECT_BIT])
						return;
				}
				// �̻��
				// Write�� �Ϸ�� ���� ��ü�� ���� �Ϸ�� ������ �ƴ� ���� ����
				else if (NetworkEvents.lNetworkEvents & FD_WRITE) {
					// OnIoWrote();
				}
				// Read �̺�Ʈ �߻�
				else if (NetworkEvents.lNetworkEvents & FD_READ) {
					// ���� �Լ� ȣ��
					OnIoRead();
				}
				// Disconnect �̺�Ʈ �߻�
				else if (NetworkEvents.lNetworkEvents & FD_CLOSE) {
					// ���� �Լ� ȣ��
					OnIoDisconnected();
					// ����
					return;
				}
			}
			break;
		default:
			return;
		}
	}
}
