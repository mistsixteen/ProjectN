/*
* 실제 서버에서 사용할 소켓 IO 관리 기법
* 소켓 뿐만 아니라 일반 파일에도 적용이 가능
* 기본적으로 IO가 발생한 것을 WorkerThread들이 감지하고
* 감지한 스레드가 작업을 처리하는 동안
* 다른 작업도 처리하기 위해서 WorkerThread들은 ThreadPool이라는 기법을 통해 제작됨
*
* IOCP(Input Output Completion Port)는 입출력 작업을 처리해주는 기능으로
* 우선 윈도우즈는 핸들(HANDLE)이라는 개체를 이용해서 입출력을 관리함
* 기존의 EventSelect라는 이벤트를 이용하면 초당 64개 이상 처리하기가 힘들고
* 하나의 메시지를 처리할 때마다 많은 양의 리소스를 소모, 그래서 나온 것이 IOCP
* 입출력 핸들이 작업을 시켜 놓으면 IOCP가 나중에 결과를 알려주는 비동기(Asynchrnous)로
* 처리하게 됨
* 윈도우즈 NT 계열에선 3.5 이상, 윈도우즈 2000 이상만 지원하므로
* 윈도우즈 98, Me 사용자도 고려해야함
*
* IOCP는 메인 프로세스와는 별도로 동작, 이는 윈도우즈 메시지 시스템과 마찬가지
* 이것은 입출력 작업이 어떻게 진행되는지 메인 프로세스는 알 필요가 없고
* 단지 결과만을 통보 받는다는 것(비동기)
* 즉 IOCP는 비동기로 동작을 하고 이는 동시에 여러 개의 핸들에 대해서 작업을 할 수 있음
* 이로 인해 여러 개의 핸들로 작업 시, 결과 통보만으로는 어느 작업의 결과인지를
* 확인할 수 없게 되는 문제가 발생하기 때문에 이를 위한 해결책으로 IOCP에서는 완료의 키를
* 같이 득록하게 되는데, 이렇게 등록된 키는 작업이 완료된 것을 알려줄 때 같이 키를 알려줌으로써
* 어느 작업이 완료되었는지 알게 해줌
* 결과들이 쌓이게 되면, 윈도우즈 메시지는 메시지 큐를 두어 그 메시지를 저장하고 IOCP 역시
* 큐를 이용해서 그 결과를 저장
* 즉, 이 큐를 검사하는 것이 WorkerThread의 역할
*
* 클래스가 상속을 받게 되면 일종의 컴퍼넌트의 이벤트처럼 Connect, Read, Write, Disconnect를
* 체크할 수 있게 됨
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

