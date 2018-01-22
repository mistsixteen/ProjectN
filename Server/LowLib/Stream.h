#pragma once
#include "stdafx.h"

class Stream
{
private:
	BYTE*	bufferPointer;
	DWORD	length;
public:
	BOOL SetBuffer(BYTE* buffer);

	BOOL ReadInt32(INT *data);
	BOOL ReadDWORD(DWORD *data);
	BOOL ReadDWORD_PTR(DWORD_PTR *data);
	BOOL ReadByte(BYTE* data);
	BOOL ReadBytes(BYTE* data, DWORD length);
	BOOL ReadFloat(FLOAT* data);
	BOOL ReadInt64(INT64* data);
	BOOL ReadUSHORT(USHORT* data);
	BOOL ReadSHORT(SHORT* data);
	BOOL ReadBOOL(BOOL* data);
	BOOL ReadWCHAR(WCHAR *data);
	BOOL ReadWCHARs(LPWSTR data, DWORD length);

	BOOL WriteInt32(INT *data);
	BOOL WriteDWORD(DWORD data);
	BOOL WriteDWORD_PTR(DWORD_PTR data);
	BOOL WriteByte(BYTE data);
	BOOL WriteBytes(BYTE* data, DWORD length);
	BOOL WriteFloat(FLOAT data);
	BOOL WriteInt64(INT64 data);
	BOOL WriteUSHORT(USHORT data);
	BOOL WriteSHORT(SHORT data);
	BOOL WriteBOOL(BOOL data);
	BOOL WriteWCHAR(WCHAR data);
	BOOL WriteWCHARs(LPCWSTR data, DWORD length);

	DWORD GetLength(VOID) const { return this->length; }
public:
	Stream();
	virtual ~Stream();
};

class StreamSP
{
private:
	Stream*	stream;
public:
	Stream* operator->(VOID) { return stream; }
	operator Stream*(VOID) { return stream; }
public:
	StreamSP()
	{
		stream = new Stream();
	}
	~StreamSP()
	{
		delete stream;
	}
};

