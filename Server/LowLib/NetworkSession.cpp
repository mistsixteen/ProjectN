#include "stdafx.h"
#include "NetworkSession.h"

DWORD WINAPI ReliableUdpThreadCallback(LPVOID parameter)
{
	NetworkSession *UdpNetwork = (NetworkSession*)parameter;
	UdpNetwork->ReliableUdpThreadCallback();

	return 0;
}

NetworkSession::NetworkSession(VOID)
{
	// Accept�� ���õ� Overlapped ����ü
	ZeroMemory(&AcceptOverlapped, sizeof(AcceptOverlapped));

	// Read�� ���õ� Overlapped ����ü
	ZeroMemory(&ReadOverlapped, sizeof(ReadOverlapped));

	// Write�� ���õ� Overlapped ����ü
	ZeroMemory(&WriteOverlapped, sizeof(WriteOverlapped));

	// UDP ��� �� ��� �ּ� ����
	ZeroMemory(&UdpRemoteInfo, sizeof(UdpRemoteInfo));

	Socket							= NULL;		// ���� �ڵ� �� ����
	ReliableUdpThreadHandle			= NULL;		// ���� ��Ŷ�� ���� ������ ��� �������ִ�
												// ������(ReliableUpdThread) �ڵ�
	ReliableUdpThreadStartupEvent	= NULL;		// ReliableUdpThread�� ������ �˸��� �̺�Ʈ
	ReliableUdpThreadDestroyEvent	= NULL;		// ReliableUdpThread�� ���Ḧ �˸��� �̺�Ʈ
	ReliableUdpThreadWakeUpEvent	= NULL;		// ReliableUdpThread�� ���� �� ����ϴ� �̺�Ʈ
	ReliableUdpWriteCompleteEvent	= NULL;		// ��밡 ��Ŷ�� �޾� �� �̻� ���� �ʿ䰡 ���� ��
												// ����ϴ� �̺�Ʈ
	IsReliableUdpSending			= NULL;		// ���� ������ �ִ� Reliable Data�� �ִ��� Ȯ��

	AcceptOverlapped.IoType			= IO_ACCEPT;// Overlapped ����ü�� ������ ����
	ReadOverlapped.IoType			= IO_READ;	// Overlapped ����ü�� ������ ����
	WriteOverlapped.IoType			= IO_WRITE;	// Overlapped ����ü�� ������ ����

	AcceptOverlapped.Object			= this;		// ���� ��ü�� �����͸� ������ ����
	ReadOverlapped.Object			= this;		// ���� ��ü�� �����͸� ������ ����
	WriteOverlapped.Object			= this;		// ���� ��ü�� �����͸� ������ ����
}


NetworkSession::~NetworkSession(VOID)
{
}

// ��ü ����
BOOL NetworkSession::Begin(VOID)
{
	ThreadSync	Sync;		// ���� ������ ����ȭ

	if (Socket)				// �̹� ����� ������ ������ ����
		return FALSE;

	// ���� �ʱ�ȭ
	ZeroMemory(ReadBuffer, sizeof(ReadBuffer));
	// UDP�� ����� �� �����͸� ���� ��� ������ �����ϱ� ���� ����
	ZeroMemory(&UdpRemoteInfo, sizeof(UdpRemoteInfo));

	// ������ ��� ���� ���� �ڵ�
	Socket							= NULL;

	// Reliable UDP���� ����� ������ ���� ����
	ReliableUdpThreadHandle			= NULL;		// Reliable UDP ���� ������ �ڵ�
	ReliableUdpThreadStartupEvent	= NULL;		// �����尡 ���۵��� �˷��ִ� �̺�Ʈ
	ReliableUdpThreadDestroyEvent	= NULL;		// �����尡 ����Ǿ����� �˷��ִ� �̺�Ʈ
	ReliableUdpThreadWakeUpEvent	= NULL;		
	ReliableUdpWriteCompleteEvent	= NULL;		// ������ �� �� ���� ������ ������ �Ϸ�Ǿ��� ��
												// �߻��ϴ� �̺�Ʈ
	
	IsReliableUdpSending			= FALSE;	// ���� �����͸� ���� ������ Ȯ���ϴ� FLAG

	return TRUE;
}

