// Client.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#define READ_PACKET(PROTOCOL)\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(Packet, Data);

class TestClientSession
	: public ClientSession
{
protected:
	// Ŭ���̾�Ʈ�� ���� ���� �� ȣ��Ǵ� ���� �Լ�
	VOID OnIoConnected(VOID)
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

	if (clientSession->BeginTcp("127.0.0.1", DEFAULT_PORT)) {
		// �� ���� ��ɾ ��� ó���ϱ� ���� ������
		while (TRUE) {
			// ��Ŷ�� ���� �� ����ϴ� ����
			BYTE writeBuffer[MAX_BUFFER_LENGTH] = { 0, };
			// ��Ŷ�� ���� �� ����ϴ� ��������, ����, ��Ŷ ����
			DWORD Protocol = 0, PacketLength = 0;
			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

			// ȭ���� �ѹ��� ������
			system("cls");

			// Ű����� �Է��� ������ �����ϴ� ����
			// ������ ��� Ű����� �Է��� ������ �̰��� ����
			TCHAR Command[32] = { 0, };
			// USEr, COMPUTER, PROGRAM�� ������ ����
			_tprintf(L"Enter command(USER/COMPUTER/PROGRAM) : ");
			ZeroMemory(Command, sizeof(Command));
			_getts_s(Command);

			// Ű����� �Է� ���� ������ �ҹ��ڷ� �ٲپ� ��
			if (!_tcscmp(_tcslwr(Command), L"user")) {
				_tprintf(L"===USER===\n");
				// REG Ȥ�� QUERY ����
				_tprintf(L"Enter command (REG/QUERY) : ");
				ZeroMemory(Command, sizeof(Command));
				_getts_s(Command);

				// REG�� ���
				if (!_tcscmp(_tcslwr(Command), L"reg")) {
					WCHAR UserID[32] = { 0, };
					WCHAR UserName[32] = { 0, };
					DWORD Age = 0;
					BYTE Sex = 0;
					WCHAR Address[64] = { 0, };

					// USER_ID�� �Է� ����
					_tprintf(L"Enter USER_ID : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					_tcscpy(UserID, Command);

					// USER_NAME�� �Է� ����
					_tprintf(L"Enter USER_NAME : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					_tcscpy(UserName, Command);

					// AGE�� �Է� ����
					_tprintf(L"Enter AGE : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					Age = _ttoi(Command);

					// SEX�� �Է� ����
					_tprintf(L"Enter SEX : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					Sex = _ttoi(Command);

					// ADDRESS�� �Է� ����
					_tprintf(L"Enter ADDRESS : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					_tcscpy(Address, Command);
					
					// ��Ŷ�� ������
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
				// QUERY�� ���
				else if (!_tcscmp(_tcslwr(Command), L"query")) {
					WCHAR UserID[32] = { 0, };

					// USER_ID�� �Է� ����
					_tprintf(L"Enter USER_ID : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					_tcscpy(UserID, Command);

					// ��Ŷ�� ������
					clientSession->WritePacket(PT_QUERY_USER,
						writeBuffer,
						WRITE_PT_QUERY_USER(writeBuffer,
							UserID));

					// ������ ��Ŷ�� ����� �� ������ ��� ��Ŷ�� ����
					// Server�� �߰��� KeepAlive ��Ŷ ����
					while (TRUE) if (!clientSession->ReadPacket(Protocol, Packet, PacketLength)) if (Protocol == PT_QUERY_USER_RESULT) break;
					
					// ��Ŷ�� ����
					READ_PACKET(PT_QUERY_USER_RESULT);

					// ���� ��Ŷ�� ���
					_tprintf(L"QUERY : USER_ID(%s), USER_NAME(%s), AGE(%d),\
								SEX(%d), ADDRESS(%s)\n", 
								Data.USER_ID, Data.USER_NAME, 
								Data.AGE, Data.SEX, Data.ADDRESS);

					_tprintf(L"Press any key...");
					getchar();
				}
				// COMPUTER�� ��� REG, QUERY�� �Է� ����
				else if (!_tcscmp(_tcslwr(Command), L"computer")) {
					_tprintf(L"===COMPUTER===\n");
					// REG Ȥ�� QUERY ����
					_tprintf(L"Enter command (REG/QUERY) : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					// REG�� ��� �Է� ��ĸ� �ٲ�� USER�� �����ϰ� ����
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


						// ��Ŷ�� ������
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
					// QUERY�� ��� USER�� �����ϰ� ����
					if (!_tcscmp(_tcslwr(Command), L"query")) {
						WCHAR ComputerName[32] = { 0, };

						_tprintf(L"Enter COMPUTER_NAME : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						_tcscpy(ComputerName, Command);


						// ��Ŷ�� ������
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
					// REG Ȥ�� QUERY ����
					_tprintf(L"Enter command (REG/QUERY) : ");
					ZeroMemory(Command, sizeof(Command));
					_getts_s(Command);
					// REG�� ��� �Է� ��ĸ� �ٲ�� USER�� �����ϰ� ����
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
					// QUERY�� ��� USER�� �����ϰ� ����
					if (!_tcscmp(_tcslwr(Command), L"query")) {
						WCHAR ProgramName[32] = { 0, };

						_tprintf(L"Enter PROGRAM_NAME : ");
						ZeroMemory(Command, sizeof(Command));
						_getts_s(Command);
						_tcscpy(ProgramName, Command);


						// ��Ŷ�� ������
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

