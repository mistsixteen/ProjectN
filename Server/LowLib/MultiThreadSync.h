#pragma once
/*
* 사용할 클래스 타입을 확인하기 위해 템플릿으로 제작
* 제작 코드는 모두 개체 단위로 동기화가 이루어짐
* 해당 개체가 MultiThreadSync 클래스를 상속받으면
* 다중 스레드에 대해서 안전하게 된다는 의미
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
			T::Sync.Enter();				// 해당 클래스의 동기화 개체를 Enter
		}

		~ThreadSync(VOID)
		{
			T::Sync.Leave();				// 해당 클래스의 동기화 개체를 Leave
		}
	};

};

template <class T>
CriticalSection MultiThreadSync<T>::Sync;	// 실제 CRITICAL_SECTION 개체