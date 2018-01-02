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
	virtual VOID OnIoConnect() PURE;
	//virtual VOID OnIoWrote() PURE;
	virtual VOID OnIoRead() PURE;
	virtual VOID OnIoDisconnected() PURE;
public:
	BOOL Begin(SOCKET socket);
	BOOL End(VOID);

	VOID SelectThreadCallback(VOID);
public:
	EventSelect(VOID);
	virtual ~EventSelect(VOID);
};

