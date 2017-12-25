#pragma once

class CriticalSection
{
private:
	CRITICAL_SECTION	Sync;					// CRITICAL_SECTION ��ü
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
		InitializeCriticalSection(&Sync);		// CRITICAL_SECTION ��ü �ʱ�ȭ
	}

	~CriticalSection(VOID)
	{
		DeleteCriticalSection(&Sync);			// CRITICAL_SECTION ��ü ����
	}
};