// Client.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

class TestClientSession
	: public ClientSession
{
protected:
	// Ŭ���̾�Ʈ�� ���� ���� �� ȣ��Ǵ� ���� �Լ�
	VOID OnIoConnect(VOID)
	{
		_tprintf(L"OnIoConnected\n");
	}

	// Ŭ���̾�Ʈ�� ���� ���� �� ȣ��Ǵ� ���� �Լ�
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

