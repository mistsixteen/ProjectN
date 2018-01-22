#include "stdafx.h"
#include "ConnectedSessionManager.h"

BOOL ConnectedSessionManager::Begin(SOCKET listenSocket)
{
	ThreadSync Sync;

	// �Ķ���ͷ� Listen ��ü�� ������ �޾ƾ� ��
	// ���� NULL�� ��� ����
	if (!listenSocket)
		return FALSE;

	// �ִ� �����ڸ�ŭ�� ��ü�� �̸� ����
	for (DWORD i = 0; i < MAX_USER; ++i) {
		ConnectedSession* connectedSession = new ConnectedSession();
		// ������ ��ü�� �����ϴ� ConnectedSessionVector �� �Է�
		ConnectedSessionVector.push_back(connectedSession);
	}

	// ������ ��ü���� �ʱ�ȭ�ϰ� Accept ���·� �������
	for (DWORD i = 0; i < MAX_USER; ++i) {
		// ��ü �ʱ�ȭ �� ������ ��� ����
		if (!ConnectedSessionVector[i]->Begin()) {
			ConnectedSessionManager::End();
			return FALSE;
		}
		// ��ü�� Accept ���·� �����ϴٰ� ������ ������ ����
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

	// �ִ� �����ڸ�ŭ�� ��ü�� ����
	for (DWORD i = 0; i < ConnectedSessionVector.size(); ++i) {
		ConnectedSessionVector[i]->End();
		delete ConnectedSessionVector[i];
	}

	ConnectedSessionVector.clear();
}

VOID ConnectedSessionManager::WriteAll(DWORD protocol, BYTE * data, DWORD length)
{
	ThreadSync Sync;

	//���ӵ� ��� ��ü�� WritePacket ����
	for (DWORD i = 0; i < ConnectedSessionVector.size(); ++i) {
		// ���ӵǾ� �ִ� ��ü�� ���ؼ� WritePacket
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
