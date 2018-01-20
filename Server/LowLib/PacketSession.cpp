#include "stdafx.h"
#include "PacketSession.h"

PacketSession::PacketSession(VOID)
{
	// NetworkSession�� ReadBuffer���� �����͸� ������ ����
	//
	// ZeroMemory ��� ������ ���߿� ���׸� ���� �� �����ϰ� �ϱ� ����
	// memset�� ��� �ʱ�ȭ �̿��� �ٸ� ���ҷε� ���� �� �ֱ� ������
	// �ʱ�ȭ�� �� �� �޸� ����� �߸� �Է��ؼ� �޸� ���� �߻� ��
	// ZeroMemory�� ���� �̸� �۾��� ���� ��� �ʱ�ȭ�� ���õ� ������
	// �Ѳ����� ���� ã�ƺ� �� �ֱ� ����
	ZeroMemory(PacketBuffer, sizeof(PacketBuffer));

	RemainLength			= 0;	// PacketBuffer�� ���� ����
									// ��Ŷ�� ó���ϰ� nagle �˰��� ����
									// �ϼ����� ���� ��Ŷ�� ���� ����

	CurrentPacketNumber		= 0;	// ���� ��뿡�� ���� �� ����� ��Ŷ ��ȣ
	LastReadPacketNumber	= 0;	// ���������� ���� ��Ŷ ��ȣ
									// �̰����� ������ ���� ��Ŷ ��ȣ������ ����
}

PacketSession::~PacketSession(VOID)
{
}

// ��ü�� �����ϴ� �Լ�
BOOL PacketSession::Begin(VOID)
{
	ThreadSync	Sync;				// ����ȭ ��ü

	// NetworkSession���� ������ �����͸� ��Ŷ �м� ���� ��Ƶ� ���۸� �ʱ�ȭ
	ZeroMemory(PacketBuffer, sizeof(PacketBuffer));

	RemainLength			= 0;
	CurrentPacketNumber		= 0;
	LastReadPacketNumber	= 0;

	// ������ �����͸� ������ ť�� �ʱ�ȭ
	if (!WriteQueue.Begin())
		return FALSE;

	return NetworkSession::Begin();	// ��ӹ��� ��ü�� NetworkSession�� �ʱ�ȭ
}

// ��ü�� �����ϴ� �Լ�
BOOL PacketSession::End(VOID)
{
	ThreadSync	Sync;							// ����ȭ ��ü
	
	LastReadPacketInfoVectorForUdp.clear();		// UDP���� ����� ���� ��Ŷ���� ���� ����
												// TCP�� �޸� �ϳ��� ���ӿ� �� ��ü�� ��������
												// �������� �ʰ�, �� ��ü�� ���� ������
												// �����ϰ� �ǹǷ� ���� �� ������ ��Ŷ ��ȣ�� �����ؾ���

	if (!WriteQueue.End())						// ť ����
		return FALSE;

	return NetworkSession::End();				// NetworkSession ��ü ����
}

// ������ �Ϸ� �� ť���� �����͸� �����ϴ� �Լ�
BOOL PacketSession::WriteComplete(VOID)
{
	ThreadSync Sync;
	// WriteQueue���� Pop�� ����
	return WriteQueue.Pop();
}