// ����
BOOL NetworkSession::End(VOID)
{
	ThreadSync	Sync;		// ���� ������ ����ȭ

	if (!Socket)			// �̹� ������ ������ ����
		return FALSE;

	Socket = NULL;

	if (ReliableUdpThreadHandle) {					// ReliableUpdThread�� ���� ���� ���
		// �ϴ� �����带 �����ϴ� �̺�Ʈ�� �߻�
		SetEvent(ReliableUdpThreadDestroyEvent);

		// �����尡 ����� ������ ���Ѵ�� ���
		// ���⼭�� INFINITE�� �̿��ؼ� ���Ѵ�� ��ٸ��� ������ �� 3~5�� ��� �� ������ ���� ���
		// TerminateThread�� �̿��ؼ� ���� �����ϴ� �͵� ���
		WaitForSingleObject(ReliableUdpThreadHandle, INFINITE);

		CloseHandle(ReliableUdpThreadHandle);		// ���ᰡ �Ǹ� �ڵ��� �ݾ���
	}

	// ���� �̺�Ʈ�� ��� ��������
	if (ReliableUdpThreadDestroyEvent)
		CloseHandle(ReliableUdpThreadDestroyEvent);

	if (ReliableUdpThreadStartupEvent)
		CloseHandle(ReliableUdpThreadStartupEvent);

	if (ReliableUdpThreadWakeUpEvent)
		CloseHandle(ReliableUdpThreadWakeUpEvent);

	if (ReliableUdpWriteCompleteEvent)
		CloseHandle(ReliableUdpWriteCompleteEvent);

	// Reliable UDP�� ���� �����͸� �����ϴ� WriteQueue�� ���
	ReliableWriteQueue.End();

	return TRUE;
}

// TCP ��� �� �� ó�� ȣ���ϴ� �Լ�
// TCP_NODELAY : TCP���� Nagle �˰��� ��� ���� ���� �ɼ�
// Nagle : ����� ��Ʈ��ũ�� ���ؼ� ����� �� �� ���� �����͸� ���������� ���� �Ѳ����� ����
// ��Ŷ ���� ������ ��Ȯ���� ���߿� ���� �Ű� ����� ���� ����
BOOL NetworkSession::TcpBind(VOID)
{
	ThreadSync	Sync;		// ���� ������ ����ȭ

	// Socket�� NULL�� �ƴ� ��� �̹� ������ �����Ǿ� Bind �Ǿ��ִ� �����̹Ƿ� �ٽ� TcpBind �Լ��� ���� ����
	if (Socket)
		return FALSE;

	// TCP ������ ���� ����
	Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (Socket == INVALID_SOCKET)
		return FALSE;

	// TCP_NODELAY �ɼ�
	// BOOL Nodelay = TRUE;
	//setsockopt(Socket, IPPROTO_TCP, TCP_NODELAY, (const char FAR*)&NoDelay, sizeof(NoDelay));

	return TRUE;
}

