#pragma once
/**
* �⺻������ �����Ǵ� rand �Լ�����
* �ӵ��� �̵��� ���� �� ����
*/

class Random
{
private:
	UINT Seed;
public:
	BOOL Init(UINT Seed);

	INT Rand(VOID);
	INT SimpleRandom(VOID);
	INT ComplexRandom(VOID);
public:
	Random();
	virtual ~Random();
};

