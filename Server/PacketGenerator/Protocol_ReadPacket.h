#pragma once

inline VOID READ_//���ǻ��������������ݿ������Ķ���͸������Ҷ�'(BYTE* buffer, S//���ǻ��������������ݿ������Ķ���͸������Ҷ�' &parameter	StreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_//�����ڵ忡��(BYTE* buffer, S//�����ڵ忡�� &parameter	StreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_CHANNEL_NICKNAME(BYTE* buffer, SPT_CHANNEL_NICKNAME &parameter	StreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadWCHARs(parameter.NICK_NAME, 32);
}

inline VOID READ_PT_CHANNEL_NICKNAME_SUCC_U(BYTE* buffer, SPT_CHANNEL_NICKNAME_SUCC_U &parameter	StreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadWCHARs(parameter.NICK_NAME, 32);
}

inline VOID READ_PT_CHANNEL_NICKNAME_FAIL_U(BYTE* buffer, SPT_CHANNEL_NICKNAME_FAIL_U &parameter	StreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.);
}

