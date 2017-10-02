#include "stdafx.h"
#include "Terrain.h"

HRESULT Terrain::Initialize()
{
	GET_SINGLE(MeshManager)->CloneMesh(L"Terrain",&information.mesh);
	D3DXMatrixIdentity(&information.world);
	return S_OK;
}

void Terrain::Progress()
{
}

void Terrain::Render()
{
	GameObject::Render();
	GET_SINGLE(MeshManager)->Mesh_Render(L"Terrain");
}

void Terrain::Release()
{
}

Terrain::Terrain()
{
}

Terrain::Terrain(const TCHAR* _key, INFO _info)
	:GameObject(_key, _info)
{
}

Terrain::~Terrain()
{
	Release();
}
