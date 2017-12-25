#pragma once
/**
* ��� ��ſ��� ���� ���� Ŭ����
* Winsock���� ����ϴ� ��� �Լ����� ������ ���� Ŭ����
*/

#include "CircularQueue.h"
#include "MultiThreadSync.h"

class NetworkSession
	:public MultiThreadSync<NetworkSession>
{
private:
	OVERLAPPED_EX		AcceptOverlapped;
	OVERLAPPED_EX		ReadOverlapped;
	OVERLAPPED_EX		WriteOverlapped;

	BYTE				ReadBuffer[MAX_BUFFER_LENGTH];
	SOCKADDR_IN			UdpRemoteInfo;

	SOCKET				Socket;

	HANDLE				ReliableUdpThreadHandle;
	HANDLE				ReliableUdpThreadStartupEvent;
	HANDLE				ReliableUdpThreadDestroyEvent;
	HANDLE				ReliableUdpThreadWakeUpEvent;
	HANDLE				ReliableUdpWriteCompleteEvent;

	CircularQueue		ReliableWriteQueue;

	BOOL				IsReliableUdpSending;
public:
	BOOL Begin(VOID);
	BOOL End(VOID);

	BOOL TcpBind(VOID);
	BOOL Listen(USHORT port, INT backLog);
	BOOL Connect(LPSTR address, USHORT port);
	BOOL Accept(SOCKET listenSocket);

	BOOL InitializeReadForIocp(VOID);
	BOOL ReadForIocp(BYTE* data, DWORD& dataLength);
	BOOL ReadForEventSelect(BYTE* data, DWORD& dataLength);

	BOOL Write(BYTE* data, DWORD dataLength);
public:
	NetworkSession(VOID);
	~NetworkSession(VOID);
};

