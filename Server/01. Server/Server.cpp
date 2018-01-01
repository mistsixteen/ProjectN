#include "stdafx.h"

#include "Iocp.h"
#include "PacketSession.h"

class TestSession
	: public PacketSession
{
public:
	BOOL Initialize(SOCKET listenSocket)
	{
		if (!Begin())
			return FALSE;

		if (!Accept(listenSocket))
			return FALSE;

		return TRUE;
	}

	BOOL OnIoConnected(VOID)
	{
		if (!PacketSession::InitializeReadForFromIocp())
			return FALSE;

		BYTE WriteData[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		return WritePacket(0, WriteData, 10);
	}

	BOOL OnIoDisconnected(SOCKET listenSocket)
	{
		return Initialize(listenSocket);
	}
};

class TestIocp
	: public Iocp
{
private:
	TestSession*	mListenSession;
	TestSession*	mTestSessions[10];
public:
	BOOL Begin(VOID)
	{
		mListenSession = new TestSession();

		if (!Iocp::Begin())
			return FALSE;

		if (!mListenSession->Begin())
			return FALSE;

		if (!mListenSession->TcpBind())
			return FALSE;

		if (!mListenSession->Listen(9876, 10))
			return FALSE;

		if (!RegisterSocketToIocp(mListenSession->GetSocket(),
			reinterpret_cast<ULONG_PTR>(mListenSession)))
			return FALSE;

		for (USHORT i = 0; i < 10; ++i) {
			mTestSessions[i] = new TestSession();

			if (!mTestSessions[i]->Initialize(mListenSession->GetSocket())) {
				for (USHORT j = 0; j < i; ++j) {
					mTestSessions[j]->End();
					delete mTestSessions[j];
				}

				return FALSE;
			}
		}

		return TRUE;
	}

	BOOL End(VOID)
	{
		mListenSession->End();

		for (USHORT i = 0; i < 10; ++i) {
			mTestSessions[i]->End();
			delete mTestSessions[i];
		}

		delete mListenSession;

		return Iocp::End();
	}

	VOID OnIoConnected(VOID* object)
	{
		_tprintf(L"OnIoConnected 0x%x\n", object);

		TestSession* Session = reinterpret_cast<TestSession*>(object);

		if (!RegisterSocketToIocp(Session->GetSocket(),
			reinterpret_cast<ULONG_PTR>(Session)))
			return;

		if (!Session->OnIoConnected())
			return;
	}

	VOID OnIoDisconnected(VOID* object)
	{
		_tprintf(L"OnIoDisconnected 0x%x\n", object);

		TestSession* Session = reinterpret_cast<TestSession*>(object);

		if (!Session->OnIoDisconnected(mListenSession->GetSocket()))
			return;
	}

	VOID OnIoWrote(VOID* object, DWORD dataLength)
	{
		_tprintf(L"OnIoWrote 0x%x\n", object);

		TestSession* Session = reinterpret_cast<TestSession*>(object);

		Session->WriteComplete();
	}

	VOID OnIoRead(VOID* object, DWORD dataLength)
	{
		DWORD DataLength = 0;
		DWORD Protocol = 0;
		BYTE Packet[MAX_BUFFER_LENGTH];
		DWORD PacketLength = 0;

		TestSession* Session = reinterpret_cast<TestSession*>(object);

		if (!Session->ReadPacketForIocp(DataLength))
			return;

		while (Session->GetPacket(Protocol, Packet, PacketLength)) {
			// 프로토콜 처리부
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);

	TestIocp Iocp;
	Iocp.Begin();
	getchar();
	Iocp.End();

	WSACleanup();

    return 0;
}

 