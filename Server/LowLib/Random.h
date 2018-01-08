#pragma once
/**
* 기본적으로 지원되는 rand 함수보다
* 속도의 이득을 얻을 수 있음
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

