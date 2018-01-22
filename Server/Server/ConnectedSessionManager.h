/*
* 생성, 삭제 이외의 모든 ConnectedSession 클래스에게 행해야 하는 행위가 있을 경우 사용
*/
#pragma once
#include "ConnectedSession.h"

// 다중 스레드 환경에서 안전하게 사용하기 위해 MultiThreadSync를 상속
class ConnectedSessionManager
	: public MultiThreadSync<ConnectedSessionManager>
{
private:
	// 생성된 ConnectedSession 개체의 포인터를 관리하는 벡터
	vector<ConnectedSession*>  ConnectedSessionVector;
public:
	BOOL Begin(SOCKET listenSocket);
	VOID End(VOID);

	// 접속된 모든 ConnectedSession에 패킷을 전송하는 함수
	VOID WriteAll(DWORD protocol, BYTE* data, DWORD length);
public:
	ConnectedSessionManager(VOID);
	virtual ~ConnectedSessionManager(VOID);
};

