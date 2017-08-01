#pragma once
#include "Define.h"
#include "HashTable.h"

#include "GameObject.h"

class ObjectManager
{
	DECLARE_SINGLETON(ObjectManager);
private:
	CHashTable<GameObject*>	hashTable;
public:
	void InsertObject(const TCHAR* objectKey, GameObject* object);
	void Progress(void);
	void Render(void);
	void Release(void);
private:
	ObjectManager();
public:
	~ObjectManager();
};

