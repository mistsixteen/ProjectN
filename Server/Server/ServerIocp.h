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

	// USER�� �����ϴ� map
	map<wstring, USER*> UserMap;
	// COMPUTER�� �����ϴ� map
	map<wstring, COMPUTER*> ComputerMap;
	// PROGRAM�� �����ϴ� map
	map<wstring, PROGRAM*> ProgramMap;
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
private:
	// ��Ŷ ó�� �Լ���
	VOID PROC_PT_REG_USER(ConnectedSession* pConnectedSession, DWORD dwProtocol,
							BYTE* pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_USER(ConnectedSession* pConnectedSession, DWORD dwProtocol,
							BYTE* pPacket, DWORD dwPacketLength);

	VOID PROC_PT_REG_COMPUTER(ConnectedSession* pConnectedSession, DWORD dwProtocol,
							BYTE* pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_COMPUTER(ConnectedSession* pConnectedSession, DWORD dwProtocol,
							BYTE* pPacket, DWORD dwPacketLength);

	VOID PROC_PT_REG_PROGRAM(ConnectedSession* pConnectedSession, DWORD dwProtocol,
							BYTE* pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_PROGRAM(ConnectedSession* pConnectedSession, DWORD dwProtocol,
							BYTE* pPacket, DWORD dwPacketLength);
public:
	ServerIocp(VOID);
	~ServerIocp(VOID);
};

