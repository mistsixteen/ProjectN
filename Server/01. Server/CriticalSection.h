#pragma once

class CriticalSection
{
private:
	CRITICAL_SECTION	Sync;					// CRITICAL_SECTION 개체
public:
	inline VOID Enter(VOID)
	{
		EnterCriticalSection(&Sync);
	}

	inline VOID Leave(VOID)
	{
		LeaveCriticalSection(&Sync);
	}
public:
	CriticalSection(VOID)
	{
		InitializeCriticalSection(&Sync);		// CRITICAL_SECTION 개체 초기화
	}

	~CriticalSection(VOID)
	{
		DeleteCriticalSection(&Sync);			// CRITICAL_SECTION 개체 삭제
	}
};