#include "stdafx.h"
#include "Terrain.h"

HRESULT Terrain::Initialize()
{
	GET_SINGLE(MeshManager)->CloneMesh(L"Terrain",&mesh);
	D3DXMatrixIdentity(&information.world);

	return S_OK;
}

void Terrain::Progress()
{
	D3DXMATRIX scale, rotX, rotY, rotZ, trans;

	D3DXMatrixScaling(&scale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationX(&rotX, information.direction.x);
	D3DXMatrixRotationY(&rotY, information.direction.y);
	D3DXMatrixRotationZ(&rotZ, information.direction.z);
	D3DXMatrixTranslation(&trans, information.position.x, information.position.y, information.position.z);
	information.world = scale * rotX * rotY * rotZ * trans;

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

Terrain::Terrain(const TCHAR* _key, INFO _info)
	:GameObject(_key, _info)
{
}

Terrain::~Terrain()
{
	Release();
}
