#pragma once
/**
* UnHandled exception���� ����� ���α׷��� ������ ��� �ִ� ������ �ǹ�
* ���α׷��� ����� ����� �޸� ������ ���� �������� ��������
* �� ������ �̿��� ���α׷��� ����� ������ �м��ϰ� ���� UnHandled exception�� ������ �� �ְ� ��
*/

class MiniDump
{
public:
	static BOOL Begin(VOID);
	static BOOL End(VOID);
};

