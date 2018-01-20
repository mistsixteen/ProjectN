#include "stdafx.h"
#include "PacketSession.h"

PacketSession::PacketSession(VOID)
{
	// NetworkSession의 ReadBuffer에서 데이터를 가져올 버퍼
	//
	// ZeroMemory 사용 이유는 나중에 버그를 잡을 때 간편하게 하기 위함
	// memset의 경우 초기화 이외의 다른 역할로도 사용될 수 있기 때문에
	// 초기화를 할 때 메모리 사이즈를 잘못 입력해서 메모리 문제 발생 시
	// ZeroMemory를 통해 미리 작업해 놓을 경우 초기화에 관련된 내용을
	// 한꺼번에 쉽게 찾아볼 수 있기 때문
	ZeroMemory(PacketBuffer, sizeof(PacketBuffer));

	RemainLength			= 0;	// PacketBuffer에 남은 길이
									// 패킷을 처리하고 nagle 알고리즘에 의해
									// 완성되지 않은 패킷이 남은 길이

	CurrentPacketNumber		= 0;	// 현재 상대에서 보낼 때 사용할 패킷 번호
	LastReadPacketNumber	= 0;	// 마지막으로 받은 패킷 번호
									// 이것으로 기존에 보낸 패킷 번호인지를 검증
}

PacketSession::~PacketSession(VOID)
{
}

// 개체를 시작하는 함수
BOOL PacketSession::Begin(VOID)
{
	ThreadSync	Sync;				// 동기화 개체

	// NetworkSession에서 가져온 데이터를 패킷 분석 전에 담아둘 버퍼를 초기화
	ZeroMemory(PacketBuffer, sizeof(PacketBuffer));

	RemainLength			= 0;
	CurrentPacketNumber		= 0;
	LastReadPacketNumber	= 0;

	// 보내는 데이터를 저장할 큐를 초기화
	if (!WriteQueue.Begin())
		return FALSE;

	return NetworkSession::Begin();	// 상속받은 개체인 NetworkSession을 초기화
}

// 개체를 종료하는 함수
BOOL PacketSession::End(VOID)
{
	ThreadSync	Sync;							// 동기화 개체
	
	LastReadPacketInfoVectorForUdp.clear();		// UDP에서 사용할 받은 패킷정보 저장 벡터
												// TCP와 달리 하나의 접속에 한 개체의 개념으로
												// 접근하지 않고, 한 개체가 여러 접속을
												// 관리하게 되므로 받은 곳 정보와 패킷 번호를 관리해야함

	if (!WriteQueue.End())						// 큐 종료
		return FALSE;

	return NetworkSession::End();				// NetworkSession 개체 종료
}

// 보내기 완료 시 큐에서 데이터를 삭제하는 함수
BOOL PacketSession::WriteComplete(VOID)
{
	ThreadSync Sync;
	// WriteQueue에서 Pop을 수행
	return WriteQueue.Pop();
}

