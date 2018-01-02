#include "Stream.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"


Stream::Stream()
	:bufferPointer(NULL), length(0)
{
}

Stream::~Stream()
{
}

BOOL Stream::SetBuffer(BYTE * buffer)
{
	if (!buffer)
		return FALSE;
	this->bufferPointer = buffer;
	this->length = 0;

	return TRUE;
}

BOOL Stream::ReadInt32(INT * data)
{
	// 지정된 버퍼에 값을 복제
	CopyMemory(data, bufferPointer + this->length, sizeof(INT));

	// 전체 길이를 데이터 길이만큼 더함
	this->length += sizeof(INT);

	return TRUE;
}

BOOL Stream::ReadDWORD(DWORD * data)
{
	// 지정된 버퍼에 값을 복제
	CopyMemory(data, bufferPointer + this->length, sizeof(DWORD));

	// 전체 길이를 데이터 길이만큼 더함
	this->length += sizeof(DWORD);

	return TRUE;
}

BOOL Stream::ReadDWORD_PTR(DWORD_PTR * data)
{
	// 지정된 버퍼에 값을 복제
	CopyMemory(data, bufferPointer + this->length, sizeof(DWORD_PTR));

	// 전체 길이를 데이터 길이만큼 더함
	this->length += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL Stream::ReadByte(BYTE * data)
{
	// 지정된 버퍼에 값을 복제
	CopyMemory(data, bufferPointer + this->length, sizeof(BYTE));

	// 전체 길이를 데이터 길이만큼 더함
	this->length += sizeof(BYTE);

	return TRUE;
}

BOOL Stream::ReadBytes(BYTE * data, DWORD length)
{
	// 지정된 버퍼에 값을 복제
	CopyMemory(data, bufferPointer + this->length, length);

	// 전체 길이를 데이터 길이만큼 더함
	this->length += length;

	return TRUE;
}

BOOL Stream::ReadFloat(FLOAT * data)
{
	// 지정된 버퍼에 값을 복제
	CopyMemory(data, bufferPointer + this->length, sizeof(FLOAT));

	// 전체 길이를 데이터 길이만큼 더함
	this->length += sizeof(FLOAT);

	return TRUE;
}

BOOL Stream::ReadInt64(INT64 * data)
{
	// 지정된 버퍼에 값을 복제
	CopyMemory(data, bufferPointer + this->length, sizeof(INT64));

	// 전체 길이를 데이터 길이만큼 더함
	this->length += sizeof(INT64);

	return TRUE;
}

BOOL Stream::ReadUSHORT(USHORT * data)
{
	// 지정된 버퍼에 값을 복제
	CopyMemory(data, bufferPointer + this->length, sizeof(USHORT));

	// 전체 길이를 데이터 길이만큼 더함
	this->length += sizeof(USHORT);

	return TRUE;
}

BOOL Stream::ReadSHORT(SHORT * data)
{
	// 지정된 버퍼에 값을 복제
	CopyMemory(data, bufferPointer + this->length, sizeof(SHORT));

	// 전체 길이를 데이터 길이만큼 더함
	this->length += sizeof(SHORT);

	return TRUE;
}

BOOL Stream::ReadBOOL(BOOL * data)
{
	// 지정된 버퍼에 값을 복제
	CopyMemory(data, bufferPointer + this->length, sizeof(BOOL));

	// 전체 길이를 데이터 길이만큼 더함
	this->length += sizeof(BOOL);

	return TRUE;
}

BOOL Stream::ReadWCHAR(WCHAR * data)
{
	memcpy(data, bufferPointer + this->length, sizeof(WCHAR));

	this->length += sizeof(WCHAR);

	return TRUE;
}

BOOL Stream::ReadWCHARs(LPWSTR data, DWORD length)
{
	memcpy(data, bufferPointer + this->length, length * sizeof(WCHAR));

	this->length += length * sizeof(WCHAR);

	return TRUE;
}

BOOL Stream::WriteInt32(INT * data)
{
	CopyMemory(bufferPointer + this->length, &data, sizeof(INT));

	this->length += sizeof(INT);

	return TRUE;
}

BOOL Stream::WriteDWORD(DWORD * data)
{
	CopyMemory(bufferPointer + this->length, &data, sizeof(DWORD));

	this->length += sizeof(DWORD);

	return TRUE;
}

BOOL Stream::WriteDWORD_PTR(DWORD_PTR data)
{
	CopyMemory(bufferPointer + this->length, &data, sizeof(DWORD_PTR));

	this->length += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL Stream::WriteByte(BYTE * data)
{
	CopyMemory(bufferPointer + this->length, &data, sizeof(BYTE));

	this->length += sizeof(BYTE);

	return TRUE;
}

BOOL Stream::WriteBytes(BYTE * data, DWORD length)
{
	CopyMemory(bufferPointer + this->length, &data, length);

	this->length += length;

	return TRUE;
}

BOOL Stream::WriteFloat(FLOAT * data)
{
	CopyMemory(bufferPointer + this->length, &data, sizeof(FLOAT));

	this->length += sizeof(FLOAT);

	return TRUE;
}

BOOL Stream::WriteInt64(INT64 * data)
{
	CopyMemory(bufferPointer + this->length, &data, sizeof(INT64));

	this->length += sizeof(INT64);

	return TRUE;
}

BOOL Stream::WriteUSHORT(USHORT * data)
{
	CopyMemory(bufferPointer + this->length, &data, sizeof(USHORT));

	this->length += sizeof(USHORT);

	return TRUE;
}

BOOL Stream::WriteSHORT(SHORT * data)
{
	CopyMemory(bufferPointer + this->length, &data, sizeof(SHORT));

	this->length += sizeof(SHORT);

	return TRUE;
}

BOOL Stream::WriteBOOL(BOOL * data)
{
	CopyMemory(bufferPointer + this->length, &data, sizeof(BOOL));

	this->length += sizeof(BOOL);

	return TRUE;
}