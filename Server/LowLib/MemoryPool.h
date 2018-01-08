#pragma once
/**
* MemoryPool
* 응용 프로그램에서 사용하는 메모리를 미리 할당해 놓고
* 사용자가 new 연산자를 사용하여 개체를 만들 때 미리 할당된 메모리를 제공
* Garbage Collection과 달리 new와 delete를 수동으로 선언해줘야 하는 차이가 있음
* 다중 스레드 환경을 지원하지 않는 단점이 있ㅇ므
*/

template<typename T, int ALLOC_BLOCK_SIZE = 50>
class MemoryPool
	: public MultiThreadSync<T>
{
private:
	static UCHAR*		freePointer;
public:
	static VOID* operator new(size_t allocLength)
	{
		ThreadSync Sync;			// 동기화 개체

		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		// 더 이상 할당할 수 있는 공간이 없으면 새로 할당
		if (!this->freePointer)
			allocBlock();
		UCHAR*	returnPointer = this->freePointer;
		//freePointer에는 리턴하는 블록 앞에 4바이트로 있던 주소가 들어감
		freePointer = &reinterpret_cast<UCHAR**>(returnPointer);

		return returnPointer;
	}
	static VOID operator delete(VOID* deletePointer)
	{
		ThreadSync Sync;			// 동기화 개체

		// delete 된 블록의 Next에 현재 freePointer 주소를 넣어줌
		*reinterpret_cast<UCHAR**>(deletePointer) = this->freePointer;
		
		// freePointer를 업데이트
		freePointer = static_cast<UCHAR*>(deletePointer);
	}
private:
	static VOID allocBlock()
	{
		// freePointer에 사용할 크기의 메모리를 할당
		this->freePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];
		
		// 할당한 메모리의 첫 블록의 포인터를 Current에 넣음
		UCHAR** Current = reinterpret_cast<UCHAR**>(freePointer);
		UCHAR* Next = freePointer;		// 할당된 메모리 첫 블록

		for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);			// 다음 블록을 계산
			*Current = Next;			// 할당된 메모리 앞 4바이트에 다음 블록의 주소를 넣음
			Current = reinterpret_cast<UCHAR**>(Next);
		}

		*Current = 0;
	}
protected:
	virtual ~MemoryPool()
	{
	}
};

template<class T, int ALLOC_BLOCK_SIZE>
UCHAR* MemoryPool<T, ALLOC_BLOCK_SIZE>::freePointer;