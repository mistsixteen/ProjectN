#include "stdafx.h"
#include "ConnectedSessionManager.h"

BOOL ConnectedSessionManager::Begin(SOCKET listenSocket)
{
	ThreadSync Sync;

	// 파라미터로 Listen 개체의 소켓을 받아야 함
	// 값이 NULL일 경우 실패
	if (!listenSocket)
		return FALSE;

	// 최대 접속자만큼의 개체를 미리 생성
	for (DWORD i = 0; i < MAX_USER; ++i) {
		ConnectedSession* connectedSession = new ConnectedSession();
		// 생성된 개체를 관리하는 ConnectedSessionVector 내 입력
		ConnectedSessionVector.push_back(connectedSession);
	}

	// 생성한 개체들을 초기화하고 Accept 상태로 만들어줌
	for (DWORD i = 0; i < MAX_USER; ++i) {
		// 개체 초기화 중 실패할 경우 종료
		if (!ConnectedSessionVector[i]->Begin()) {
			ConnectedSessionManager::End();
			return FALSE;
		}
		// 개체를 Accept 상태로 변경하다가 문제가 있으면 종료
		if (!ConnectedSessionVector[i]->Accept(listenSocket)) {
			ConnectedSessionManager::End();
			return FALSE;
		}
	}

	return TRUE;
}

VOID ConnectedSessionManager::End(VOID)
{
	ThreadSync Sync;

	// 최대 접속자만큼의 개체를 삭제
	for (DWORD i = 0; i < ConnectedSessionVector.size(); ++i) {
		ConnectedSessionVector[i]->End();
		delete ConnectedSessionVector[i];
	}

	ConnectedSessionVector.clear();
}

VOID ConnectedSessionManager::WriteAll(DWORD protocol, BYTE * data, DWORD length)
{
	ThreadSync Sync;

	//접속된 모든 개체에 WritePacket 수행
	for (DWORD i = 0; i < ConnectedSessionVector.size(); ++i) {
		// 접속되어 있는 개체에 한해서 WritePacket
		if (ConnectedSessionVector[i]->GetConnected())
			ConnectedSessionVector[i]->WritePacket(protocol, data, length);
	}
}

ConnectedSessionManager::ConnectedSessionManager(VOID)
{
}


ConnectedSessionManager::~ConnectedSessionManager(VOID)
{
}
