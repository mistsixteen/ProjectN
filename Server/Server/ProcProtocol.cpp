// ��Ŷ ó���� ���ؼ� ���� �ۼ��� ProcProtocol.cpp
#include "stdafx.h"
#include "ServerIocp.h"

#define READ_PACKET(PROTOCOL)\
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(pPacket, Data);

// ����� ��� �������� ó��
VOID ServerIocp::PROC_PT_REG_USER(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
									BYTE * pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_REG_USER);
	// ���� �ڵ�
	// BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	// S_PT_REG_USER Data;
	// READ_PT_REG_USER(pPacket, Data);

	// USER ��ü�� �� �� ����
	USER* user = new USER();

	// �������ݿ��� �����͸� ������ ��ü�� �ű�
	// USER_ID ����
	_tcscpy(user->UserID, Data.USER_ID);
	// USER_NAME ����
	_tcscpy(user->UserName, Data.USER_NAME);
	// ADDRESS ����
	_tcscpy(user->Address, Data.ADDRESS);
	// SEX �Է�
	user->Sex = Data.SEX;
	// AGE �Է�
	user->Age = Data.AGE;

	// USER ��ü�� �����ϴ� UserMap�� ������ �Է�
	UserMap.insert(make_pair(Data.USER_ID, user));
}

// ����� �˻� �������� ó��
VOID ServerIocp::PROC_PT_QUERY_USER(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
									BYTE * pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_QUERY_USER);

	// USER_ID�� �̿��ؼ� �����͸� �˻�
	auto iter = UserMap.find(Data.USER_ID);

	// �˻� ����� ���� ���
	if (iter != UserMap.end()) {
		pConnectedSession->WritePacket(
			PT_QUERY_USER_RESULT,						// ��� ��Ŷ
			WriteBuffer,								// ������ ������ ����
			WRITE_PT_QUERY_USER_RESULT(WriteBuffer,		// PT_WRITE_PACKET.h�� �ִ� ����� �Լ�
				iter->second->UserID,					// �˻��� USER_ID
				iter->second->UserName,					// �˻��� USER_NAME
				iter->second->Age,						// �˻��� AGE
				iter->second->Sex,						// �˻��� SEX
				iter->second->Address));				// �˻��� ADDRESS
	}
	// �˻� ����� ���� ��� �� ���� ����
	else{
		USER user;
		ZeroMemory(&user, sizeof(USER));

		// ��� ����ִ� �����͸� ����
		pConnectedSession->WritePacket(
			PT_QUERY_USER_RESULT,
			WriteBuffer,
			WRITE_PT_QUERY_USER_RESULT(WriteBuffer,
				user.UserID,
				user.UserName,
				user.Age,
				user.Sex,
				user.Address));
	}
}

// ��ǻ�� ��� �������� ó��
VOID ServerIocp::PROC_PT_REG_COMPUTER(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
										BYTE * pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_REG_COMPUTER);

	// COMPUTER ��ü�� �� �� ����
	COMPUTER* computer = new COMPUTER();

	// �������ݿ��� �����͸� ������ ��ü�� �ű�
	//COMPUTER_NAME ����
	_tcscpy(computer->ComputerName, Data.COMPUTER_NAME);
	// IP_ADDRESS ����
	_tcscpy(computer->IPAddress, Data.IP_ADDRESS);
	// CPU_TYPE �Է�
	computer->CPUType = Data.CPU_TYPE;
	// RAM �Է�
	computer->Ram = Data.RAM;
	// HDD �Է�
	computer->HDD = Data.HDD;

	// COMPUTER ��ü�� �����ϴ� ComputerMap�� ������ �Է�
	ComputerMap.insert(make_pair(Data.COMPUTER_NAME, computer));
}

// ��ǻ�� �˻� �������� ó��
VOID ServerIocp::PROC_PT_QUERY_COMPUTER(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
										BYTE * pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_QUERY_COMPUTER);

	// COMPUTER_NAME�� �̿��ؼ� �����͸� �˻�
	auto iter = ComputerMap.find(Data.COMPUTER_NAME);

	// �˻� ����� ���� ���
	if (iter != ComputerMap.end()) {
		pConnectedSession->WritePacket(
			PT_QUERY_COMPUTER_RESULT,						// ��� ��Ŷ
			WriteBuffer,									// ������ ������ ����
			WRITE_PT_QUERY_COMPUTER_RESULT(WriteBuffer,		// PT_WRITE_PACKET.h�� �ִ� ����� �Լ�
				iter->second->ComputerName,					// �˻��� ComputerName
				iter->second->IPAddress,					// �˻��� IPAddress
				iter->second->CPUType,						// �˻��� CPU Ÿ��
				iter->second->Ram,							// �˻��� RAM
				iter->second->HDD));						// �˻��� HDD
	}
	// �˻� ����� ���� ��� �� ���� ����
	else {
		COMPUTER computer;
		ZeroMemory(&computer, sizeof(COMPUTER));

		// ��� ����ִ� �����͸� ����
		pConnectedSession->WritePacket(
			PT_QUERY_USER_RESULT,
			WriteBuffer,
			WRITE_PT_QUERY_COMPUTER_RESULT(WriteBuffer,
				computer.ComputerName,
				computer.IPAddress,
				computer.CPUType,
				computer.Ram,
				computer.HDD));
	}
}

// ���α׷� ��� �������� ó��
VOID ServerIocp::PROC_PT_REG_PROGRAM(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
										BYTE * pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_REG_PROGRAM);

	// PROGRAM ��ü�� �� �� ����
	PROGRAM* program = new PROGRAM();

	// �������ݿ��� �����͸� ������ ��ü�� �ű�
	// PROGRAM_NAME ����
	_tcscpy(program->ProgramName, Data.PROGRAM_NAME);
	// COMMENT ����
	_tcscpy(program->Comment, Data.COMMENT);
	// CPU_TYPE �Է�
	program->Cost = Data.COST;

	// PRGORAM ��ü�� �����ϴ� ProgramMap�� ������ �Է�
	ProgramMap.insert(make_pair(Data.PROGRAM_NAME, program));
}

// ���α׷� �˻� �������� ó��
VOID ServerIocp::PROC_PT_QUERY_PROGRAM(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
										BYTE * pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_QUERY_PROGRAM);

	// PROGRAM_NAME�� �̿��ؼ� �����͸� �˻�
	auto iter = ProgramMap.find(Data.PROGRAM_NAME);

	// �˻� ����� ���� ���
	if (iter != ProgramMap.end()) {
		pConnectedSession->WritePacket(
			PT_QUERY_PROGRAM_RESULT,						// ��� ��Ŷ
			WriteBuffer,									// ������ ������ ����
			WRITE_PT_QUERY_PROGRAM_RESULT(WriteBuffer,		// PT_WRITE_PACKET.h�� �ִ� ����� �Լ�
				iter->second->ProgramName,					// �˻��� ProgramName
				iter->second->Cost,							// �˻��� Cost
				iter->second->Comment));					// �˻��� Comment
	}
	// �˻� ����� ���� ��� �� ���� ����
	else {
		PROGRAM program;
		ZeroMemory(&program, sizeof(PROGRAM));

		// ��� ����ִ� �����͸� ����
		pConnectedSession->WritePacket(
			PT_QUERY_PROGRAM_RESULT,
			WriteBuffer,
			WRITE_PT_QUERY_PROGRAM_RESULT(WriteBuffer,
				program.ProgramName,
				program.Cost,
				program.Comment));
	}
}