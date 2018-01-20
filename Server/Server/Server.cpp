// Server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


int main(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);

	// Winsock을 사용하기 위한 DLL 로드
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);

	// 서버가 크래쉬 되었을 때 처리할 수 있게 하는 MiniDump
	if (!MiniDump::Begin()) {
		return 0;
	}

	// 서버 시작
	ServerIocp serverIocp;
	if (serverIocp.Begin()) {
		getchar();
		serverIocp.End();
	}

	// 종료시 DLL을 업로드
	WSACleanup();
	MiniDump::End();
	CoUninitialize();

    return 0;
}

