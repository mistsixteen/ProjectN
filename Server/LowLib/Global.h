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
	OVERLAPPED		Overlapped;		// 원 구조체
	IO_TYPE			IoType;			// 해당 구조체가 하는 일
									// IO_TYPE의 종류
									// IO_READ : 읽기 작업
									// IO_WRITE : 쓰기 작업
									// IO_ACCEPT : Accept를 받았을 때 사용
	VOID*			Object;			// 부모 개체 주소
}OVERLAPPED_EX;

const INT MAX_BUFFER_LENGTH = 4096;

#ifdef _SERVER_SIDE
const INT MAX_QUEUE_LENGTH = 50;
#else
const INT MAX_QUEUE_LENGTH = 500;
#endif

#include "Log.h"