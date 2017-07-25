#include "stdafx.h"
#include "ObjMgr.h"

CObjMgr::CObjMgr(void)
	:hashTable(20)
{
}


CObjMgr::~CObjMgr(void)
{
	Release();
}

void CObjMgr::InsertObject(const TCHAR* objectKey, CObj* object)
{
	hashTable.Insert(objectKey, object);
}

void CObjMgr::Progress(void)
{
	for (int i = 0; i < hashTable.Size(); ++i)
	{
		if (!hashTable[i]->empty())
		{
			for (list<list<tagDataSet<CObj*>*>*>::iterator iter = hashTable[i]->begin();
			iter != hashTable[i]->end(); ++iter)
			{
				for (list<tagDataSet<CObj*>*>::iterator iter1 = (*iter)->begin();
				iter1 != (*iter)->end(); ++iter1)
				{
					(*iter1)->Value->Progress();
				}
			}

		}
	}

}

void CObjMgr::Render(void)
{
	for (int i = 0; i < hashTable.Size(); ++i)
	{
		if (!hashTable[i]->empty())
		{
			for (list<list<tagDataSet<CObj*>*>*>::iterator iter = hashTable[i]->begin();
			iter != hashTable[i]->end(); ++iter)
			{
				for (list<tagDataSet<CObj*>*>::iterator iter1 = (*iter)->begin();
				iter1 != (*iter)->end(); ++iter1)
				{
					(*iter1)->Value->Render();
				}
			}

		}
	}
}

void CObjMgr::Release(void)
{
	hashTable.Release();
}