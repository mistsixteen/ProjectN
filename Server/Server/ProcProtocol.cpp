// 패킷 처리를 위해서 새로 작성한 ProcProtocol.cpp
#include "stdafx.h"
#include "ServerIocp.h"

#define READ_PACKET(PROTOCOL)\
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(pPacket, Data);

// 사용자 등록 프로토콜 처리
VOID ServerIocp::PROC_PT_REG_USER(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
									BYTE * pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_REG_USER);
	// 실제 코드
	// BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	// S_PT_REG_USER Data;
	// READ_PT_REG_USER(pPacket, Data);

	// USER 객체를 한 개 생성
	USER* user = new USER();

	// 프로토콜에서 데이터를 생성한 개체로 옮김
	// USER_ID 복사
	_tcscpy(user->UserID, Data.USER_ID);
	// USER_NAME 복사
	_tcscpy(user->UserName, Data.USER_NAME);
	// ADDRESS 복사
	_tcscpy(user->Address, Data.ADDRESS);
	// SEX 입력
	user->Sex = Data.SEX;
	// AGE 입력
	user->Age = Data.AGE;

	// USER 객체를 관리하는 UserMap에 데이터 입력
	UserMap.insert(make_pair(Data.USER_ID, user));
}

// 사용자 검색 프로토콜 처리
VOID ServerIocp::PROC_PT_QUERY_USER(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
									BYTE * pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_QUERY_USER);

	// USER_ID를 이용해서 데이터를 검색
	auto iter = UserMap.find(Data.USER_ID);

	// 검색 결과가 있을 경우
	if (iter != UserMap.end()) {
		pConnectedSession->WritePacket(
			PT_QUERY_USER_RESULT,						// 결과 패킷
			WriteBuffer,								// 내용을 저장할 버퍼
			WRITE_PT_QUERY_USER_RESULT(WriteBuffer,		// PT_WRITE_PACKET.h에 있는 쓰기용 함수
				iter->second->UserID,					// 검색한 USER_ID
				iter->second->UserName,					// 검색한 USER_NAME
				iter->second->Age,						// 검색한 AGE
				iter->second->Sex,						// 검색한 SEX
				iter->second->Address));				// 검색한 ADDRESS
	}
	// 검색 결과가 없을 경우 빈 값을 보냄
	else{
		USER user;
		ZeroMemory(&user, sizeof(USER));

		// 모두 비어있는 데이터를 보냄
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

// 컴퓨터 등록 프로토콜 처리
VOID ServerIocp::PROC_PT_REG_COMPUTER(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
										BYTE * pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_REG_COMPUTER);

	// COMPUTER 객체를 한 개 생성
	COMPUTER* computer = new COMPUTER();

	// 프로토콜에서 데이터를 생성한 개체로 옮김
	//COMPUTER_NAME 복사
	_tcscpy(computer->ComputerName, Data.COMPUTER_NAME);
	// IP_ADDRESS 복사
	_tcscpy(computer->IPAddress, Data.IP_ADDRESS);
	// CPU_TYPE 입력
	computer->CPUType = Data.CPU_TYPE;
	// RAM 입력
	computer->Ram = Data.RAM;
	// HDD 입력
	computer->HDD = Data.HDD;

	// COMPUTER 객체를 관리하는 ComputerMap에 데이터 입력
	ComputerMap.insert(make_pair(Data.COMPUTER_NAME, computer));
}

// 컴퓨터 검색 프로토콜 처리
VOID ServerIocp::PROC_PT_QUERY_COMPUTER(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
										BYTE * pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_QUERY_COMPUTER);

	// COMPUTER_NAME를 이용해서 데이터를 검색
	auto iter = ComputerMap.find(Data.COMPUTER_NAME);

	// 검색 결과가 있을 경우
	if (iter != ComputerMap.end()) {
		pConnectedSession->WritePacket(
			PT_QUERY_COMPUTER_RESULT,						// 결과 패킷
			WriteBuffer,									// 내용을 저장할 버퍼
			WRITE_PT_QUERY_COMPUTER_RESULT(WriteBuffer,		// PT_WRITE_PACKET.h에 있는 쓰기용 함수
				iter->second->ComputerName,					// 검색한 ComputerName
				iter->second->IPAddress,					// 검색한 IPAddress
				iter->second->CPUType,						// 검색한 CPU 타입
				iter->second->Ram,							// 검색한 RAM
				iter->second->HDD));						// 검색한 HDD
	}
	// 검색 결과가 없을 경우 빈 값을 보냄
	else {
		COMPUTER computer;
		ZeroMemory(&computer, sizeof(COMPUTER));

		// 모두 비어있는 데이터를 보냄
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

// 프로그램 등록 프로토콜 처리
VOID ServerIocp::PROC_PT_REG_PROGRAM(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
										BYTE * pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_REG_PROGRAM);

	// PROGRAM 객체를 한 개 생성
	PROGRAM* program = new PROGRAM();

	// 프로토콜에서 데이터를 생성한 개체로 옮김
	// PROGRAM_NAME 복사
	_tcscpy(program->ProgramName, Data.PROGRAM_NAME);
	// COMMENT 복사
	_tcscpy(program->Comment, Data.COMMENT);
	// CPU_TYPE 입력
	program->Cost = Data.COST;

	// PRGORAM 객체를 관리하는 ProgramMap에 데이터 입력
	ProgramMap.insert(make_pair(Data.PROGRAM_NAME, program));
}

// 프로그램 검색 프로토콜 처리
VOID ServerIocp::PROC_PT_QUERY_PROGRAM(ConnectedSession * pConnectedSession, DWORD dwProtocol, 
										BYTE * pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_QUERY_PROGRAM);

	// PROGRAM_NAME을 이용해서 데이터를 검색
	auto iter = ProgramMap.find(Data.PROGRAM_NAME);

	// 검색 결과가 있을 경우
	if (iter != ProgramMap.end()) {
		pConnectedSession->WritePacket(
			PT_QUERY_PROGRAM_RESULT,						// 결과 패킷
			WriteBuffer,									// 내용을 저장할 버퍼
			WRITE_PT_QUERY_PROGRAM_RESULT(WriteBuffer,		// PT_WRITE_PACKET.h에 있는 쓰기용 함수
				iter->second->ProgramName,					// 검색한 ProgramName
				iter->second->Cost,							// 검색한 Cost
				iter->second->Comment));					// 검색한 Comment
	}
	// 검색 결과가 없을 경우 빈 값을 보냄
	else {
		PROGRAM program;
		ZeroMemory(&program, sizeof(PROGRAM));

		// 모두 비어있는 데이터를 보냄
		pConnectedSession->WritePacket(
			PT_QUERY_PROGRAM_RESULT,
			WriteBuffer,
			WRITE_PT_QUERY_PROGRAM_RESULT(WriteBuffer,
				program.ProgramName,
				program.Cost,
				program.Comment));
	}
}