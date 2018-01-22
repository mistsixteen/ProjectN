#include "stdafx.h"
#include "Random.h"

// �⺻�� �Ǵ� ������ �־���
BOOL Random::Init(UINT Seed)
{
	this->Seed = Seed;

	return TRUE;
}

// �Ϲ����� ���� �޾ƿ�
INT Random::Rand(VOID)
{
	return (((this->Seed = this->Seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

// ����� ������ �ؼ� ���� �ּ����� Random ���� �޾ƿ�
INT Random::SimpleRandom(VOID)
{
	return (((this->Seed += 2531011L) >> 3) & 0x7fff);
}

// ������ ������ ���� Random �� ��ȯ
INT Random::ComplexRandom(VOID)
{
	UINT next = this->Seed;
	INT result;

	next *= 1103515245;
	next += 12345;
	result = (UINT)(next / 65536) % 2048;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (UINT)(next / 65536) % 1024;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (UINT)(next / 65536) % 1024;

	this->Seed = next;
	return result;
}

Random::Random()
{
}


Random::~Random()
{
}
