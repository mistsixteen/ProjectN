#include "stdafx.h"
#include "MeshManager.h"


const D3DXVECTOR3 * MeshManager::GetMin(const TCHAR * mapKey)
{
	auto iterMesh = meshMap.find(mapKey);
	if (iterMesh == meshMap.end())
		return NULL;
	return iterMesh->second->GetMin();
}

const D3DXVECTOR3 * MeshManager::GetMax(const TCHAR * mapKey)
{
	auto iterMesh = meshMap.find(mapKey);
	if (iterMesh == meshMap.end())
		return NULL;
	return iterMesh->second->GetMax();
}

HRESULT MeshManager::AddMesh(const TCHAR * mapKey, const TCHAR * meshKey)
{
	auto iterMesh = meshMap.find(mapKey);
	if (iterMesh != meshMap.end())
		return E_FAIL;

	Mesh* mesh = NULL;
	if (_tcscmp(meshKey, L"Ground") == 0)
	{
		mesh = new TerrainMesh;
		if (FAILED(mesh->Initialize(129, 129, 1)))
			return E_FAIL;
	}
	if (_tcscmp(meshKey, L"Box") == 0)
	{
		mesh = new BoxMesh;
		if (FAILED(mesh->Initialize(129, 129, 1)))
			return E_FAIL;
	}

	meshMap.insert(make_pair(mapKey, mesh));
	return S_OK;
}

HRESULT MeshManager::CloneMesh(const TCHAR * mapKey, LPD3DXMESH* ppMesh)
{
	auto iterMesh = meshMap.find(mapKey);
	if (iterMesh == meshMap.end())
		return  E_FAIL;
	return iterMesh->second->CloneMesh(ppMesh);
}

void MeshManager::Mesh_Render(const TCHAR * mapKey)
{
	auto iterMesh = meshMap.find(mapKey);
	if (iterMesh == meshMap.end())
		return;
	iterMesh->second->Mesh_Render();
}

void MeshManager::Release()
{
	for (auto iterMesh = meshMap.begin();
		iterMesh != meshMap.end(); ++iterMesh)
	{
		SAFE_DELETE(iterMesh->second);
	}
	meshMap.clear();
}

MeshManager::MeshManager()
{
}


MeshManager::~MeshManager()
{
	Release();
}
