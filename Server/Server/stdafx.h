// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#define MAX_USER 100
#define DEFAULT_PORT 1820

#include "../LowLib/Global.h"

typedef struct _USER
{
	WCHAR UserID[32];
	WCHAR UserName[32];
	DWORD Age;
	BYTE Sex;
	WCHAR Address[32];
}USER;

typedef struct _COMPUTER
{
	WCHAR ComputerName[32];
	WCHAR IPAddress[32];
	BYTE CPUType;
	DWORD Ram;
	DWORD HDD;
} COMPUTER;

typedef struct _PROGRAM
{
	WCHAR ProgramName[32];
	DWORD Cost;
	WCHAR Comment[32];
}PROGRAM;

#include "../LowLib/CriticalSection.h"
#include "../LowLib/MultiThreadSync.h"
#include "../LowLib/MemoryPool.h"
#include "../LowLib/CircularQueue.h"
#include "../LowLib/NetworkSession.h"
#include "../LowLib/PacketSession.h"
#include "../LowLib/Iocp.h"
#include "../LowLib/Stream.h"
#include "../LowLib/MiniDump.h"
#include "../LowLib/IniFile.h"

#include "Protocol.h"
#include "PT_Structure.h"
#include "PT_ReadPacket.h"
#include "PT_WritePacket.h"

#include "ConnectedSession.h"
#include "ConnectedSessionManager.h"
#include "ServerIocp.h"