/*
* Listen�� ����� ������ �����ϰ� �������� ����� IOCP�� ���۽�Ű�� �۾��� ����
*/
#pragma once

class ServerIocp
	: public Iocp
{
private:
	NetworkSession*				listen;
	ConnectedSessionManager		connectedSessionManager;

	HANDLE						keepThread;
	HANDLE						keepThreadDestroyEvent;
protected:
	virtual VOID OnIoRead(VOID* object, DWORD dataLength);
	virtual VOID OnIoWrote(VOID* Object, DWORD dataLength);
	virtual VOID OnIoConnected(VOID* object);
	virtual VOID OnIoDisconnected(VOID* object);
public:
	// ��ü�� �ʱ�ȭ�ϰ� �����ϱ� ���� �Լ�
	BOOL Begin(VOID);
	// ��ü ���� �Լ�
	BOOL End(VOID);
	VOID KeepThreadCallback(VOID);
public:
	ServerIocp(VOID);
	~ServerIocp(VOID);
};

