/*
* ����, ���� �̿��� ��� ConnectedSession Ŭ�������� ���ؾ� �ϴ� ������ ���� ��� ���
*/
#pragma once
#include "ConnectedSession.h"

// ���� ������ ȯ�濡�� �����ϰ� ����ϱ� ���� MultiThreadSync�� ���
class ConnectedSessionManager
	: public MultiThreadSync<ConnectedSessionManager>
{
private:
	// ������ ConnectedSession ��ü�� �����͸� �����ϴ� ����
	vector<ConnectedSession*>  ConnectedSessionVector;
public:
	BOOL Begin(SOCKET listenSocket);
	VOID End(VOID);

	// ���ӵ� ��� ConnectedSession�� ��Ŷ�� �����ϴ� �Լ�
	VOID WriteAll(DWORD protocol, BYTE* data, DWORD length);
public:
	ConnectedSessionManager(VOID);
	virtual ~ConnectedSessionManager(VOID);
};

