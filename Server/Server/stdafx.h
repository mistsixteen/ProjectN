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

#include "ConnectedSession.h"
#include "ConnectedSessionManager.h"
#include "ServerIocp.h"