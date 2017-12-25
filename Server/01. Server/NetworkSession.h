#pragma once
/**
* 모든 통신에서 사용될 기초 클래스
* Winsock에서 사용하는 통신 함수들을 래핑해 놓은 클래스
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

