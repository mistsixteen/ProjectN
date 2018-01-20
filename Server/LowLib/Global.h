#pragma once

#include <iostream>
#include <tchar.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <windows.h>
#include <dbghelp.h>
#include <assert.h>
#include <time.h>

#include <vector>
#include <queue>
#include <list>
#include <map>
#include <string>
#include <utility>


using namespace std;

enum IO_TYPE
{
	IO_ACCEPT,
	IO_READ,
	IO_WRITE,
};

typedef struct _OVERLAPPED_EX
{
	OVERLAPPED		Overlapped;		// �� ����ü
	IO_TYPE			IoType;			// �ش� ����ü�� �ϴ� ��
									// IO_TYPE�� ����
									// IO_READ : �б� �۾�
									// IO_WRITE : ���� �۾�
									// IO_ACCEPT : Accept�� �޾��� �� ���
	VOID*			Object;			// �θ� ��ü �ּ�
}OVERLAPPED_EX;

const INT MAX_BUFFER_LENGTH = 4096;

#ifdef _SERVER_SIDE
const INT MAX_QUEUE_LENGTH = 50;
#else
const INT MAX_QUEUE_LENGTH = 500;
#endif

#include "Log.h"