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

	// ����Ʈ ������ ��ȣȭ
	for (DWORD i = 0; i < length; ++i)
	{
		// 1����Ʈ�� Key ����ŭ �����ϰ� 8 ��Ʈ �о���
		destination[i] = source[i] ^ Key >> 8;
		// ��������Ʈ������ ����� Ű�� C1, C2�� �̿��� ����
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

	// ����Ʈ ������ ��ȣȭ
	for (DWORD i = 0; i < length; ++i)
	{
		// ���� ����� ������ �ξ��� ��ȣȭ�� ���Ǵ� Ű�� �˾Ƴ�
		previousBlock = source[i];
		destination[i] = source[i] ^ Key >> 8;
		Key = (previousBlock + Key) * C1 + C2;
	}

	return TRUE;
}
