// Server.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"


int main(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);

	// Winsock�� ����ϱ� ���� DLL �ε�
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);

	// ������ ũ���� �Ǿ��� �� ó���� �� �ְ� �ϴ� MiniDump
	if (!MiniDump::Begin()) {
		return 0;
	}

	// ���� ����
	ServerIocp serverIocp;
	if (serverIocp.Begin()) {
		getchar();
		serverIocp.End();
	}

	// ����� DLL�� ���ε�
	WSACleanup();
	MiniDump::End();
	CoUninitialize();

    return 0;
}

