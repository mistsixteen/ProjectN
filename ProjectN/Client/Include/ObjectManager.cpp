#include "stdafx.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager(void)
{
}


ObjectManager::~ObjectManager(void)
{
	Release();
}

void ObjectManager::AddObject(const TCHAR* key, GameObject* object)
{
	objectMap[key].push_back(object);
}

void ObjectManager::Progress(void)
{
	(*objectMap[L"Background"].begin())->Progress();
	for(auto iterList = objectMap[L"Terrain"].begin() ; iterList != objectMap[L"Terrain"].end() ; ++iterList)
		(*iterList)->Progress();

	for(auto iterMap = objectMap.begin() ; iterMap != objectMap.end() ; ++iterMap)
	{ 
		if ((*iterMap).first != L"Terrain" && (*iterMap).first != L"Background")
		{
			for (auto iterList = (*iterMap).second.begin();
				iterList != (*iterMap).second.end(); ++iterList)
			{
				(*iterList)->Progress();
				GET_SINGLE(SortManager)->Insert(*iterList);
			}
		}
	}
}

void ObjectManager::Render(void)
{
	(*objectMap[L"Background"].begin())->Render();
	for (auto iterList = objectMap[L"Terrain"].begin(); iterList != objectMap[L"Terrain"].end(); ++iterList)
		(*iterList)->Render();
	GET_SINGLE(SortManager)->Render();
	/*for (auto iterMap = objectMap.begin(); iterMap != objectMap.end(); ++iterMap)
	{
		for (auto iterList = (*iterMap).second.begin();
			iterList != (*iterMap).second.end(); ++iterList)
		{
			(*iterList)->Render();
		}
	}*/
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
	SAFE_DELETE_SINGLE(SortManager);
}

GameObject* ObjectManager::GetGameObject(const TCHAR * key, int count)
{
	if (objectMap[key].size() == 0)
		return NULL;

	int i = 0;
	for (auto iterObj = objectMap[key].begin();
		iterObj != objectMap[key].end(); ++iterObj)
	{
		if (i == count)
			return (*iterObj);
		++i;
	}

	return NULL;
}


float ObjectManager::GetTerrainHeight(INFO srcInfo)
{
	for (auto iter = objectMap[L"Terrain"].begin();
		iter != objectMap[L"Terrain"].end(); ++iter)
	{
		D3DXVECTOR3 destMin, destMax;
		D3DXVECTOR3 srcMin, srcMax;
		D3DXMATRIX world;
		
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&world, srcInfo.position.x, srcInfo.position.y, srcInfo.position.z);
		D3DXVec3TransformCoord(&srcMin, &srcInfo.min, &world);
		D3DXVec3TransformCoord(&srcMax, &srcInfo.max, &world);

		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&world, (*iter)->GetInfo().position.x, (*iter)->GetInfo().position.y, (*iter)->GetInfo().position.z);
		D3DXVec3TransformCoord(&destMin, &(*iter)->GetInfo().min, &world);
		D3DXVec3TransformCoord(&destMax, &(*iter)->GetInfo().max, &world);

		// 오브젝트 기준 연산
		/*destMin.x <= srcMin.x && srcMax.x <= destMax.x
			&& destMin.z <= srcMin.z && srcMax.z <= destMax.z*/

		// 터레인 내부에 있을 경우 연산
		if (destMin.x <= srcInfo.position.x && srcInfo.position.x <= destMax.x
			&& destMin.z <= srcInfo.position.z && srcInfo.position.z <= destMax.z)
		{
			int index = int((*iter)->GetInfo().position.z / VTXGAP) * VTXCNTX
				+ int((*iter)->GetInfo().position.x / VTXGAP);

			float ratioX = (srcInfo.position.x - (*iter)->GetVtxTex()[index + VTXCNTX].position.x) / VTXGAP;
			float ratioZ = ((*iter)->GetVtxTex()[index + VTXCNTX].position.z - srcInfo.position.z) / VTXGAP;

			float fY[4] = {
				(*iter)->GetVtxTex()[index + VTXCNTX].position.y,			// 좌측 상단
				(*iter)->GetVtxTex()[index + VTXCNTX + 1].position.y,		// 우측 상단
				(*iter)->GetVtxTex()[index + 1].position.y,					// 우측 하단
				(*iter)->GetVtxTex()[index].position.y,						// 좌측 하단
			};

			// 사각형부분에서 좌우 위치에 따른 높이 + 오브젝트 최소 충돌 y 반환
			if (ratioX > ratioZ)
				return fY[0] + (fY[1] - fY[0]) * ratioX + (fY[2] - fY[1]) * ratioZ + abs(srcInfo.min.y);
			else
				return fY[0] + (fY[2] - fY[3]) * ratioX + (fY[3] - fY[0]) * ratioZ + abs(srcInfo.min.y);
		}
	}
	
	return -1.f;
}


