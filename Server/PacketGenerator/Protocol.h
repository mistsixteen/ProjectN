#pragma once
// PT_CHANNEL_NICKNAME는 USER_ID라는 32크기의 WCHAR 문자열과
// NICK_NAME이라는 32크기의 WCHAR 문자열의 내용을 가지고 있는
// 프로토콜을 의미
// 
// 단순히 프로토콜을 정의하고 거기에 들어갈 내용을 주석 처리 해 놓은 것 같으나
// 아래의 문법대로 사용하면 Packet Generator를 통해서 편하게 프로토콜 처리가 가능
// 
// 주의 사항으로 프로토콜에 딸린 파라미터를 정의할 땐 ','를 쓰면 안됨
// 현재 코드에서 ,로 체크하는 부분이 들어 있기 때문


typedef enum TCP_PROTOCOL
{
	PT_VERSION = 0x100000,

	PT_CHANNEL_NICKNAME,
	/*
	WCHAR		:	USER_ID[32]
	WCHAR		:	NICK_NAME[32]
	*/
	PT_CHANNEL_NICKNAME_SUCC_U,
	/*
	WCHAR		:	USER_ID[32]
	WCHAR		:	NICK_NAME[32]
	*/
	PT_CHANNEL_NICKNAME_FAIL_U,
	/*
	DWORD		:	ERROR_CODE
	*/
	PT_END
};