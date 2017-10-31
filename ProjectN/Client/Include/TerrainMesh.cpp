#include "stdafx.h"
#include "TerrainMesh.h"


HRESULT TerrainMesh::Initialize(const TCHAR* path, const TCHAR* fileName)
{
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	vtxCnt = VTXCNTX * VTXCNTZ;
	idxCnt = (VTXCNTX - 1) * (VTXCNTZ - 1) * 2;

	// 버텍스 버퍼 초기화
	VTXTEX* vertices = new VTXTEX[vtxCnt];
	for (int z = 0; z < VTXCNTZ; ++z)
	{
		for (int x = 0; x < VTXCNTX; ++x)
		{
			int idx = (z * VTXCNTX) + x;
			vertices[idx].position = D3DXVECTOR3(float(x * VTXGAP), 0, float(z * VTXGAP));
			vertices[idx].texture = D3DXVECTOR2(float(x) / (VTXCNTX - 1) , float(z) / (VTXCNTZ - 1));
		}
	}

	// 인덱스 버퍼 초기화
	INDEX* indices = new INDEX[idxCnt];
	D3DXVECTOR3 dest, src, result;
	int triCnt = 0;
	for (int z = 0; z < VTXCNTZ - 1; ++z)
	{
		for (int x = 0; x < VTXCNTX - 1; ++x)
		{
			int idx = (z * VTXCNTX) + x;

			//우측 상단
			indices[triCnt]._1 = VTXCNTX + idx;
			indices[triCnt]._2 = VTXCNTX + 1 + idx;
			indices[triCnt]._3 = idx + 1;

			dest = vertices[indices[triCnt]._2].position
				- vertices[indices[triCnt]._1].position;

			src = vertices[indices[triCnt]._3].position
				- vertices[indices[triCnt]._1].position;

			D3DXVec3Cross(&result, &dest, &src);
			D3DXVec3Normalize(&result, &result);

			vertices[indices[triCnt]._1].normal += result;
			vertices[indices[triCnt]._2].normal += result;
			vertices[indices[triCnt]._3].normal += result;

			++triCnt;

			//좌측 하단
			indices[triCnt]._1 = VTXCNTX + idx;
			indices[triCnt]._2 = idx + 1;
			indices[triCnt]._3 = idx;

			dest = vertices[indices[triCnt]._2].position
				- vertices[indices[triCnt]._1].position;
			src = vertices[indices[triCnt]._3].position
				- vertices[indices[triCnt]._1].position;

			D3DXVec3Cross(&result, &dest, &src);
			D3DXVec3Normalize(&result, &result);

			vertices[indices[triCnt]._1].normal += result;
			vertices[indices[triCnt]._2].normal += result;
			vertices[indices[triCnt]._3].normal += result;

			++triCnt;
		}
	}

	for (int i = 0; i < vtxCnt; ++i)
		D3DXVec3Normalize(&vertices[i].normal, &vertices[i].normal);

	// 메쉬 생성
	D3DXCreateMesh(idxCnt * 3, vtxCnt, D3DXMESH_MANAGED | D3DXMESH_32BIT, 
					decl, GET_SINGLE(DXFramework)->GetDevice(), &mesh);

	LPVOID pData = NULL;
	mesh->LockVertexBuffer(0, (void**)&pData);
	memcpy(pData, vertices, sizeof(VTXTEX) * vtxCnt);
	mesh->UnlockVertexBuffer();

	mesh->LockIndexBuffer(0, (void**)&pData);
	memcpy(pData, indices, sizeof(INDEX) * idxCnt);
	mesh->UnlockIndexBuffer();

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	// 최소 최대 지점 설정
	D3DXVECTOR3* vtx = NULL;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtx);
	D3DXComputeBoundingBox(vtx, mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&min, &max);
	mesh->UnlockVertexBuffer();

	return S_OK;
}

HRESULT TerrainMesh::CloneMesh(LPD3DXMESH* ppMesh)
{
	D3DVERTEXELEMENT9 decl[MAXD3DDECLLENGTH];
	mesh->GetDeclaration(decl);
	mesh->CloneMesh(mesh->GetOptions(), decl,
					GET_SINGLE(DXFramework)->GetDevice(), ppMesh);
	return S_OK;
}

void TerrainMesh::Render()
{
	mesh->DrawSubset(0);
}

TerrainMesh::TerrainMesh()
{
}


TerrainMesh::~TerrainMesh()
{
}
