#pragma once

class ConnectedSession :
	public PacketSession
{
private:
	// 접속되었는지 판별하는 함수
	BOOL			Connected;
public:
	// 개체를 종료하고 다시 Accept 상태로 만들어주는 함수
	// ServerIocp에서 OnIoDisconnected가 되면 개체를 종료하고 다시 초기화할 때 사용됨
	BOOL Restart(SOCKET listenSocket);

	// isConnected의 값을 설정하고 가져오는 함수
	VOID SetConnected(BOOL isConnected) { ThreadSync Sync; Connected = isConnected; }
	BOOL GetConnected(VOID) { ThreadSync Sync; return Connected; }
public:
	ConnectedSession(VOID);
	virtual ~ConnectedSession(VOID);
};

