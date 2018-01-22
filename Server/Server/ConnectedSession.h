#pragma once

class ConnectedSession :
	public PacketSession
{
private:
	// ���ӵǾ����� �Ǻ��ϴ� �Լ�
	BOOL			Connected;
public:
	// ��ü�� �����ϰ� �ٽ� Accept ���·� ������ִ� �Լ�
	// ServerIocp���� OnIoDisconnected�� �Ǹ� ��ü�� �����ϰ� �ٽ� �ʱ�ȭ�� �� ����
	BOOL Restart(SOCKET listenSocket);

	// isConnected�� ���� �����ϰ� �������� �Լ�
	VOID SetConnected(BOOL isConnected) { ThreadSync Sync; Connected = isConnected; }
	BOOL GetConnected(VOID) { ThreadSync Sync; return Connected; }
public:
	ConnectedSession(VOID);
	virtual ~ConnectedSession(VOID);
};

