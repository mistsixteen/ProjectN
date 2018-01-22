// Client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#define READ_PACKET(PROTOCOL)\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(Packet, Data);

class TestClientSession
	: public ClientSession
{
protected:
	// 클라이언트가 접속 성공 시 호출되는 가상 함수
	VOID OnIoConnected(VOID)
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

	if (clientSession->BeginTcp("127.0.0.1", DEFAULT_PORT)) {
		// 한 개의 명령어씩 계속 처리하기 위한 루프문
		while (TRUE) {
			// 패킷을 보낼 때 사용하는 버퍼
			BYTE writeBuffer[MAX_BUFFER_LENGTH] = { 0, };
			// 패킷을 받을 때 사용하는 프로토콜, 길이, 패킷 변수
			DWORD Protocol = 0, PacketLength = 0;
			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

			// 화면을 한번씩 지워줌
			system("cls");

			// 키보드로 입력한 내용을 저장하는 변수
			// 앞으로 모든 키보드로 입력한 내용이 이곳에 저장
			TCHAR Command[32] = { 0, };
			// USEr, COMPUTER, PROGRAM의 선택을 받음
			_tprintf(L"Enter command(USER/COMPUTER/PROGRAM) : ");
			ZeroMemory(Command, sizeof(Command));
			_getts_s(Command);

			// 키보드로 입력 받은 내용을 소문자로 바꾸어 비교
			if (!_tcscmp(_tcslwr(Command), L"user")) {
				_tprintf(L"===USER===\n");
				// REG 혹은 QUERY 여부
				_tprintf(L"Enter command (REG/QUERY) : ");
				ZeroMemory(Command, sizeof(Command));
				_getts_s(Command);

				// REG일 경우
				if (!_tcscmp(_tcslwr(Command), L"reg")) {
					WCHAR UserID[32] = { 0, };
					WCHAR UserName[32] = { 0, };
					DWORD Age = 0;
					BYTE Sex = 0;
					WCHAR Address[64] = { 0, };

					// USER_ID를 입력 받음
					_tprintf(L"Enter USER_ID : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					_tcscpy(UserID, Command);

					// USER_NAME을 입력 받음
					_tprintf(L"Enter USER_NAME : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					_tcscpy(UserName, Command);

					// AGE를 입력 받음
					_tprintf(L"Enter AGE : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					Age = _ttoi(Command);

					// SEX를 입력 받음
					_tprintf(L"Enter SEX : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					Sex = _ttoi(Command);

					// ADDRESS을 입력 받음
					_tprintf(L"Enter ADDRESS : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					_tcscpy(Address, Command);
					
					// 패킷을 전송함
					clientSession->WritePacket(PT_REG_USER,
						writeBuffer,
						WRITE_PT_REG_USER(writeBuffer,
							UserID,
							UserName,
							Age,
							Sex,
							Address));

					_tprintf(L"Press any key...");
					getchar();
				}
				// QUERY일 경우
				else if (!_tcscmp(_tcslwr(Command), L"query")) {
					WCHAR UserID[32] = { 0, };

					// USER_ID를 입력 받음
					_tprintf(L"Enter USER_ID : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					_tcscpy(UserID, Command);

					// 패킷을 전송함
					clientSession->WritePacket(PT_QUERY_USER,
						writeBuffer,
						WRITE_PT_QUERY_USER(writeBuffer,
							UserID));

					// 전송할 패킷의 결과가 올 때까지 계속 패킷을 읽음
					// Server에 추가한 KeepAlive 패킷 때문
					while (TRUE) if (!clientSession->ReadPacket(Protocol, Packet, PacketLength)) if (Protocol == PT_QUERY_USER_RESULT) break;
					
					// 패킷을 읽음
					READ_PACKET(PT_QUERY_USER_RESULT);

					// 읽은 패킷을 출력
					_tprintf(L"QUERY : USER_ID(%s), USER_NAME(%s), AGE(%d),\
								SEX(%d), ADDRESS(%s)\n", 
								Data.USER_ID, Data.USER_NAME, 
								Data.AGE, Data.SEX, Data.ADDRESS);

					_tprintf(L"Press any key...");
					getchar();
				}
				// COMPUTER일 경우 REG, QUERY를 입력 받음
				else if (!_tcscmp(_tcslwr(Command), L"computer")) {
					_tprintf(L"===COMPUTER===\n");
					// REG 혹은 QUERY 여부
					_tprintf(L"Enter command (REG/QUERY) : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					// REG일 경우 입력 방식만 바뀌고 USER와 동일하게 진행
					if (!_tcscmp(_tcslwr(Command), L"reg")) {
						WCHAR ComputerName[32] = { 0, };
						WCHAR IPAddress[32] = { 0, };
						BYTE CPUType = 0;
						DWORD Ram = 0;
						DWORD HDD = 0;

						_tprintf(L"Enter COMPUTER_NAME : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						_tcscpy(ComputerName, Command);

						_tprintf(L"Enter IP_ADDRESS : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						_tcscpy(IPAddress, Command);

						_tprintf(L"Enter CPU_TYPE : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						CPUType = _ttoi(Command);

						_tprintf(L"Enter RAM : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						Ram = _ttoi(Command);

						_tprintf(L"Enter HDD : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						HDD = _ttoi(Command);


						// 패킷을 전송함
						clientSession->WritePacket(PT_REG_COMPUTER,
							writeBuffer,
							WRITE_PT_REG_COMPUTER(writeBuffer,
								ComputerName,
								IPAddress,
								CPUType,
								Ram,
								HDD));

						_tprintf(L"Press any key...");
						getchar();
					}
					// QUERY일 경우 USER와 동일하게 진행
					if (!_tcscmp(_tcslwr(Command), L"query")) {
						WCHAR ComputerName[32] = { 0, };

						_tprintf(L"Enter COMPUTER_NAME : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						_tcscpy(ComputerName, Command);


						// 패킷을 전송함
						clientSession->WritePacket(PT_QUERY_COMPUTER,
							writeBuffer,
							WRITE_PT_QUERY_COMPUTER(writeBuffer,
								ComputerName));

						while (TRUE) {
							if (!clientSession->ReadPacket(Protocol, Packet, PacketLength)) {
								if(Protocol == PT_QUERY_COMPUTER_RESULT)
									break;
							}
						}

						READ_PACKET(PT_QUERY_COMPUTER_RESULT);

						_tprintf(L"QUERY : COMPUTER_NAME(%s), IP_ADDRESS(%s),\
								CPU_TYPE(%s), RAM(%d), HDD(%d)\n",
								Data.COMPUTER_NAME, Data.IP_ADDRESS,
								Data.CPU_TYPE, Data.RAM, Data.HDD);

						_tprintf(L"Press any key...");
						getchar();
					}
				}
				else if (!_tcscmp(_tcslwr(Command), L"program")) {
					_tprintf(L"===PROGRAM===\n");
					// REG 혹은 QUERY 여부
					_tprintf(L"Enter command (REG/QUERY) : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					// REG일 경우 입력 방식만 바뀌고 USER와 동일하게 진행
					if (!_tcscmp(_tcslwr(Command), L"reg")) {
						WCHAR ProgramName[32] = { 0, };
						DWORD Cost = 0;
						WCHAR Comment[32] = { 0, };

						_tprintf(L"Enter PROGRAM_NAME : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						_tcscpy(ProgramName, Command);

						_tprintf(L"Enter COST : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						Cost = _ttoi(Command);

						_tprintf(L"Enter COMMENT : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						_tcscpy(Comment, Command);

						clientSession->WritePacket(PT_REG_PROGRAM,
							writeBuffer,
							WRITE_PT_REG_PROGRAM(writeBuffer,
								ProgramName,
								Cost,
								Comment));

						_tprintf(L"Press any key...");
						getchar();
					}
					// QUERY일 경우 USER와 동일하게 진행
					if (!_tcscmp(_tcslwr(Command), L"query")) {
						WCHAR ProgramName[32] = { 0, };

						_tprintf(L"Enter PROGRAM_NAME : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						_tcscpy(ProgramName, Command);


						// 패킷을 전송함
						clientSession->WritePacket(PT_QUERY_PROGRAM,
							writeBuffer,
							WRITE_PT_QUERY_PROGRAM(writeBuffer,
								ProgramName));

						while (TRUE) {
							if (!clientSession->ReadPacket(Protocol, Packet, PacketLength)) {
								if (Protocol == PT_QUERY_PROGRAM_RESULT)
									break;
							}
						}

						READ_PACKET(PT_QUERY_PROGRAM_RESULT);

						_tprintf(L"QUERY : PROGRAM_NAME(%s), COST(%d), COMMENT(%s)\n",
							Data.PROGRAM_NAME, Data.COST, Data.COMMENT);

						_tprintf(L"Press any key...");
						getchar();
					}
				}
			}
		}
	}

	clientSession->End();
	
	WSACleanup();

    return 0;
}

