#pragma once

inline VOID READ_//주의사항으로프로토콜에딸린파라미터를정의할땐'(BYTE* buffer, S//주의사항으로프로토콜에딸린파라미터를정의할땐' &parameter	StreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_//현재코드에서(BYTE* buffer, S//현재코드에서 &parameter	StreamSP Stream;
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

