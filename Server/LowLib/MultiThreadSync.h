#pragma once
/*
* ����� Ŭ���� Ÿ���� Ȯ���ϱ� ���� ���ø����� ����
* ���� �ڵ�� ��� ��ü ������ ����ȭ�� �̷����
* �ش� ��ü�� MultiThreadSync Ŭ������ ��ӹ�����
* ���� �����忡 ���ؼ� �����ϰ� �ȴٴ� �ǹ�
*/

#include "CriticalSection.h"

template <class T>
class MultiThreadSync
{
	friend class ThreadSync;
private:
	static CriticalSection Sync;
public:
	class ThreadSync
	{
	public:
		ThreadSync(VOID)
		{
			T::Sync.Enter();				// �ش� Ŭ������ ����ȭ ��ü�� Enter
		}

		~ThreadSync(VOID)
		{
			T::Sync.Leave();				// �ش� Ŭ������ ����ȭ ��ü�� Leave
		}
	};

};

template <class T>
CriticalSection MultiThreadSync<T>::Sync;	// ���� CRITICAL_SECTION ��ü