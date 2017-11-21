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

void MeshManager::CopyVertexTextureInfo(const TCHAR * mapKey, VTXTEX * vtxTex)
{
	auto iterMesh = meshMap.find(mapKey);
	if (iterMesh == meshMap.end())
		return ;
	iterMesh->second->CopyVertexTextureInfo(vtxTex);
}

void MeshManager::PasteVertexTextureInfo(const TCHAR * mapKey, VTXTEX * vtxTex)
{
	auto iterMesh = meshMap.find(mapKey);
	if (iterMesh == meshMap.end())
		return ;
	iterMesh->second->PasteVertexTextureInfo(vtxTex);
}

HRESULT MeshManager::AddMesh(const TCHAR * mapKey, const TCHAR * meshKey)
{
	auto iterMap = meshMap.find(mapKey);
	if (iterMap != meshMap.end())
		return E_FAIL;

	Mesh* mesh = NULL;
	if (_tcscmp(meshKey, L"Terrain") == 0)
	{
		mesh = new TerrainMesh;
		if (FAILED(mesh->Initialize()))
			return E_FAIL;
	}
	else if (_tcscmp(meshKey, L"Box") == 0)
	{
		mesh = new BoxMesh;
		if (FAILED(mesh->Initialize()))
			return E_FAIL;
	}
	else if (_tcscmp(meshKey, L"Sphere") == 0)
	{
		mesh = new SphereMesh;
		if (FAILED(mesh->Initialize()))
			return E_FAIL;
	}

	meshMap.insert(make_pair(mapKey, mesh));
	return S_OK;
}

HRESULT MeshManager::CloneMesh(const TCHAR * mapKey, LPD3DXMESH* ppMesh)
{
	auto iterMap = meshMap.find(mapKey);
	if (iterMap == meshMap.end())
		return  E_FAIL;
	return iterMap->second->CloneMesh(ppMesh);
}

void MeshManager::Render(const TCHAR * mapKey)
{
	auto iterMap = meshMap.find(mapKey);
	if (iterMap == meshMap.end())
		return;
	iterMap->second->Render();
}

void MeshManager::Release()
{
	for (auto iterMap = meshMap.begin();
		iterMap != meshMap.end(); ++iterMap)
	{
		SAFE_DELETE(iterMap->second);
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
