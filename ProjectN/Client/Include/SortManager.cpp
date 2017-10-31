#include "stdafx.h"
#include "SortManager.h"

void SortManager::Insert(GameObject* object)
{
	sortVector.push_back(object);
}

void SortManager::Render()
{
	sort(sortVector.begin(), sortVector.end());
	for (auto iter = sortVector.begin(); iter != sortVector.end(); ++iter)
		(*iter)->Render();
	sortVector.clear();
}

void SortManager::Release()
{
	for (auto iter = sortVector.begin(); iter != sortVector.end(); ++iter)
		SAFE_DELETE(*iter);
	sortVector.clear();
}

SortManager::SortManager()
{
	
}

SortManager::~SortManager()
{
	Release();
}
