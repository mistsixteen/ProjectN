#include "stdafx.h"
#include "Random.h"

// 기본이 되는 정수를 넣어줌
BOOL Random::Init(UINT Seed)
{
	this->Seed = Seed;

	return TRUE;
}

// 일반적인 값을 받아옴
INT Random::Rand(VOID)
{
	return (((this->Seed = this->Seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

// 계산을 간단히 해서 가장 최소한의 Random 값을 받아옴
INT Random::SimpleRandom(VOID)
{
	return (((this->Seed += 2531011L) >> 3) & 0x7fff);
}

// 복잡한 계산식을 통한 Random 값 반환
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