// 받은 패킷을 분석하는 함수
// 받은 데이터를 확인해서 정상적인 패킷일 경우 패킷을 분석
// NetworkSession 클래스를 통해서 받은 데이터를 PacketSession 클래스로
// 복제 후 해당 내용이 최소 4바이트 이상일 경우, 즉 패킷의 사이즈를 알 수 있을 경우
// 함수는 계속 진행
// 우선 앞의 4바이트를 읽어 패킷의 길이를 알고 이후 데이터가
// 패킷 길이를 알고 이후 데이터가 패킷 길이만큼 있을 경우
// 프로토콜 번호, 패킷 번호, 복호화 등의 작업을 하고 하나의 완성된 패킷으로
// 출력하게 됨
// TCP에서는 Nagle 알고리즘으로 인해 패킷이 끊어지거나 덜 올 수 있기 때문에
// 꼭 이러한 과정을 거쳐서 데이터를 받아야만 완전한 데이터를 처리할 수 있음
BOOL PacketSession::GetPacket(DWORD & protocol, BYTE * packet, DWORD & packetLength)
{
	ThreadSync Sync;					//  동기화 개체

	if (!packet)
		return FALSE;

	// RemainLength는 PacketBuffer의 데이터 길이
	// 4 바이트 이상을 받지 않으면 처리가 되지 않음
	if (RemainLength < sizeof(DWORD))
		return FALSE;

	INT PacketLength = 0;
	// 패킷 앞에 4바이트를 읽어 패킷 길이를 알아옴
	CopyMemory(&PacketLength, PacketBuffer, sizeof(INT));

	// 패킷 길이가 최대 버퍼 길이를 넘거나 0보다 작은 경우 잘못된 패킷으로 판단, 버퍼를 지움
	if (PacketLength <= 0 || PacketLength > MAX_BUFFER_LENGTH) {
		RemainLength = 0;
		return FALSE;
	}

	// 읽은 패킷 길이가 버퍼에 있는 길이보다 작거나 같을 때, 
	// 즉 온전한 한 개의 패킷이 있거나 그 이상 있을 때
	if (PacketLength <= RemainLength) {
		// 패킷 길이 이하의 부분을 복호화
		Crypt::Decrypt(PacketBuffer + sizeof(DWORD)	/* LENGTH */,
			PacketBuffer + sizeof(DWORD)			/* LENGTH */,
			PacketLength + sizeof(DWORD)			/* LENGTH */);

		DWORD PacketNumber	= 0;
		DWORD Protocol		= 0;

		// 패킷 길이와 번호를 알아옴
		CopyMemory(&PacketNumber, PacketBuffer + sizeof(DWORD)	/* LENGTH */, 
					sizeof(DWORD));
		CopyMemory(&Protocol, PacketBuffer + sizeof(DWORD)	/* LENGTH */ + sizeof(DWORD) /* PACKETNUMBER */,
					sizeof(DWORD));
		
		// 파라미터에 값을 입력
		protocol = Protocol;
		packetLength = PacketLength -
						sizeof(DWORD)	/* LENGTH */ -
						sizeof(DWORD)	/* PACKETNUMBER */ -
						sizeof(DWORD)	/* PROTOCOL */;

		CopyMemory(packet, PacketBuffer +
					sizeof(DWORD)	/* LENGTH */ +
					sizeof(DWORD)	/* PACKETNUMBER */ +
					sizeof(DWORD)	/* PROTOCOL */, 
					packetLength);

		// 한 패킷을 처리하고 남은 길이가 0보다 클 경우 뒤의 메모리를 앞으로 당겨줌
		if (RemainLength - PacketLength > 0)
			MoveMemory(PacketBuffer, PacketBuffer + PacketLength, RemainLength - PacketLength);

		// 한 패킷을 처리한 만큼 남은 길이를 줄여줌
		RemainLength -= PacketLength;

		// 남은 길이가 0이거나 0보다 작은 경우 리셋
		if (RemainLength <= 0) {
			RemainLength = 0;
			ZeroMemory(PacketBuffer, sizeof(PacketBuffer));
		}

		// 패킷 번호가 같은 것이 왔을 경우 버퍼에서 제외시키고 무시
		if (PacketNumber <= LastReadPacketNumber) {
			// 패킷 번호가 이전 것이거나 같을 경우(복사) 해당 패킷은 무시
			return FALSE;
		}
		else {
			// 새로운 패킷 번호일 경우 마지막 받은 패킷을 저장하는 변수를 업데이트
			LastReadPacketNumber = PacketNumber;
			return TRUE;
		}
	}

	return FALSE;
}

