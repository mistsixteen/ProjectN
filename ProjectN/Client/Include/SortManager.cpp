#include "stdafx.h"
#include "SortManager.h"


void SortManager::Insert(GameObject* object)
{
	sortList.push_back(object);
}

void SortManager::Render()
{
	sortList.sort();
	for (auto iter = sortList.begin(); iter != sortList.end(); ++iter)
		(*iter)->Render();
	sortList.clear();
}

void SortManager::Release()
{
	for (auto iter = sortList.begin(); iter != sortList.end(); ++iter)
		SAFE_DELETE(*iter);
	sortList.clear();
}

SortManager::SortManager()
{
	
}

SortManager::~SortManager()
{
	Release();
}
