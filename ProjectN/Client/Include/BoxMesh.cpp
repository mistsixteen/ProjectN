#include "stdafx.h"
#include "BoxMesh.h"


HRESULT BoxMesh::Initialize(const TCHAR * path, const TCHAR * fileName)
{
	if (FAILED(D3DXCreateBox(device, 1.f, 1.f, 1.f, &mesh, NULL)))
		return E_FAIL;

	void* vertices;
	mesh->LockVertexBuffer(0, &vertices);
	
	int vtxCnt = mesh->GetNumVertices();
	DWORD fvf = mesh->GetFVF();
	int vtxSize = D3DXGetFVFVertexSize(fvf);
	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, vtxCnt, vtxSize, &min, &max);

	mesh->UnlockVertexBuffer();

	return S_OK;
}

HRESULT BoxMesh::CloneMesh(LPD3DXMESH * ppMesh)
{
	D3DVERTEXELEMENT9 decl[MAXD3DDECLLENGTH];
	mesh->GetDeclaration(decl);
	mesh->CloneMesh(mesh->GetOptions(), decl, device, ppMesh);
	return S_OK;
}

void BoxMesh::Mesh_Render()
{
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	mesh->DrawSubset(0);
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
}

BoxMesh::BoxMesh()
{
}


BoxMesh::~BoxMesh()
{
}