// UDP방식의 받은 패킷을 분석하는 함수
//
// UDP의 경우는 한 개체의 하나의 연결에서만 데이터를 받는 것이 아니기 때문에
// 오는 데이터들마다 보낸 주소를 확인해서 패킷이 정상적으로 오는지 확인할 필요가 있음
// 그래서 여기서는 그 정보를 벡터에 저장해서 확인
// 그리고 Reliable UDP에서 패킷의 번호와 주소 정보를 이용하여 같은 내용의 패킷을 걸러주는
// 역할도 함
BOOL PacketSession::GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD & protocol, BYTE * packet, DWORD & packetLength)
{
	ThreadSync Sync;

	if (!packet)
		return FALSE;

	if (RemainLength < sizeof(DWORD))
		return FALSE;

	// 패킷 사이즈 복제
	INT PacketLength = 0;
	CopyMemory(&PacketLength, PacketBuffer, sizeof(INT));

	// 패킷의 사이즈가 버퍼보다 크거나 0보다 작을 경우 무시
	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0) {	// Invalid Packet
		RemainLength = 0;
		return FALSE;
	}

	// 만약 한 개 이상의 완전한 패킷이 있을 경우
	if (PacketLength <= RemainLength) {
		// 패킷 사이즈 이후의 데이터들을 복호화 해줌
		Crypt::Decrypt(PacketBuffer + sizeof(DWORD) /* LENGTH */,
			PacketBuffer + sizeof(DWORD) /* LENGTH */,
			PacketLength - sizeof(DWORD) /* LENGTH */);

		DWORD PacketNumber = 0;
		DWORD Protocol = 0;

		// 패킷 번호를 읽어옴
		CopyMemory(&PacketNumber, PacketBuffer + sizeof(DWORD) /* LENGTH */, sizeof(DWORD));
		// 프로토콜 번호를 읽어옴
		CopyMemory(&Protocol, PacketBuffer +
			sizeof(DWORD) /* LENGTH */ +
			sizeof(DWORD) /* PACKETNUMBER */, sizeof(DWORD));

		protocol = Protocol;
		packetLength = PacketLength -
			sizeof(DWORD) /* LENGTH */ -
			sizeof(DWORD) /* PACKETNUMBER */ -
			sizeof(DWORD) /* PROTOCOL */;

		// 실제 패킷 데이터를 읽어옴
		CopyMemory(&packet, PacketBuffer +
			sizeof(DWORD) /* LENGTH */ +
			sizeof(DWORD) /* PACKETNUMBER */ +
			sizeof(DWORD) /* PROTOCOL */, packetLength);

		// 버퍼에서 패킷을 하나 읽고 버퍼를 정리
		if (RemainLength - PacketLength < 0)
			MoveMemory(PacketBuffer, PacketBuffer + PacketLength, RemainLength - PacketLength);

		RemainLength -= PacketLength;

		if (RemainLength <= 0) {
			RemainLength = 0;
			ZeroMemory(PacketBuffer, sizeof(PacketBuffer));
		}

		// TCP와 다른 부분
		// LastReadPacketInfoVectorForUdp에서 데이터를 받은 적이 있는지 검사
		// 있을 경우 그 정보에 패킷 번호만을 업데이트
		for (DWORD i = 0; i < LastReadPacketInfoVectorForUdp.size(); ++i) {
			READ_PACKET_INFO ReadPacketInfo = LastReadPacketInfoVectorForUdp[i];

			// 같은 주소, 같은 포트의 정보가 이미 벡터 LastReadPacketInfoVectorForUdpdp에
			// 있는지 검사
			if (!strcmp(ReadPacketInfo.RemoteAddress, remoteAddress) &&
				ReadPacketInfo.RemotePort == remotePort) {
				// 만약 받은 데이터의 패킷 번호가 지금까지의 번호보다 큰 번호일 경우
				// 업데이트를 수행. 그리고 TRUE를 반환
				if (ReadPacketInfo.PacketNumber < PacketNumber) {
					LastReadPacketInfoVectorForUdp[i].PacketNumber = PacketNumber;
					return TRUE;
				}
				else
					return FALSE;
			}
		}

		// 만약 새로운 주소에서 받은 데이터일 경우 새로 벡터에 추가
		READ_PACKET_INFO ReadPacketInfo;
		ReadPacketInfo.PacketNumber = PacketNumber;
		ReadPacketInfo.RemotePort = remotePort;
		strcpy(ReadPacketInfo.RemoteAddress, remoteAddress);

		LastReadPacketInfoVectorForUdp.push_back(ReadPacketInfo);

		return TRUE;
	}

	return FALSE;
}

