#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"


CircularQueue::CircularQueue(VOID)
{
	memset(Queue, 0, sizeof(Queue));

	this->QueueHead = 0;
	this->QueueTail = 0;
}


CircularQueue::~CircularQueue(VOID)
{
}

BOOL CircularQueue::Begin(VOID)
{
	memset(Queue, 0, sizeof(Queue));

	QueueHead = 0;
	QueueTail = 0;

	return TRUE;
}

BOOL CircularQueue::End(VOID)
{
	return TRUE;
}

BYTE * CircularQueue::Push(VOID * object, BYTE * data, DWORD dataLength)
{
	ThreadSync Sync;		// 동기화 개체

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (QueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == QueueHead)
		return NULL;

	Queue[TempTail].Object = object;
	Queue[TempTail].DataLength = dataLength;

	memcpy(Queue[TempTail].Data, data, dataLength);

	QueueTail = TempTail;

	return Queue[TempTail].Data;
}

BYTE * CircularQueue::Push(VOID * object, BYTE * data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort)
{
	ThreadSync Sync;		// 동기화 개체

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (QueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == QueueHead)
		return NULL;

	Queue[TempTail].Object = object;
	Queue[TempTail].DataLength = dataLength;
	Queue[TempTail].RemotePort = remotePort;

	memset(Queue[TempTail].RemoteAddress, 0, sizeof(Queue[TempTail].RemoteAddress));
	strncpy(Queue[TempTail].RemoteAddress, remoteAddress, sizeof(Queue[TempTail].RemoteAddress));

	memset(Queue[TempTail].Data, 0, sizeof(Queue[TempTail].Data));
	memcpy(Queue[TempTail].Data, data, dataLength);

	QueueTail = TempTail;

	return Queue[TempTail].Data;
}

BYTE * CircularQueue::Push(VOID * object, DWORD protocol, BYTE * data, DWORD dataLength)
{
	ThreadSync Sync;		// 동기화 개체

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (QueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == QueueHead)
		return NULL;

	Queue[TempTail].Object = object;
	Queue[TempTail].DataLength = dataLength;
	Queue[TempTail].Protocol = protocol;

	memset(Queue[TempTail].Data, 0, sizeof(Queue[TempTail].Data));
	memcpy(Queue[TempTail].Data, data, dataLength);

	QueueTail = TempTail;

	return Queue[TempTail].Data;
}

BYTE * CircularQueue::Push(VOID * object, DWORD protocol, BYTE * data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort)
{
	ThreadSync Sync;		// 동기화 개체

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (QueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == QueueHead)
		return NULL;

	Queue[TempTail].Object = object;
	Queue[TempTail].DataLength = dataLength;
	Queue[TempTail].RemotePort = remotePort;
	Queue[TempTail].Protocol = protocol;

	memset(Queue[TempTail].RemoteAddress, 0, sizeof(Queue[TempTail].RemoteAddress));
	strncpy(Queue[TempTail].RemoteAddress, remoteAddress, sizeof(Queue[TempTail].RemoteAddress));

	memset(Queue[TempTail].Data, 0, sizeof(Queue[TempTail].Data));
	memcpy(Queue[TempTail].Data, data, dataLength);

	QueueTail = TempTail;

	return Queue[TempTail].Data;
}

BOOL CircularQueue::Pop(VOID ** object, BYTE * data, DWORD & dataLength)
{
	ThreadSync Sync;		// 동기화 개체

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (QueueHead == QueueTail)
		return FALSE;

	DWORD TempHead = (QueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = Queue[TempHead].DataLength;
	*object = Queue[TempHead].Object;

	memcpy(data, Queue[TempHead].Data, Queue[TempHead].DataLength);

	QueueHead = TempHead;

	return TRUE;
}

BOOL CircularQueue::Pop(VOID ** object, BYTE * data, DWORD & dataLength, LPSTR remoteAddress, USHORT & remotePort)
{
	ThreadSync Sync;		// 동기화 개체

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (QueueHead == QueueTail)
		return FALSE;

	DWORD TempHead = (QueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = Queue[TempHead].DataLength;
	*object = Queue[TempHead].Object;
	remotePort = Queue[TempHead].RemotePort;

	strncpy(remoteAddress, Queue[TempHead].RemoteAddress, sizeof(Queue[TempHead].RemoteAddress));
	memcpy(data, Queue[TempHead].Data, Queue[TempHead].DataLength);

	QueueHead = TempHead;

	return TRUE;
}

BOOL CircularQueue::Pop(VOID ** object, DWORD & protocol, BYTE * data, DWORD & dataLength)
{
	ThreadSync Sync;		// 동기화 개체

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (QueueHead == QueueTail)
		return FALSE;

	DWORD TempHead = (QueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = Queue[TempHead].DataLength;
	*object = Queue[TempHead].Object;
	protocol = Queue[TempHead].Protocol;

	memcpy(data, Queue[TempHead].Data, Queue[TempHead].DataLength);

	QueueHead = TempHead;

	return TRUE;
}

BOOL CircularQueue::Pop(VOID ** object, DWORD & protocol, BYTE * data, DWORD & dataLength, LPSTR remoteAddress, USHORT & remotePort)
{
	ThreadSync Sync;		// 동기화 개체

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (QueueHead == QueueTail)
		return FALSE;

	DWORD TempHead = (QueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = Queue[TempHead].DataLength;
	*object = Queue[TempHead].Object;
	remotePort = Queue[TempHead].RemotePort;

	strncpy(remoteAddress, Queue[TempHead].RemoteAddress, sizeof(Queue[TempHead].RemoteAddress));
	memcpy(data, Queue[TempHead].Data, Queue[TempHead].DataLength);

	QueueHead = TempHead;

	return TRUE;
}

BOOL CircularQueue::Pop(VOID)
{
	ThreadSync Sync;		// 동기화 개체

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (QueueHead == QueueTail)
	return FALSE;

	DWORD TempHead = (QueueHead + 1) % MAX_QUEUE_LENGTH;

	QueueHead = TempHead;

	return TRUE;
}

BOOL CircularQueue::IsEmpty(VOID)
{
	ThreadSync Sync;		// 동기화 개체

	if (QueueHead == QueueTail)
		return TRUE;

	return FALSE;
}

