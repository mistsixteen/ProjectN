#pragma once

inline DWORD WRITE_PT_CHANNEL_NICKNAME(BYTE* buffer, SPT_CHANNEL_NICKNAME &parameter)
{
	StreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteWCHARs(parameter.NICK_NAME, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_NICKNAME_SUCC_U(BYTE* buffer, SPT_CHANNEL_NICKNAME_SUCC_U &parameter)
{
	StreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteWCHARs(parameter.NICK_NAME, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_NICKNAME_FAIL_U(BYTE* buffer, SPT_CHANNEL_NICKNAME_FAIL_U &parameter)
{
	StreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(&parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_NICKNAME(BYTE* buffer, SPT_CHANNEL_NICKNAME &parameter, WCHAR *user_id, WCHAR *nick_name)
{
	StreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	Stream->WriteWCHARs(_user_id, 32);
	WCHAR _nick_name[32] = {0,};
	_tcsncpy(_nick_name, nick_name, 32);
	Stream->WriteWCHARs(_nick_name, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_NICKNAME_SUCC_U(BYTE* buffer, SPT_CHANNEL_NICKNAME_SUCC_U &parameter, WCHAR *user_id, WCHAR *nick_name)
{
	StreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	Stream->WriteWCHARs(_user_id, 32);
	WCHAR _nick_name[32] = {0,};
	_tcsncpy(_nick_name, nick_name, 32);
	Stream->WriteWCHARs(_nick_name, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_NICKNAME_FAIL_U(BYTE* buffer, SPT_CHANNEL_NICKNAME_FAIL_U &parameter, DWORD error_code)
{
	StreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