// ���� ��Ŷ�� �м��ϴ� �Լ�
// ���� �����͸� Ȯ���ؼ� �������� ��Ŷ�� ��� ��Ŷ�� �м�
// NetworkSession Ŭ������ ���ؼ� ���� �����͸� PacketSession Ŭ������
// ���� �� �ش� ������ �ּ� 4����Ʈ �̻��� ���, �� ��Ŷ�� ����� �� �� ���� ���
// �Լ��� ��� ����
// �켱 ���� 4����Ʈ�� �о� ��Ŷ�� ���̸� �˰� ���� �����Ͱ�
// ��Ŷ ���̸� �˰� ���� �����Ͱ� ��Ŷ ���̸�ŭ ���� ���
// �������� ��ȣ, ��Ŷ ��ȣ, ��ȣȭ ���� �۾��� �ϰ� �ϳ��� �ϼ��� ��Ŷ����
// ����ϰ� ��
// TCP������ Nagle �˰������� ���� ��Ŷ�� �������ų� �� �� �� �ֱ� ������
// �� �̷��� ������ ���ļ� �����͸� �޾ƾ߸� ������ �����͸� ó���� �� ����
BOOL PacketSession::GetPacket(DWORD & protocol, BYTE * packet, DWORD & packetLength)
{
	ThreadSync Sync;					//  ����ȭ ��ü

	if (!packet)
		return FALSE;

	// RemainLength�� PacketBuffer�� ������ ����
	// 4 ����Ʈ �̻��� ���� ������ ó���� ���� ����
	if (RemainLength < sizeof(DWORD))
		return FALSE;

	INT PacketLength = 0;
	// ��Ŷ �տ� 4����Ʈ�� �о� ��Ŷ ���̸� �˾ƿ�
	CopyMemory(&PacketLength, PacketBuffer, sizeof(INT));

	// ��Ŷ ���̰� �ִ� ���� ���̸� �Ѱų� 0���� ���� ��� �߸��� ��Ŷ���� �Ǵ�, ���۸� ����
	if (PacketLength <= 0 || PacketLength > MAX_BUFFER_LENGTH) {
		RemainLength = 0;
		return FALSE;
	}

	// ���� ��Ŷ ���̰� ���ۿ� �ִ� ���̺��� �۰ų� ���� ��, 
	// �� ������ �� ���� ��Ŷ�� �ְų� �� �̻� ���� ��
	if (PacketLength <= RemainLength) {
		// ��Ŷ ���� ������ �κ��� ��ȣȭ
		Crypt::Decrypt(PacketBuffer + sizeof(DWORD)	/* LENGTH */,
			PacketBuffer + sizeof(DWORD)			/* LENGTH */,
			PacketLength + sizeof(DWORD)			/* LENGTH */);

		DWORD PacketNumber	= 0;
		DWORD Protocol		= 0;

		// ��Ŷ ���̿� ��ȣ�� �˾ƿ�
		CopyMemory(&PacketNumber, PacketBuffer + sizeof(DWORD)	/* LENGTH */, 
					sizeof(DWORD));
		CopyMemory(&Protocol, PacketBuffer + sizeof(DWORD)	/* LENGTH */ + sizeof(DWORD) /* PACKETNUMBER */,
					sizeof(DWORD));
		
		// �Ķ���Ϳ� ���� �Է�
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

		// �� ��Ŷ�� ó���ϰ� ���� ���̰� 0���� Ŭ ��� ���� �޸𸮸� ������ �����
		if (RemainLength - PacketLength > 0)
			MoveMemory(PacketBuffer, PacketBuffer + PacketLength, RemainLength - PacketLength);

		// �� ��Ŷ�� ó���� ��ŭ ���� ���̸� �ٿ���
		RemainLength -= PacketLength;

		// ���� ���̰� 0�̰ų� 0���� ���� ��� ����
		if (RemainLength <= 0) {
			RemainLength = 0;
			ZeroMemory(PacketBuffer, sizeof(PacketBuffer));
		}

		// ��Ŷ ��ȣ�� ���� ���� ���� ��� ���ۿ��� ���ܽ�Ű�� ����
		if (PacketNumber <= LastReadPacketNumber) {
			// ��Ŷ ��ȣ�� ���� ���̰ų� ���� ���(����) �ش� ��Ŷ�� ����
			return FALSE;
		}
		else {
			// ���ο� ��Ŷ ��ȣ�� ��� ������ ���� ��Ŷ�� �����ϴ� ������ ������Ʈ
			LastReadPacketNumber = PacketNumber;
			return TRUE;
		}
	}

	return FALSE;
}

