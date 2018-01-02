/*
* ���� �������� ����� ���� IO ���� ���
* ���� �Ӹ� �ƴ϶� �Ϲ� ���Ͽ��� ������ ����
* �⺻������ IO�� �߻��� ���� WorkerThread���� �����ϰ�
* ������ �����尡 �۾��� ó���ϴ� ����
* �ٸ� �۾��� ó���ϱ� ���ؼ� WorkerThread���� ThreadPool�̶�� ����� ���� ���۵�
*
* IOCP(Input Output Completion Port)�� ����� �۾��� ó�����ִ� �������
* �켱 ��������� �ڵ�(HANDLE)�̶�� ��ü�� �̿��ؼ� ������� ������
* ������ EventSelect��� �̺�Ʈ�� �̿��ϸ� �ʴ� 64�� �̻� ó���ϱⰡ �����
* �ϳ��� �޽����� ó���� ������ ���� ���� ���ҽ��� �Ҹ�, �׷��� ���� ���� IOCP
* ����� �ڵ��� �۾��� ���� ������ IOCP�� ���߿� ����� �˷��ִ� �񵿱�(Asynchrnous)��
* ó���ϰ� ��
* �������� NT �迭���� 3.5 �̻�, �������� 2000 �̻� �����ϹǷ�
* �������� 98, Me ����ڵ� ����ؾ���
*
* IOCP�� ���� ���μ����ʹ� ������ ����, �̴� �������� �޽��� �ý��۰� ��������
* �̰��� ����� �۾��� ��� ����Ǵ��� ���� ���μ����� �� �ʿ䰡 ����
* ���� ������� �뺸 �޴´ٴ� ��(�񵿱�)
* �� IOCP�� �񵿱�� ������ �ϰ� �̴� ���ÿ� ���� ���� �ڵ鿡 ���ؼ� �۾��� �� �� ����
* �̷� ���� ���� ���� �ڵ�� �۾� ��, ��� �뺸�����δ� ��� �۾��� ���������
* Ȯ���� �� ���� �Ǵ� ������ �߻��ϱ� ������ �̸� ���� �ذ�å���� IOCP������ �Ϸ��� Ű��
* ���� ����ϰ� �Ǵµ�, �̷��� ��ϵ� Ű�� �۾��� �Ϸ�� ���� �˷��� �� ���� Ű�� �˷������ν�
* ��� �۾��� �Ϸ�Ǿ����� �˰� ����
* ������� ���̰� �Ǹ�, �������� �޽����� �޽��� ť�� �ξ� �� �޽����� �����ϰ� IOCP ����
* ť�� �̿��ؼ� �� ����� ����
* ��, �� ť�� �˻��ϴ� ���� WorkerThread�� ����
*
* Ŭ������ ����� �ް� �Ǹ� ������ ���۳�Ʈ�� �̺�Ʈó�� Connect, Read, Write, Disconnect��
* üũ�� �� �ְ� ��
*/
#pragma once

class Iocp
{
private:
	HANDLE			IocpHandle			= NULL;
	HANDLE			StartupEventHandle	= NULL;

	INT				WorkerThreadCount	= 0;

	vector<HANDLE>	WorkerThreadVector;

protected:
	virtual VOID OnIoRead(VOID *object, DWORD dataLength) PURE;
	virtual VOID OnIoWrote(VOID *object, DWORD dataLength) PURE;
	virtual VOID OnIoConnected(VOID *object) PURE;
	virtual VOID OnIoDisconnected(VOID *object) PURE;
public:
	BOOL Begin(VOID);
	BOOL End(VOID);

	BOOL RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey);

	VOID WorkerThreadCallback(VOID);
public:
	Iocp(VOID);
	~Iocp(VOID);
};

