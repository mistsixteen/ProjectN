#include "stdafx.h"
#include "SortManager.h"

struct RenderingSort
{
	bool operator()(GameObject* a, GameObject* b)
	{
		return D3DXVec3LengthSq(new D3DXVECTOR3(gCameraEye - a->GetInfo().position)) 
			< D3DXVec3LengthSq(new D3DXVECTOR3(gCameraEye - b->GetInfo().position));
	}
};

void SortManager::Insert(GameObject* object)
{
	sortVector.push_back(object);
}

void SortManager::Render()
{
	sort(sortVector.begin(), sortVector.end(), RenderingSort());
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