// UDP����� ���� ��Ŷ�� �м��ϴ� �Լ�
//
// UDP�� ���� �� ��ü�� �ϳ��� ���ῡ���� �����͸� �޴� ���� �ƴϱ� ������
// ���� �����͵鸶�� ���� �ּҸ� Ȯ���ؼ� ��Ŷ�� ���������� ������ Ȯ���� �ʿ䰡 ����
// �׷��� ���⼭�� �� ������ ���Ϳ� �����ؼ� Ȯ��
// �׸��� Reliable UDP���� ��Ŷ�� ��ȣ�� �ּ� ������ �̿��Ͽ� ���� ������ ��Ŷ�� �ɷ��ִ�
// ���ҵ� ��
BOOL PacketSession::GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD & protocol, BYTE * packet, DWORD & packetLength)
{
	ThreadSync Sync;

	if (!packet)
		return FALSE;

	if (RemainLength < sizeof(DWORD))
		return FALSE;

	// ��Ŷ ������ ����
	INT PacketLength = 0;
	CopyMemory(&PacketLength, PacketBuffer, sizeof(INT));

	// ��Ŷ�� ����� ���ۺ��� ũ�ų� 0���� ���� ��� ����
	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0) {	// Invalid Packet
		RemainLength = 0;
		return FALSE;
	}

	// ���� �� �� �̻��� ������ ��Ŷ�� ���� ���
	if (PacketLength <= RemainLength) {
		// ��Ŷ ������ ������ �����͵��� ��ȣȭ ����
		Crypt::Decrypt(PacketBuffer + sizeof(DWORD) /* LENGTH */,
			PacketBuffer + sizeof(DWORD) /* LENGTH */,
			PacketLength - sizeof(DWORD) /* LENGTH */);

		DWORD PacketNumber = 0;
		DWORD Protocol = 0;

		// ��Ŷ ��ȣ�� �о��
		CopyMemory(&PacketNumber, PacketBuffer + sizeof(DWORD) /* LENGTH */, sizeof(DWORD));
		// �������� ��ȣ�� �о��
		CopyMemory(&Protocol, PacketBuffer +
			sizeof(DWORD) /* LENGTH */ +
			sizeof(DWORD) /* PACKETNUMBER */, sizeof(DWORD));

		protocol = Protocol;
		packetLength = PacketLength -
			sizeof(DWORD) /* LENGTH */ -
			sizeof(DWORD) /* PACKETNUMBER */ -
			sizeof(DWORD) /* PROTOCOL */;

		// ���� ��Ŷ �����͸� �о��
		CopyMemory(&packet, PacketBuffer +
			sizeof(DWORD) /* LENGTH */ +
			sizeof(DWORD) /* PACKETNUMBER */ +
			sizeof(DWORD) /* PROTOCOL */, packetLength);

		// ���ۿ��� ��Ŷ�� �ϳ� �а� ���۸� ����
		if (RemainLength - PacketLength < 0)
			MoveMemory(PacketBuffer, PacketBuffer + PacketLength, RemainLength - PacketLength);

		RemainLength -= PacketLength;

		if (RemainLength <= 0) {
			RemainLength = 0;
			ZeroMemory(PacketBuffer, sizeof(PacketBuffer));
		}

		// TCP�� �ٸ� �κ�
		// LastReadPacketInfoVectorForUdp���� �����͸� ���� ���� �ִ��� �˻�
		// ���� ��� �� ������ ��Ŷ ��ȣ���� ������Ʈ
		for (DWORD i = 0; i < LastReadPacketInfoVectorForUdp.size(); ++i) {
			READ_PACKET_INFO ReadPacketInfo = LastReadPacketInfoVectorForUdp[i];

			// ���� �ּ�, ���� ��Ʈ�� ������ �̹� ���� LastReadPacketInfoVectorForUdpdp��
			// �ִ��� �˻�
			if (!strcmp(ReadPacketInfo.RemoteAddress, remoteAddress) &&
				ReadPacketInfo.RemotePort == remotePort) {
				// ���� ���� �������� ��Ŷ ��ȣ�� ���ݱ����� ��ȣ���� ū ��ȣ�� ���
				// ������Ʈ�� ����. �׸��� TRUE�� ��ȯ
				if (ReadPacketInfo.PacketNumber < PacketNumber) {
					LastReadPacketInfoVectorForUdp[i].PacketNumber = PacketNumber;
					return TRUE;
				}
				else
					return FALSE;
			}
		}

		// ���� ���ο� �ּҿ��� ���� �������� ��� ���� ���Ϳ� �߰�
		READ_PACKET_INFO ReadPacketInfo;
		ReadPacketInfo.PacketNumber = PacketNumber;
		ReadPacketInfo.RemotePort = remotePort;
		strcpy(ReadPacketInfo.RemoteAddress, remoteAddress);

		LastReadPacketInfoVectorForUdp.push_back(ReadPacketInfo);

		return TRUE;
	}

	return FALSE;
}

// NetworkSession���� �����͸� �޾ƿ��� �Լ�
// ReadPacketForIocp�� FALSE�� ������ ������ while���� ����
//
// NetworkSession Ŭ������ ���ۿ��� PacketSession Ŭ������ 
// ���۷� �����͸� �����ϴ� �Լ�
//
// GetPacket ������ ����
// �⺻���� �帧�� ReadPacketForIocp�� �����ؼ� NetworkSession Ŭ������ ���ۿ���
// �����͸� ������ �� �� �����͸� GetPacket �Լ��� ���� ��Ŷ���� �����
// Ȯ���ϴ� ������� �귯���� ��
BOOL PacketSession::ReadPacketForIocp(DWORD readLength)
{
	ThreadSync Sync;			// ����ȭ ��ü

	// NetworkSession�� �Լ��� �̿��ؼ� packetBuffer�� ���� �����͸� �Է�
	if (NetworkSession::ReadForIocp(PacketBuffer + RemainLength, readLength))
		return FALSE;

	// PacketBuffer�� ������ ���̸� �������� ��
	RemainLength += readLength;

	return TRUE;
}

// FALSE�� �������� ���� while���� ����
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

