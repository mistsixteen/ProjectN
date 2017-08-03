#include "stdafx.h"
#include "MeshManager.h"


const D3DXVECTOR3 * MeshManager::GetMin(const TCHAR * meshKey)
{
	auto iterMesh = meshMap.find(meshKey);
	if (iterMesh == meshMap.end())
		return NULL;
	return iterMesh->second->GetMin();
}

const D3DXVECTOR3 * MeshManager::GetMax(const TCHAR * meshKey)
{
	auto iterMesh = meshMap.find(meshKey);
	if (iterMesh == meshMap.end())
		return NULL;
	return iterMesh->second->GetMax();
}

HRESULT MeshManager::AddMesh(const TCHAR * meshKey, MESH meshType, const TCHAR * pPath, const TCHAR * pFileName)
{
	auto iterMesh = meshMap.find(meshKey);
	if (iterMesh != meshMap.end())
		return E_FAIL;

	Mesh* mesh = NULL;
	switch (meshType)
	{
	case MESH_TERRAIN:
		mesh = new TerrainMesh;
		if (FAILED(mesh->Initialize()))
			return E_FAIL;
		break;
	}

	meshMap.insert(make_pair(meshKey, mesh));

	return S_OK;
}

HRESULT MeshManager::CloneMesh(const TCHAR * meshKey, LPD3DXMESH* ppMesh)
{
	auto iterMesh = meshMap.find(meshKey);
	if (iterMesh == meshMap.end())
		return  E_FAIL;
	return iterMesh->second->CloneMesh(ppMesh);
}

void MeshManager::Mesh_Render(const TCHAR * meshKey)
{
	auto iterMesh = meshMap.find(meshKey);
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