// NetworkSession에서 데이터를 받아오는 함수
// ReadPacketForIocp는 FALSE가 떨어질 때까지 while문을 돌림
//
// NetworkSession 클래스의 버퍼에서 PacketSession 클래스의 
// 버퍼로 데이터를 복사하는 함수
//
// GetPacket 이전에 실행
// 기본적인 흐름은 ReadPacketForIocp를 실행해서 NetworkSession 클래스의 버퍼에서
// 데이터를 복사한 후 그 데이터를 GetPacket 함수를 통해 패킷으로 만들어
// 확인하는 흘므으로 흘러가게 됨
BOOL PacketSession::ReadPacketForIocp(DWORD readLength)
{
	ThreadSync Sync;			// 동기화 개체

	// NetworkSession의 함수를 이요해서 packetBuffer에 받은 데이터를 입력
	if (NetworkSession::ReadForIocp(PacketBuffer + RemainLength, readLength))
		return FALSE;

	// PacketBuffer에 데이터 길이를 증가시켜 줌
	RemainLength += readLength;

	return TRUE;
}

// FALSE가 떨어질때 까지 while문을 돌림
BOOL PacketSession::ReadPacketForEventSelect(VOID)
{
	ThreadSync Sync;

	DWORD ReadLength = 0;

	if (!NetworkSession::ReadForEventSelect(PacketBuffer + RemainLength, ReadLength))
		return FALSE;

	RemainLength += ReadLength;

	//return getPacket(protocol, packet, packetLength);
	return TRUE;
}

// FALSE가 떨어질때 까지 while문을 돌림
BOOL PacketSession::ReadFromPacketForIocp(LPSTR remoteAddress, USHORT & remotePort, DWORD readLength)
{
	ThreadSync Sync;

	if (!NetworkSession::ReadFromForIocp(remoteAddress, remotePort, 
										PacketBuffer + RemainLength, readLength))
		return FALSE;

	RemainLength += readLength;

	//return getPacket(remoteAddress, remotePort, protocol, packet, packetLength);
	return TRUE;
}

// FALSE가 떨어질때 까지 while문을 돌림
BOOL PacketSession::ReadFromPacketForEventSelect(LPSTR remoteAddress, USHORT & remotePort)
{
	ThreadSync Sync;

	DWORD ReadLength = 0;

	if (!NetworkSession::ReadFromForEventSelect(remoteAddress, remotePort, 
												PacketBuffer + RemainLength, ReadLength))
		return FALSE;

	RemainLength += ReadLength;

	//return getPacket(remoteAddress, remotePort, protocol, packet, packetLength);
	return TRUE;
}