// FALSE�� �������� ���� while���� ����
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

// FALSE�� �������� ���� while���� ����
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

// ������ ���� ���� ť�� �����͸� �ְ� �� ť���� ��ȯ�� ������ �����͸�
// ���� NetworkSession Ŭ������ Write �Լ��� ȣ���� �� �־� �ִµ�,
// �̴� ���� �����͸� ������ �� �޸𸮰� �����Ǹ� ������ �߻��ϱ� ������
// ť�� �־��ְ� ������ �����Ⱑ �Ϸ� �Ǿ��� �� ������ �̸� �˾ƺ�
// WriteComplete �Լ��� �����͸� �����ϱ� ���ؼ���
//
// ť�� �����͸� �ְ� ���ϵ� �����ͷ� Write �Լ��� ȣ���ϰ� �Ǵµ�
// �̴� Push �Լ����� �Էµ� �����͸� ť�� �̸� �Ҵ�� �޸� ��Ͽ� ���� ��
// �� ����� �����͸� �����ϱ� ����
// ��ȯ�� �����ʹ� ������ �Ϸᰡ �� ������ �Һ��ϹǷ� �ش� �����͸� �̿�
// �ݸ�, TempBuffer�� ���� �����̱� ������ �Լ��� ����Ǵ� ����
// �����Ͱ� ������� WriteComplete �����͸� ����� �� ����
BOOL PacketSession::WritePacket(DWORD protocol, const BYTE * packet, DWORD packetLength)
{
	ThreadSync Sync;			// ����ȭ ��ü
	
	if (!packet)
		return FALSE;

	// LENGTH(4) | PACKET_NUMBER(4) | PROTOCOL(4) | DATA(4084)
	// ��Ŷ�� ��ü ���� ���
	// ��Ŷ ��ü ���� = ��Ŷ ���� + ��Ŷ ���� ��ȣ + �������� + ������ ����
	DWORD PacketLength = sizeof(DWORD)	/* LENGTH */ +
						sizeof(DWORD)	/* PACKETNUMBER */ +
						sizeof(DWORD)	/* PROTOCOL */ + 
						packetLength;

	// ��Ŷ ���̰� �ִ� ���� ���̺��� ��� ����
	if (PacketLength >= MAX_BUFFER_LENGTH)
		return FALSE;

	// ��Ŷ ��ȣ �ϳ��� ����
	CurrentPacketNumber++;

	// ���� �����͸� �̿��ؼ� ��Ŷ�� ����� ���� ����
	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// ��Ŷ�� ����
	// ��Ŷ�� �ε����� �ٿ� ���� ������ ���־�� ��

	// ��Ŷ ���̸� ����
	CopyMemory(TempBuffer, &PacketLength, sizeof(DWORD));

	// ��Ŷ ��ȣ�� ����
	CopyMemory(TempBuffer + 
				sizeof(DWORD) /* LENGTH */,
				&CurrentPacketNumber, sizeof(DWORD));

	// ���������� ����
	CopyMemory(TempBuffer +
		sizeof(DWORD) /* LENGTH */ +
		sizeof(DWORD) /* PACKETNUMBER */,
		&protocol, sizeof(DWORD));

	// �����͸� ����
	CopyMemory(TempBuffer +
		sizeof(DWORD) /* LENGTH */ +
		sizeof(DWORD) /* PACKETNUMBER */ +
		sizeof(DWORD) /* PROTOCOL */,
		&packet, packetLength);

	// ������ �����͸� Encrypt �Լ��� �̿��ؼ� ��ȣȭ
	Crypt::Encrypt(TempBuffer + sizeof(DWORD), TempBuffer + sizeof(DWORD),
					PacketLength - sizeof(DWORD));

	// WriteQueue�� �̿��ؼ� ��Ŷ�� ���� �Ϸ�Ǿ��� ������ �޸𸮸� �����

	// ť�� �ְ� ��ȯ�Ǵ� �����ʹ� WriteQueue�� ���� �̸� �Ҵ�� �޸� ������
	BYTE* WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	// ť�� ������ �����͸� �־ ������
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

	//  ��Ŷ�� �ε����� �ٿ� ���� ������ �� �־�� �Ѵ�.
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

	// WriteQueue�� �̿��ؼ� ��Ŷ�� ���� �Ϸᰡ �Ǿ������� �޸𸮸� ����д�.
	BYTE *WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return NetworkSession::WriteTo(remoteAddress, remotePort, WriteData, PacketLength);
}

// UDP ���� �ּ� ���͸� ����
BOOL PacketSession::ResetUdp(VOID)
{
	ThreadSync Sync;

	LastReadPacketInfoVectorForUdp.clear();

	return TRUE;
}
