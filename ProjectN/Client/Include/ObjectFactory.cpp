#include "stdafx.h"
#include "ObjectFactory.h"

ObjectFactory::ObjectFactory(void)
	:hashTable(20)
{
}


ObjectFactory::~ObjectFactory(void)
{
	Release();
}

void ObjectFactory::InsertObject(const TCHAR* objectKey, GameObject* object)
{
	hashTable.Insert(objectKey, object);
}

void ObjectFactory::Progress(void)
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

void ObjectFactory::Render(void)
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

void ObjectFactory::Release(void)
{
	hashTable.Release();
}