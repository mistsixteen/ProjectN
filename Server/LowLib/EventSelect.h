/**
* IO�� �߻���Ű�� Ŭ����
* Ŭ���̾�Ʈ���� ���� ���̴� ������� �Ϲ�������
* ������ ����ϴ� ���
*
* �����带 �ϳ� �����ؼ� �� ������ �ȿ���
* ������ ������ ��� �̺�Ʈ�� ������ ����
* ������ �Ǿ�����
*/

#pragma once
class EventSelect
{
private:
	HANDLE			SelectEventHandle;
	HANDLE			StartupEventHandle;
	HANDLE			DestroyEventHandle;

	HANDLE			SelectThreadHandle;

	SOCKET			Socket;
protected:
	virtual VOID OnIoRead(VOID) PURE;
	//virtual VOID OnIoWrote(VOID) PURE;
	virtual VOID OnIoConnected(VOID) PURE;
	virtual VOID OnIoDisconnected(VOID) PURE;
public:
	BOOL Begin(SOCKET socket);
	BOOL End(VOID);

	VOID SelectThreadCallback(VOID);
public:
	EventSelect(VOID);
	virtual ~EventSelect(VOID);
};

