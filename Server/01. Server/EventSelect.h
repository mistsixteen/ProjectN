/**
* IO를 발생시키는 클래스
* 클라이언트에서 많이 쓰이는 방법으로 일번적으로
* 소켓을 사용하는 방법
*
* 스레드를 하나 생성해서 그 스레드 안에서
* 지정한 소켓의 모든 이벤트를 검출해 내는
* 구조로 되어있음
*/

#pragma once
class EventSelect
{
private:
	HANDLE			SelectEventHandle;
	HANDLE			StartupEventHandle;
	HANDLE			DestroyEventHandle;

	HANDLE			SelectThreadHandle;

	SOCKET			Socket;
protected:
	virtual VOID OnIoConnect() PURE;
	//virtual VOID OnIoWrote() PURE;
	virtual VOID OnIoRead() PURE;
	virtual VOID OnIoDisconnected() PURE;
public:
	BOOL Begin(SOCKET socket);
	BOOL End(VOID);

	VOID SelectThreadCallback(VOID);
public:
	EventSelect(VOID);
	virtual ~EventSelect(VOID);
};

