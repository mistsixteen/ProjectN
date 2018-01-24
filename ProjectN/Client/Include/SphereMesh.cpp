#include "stdafx.h"
#include "SphereMesh.h"


HRESULT SphereMesh::Initialize(const TCHAR * path, const TCHAR * fileName)
{
	if (FAILED(D3DXCreateSphere(GET_SINGLE(DXFramework)->GetDevice(), 0.01f, 15, 10, &mesh, NULL))) {
		return E_FAIL;
	}

	// 최소 최대 충돌 지점 설정
	D3DXVECTOR3* vertices;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vertices);

	D3DXComputeBoundingBox(vertices, mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&min, &max);

	mesh->UnlockVertexBuffer();

	return S_OK;
}

HRESULT SphereMesh::CloneMesh(LPD3DXMESH * ppMesh)
{
	D3DVERTEXELEMENT9 decl[MAXD3DDECLLENGTH];
	mesh->GetDeclaration(decl);
	mesh->CloneMesh(mesh->GetOptions(), decl,
		GET_SINGLE(DXFramework)->GetDevice(), ppMesh);
	return S_OK;
}

void SphereMesh::Render()
{
	mesh->DrawSubset(0);
}

SphereMesh::SphereMesh()
{
}


SphereMesh::~SphereMesh()
{
}