// 보내기 전에 따로 큐에 데이터를 넣고 그 큐에서 반환된 데이터 포인터를
// 실제 NetworkSession 클래스의 Write 함수를 호출할 때 넣어 주는데,
// 이는 만약 데이터를 보내는 중 메모리가 삭제되면 문제가 발생하기 때문에
// 큐에 넣어주고 데이터 보내기가 완료 되었을 때 위에서 미리 알아본
// WriteComplete 함수로 데이터를 삭제하기 위해서임
//
// 큐에 데이터를 넣고 맅턴된 포인터로 Write 함수를 호출하게 되는데
// 이는 Push 함수에서 입력된 데이터를 큐에 미리 할당된 메모리 블록에 복제 후
// 그 블록의 포인터를 리턴하기 때문
// 반환된 포인터는 보내기 완료가 될 때까지 불변하므로 해당 포인터를 이용
// 반면, TempBuffer는 지역 변수이기 때문에 함수가 종료되는 순간
// 포인터가 사라져서 WriteComplete 데이터를 살려둘 수 없음
BOOL PacketSession::WritePacket(DWORD protocol, const BYTE * packet, DWORD packetLength)
{
	ThreadSync Sync;			// 동기화 개체
	
	if (!packet)
		return FALSE;

	// LENGTH(4) | PACKET_NUMBER(4) | PROTOCOL(4) | DATA(4084)
	// 패킷의 전체 길이 계산
	// 패킷 전체 길이 = 패킷 길이 + 패킷 고유 번호 + 프로토콜 + 데이터 길이
	DWORD PacketLength = sizeof(DWORD)	/* LENGTH */ +
						sizeof(DWORD)	/* PACKETNUMBER */ +
						sizeof(DWORD)	/* PROTOCOL */ + 
						packetLength;

	// 패킷 길이가 최대 버퍼 길이보다 길면 실패
	if (PacketLength >= MAX_BUFFER_LENGTH)
		return FALSE;

	// 패킷 번호 하나를 증가
	CurrentPacketNumber++;

	// 들어온 데이터를 이용해서 패킷을 만들기 위한 버퍼
	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// 패킷을 만듦
	// 패킷에 인덱스를 붙여 순서 보정을 해주어야 함

	// 패킷 길이를 써줌
	CopyMemory(TempBuffer, &PacketLength, sizeof(DWORD));

	// 패킷 번호를 써줌
	CopyMemory(TempBuffer + 
				sizeof(DWORD) /* LENGTH */,
				&CurrentPacketNumber, sizeof(DWORD));

	// 프로토콜을 써줌
	CopyMemory(TempBuffer +
		sizeof(DWORD) /* LENGTH */ +
		sizeof(DWORD) /* PACKETNUMBER */,
		&protocol, sizeof(DWORD));

	// 데이터를 써줌
	CopyMemory(TempBuffer +
		sizeof(DWORD) /* LENGTH */ +
		sizeof(DWORD) /* PACKETNUMBER */ +
		sizeof(DWORD) /* PROTOCOL */,
		&packet, packetLength);

	// 보내는 데이터를 Encrypt 함수를 이용해서 암호화
	Crypt::Encrypt(TempBuffer + sizeof(DWORD), TempBuffer + sizeof(DWORD),
					PacketLength - sizeof(DWORD));

	// WriteQueue를 이용해서 패킷이 전송 완료되었을 때까지 메모리를 살려둠

	// 큐에 넣고 반환되는 포인터는 WriteQueue에 따라 미리 할당된 메모리 포인터
	BYTE* WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	// 큐의 데이터 포인터를 넣어서 보내줌
	return NetworkSession::Write(WriteData, PacketLength);
}

BOOL PacketSession::WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE * packet, DWORD packetLength)
{

	ThreadSync Sync;

	if (!packet)
		return FALSE;

	// LENGTH(4) | PACKET_NUMBER(4) | PROTOCOL(4) | DATA (4084)
	DWORD PacketLength = sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/ +
		packetLength;

	if (PacketLength >= MAX_BUFFER_LENGTH)
		return FALSE;

	CurrentPacketNumber++;

	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0, };

	//  패킷에 인덱스를 붙여 순서 보정을 해 주어야 한다.
	memcpy(TempBuffer, &PacketLength, sizeof(DWORD));

	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/,
		&CurrentPacketNumber, sizeof(DWORD));

	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/,
		&protocol, sizeof(DWORD));

	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/,
		packet, packetLength);

	Crypt::Encrypt(TempBuffer + sizeof(DWORD), TempBuffer + sizeof(DWORD), PacketLength - sizeof(DWORD));

	// WriteQueue를 이용해서 패킷이 전송 완료가 되었을까지 메모리를 살려둔다.
	BYTE *WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return NetworkSession::WriteTo(remoteAddress, remotePort, WriteData, PacketLength);
}

// UDP 받은 주소 벡터를 리셋
BOOL PacketSession::ResetUdp(VOID)
{
	ThreadSync Sync;

	LastReadPacketInfoVectorForUdp.clear();

	return TRUE;
}
