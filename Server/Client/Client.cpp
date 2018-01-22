// Client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

class TestClientSession
	: public ClientSession
{
protected:
	// 클라이언트가 접속 성공 시 호출되는 가상 함수
	VOID OnIoConnect(VOID)
	{
		_tprintf(L"OnIoConnected\n");
	}

	// 클라이언트가 접속 종료 시 호출되는 가상 함수
	VOID OnIoDisconnected(VOID)
	{
		_tprintf(L"OnIoDisconnected\n");
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	TestClientSession* clientSession = new TestClientSession();

	if (clientSession->BeginTcp("127.0.0.1", DEFAULT_PORT))
		getchar();

	clientSession->End();
	
	WSACleanup();

    return 0;
}

