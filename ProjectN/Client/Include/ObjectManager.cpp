#include "stdafx.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager(void)
{
}


ObjectManager::~ObjectManager(void)
{
	Release();
}

void ObjectManager::InsertObject(const TCHAR* objectKey, GameObject* object)
{
	objectMap[objectKey].push_back(object);
}

void ObjectManager::Progress(void)
{
	for(auto iterMap = objectMap.begin() ; iterMap != objectMap.end() ; ++iterMap)
	{ 
		for (auto iterList = (*iterMap).second.begin();
			iterList != (*iterMap).second.end(); ++iterList)
		{
			(*iterList)->Progress();
		}
	}
}

void ObjectManager::Render(void)
{
	for (auto iterMap = objectMap.begin(); iterMap != objectMap.end(); ++iterMap)
	{
		for (auto iterList = (*iterMap).second.begin();
			iterList != (*iterMap).second.end(); ++iterList)
		{
			(*iterList)->Render();
		}
	}
}

void ObjectManager::Release(void)
{
	for (auto iterMap = objectMap.begin(); iterMap != objectMap.end(); ++iterMap)
	{
		for (auto iterList = (*iterMap).second.begin();
			iterList != (*iterMap).second.end(); ++iterList)
		{
			SAFE_DELETE(*iterList);
		}
		(*iterMap).second.clear();
	}
	objectMap.clear();
}

GameObject* ObjectManager::GetGameObject(const TCHAR * objectKey, int count)
{
	int i = 0;
	for (auto iterObj = objectMap[objectKey].begin();
		iterObj != objectMap[objectKey].end(); ++iterObj)
	{
		if (i == count)
			return (*iterObj);
		++i;
	}
	return NULL;
}
