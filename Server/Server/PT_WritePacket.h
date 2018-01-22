#pragma once

inline DWORD WRITE_PT_REG_USER(BYTE* buffer, S_PT_REG_USER &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->WriteWCHARs(parameter.USER_ID, 32);
	stream->WriteWCHARs(parameter.USER_NAME, 32);
	stream->WriteDWORD(parameter.AGE);
	stream->WriteByte(parameter.SEX);
	stream->WriteWCHARs(parameter.ADDRESS, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_USER(BYTE* buffer, S_PT_QUERY_USER &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->WriteWCHARs(parameter.USER_ID, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_USER_RESULT(BYTE* buffer, S_PT_QUERY_USER_RESULT &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->WriteWCHARs(parameter.USER_ID, 32);
	stream->WriteWCHARs(parameter.USER_NAME, 32);
	stream->WriteDWORD(parameter.AGE);
	stream->WriteByte(parameter.SEX);
	stream->WriteWCHARs(parameter.ADDRESS, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_REG_COMPUTER(BYTE* buffer, S_PT_REG_COMPUTER &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->WriteWCHARs(parameter.COMPUTER_NAME, 32);
	stream->WriteWCHARs(parameter.IP_ADDRESS, 32);
	stream->WriteByte(parameter.CPU_TYPE);
	stream->WriteDWORD(parameter.RAM);
	stream->WriteDWORD(parameter.HDD);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_COMPUTER(BYTE* buffer, S_PT_QUERY_COMPUTER &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->WriteWCHARs(parameter.COMPUTER_NAME, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_COMPUTER_RESULT(BYTE* buffer, S_PT_QUERY_COMPUTER_RESULT &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->WriteWCHARs(parameter.COMPUTER_NAME, 32);
	stream->WriteWCHARs(parameter.IP_ADDRESS, 32);
	stream->WriteByte(parameter.CPU_TYPE);
	stream->WriteDWORD(parameter.RAM);
	stream->WriteDWORD(parameter.HDD);

	return stream->GetLength();
}

inline DWORD WRITE_PT_REG_PROGRAM(BYTE* buffer, S_PT_REG_PROGRAM &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->WriteWCHARs(parameter.PROGRAM_NAME, 32);
	stream->WriteDWORD(parameter.COST);
	stream->WriteWCHARs(parameter.COMMENT, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_PROGRAM(BYTE* buffer, S_PT_QUERY_PROGRAM &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->WriteWCHARs(parameter.PROGRAM_NAME, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_PROGRAM_RESULT(BYTE* buffer, S_PT_QUERY_PROGRAM_RESULT &parameter)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	stream->WriteWCHARs(parameter.PROGRAM_NAME, 32);
	stream->WriteDWORD(parameter.COST);
	stream->WriteWCHARs(parameter.COMMENT, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_REG_USER(BYTE* buffer, WCHAR *user_id, WCHAR *user_name, DWORD age, BYTE sex, WCHAR *address)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	stream->WriteWCHARs(_user_id, 32);
	WCHAR _user_name[32] = {0,};
	_tcsncpy(_user_name, user_name, 32);
	stream->WriteWCHARs(_user_name, 32);
	stream->WriteDWORD(age);
	stream->WriteByte(sex);
	WCHAR _address[32] = {0,};
	_tcsncpy(_address, address, 32);
	stream->WriteWCHARs(_address, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_USER(BYTE* buffer, WCHAR *user_id)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	stream->WriteWCHARs(_user_id, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_USER_RESULT(BYTE* buffer, WCHAR *user_id, WCHAR *user_name, DWORD age, BYTE sex, WCHAR *address)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	stream->WriteWCHARs(_user_id, 32);
	WCHAR _user_name[32] = {0,};
	_tcsncpy(_user_name, user_name, 32);
	stream->WriteWCHARs(_user_name, 32);
	stream->WriteDWORD(age);
	stream->WriteByte(sex);
	WCHAR _address[32] = {0,};
	_tcsncpy(_address, address, 32);
	stream->WriteWCHARs(_address, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_REG_COMPUTER(BYTE* buffer, WCHAR *computer_name, WCHAR *ip_address, BYTE cpu_type, DWORD ram, DWORD hdd)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	WCHAR _computer_name[32] = {0,};
	_tcsncpy(_computer_name, computer_name, 32);
	stream->WriteWCHARs(_computer_name, 32);
	WCHAR _ip_address[32] = {0,};
	_tcsncpy(_ip_address, ip_address, 32);
	stream->WriteWCHARs(_ip_address, 32);
	stream->WriteByte(cpu_type);
	stream->WriteDWORD(ram);
	stream->WriteDWORD(hdd);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_COMPUTER(BYTE* buffer, WCHAR *computer_name)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	WCHAR _computer_name[32] = {0,};
	_tcsncpy(_computer_name, computer_name, 32);
	stream->WriteWCHARs(_computer_name, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_COMPUTER_RESULT(BYTE* buffer, WCHAR *computer_name, WCHAR *ip_address, BYTE cpu_type, DWORD ram, DWORD hdd)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	WCHAR _computer_name[32] = {0,};
	_tcsncpy(_computer_name, computer_name, 32);
	stream->WriteWCHARs(_computer_name, 32);
	WCHAR _ip_address[32] = {0,};
	_tcsncpy(_ip_address, ip_address, 32);
	stream->WriteWCHARs(_ip_address, 32);
	stream->WriteByte(cpu_type);
	stream->WriteDWORD(ram);
	stream->WriteDWORD(hdd);

	return stream->GetLength();
}

inline DWORD WRITE_PT_REG_PROGRAM(BYTE* buffer, WCHAR *program_name, DWORD cost, WCHAR *comment)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	WCHAR _program_name[32] = {0,};
	_tcsncpy(_program_name, program_name, 32);
	stream->WriteWCHARs(_program_name, 32);
	stream->WriteDWORD(cost);
	WCHAR _comment[32] = {0,};
	_tcsncpy(_comment, comment, 32);
	stream->WriteWCHARs(_comment, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_PROGRAM(BYTE* buffer, WCHAR *program_name)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	WCHAR _program_name[32] = {0,};
	_tcsncpy(_program_name, program_name, 32);
	stream->WriteWCHARs(_program_name, 32);

	return stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_PROGRAM_RESULT(BYTE* buffer, WCHAR *program_name, DWORD cost, WCHAR *comment)
{
	StreamSP stream;
	stream->SetBuffer(buffer);

	WCHAR _program_name[32] = {0,};
	_tcsncpy(_program_name, program_name, 32);
	stream->WriteWCHARs(_program_name, 32);
	stream->WriteDWORD(cost);
	WCHAR _comment[32] = {0,};
	_tcsncpy(_comment, comment, 32);
	stream->WriteWCHARs(_comment, 32);

	return stream->GetLength();
}

