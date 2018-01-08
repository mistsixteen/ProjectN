#pragma once
#include "MultiThreadSync.h"

typedef struct _QUEUE_DATA
{
	VOID	*Object;
	BYTE	Data[MAX_BUFFER_LENGTH];
	DWORD	DataLength;

	CHAR	RemoteAddress[32];
	USHORT	RemotePort;

	DWORD	Protocol;
} QUEUE_DATA;

class CircularQueue :
	public MultiThreadSync<CircularQueue>
{
private:
	QUEUE_DATA	Queue[MAX_QUEUE_LENGTH];
	DWORD		QueueHead;
	DWORD		QueueTail;
public:
	BOOL		Begin(VOID);
	BOOL		End(VOID);

	BYTE*		Push(VOID *object, BYTE *data, DWORD dataLength);
	BYTE*		Push(VOID *object, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);
	BYTE*		Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength);
	BYTE*		Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);

	BOOL		Pop(VOID **object, BYTE *data, DWORD &dataLength);
	BOOL		Pop(VOID **object, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort);
	BOOL		Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength);
	BOOL		Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort);
	BOOL		Pop(VOID);

	BOOL		IsEmpty(VOID);
public:
	CircularQueue(VOID);
	virtual ~CircularQueue(VOID);
};

