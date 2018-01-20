#pragma once
/**
* MemoryPool
* ���� ���α׷����� ����ϴ� �޸𸮸� �̸� �Ҵ��� ����
* ����ڰ� new �����ڸ� ����Ͽ� ��ü�� ���� �� �̸� �Ҵ�� �޸𸮸� ����
* Garbage Collection�� �޸� new�� delete�� �������� ��������� �ϴ� ���̰� ����
* ���� ������ ȯ���� �������� �ʴ� ������ �֤���
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
		ThreadSync Sync;			// ����ȭ ��ü

		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		// �� �̻� �Ҵ��� �� �ִ� ������ ������ ���� �Ҵ�
		if (!this->freePointer)
			allocBlock();
		UCHAR*	returnPointer = this->freePointer;
		//freePointer���� �����ϴ� ��� �տ� 4����Ʈ�� �ִ� �ּҰ� ��
		freePointer = &reinterpret_cast<UCHAR**>(returnPointer);

		return returnPointer;
	}
	static VOID operator delete(VOID* deletePointer)
	{
		ThreadSync Sync;			// ����ȭ ��ü

		// delete �� ����� Next�� ���� freePointer �ּҸ� �־���
		*reinterpret_cast<UCHAR**>(deletePointer) = this->freePointer;
		
		// freePointer�� ������Ʈ
		freePointer = static_cast<UCHAR*>(deletePointer);
	}
private:
	static VOID allocBlock()
	{
		// freePointer�� ����� ũ���� �޸𸮸� �Ҵ�
		this->freePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];
		
		// �Ҵ��� �޸��� ù ����� �����͸� Current�� ����
		UCHAR** Current = reinterpret_cast<UCHAR**>(freePointer);
		UCHAR* Next = freePointer;		// �Ҵ�� �޸� ù ���

		for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);			// ���� ����� ���
			*Current = Next;			// �Ҵ�� �޸� �� 4����Ʈ�� ���� ����� �ּҸ� ����
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