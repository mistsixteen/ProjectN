#include "stdafx.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager(void)
	:hashTable(20)
{
}


ObjectManager::~ObjectManager(void)
{
	Release();
}

void ObjectManager::InsertObject(const TCHAR* objectKey, GameObject* object)
{
	hashTable.Insert(objectKey, object);
}

void ObjectManager::Progress(void)
{
	for (int i = 0; i < hashTable.Size(); ++i)
	{
		if (!hashTable[i]->empty())
		{
			for (list<list<tagDataSet<GameObject*>*>*>::iterator iter = hashTable[i]->begin();
			iter != hashTable[i]->end(); ++iter)
			{
				for (list<tagDataSet<GameObject*>*>::iterator iter1 = (*iter)->begin();
				iter1 != (*iter)->end(); ++iter1)
				{
					(*iter1)->Value->Progress();
				}
			}

		}
	}

}

void ObjectManager::Render(void)
{
	for (int i = 0; i < hashTable.Size(); ++i)
	{
		if (!hashTable[i]->empty())
		{
			for (list<list<tagDataSet<GameObject*>*>*>::iterator iter = hashTable[i]->begin();
			iter != hashTable[i]->end(); ++iter)
			{
				for (list<tagDataSet<GameObject*>*>::iterator iter1 = (*iter)->begin();
				iter1 != (*iter)->end(); ++iter1)
				{
					(*iter1)->Value->Render();
				}
			}

		}
	}
}

void ObjectManager::Release(void)
{
	hashTable.Release();
}