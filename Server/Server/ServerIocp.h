/*
* Listen을 담당할 소켓을 생성하고 서버에서 사용할 IOCP를 시작시키는 작업을 수행
*/
#pragma once

class ServerIocp
	: public Iocp
{
private:
	NetworkSession*				listen;
	ConnectedSessionManager		connectedSessionManager;

	HANDLE						keepThread;
	HANDLE						keepThreadDestroyEvent;
protected:
	virtual VOID OnIoRead(VOID* object, DWORD dataLength);
	virtual VOID OnIoWrote(VOID* Object, DWORD dataLength);
	virtual VOID OnIoConnected(VOID* object);
	virtual VOID OnIoDisconnected(VOID* object);
public:
	// 개체를 초기화하고 시작하기 위한 함수
	BOOL Begin(VOID);
	// 개체 종료 함수
	BOOL End(VOID);
	VOID KeepThreadCallback(VOID);
public:
	ServerIocp(VOID);
	~ServerIocp(VOID);
};

