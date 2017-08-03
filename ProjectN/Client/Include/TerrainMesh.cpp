#include "stdafx.h"
#include "TerrainMesh.h"

HRESULT TerrainMesh::Initialize(const TCHAR * path, const TCHAR * fileName)
{
	// 버텍스 데이터 입력
	D3DVERTEXELEMENT9 vertexDecl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	int vertexCount = TERRAINX * TERRAINZ;
	int triangleCount = (TERRAINX - 1) * (TERRAINZ - 1) * 2;

	VTXTEX* vertices = new VTXTEX[vertexCount];
	for (int z = 0; z < TERRAINZ; ++z)
	{
		for (int x = 0; x < TERRAINX; ++x)
		{
			int idx = (z * TERRAINX) + x; 
			vertices[idx].vPos = D3DXVECTOR3(float(x * TERRAINGAP), 0.f, float(z * TERRAINGAP));
			vertices[idx].vTexture = D3DXVECTOR2(float(x) / (TERRAINX - 1), float(z) / (TERRAINZ - 1));
		}
	}

	INDEX*	indices = new INDEX[triangleCount];
	int triCnt = 0;
	D3DXVECTOR3 dest, src, result;
	for (int z = 0; z < TERRAINZ - 1; ++z)
	{
		for (int x = 0; x < TERRAINX - 1; ++x)
		{
			int idx = (z * TERRAINX) + x;

			//우측 상단
			indices[triCnt]._1 = TERRAINX + idx;
			indices[triCnt]._2 = TERRAINX + 1 + idx;
			indices[triCnt]._3 = idx + 1;

			dest = vertices[indices[triCnt]._2].vPos
				- vertices[indices[triCnt]._1].vPos;

			src = vertices[indices[triCnt]._3].vPos
				- vertices[indices[triCnt]._1].vPos;
			D3DXVec3Cross(&result, &dest, &src);
			D3DXVec3Normalize(&result, &result);
			vertices[indices[triCnt]._1].vNormal += result;
			vertices[indices[triCnt]._2].vNormal += result;
			vertices[indices[triCnt]._3].vNormal += result;
			++triCnt;

			//좌측 하단
			indices[triCnt]._1 = TERRAINX + idx;
			indices[triCnt]._2 = idx + 1;
			indices[triCnt]._3 = idx;

			dest = vertices[indices[triCnt]._2].vPos
				- vertices[indices[triCnt]._1].vPos;
			src = vertices[indices[triCnt]._3].vPos
				- vertices[indices[triCnt]._1].vPos;
			D3DXVec3Cross(&result, &dest, &src);
			D3DXVec3Normalize(&result, &result);
			vertices[indices[triCnt]._1].vNormal += result;
			vertices[indices[triCnt]._2].vNormal += result;
			vertices[indices[triCnt]._3].vNormal += result;
			++triCnt;
		}
	}

	for (int i = 0; i < vertexCount; ++i)
		D3DXVec3Normalize(&vertices[i].vNormal, &vertices[i].vNormal);

	D3DXCreateMesh(triangleCount, vertexCount,
					D3DPOOL_MANAGED | D3DXMESH_32BIT, vertexDecl,
					device, &mesh);

	LPVOID pData;
	mesh->LockVertexBuffer(0, (void**)&pData);
	memcpy(pData, vertices, sizeof(VTXTEX) * vertexCount);
	mesh->UnlockVertexBuffer();

	mesh->LockIndexBuffer(0, (void**)&pData);
	memcpy(pData, indices, sizeof(INDEX) * triangleCount);
	mesh->UnlockIndexBuffer();

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	return S_OK;
}

HRESULT TerrainMesh::CloneMesh(LPD3DXMESH* ppMesh)
{
	D3DVERTEXELEMENT9 vertexDecl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	mesh->CloneMesh(mesh->GetOptions(), vertexDecl, device, ppMesh);
	if (mesh == *ppMesh)
		return S_OK;
	return S_OK;
}

void TerrainMesh::Mesh_Render()
{
	mesh->DrawSubset(0);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
}

TerrainMesh::TerrainMesh()
{
}


TerrainMesh::~TerrainMesh()
{
}
