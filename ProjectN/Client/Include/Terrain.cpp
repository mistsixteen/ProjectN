#include "stdafx.h"
#include "Terrain.h"

HRESULT Terrain::Initialize()
{
	information.position = D3DXVECTOR3(0.f, 0.f, 0.f);
	information.direction = D3DXVECTOR3(0.f, 0.f, 0.f);
	information.look = D3DXVECTOR3(0.f, 1.f, 0.f);

	GET_SINGLE(MeshManager)->CloneMesh(L"Terrain",&mesh);

	return S_OK;
}

void Terrain::Progress()
{
	device->SetTransform(D3DTS_WORLD, &information.world);
}

void Terrain::Render()
{
	GET_SINGLE(MeshManager)->Mesh_Render(L"Terrain");
}

void Terrain::Release()
{
}

Terrain::Terrain()
{
}

Terrain::Terrain(const TCHAR* _key)
	:GameObject(_key)
{
	ZeroMemory(&information, sizeof(information));
	D3DXMatrixIdentity(&information.world);
}

Terrain::~Terrain()
{
	Release();
}
