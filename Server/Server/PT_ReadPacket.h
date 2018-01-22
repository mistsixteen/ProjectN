#pragma once

inline VOID READ_PT_REG_USER(BYTE* buffer, S_PT_REG_USER &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->ReadWCHARs(parameter.USER_ID, 32);
	stream->ReadWCHARs(parameter.USER_NAME, 32);
	stream->ReadDWORD(&parameter.AGE);
	stream->ReadByte(&parameter.SEX);
	stream->ReadWCHARs(parameter.ADDRESS, 32);
}

inline VOID READ_PT_QUERY_USER(BYTE* buffer, S_PT_QUERY_USER &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->ReadWCHARs(parameter.USER_ID, 32);
}

inline VOID READ_PT_QUERY_USER_RESULT(BYTE* buffer, S_PT_QUERY_USER_RESULT &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->ReadWCHARs(parameter.USER_ID, 32);
	stream->ReadWCHARs(parameter.USER_NAME, 32);
	stream->ReadDWORD(&parameter.AGE);
	stream->ReadByte(&parameter.SEX);
	stream->ReadWCHARs(parameter.ADDRESS, 32);
}

inline VOID READ_PT_REG_COMPUTER(BYTE* buffer, S_PT_REG_COMPUTER &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->ReadWCHARs(parameter.COMPUTER_NAME, 32);
	stream->ReadWCHARs(parameter.IP_ADDRESS, 32);
	stream->ReadByte(&parameter.CPU_TYPE);
	stream->ReadDWORD(&parameter.RAM);
	stream->ReadDWORD(&parameter.HDD);
}

inline VOID READ_PT_QUERY_COMPUTER(BYTE* buffer, S_PT_QUERY_COMPUTER &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->ReadWCHARs(parameter.COMPUTER_NAME, 32);
}

inline VOID READ_PT_QUERY_COMPUTER_RESULT(BYTE* buffer, S_PT_QUERY_COMPUTER_RESULT &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->ReadWCHARs(parameter.COMPUTER_NAME, 32);
	stream->ReadWCHARs(parameter.IP_ADDRESS, 32);
	stream->ReadByte(&parameter.CPU_TYPE);
	stream->ReadDWORD(&parameter.RAM);
	stream->ReadDWORD(&parameter.HDD);
}

inline VOID READ_PT_REG_PROGRAM(BYTE* buffer, S_PT_REG_PROGRAM &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->ReadWCHARs(parameter.PROGRAM_NAME, 32);
	stream->ReadDWORD(&parameter.COST);
	stream->ReadWCHARs(parameter.COMMENT, 32);
}

inline VOID READ_PT_QUERY_PROGRAM(BYTE* buffer, S_PT_QUERY_PROGRAM &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->ReadWCHARs(parameter.PROGRAM_NAME, 32);
}

inline VOID READ_PT_QUERY_PROGRAM_RESULT(BYTE* buffer, S_PT_QUERY_PROGRAM_RESULT &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->ReadWCHARs(parameter.PROGRAM_NAME, 32);
	stream->ReadDWORD(&parameter.COST);
	stream->ReadWCHARs(parameter.COMMENT, 32);
}