BOOL NetworkSession::UdpBind(USHORT port)
{
	ThreadSync	Sync;							// ���� ������ ����ȭ

	if (Socket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo;				// ����� UDP �ּ�, ��Ʈ�� ����
	RemoteAddressInfo.sin_family = AF_INET;
	// ������ ��Ʈ�� �����͸� �ޱ� ���� ����
	RemoteAddressInfo.sin_port = htons(port);	
	RemoteAddressInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// UDP�� ������ ����. SOCK_DRAM�� IPPROTO_UDP�� �����ؼ� ������ ���� Ȯ��
	Socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (Socket == INVALID_SOCKET)
		return FALSE;
	
	// ������ �ּҸ� Bind
	if (bind(Socket, (struct sockaddr*) &RemoteAddressInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		End();
		return FALSE;
	}

	// ReliableUdpThread���� ����� �ڵ�, �̺�Ʈ���� ����
	// ReliableUdpThreadDestroyEvent
	// ReliableUdpThread�� ������ �� �߻���Ű�� �̺�Ʈ
	ReliableUdpThreadDestroyEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (ReliableUdpThreadDestroyEvent == NULL) {
		End();
		return FALSE;
	}

	// ReliableUdpThreadStartupEvent
	// ReliableUdpThread�� ���۵Ǿ��� �� �߻���Ű�� �̺�Ʈ
	ReliableUdpThreadStartupEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (ReliableUdpThreadStartupEvent == NULL) {
		End();
		return FALSE;
	}

	// ReliableUdpThreadWakeUpEvent
	// ReliableUdpThread�� ���� �� ����ϴ� �̺�Ʈ
	ReliableUdpThreadWakeUpEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (ReliableUdpThreadWakeUpEvent == NULL) {
		End();
		return FALSE;
	}

	// ReliableUdpWriteCompleteEvent
	// ReliableUdpThread���� ������ �ϳ��� ������ �Ϸ����� �� �߻��ϴ� �̺�Ʈ
	ReliableUdpWriteCompleteEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (ReliableUdpWriteCompleteEvent == NULL) {
		End();
		return FALSE;
	}

	// ReliableUdpThread���� ����� ť�� �ʱ�ȭ
	if (!ReliableWriteQueue.Begin()) {
		End();
		return FALSE;
	}

	// ReliableUdpThread�� ����
	DWORD ReliableUdpThreadID = 0;
	// 2��°�� DWORD WINAPI ������ �Լ� ������, 3��°�� LPVOID�� ������ �Լ� �Ű�����
	// this�� �������ν� Ŭ���� �����͸� �ʱ�ȭ�� ��
	// Ŭ������ ReliableUdpThreadCallback �Լ��� ���
	ReliableUdpThreadHandle = CreateThread(NULL, 0, ::ReliableUdpThreadCallback,
											this, 0, &ReliableUdpThreadID);

	// ������ �� ������ ���� ���
	WaitForSingleObject(ReliableUdpThreadStartupEvent, INFINITE);
	
	return TRUE;
}

// ReliableUDPThread ����
VOID NetworkSession::ReliableUdpThreadCallback(VOID)
{
	DWORD EventID = 0;										// ���� ���Ḧ üũ�ϴ� �̺�Ʈ ID ��
	// ReliableUdpThread�� ���۰� ���Ḧ ����ϴ� �̺�Ʈ
	HANDLE ThreadEvent[2] = { ReliableUdpThreadDestroyEvent, 
								ReliableUdpThreadWakeUpEvent };

	CHAR RemoteAddress[32]			= { 0, };				// �����͸� ���� �ּ�
	USHORT RemotePort				= 0;					// �����͸� ���� ��Ʈ
	BYTE Data[MAX_BUFFER_LENGTH]	= { 0, };				// ���� ������
	DWORD DataLength				= 0;					// ���� ������ ����
	VOID* Object = NULL;									// ������Ʈ �̻��
	
	while(TRUE) {
		SetEvent(ReliableUdpThreadStartupEvent);			// ReliableUdpThread ������ �˸��� �̺�Ʈ
															// UdpBind�� wait�� �����Ŵ
		EventID = WaitForMultipleObjects(2, ThreadEvent,	// �̺�Ʈ �߻����� ���� ���
										FALSE, INFINITE);

		switch (EventID) {
		// ReliableUdpThreadDestroyEvent �߻� �� ����
		case WAIT_OBJECT_0:		// ����
			return;
		case WAIT_OBJECT_0 + 1:
		NEXT_DATA:
			// write�� ��� 1���� ���� �����͸� Pop���ְ�
			if (ReliableWriteQueue.Pop(&Object, Data, DataLength, RemoteAddress, RemotePort)) {
				// �����Ͱ� ���� ���
				// ���� Write�� ���ְ� WaitForSingleObject�� ����
				// �޾��� �� �� SetEvent�� ���ָ� Ǯ��
			RETRY:
				// ������ �����͸� �����ϴ� �Լ�
				if (!WriteTo2(RemoteAddress, RemotePort, Data, DataLength)) {
					return;
				}

				// ������ ���⸦ 0.01�� ���� ���
				DWORD Result = WaitForSingleObject(ReliableUdpWriteCompleteEvent, 10);

				// �޾��� ��� ���� ������ ó��
				if (Result == WAIT_OBJECT_0)
					goto NEXT_DATA;
				// ���� �ʾ��� ��� ������ ������
				else
					goto RETRY;
			}
			else {
				// WriteTo �Լ����� ���� ������ ������ Ȯ�� FLAG ����
				IsReliableUdpSending = FALSE;				// ť�� ���̻� ���� �����Ͱ� ���� ���
			}
			break;
		}
	}

}

BOOL NetworkSession::Listen(USHORT port, INT backLog)
{
	ThreadSync	Sync;		// ���� ������ ����ȭ

	if (port <= 0 || backLog <= 0)	// ���� ��Ʈ�� 0���� �۰ų� backLog�� 0���� ������ ����
		return FALSE;

	if (!Socket)
		return FALSE;

	// Listen�� �� ������ �����ϴ� ����
	SOCKADDR_IN	ListenSocketInfo;
	ListenSocketInfo.sin_family				= AF_INET;
	ListenSocketInfo.sin_port				= htons(port);			// Listen�� ��Ʈ
	ListenSocketInfo.sin_addr.S_un.S_addr	= htonl(INADDR_ANY);	// Listen�� �ּҴ� ��ü �ּҷ� ��

	// ������ ������ �ּ� ������ Bind
	if (bind(Socket, (struct sockaddr*)&ListenSocketInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		End();
		return FALSE;
	}

	// Listen ����
	// backLog : ���� ���ť�� ����
	// SOMAXCONN���� �����ϸ� �ִ� ������ ����
	if (listen(Socket, backLog) == SOCKET_ERROR) {
		End();
		return FALSE;
	}

	// LINGER : ������ Close�� �� ������ ��ġ�� �Լ�
	// ���� ������ �ְų� �޴� �����Ͱ� ���� ��� �� �����͸� ��� ���� �� ������ Close�ϰ� �ϴ� �ɼ�
	// setsockopt �Լ��� �����ϰ� l_onoff ���� 1�� �ʱ�ȭ�� ����
	// �ʿ��� �ɼ��̳� ������ �޴� �����Ͱ� ���� ��� ���ῡ ���� �ð��� �ɸ� �� ������ �Ǵ� ���ؾ� ��
	LINGER Linger;
	Linger.l_onoff	= 1;
	Linger.l_linger = 0;

	if(setsockopt(Socket, SOL_SOCKET, SO_LINGER, (char*)&Linger, sizeof(LINGER)) == SOCKET_ERROR) {
		End();
		return FALSE;
	}

	return TRUE;
}

// ���� �Լ�
BOOL NetworkSession::Connect(LPSTR address, USHORT port)
// ���⼭�� TCHAR ���·� �ּҸ� ���� ����
{
	ThreadSync	Sync;					// ���� ������ ����ȭ
	if (!address || port <= 0)
		return FALSE;

	if (!Socket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo;		// ������ �ּ��� ������ �����ϴ� ����ü

	RemoteAddressInfo.sin_family			= AF_INET;
	RemoteAddressInfo.sin_port				= htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr	= inet_addr(address);

	// ������ ������ �ּҷ� ������ �õ�
	// ���⼭�� WSAConnect�� �̿��ؼ� ���������� �Ϲ� ��Ŭ�� ���� �Լ� ��� ��
	// connect �Լ��� �̿��ؼ� ���� ����
	if (WSAConnect(Socket, (LPSOCKADDR)&RemoteAddressInfo,
					sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
		// ���������� �����¶�� ����� ������ ��쿣 �������� �ν�
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			End();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL NetworkSession::Accept(SOCKET listenSocket)
{
	ThreadSync	Sync;					// ���� ������ ����ȭ

	if (!listenSocket)					// Listen ���� ������� ������ ���� �Ұ�
		return FALSE;

	if (Socket)
		return FALSE;

	// Accept�� ����� ������ �̸� ����
	Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (Socket == INVALID_SOCKET) {
		End();
		return FALSE;
	}

	// BOOL Nodelay = TRUE;
	//setsockopt(Socket, IPPROTO_TCP, TCP_NODELAY, (const char FAR*)&NoDelay, sizeof(NoDelay));
	
	// AcceptEx�� ���
	// �Ϲ������� while���� ����� WSAAccept�� �ϳ��� Accept�� �Ϸ�Ǳ� ����
	// �ٸ� ���ῡ ���� Accept�� �� ���� ����
	// �׷��� ���ڱ� ���� ������ ���� ��� ������ ���еǴ� ��찡 �߻�
	// AcceptEx�� ���� AcceptEx�� �ϳ��� ������ �̸� �����ΰ�
	// ACcept ��û�� ���� �� ������ ������ Ȱ���ϴ� ���
	// �̸� ���� ���� ���� AcceptEx�� ȣ���� ������ ���ڱ� ���� ������ ���� ��츦 ��� ����
	if (!AcceptEx(listenSocket,				// AcceptEx �Լ��� ����Ϸ���
											// mswsock.lib ���̺귯�� ����
		Socket,
		ReadBuffer,							// ���۴� ������ �ֳ�,
		0,									// ũ�⸦ 0���� ��Ƽ� ���� ����
		sizeof(sockaddr_in) + 16,			// 16 ����Ʈ �� Ŀ����
		sizeof(sockaddr_in) + 16,			// 16 ����Ʈ �� Ŀ����
		NULL,
		&AcceptOverlapped.Overlapped)) {	// AcceptOverlapped
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK) {
			End();
			return FALSE;
		}
	}

	return TRUE;
}

// IOCP ����� �� ����ϴ� �ʱ� ����
BOOL NetworkSession::InitializeReadForIocp(VOID)
{
	ThreadSync	Sync;					// ���� ������ ����ȭ
	
	if (!Socket)
		return FALSE;

	WSABUF			Wsabuf;								// WSABUF buf, len���� ����
	DWORD			ReadBytes	= 0;
	DWORD			ReadFlag	= 0;

	Wsabuf.buf					= (CHAR*)ReadBuffer;	// ����� ����� ReadBuffer �����͸� �Ѱ���
	Wsabuf.len					= MAX_BUFFER_LENGTH;	// �ִ� ���� ����

	// WSARecv�� ȣ��
	INT ReturnValue = WSARecv(Socket,
		&Wsabuf,
		1,									// ���� ������ ����. WSABUF�� �̿��� ���� ���۸� ����� ���
											// 1�̻��� ���ڸ� �Է�
		&ReadBytes,
		&ReadFlag,
		&ReadOverlapped.Overlapped,			// ReadOverlapped�� ���
		NULL);

	// SOCKET_ERROR������ WSA_IO_PENDING �̰ų� WSAEWOULDBLOCK�̸� ���������� ����
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	return TRUE;
}

BOOL NetworkSession::InitializeReadForFromIocp(VOID)
{
	ThreadSync	Sync;					// ���� ������ ����ȭ

	if (!Socket)
		return FALSE;

	WSABUF			Wsabuf;											// WSABUF buf, len���� ����
	DWORD			ReadBytes = 0;
	DWORD			ReadFlag = 0;
	INT				RemoteAddressInfoSize = sizeof(UdpRemoteInfo);	// ������ ���� �ּҸ� ����

	Wsabuf.buf = (CHAR*)ReadBuffer;	// ����� ����� ReadBuffer �����͸� �Ѱ���
	Wsabuf.len = MAX_BUFFER_LENGTH;	// �ִ� ���� ����

	// WSARecvFrom�� ȣ��
	INT ReturnValue = WSARecvFrom(Socket,
		&Wsabuf,
		1,					
		&ReadBytes,
		&ReadFlag,
		(SOCKADDR*)& UdpRemoteInfo,			// WSARecv�ʹ� �ٸ� �Ķ����
											// �����͸� ���� �ּ� ������ ��
		&RemoteAddressInfoSize,				// �ּ� ���� ����
		&ReadOverlapped.Overlapped,
		NULL
		);

	// SOCKET_ERROR������ WSA_IO_PENDING �̰ų� WSAEWOULDBLOCK�̸� ���������� ����
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	return TRUE;
}

// ���� ������ ��������
BOOL NetworkSession::ReadForIocp(BYTE * data, DWORD & dataLength)
{
	ThreadSync	Sync;					// ���� ������ ����ȭ

	if (!Socket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	// WSARecv�� ���ؼ� �޾ƿ� �����Ͱ� ����ִ� ReadBuffer���� �����͸� ����
	CopyMemory(data, ReadBuffer, dataLength);

	return TRUE;
}

// IOCP���� ������ ���� ������ Ȯ��
BOOL NetworkSession::ReadFromForIocp(LPSTR remoteAddress, USHORT & remotePort, BYTE * data, DWORD & dataLength)
{
	ThreadSync	Sync;					// ���� ������ ����ȭ

	if (!Socket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	// WSARecvFrom�� ���ؼ� �޾ƿ� �����Ͱ� ����ִ� ReadBuffer���� �����͸� ����
	CopyMemory(data, ReadBuffer, dataLength);

	strcpy(remoteAddress, inet_ntoa(UdpRemoteInfo.sin_addr));		// �����͸� ���� �ּ� Ȯ��
	remotePort = ntohs(UdpRemoteInfo.sin_port);						// �����͸� ���� ��Ʈ Ȯ��

	USHORT Ack = 0;
	// ���� �������� �պκ� 2����Ʈ�� Ȯ���� ��
	CopyMemory(&Ack, ReadBuffer, sizeof(USHORT));

	if (Ack == 9999) {
		// ���� 2����Ʈ�� 9999�� ��� �����͸� �� ���� ���� ��Ŷ���� �ν�
		return FALSE;
	}
	else {
		Ack = 9999;			// ���ο� �������� ���
		// �����͸� ���� �ּҷ� 2����Ʈ�� ����
		WriteTo2(remoteAddress, remotePort, (BYTE*)&Ack, sizeof(USHORT));
	}

	return TRUE;
}

// EventSelect ����� ����� ���� Read
// Iocp���� ���̴� ��� ���� �����͸� �б� ���� ����

// EventSelect ��� �� ������ ���� ��ȣ �߻� ��,
// WSARecv�� ȣ���ϸ� ���� �����͸� ��� �о�� �� ����

// �̿� ���� IOCP�� �̸� WSARecv�� ȣ���� ���� ���� ������ 
// �����͸� ���� �� ������ IOCP���� ��ȣ�� ���� �� WSARecv�� �޴� ���� �ƴ�
// �̸� ȣ���� ���Ҵ� WSARecv�� ���ۿ��� �����͸� Ȯ���� �� ����
BOOL NetworkSession::ReadForEventSelect(BYTE * data, DWORD & dataLength)
{
	ThreadSync	Sync;					// ���� ������ ����ȭ

	if (!Socket)
		return FALSE;
	
	if (!data)
		return FALSE;

	if (!Socket)
		return FALSE;

	WSABUF			Wsabuf;								// WSABUF buf, len���� ����
	DWORD			ReadBytes	= 0;
	DWORD			ReadFlag	= 0;

	Wsabuf.buf					= (CHAR*)ReadBuffer;	// ����� ����� ReadBuffer �����͸� �Ѱ���
	Wsabuf.len					= MAX_BUFFER_LENGTH;	// �ִ� ���� ����
														// WSARecv�� ȣ��
	INT ReturnValue = WSARecv(Socket,
		&Wsabuf,
		1,									// ���� ������ ����. WSABUF�� �̿��� ���� ���۸� ����� ���
											// 1�̻��� ���ڸ� �Է�
		&ReadBytes,
		&ReadFlag,
		&ReadOverlapped.Overlapped,			// ReadOverlapped�� ���
		NULL);

	// SOCKET_ERROR������ WSA_IO_PENDING �̰ų� WSAEWOULDBLOCK�̸� ���������� ����
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	// ���� ������ ����
	CopyMemory(data, ReadBuffer, ReadBytes);
	// ���� ������ ���̵� �ʱ�ȭ
	dataLength = ReadBytes;

	return TRUE;
}

// EventSelect ����� ����� ���� ReadFrom
BOOL NetworkSession::ReadFromForEventSelect(LPSTR remoteAddress, USHORT & remotePort, BYTE * data, DWORD & dataLength)
{
	ThreadSync	Sync;					// ���� ������ ����ȭ

	if (!Socket)
		return FALSE;

	if (!data)
		return FALSE;

	if (!Socket)
		return FALSE;

	WSABUF			Wsabuf;											// WSABUF buf, len���� ����
	DWORD			ReadBytes = 0;
	DWORD			ReadFlag = 0;
	INT				RemoteAddressInfoSize = sizeof(UdpRemoteInfo);	// �ּ� ������ ����

	Wsabuf.buf = (CHAR*)ReadBuffer;	// ����� ����� ReadBuffer �����͸� �Ѱ���
	Wsabuf.len = MAX_BUFFER_LENGTH;	// �ִ� ���� ����
									// WSARecv�� ȣ��
	INT ReturnValue = WSARecvFrom(Socket,
		&Wsabuf,
		1,									// ���� ������ ����. WSABUF�� �̿��� ���� ���۸� ����� ���
											// 1�̻��� ���ڸ� �Է�
		&ReadBytes,
		&ReadFlag,
		(SOCKADDR*)&UdpRemoteInfo,
		&RemoteAddressInfoSize,
		&ReadOverlapped.Overlapped,			// ReadOverlapped�� ���
		NULL);

	// SOCKET_ERROR������ WSA_IO_PENDING �̰ų� WSAEWOULDBLOCK�̸� ���������� ����
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	// IOCP�� �޸� �ٷ� ����� ����
	
	CopyMemory(data, ReadBuffer, ReadBytes);		// �����͸� ����
	dataLength = ReadBytes;							// �Ķ���ͷ� ������ ���̸� �޾ƿ�

	// �ּ� ������ ����
	strcpy(remoteAddress, inet_ntoa(UdpRemoteInfo.sin_addr));
	remotePort = ntohs(UdpRemoteInfo.sin_port);

	// IOCP �ʰ� ���� ReliableUDP �ڵ�

	USHORT Ack = 0;
	CopyMemory(&Ack, ReadBuffer, sizeof(USHORT));

	if (Ack == 9999) {
		// ���� 2����Ʈ�� 9999�� ��� �����͸� �� ���� ���� ��Ŷ���� �ν�
		SetEvent(ReliableUdpWriteCompleteEvent);
		return FALSE;
	}
	else {
		Ack = 9999;			// ���ο� �������� ���
							// �����͸� ���� �ּҷ� 2����Ʈ�� ����
		WriteTo2(remoteAddress, remotePort, (BYTE*)&Ack, sizeof(USHORT));
	}

	return TRUE;
}

// IOCP��� EventSelect������ ���� �������� ����
// Write�� �Ҷ� �־��� ������ �����͸� ������ �Ϸᰡ �� ������ ����ξ�� ��
// �׷��� ������ ���߿� ���� �߻� ���ΰ� ����
// ��κ� �� �κ��� ó���ϱ� ���� Write�� �����͸� ť�� �־�ΰ�
// �Ϸ�Ǿ��� �� ť���� �����͸� �����ϴ� ���·� ����
BOOL NetworkSession::Write(BYTE * data, DWORD dataLength)
{
	ThreadSync	Sync;					// ���� ������ ����ȭ

	if (!Socket)
		return FALSE;

	if (!data)
		return FALSE;

	if (!Socket)
		return FALSE;

	WSABUF			Wsabuf;								// write������ WSABUF�� ���
	DWORD			WriteBytes	= 0;
	DWORD			WriteFlag	= 0;

	Wsabuf.buf = (CHAR*)data;
	Wsabuf.len = dataLength;

	INT ReturnValue = WSASend(Socket,
		&Wsabuf,
		1,
		&WriteBytes,
		WriteFlag,
		&WriteOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	return TRUE;
}

// ����ڵ��� ȣ���ϴ� WriteTo �Լ�
BOOL NetworkSession::WriteTo(LPCSTR remoteAddress, USHORT remotePort, BYTE * data, DWORD dataLength)
{
	ThreadSync	Sync;		// ���� ������ ����ȭ

	if (!Socket)
		return FALSE;

	if (!remoteAddress || remotePort <= 0 || !data || dataLength <= 0)
		return FALSE;

	// ť�� �����͸� �Է�
	if (!ReliableWriteQueue.Push(this, data, dataLength, remoteAddress, remotePort))
		return FALSE;

	// ���� ���� ������ �����Ͱ� ���� ���
	if (!IsReliableUdpSending) {
		// ������ �����Ͱ� �ִٰ� �÷��׸� �ٲ��ְ� ReliableUDPThread�� �ƿ�
		IsReliableUdpSending = TRUE;
		SetEvent(ReliableUdpThreadWakeUpEvent);
	}

	return TRUE;
}

BOOL NetworkSession::WriteTo2(LPCSTR remoteAddress, USHORT remotePort, BYTE * data, DWORD dataLength)
{
	ThreadSync	Sync;					// ���� ������ ����ȭ

	if (!Socket)
		return FALSE;

	if (!remoteAddress || remotePort <= 0 || !data || dataLength <= 0)
		return FALSE;

	WSABUF			Wsabuf;											// WSABUF buf, len���� ����
	DWORD			WriteBytes = 0;
	DWORD			WriteFlag = 0;

	SOCKADDR_IN		RemoteAddressInfo;
	INT				RemoteAddressInfoSize = sizeof(UdpRemoteInfo);	// �ּ� ������ ����

	Wsabuf.buf = (CHAR*)data;
	Wsabuf.len = dataLength;

	// ���� ������ �ּҸ� �Է�
	RemoteAddressInfo.sin_family = AF_INET;
	RemoteAddressInfo.sin_addr.S_un.S_addr = inet_addr(remoteAddress);
	RemoteAddressInfo.sin_port = htons(remotePort);

	// WSASendTo �Լ��� �̿��ؼ� ����
	INT ReturnValue = WSASendTo(Socket,
		&Wsabuf,
		1,
		&WriteBytes,
		WriteFlag,
		(SOCKADDR*)&RemoteAddressInfo,				// ���� �ּ� ����
		RemoteAddressInfoSize,						// ���� �ּ� ���� ����
		&WriteOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING
		&& WSAGetLastError() != WSAEWOULDBLOCK) {
		End();
		return FALSE;
	}

	return TRUE;
}

SOCKET NetworkSession::GetSocket(VOID)
{
	ThreadSync Sync;

	return Socket;
}
