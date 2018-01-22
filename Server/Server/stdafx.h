// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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