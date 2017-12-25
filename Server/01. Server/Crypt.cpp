#include "stdafx.h"
#include "Crypt.h"

const INT C1 = 52845;
const INT C2 = 22719;
const INT KEY = 72957;

BOOL Crypt::Encrypt(BYTE * source, BYTE * destination, DWORD length)
{
	INT Key = KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	// 바이트 단위로 암호화
	for (DWORD i = 0; i < length; ++i)
	{
		// 1바이트를 Key 값만큼 제곱하고 8 비트 밀어줌
		destination[i] = source[i] ^ Key >> 8;
		// 다음바이트에서는 사용할 키를 C1, C2를 이용해 변경
		Key = (destination[i] + Key) * C1 + C2;
	}

	return TRUE;
}

BOOL Crypt::Decrypt(BYTE * source, BYTE * destination, DWORD length)
{
	BYTE previousBlock;
	INT Key = KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	// 바이트 단위로 암호화
	for (DWORD i = 0; i < length; ++i)
	{
		// 이전 블록을 저장해 두었다 복호화에 사용되는 키를 알아냄
		previousBlock = source[i];
		destination[i] = source[i] ^ Key >> 8;
		Key = (previousBlock + Key) * C1 + C2;
	}

	return TRUE;
}
