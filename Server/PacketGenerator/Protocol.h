#pragma once
// PT_CHANNEL_NICKNAME�� USER_ID��� 32ũ���� WCHAR ���ڿ���
// NICK_NAME�̶�� 32ũ���� WCHAR ���ڿ��� ������ ������ �ִ�
// ���������� �ǹ�
// 
// �ܼ��� ���������� �����ϰ� �ű⿡ �� ������ �ּ� ó�� �� ���� �� ������
// �Ʒ��� ������� ����ϸ� Packet Generator�� ���ؼ� ���ϰ� �������� ó���� ����
// 
// ���� �������� �������ݿ� ���� �Ķ���͸� ������ �� ','�� ���� �ȵ�
// ���� �ڵ忡�� ,�� üũ�ϴ� �κ��� ��� �ֱ� ����


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