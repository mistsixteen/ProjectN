#include "stdafx.h"
#include "TerrainBuffer.h"


HRESULT TerrainBuffer::Initialize()
{
	vertexCnt = VTXCNTX * VTXCNTZ;
	vertexSize = sizeof(VTXTEX);
	vertexFVF = VTXFVF_TEXTURE;
	indexFormat = D3DFMT_INDEX32;
	indexSize = sizeof(INDEX);
	triCnt = (VTXCNTX - 1) * (VTXCNTZ - 1) * 2;

	if (FAILED(Buffer::Initialize()))
	{
		MSGBOX(L"터레인 버퍼 생성실패");
		return E_FAIL;
	}

	VTXTEX* pVtxTex = NULL;
	vertexBuffer->Lock(0, 0, (void**)&pVtxTex, 0);

	for (int z = 0; z < VTXCNTZ; ++z)
	{
		for (int x = 0; x < VTXCNTX; ++x)
		{
			int iIndex = (z * VTXCNTX) + x;
			pVtxTex[iIndex].position = D3DXVECTOR3(float(x * VTXGAP), 0, float(z * VTXGAP));
			pVtxTex[iIndex].texture = D3DXVECTOR2(float(x) / (VTXCNTX - 1), float(z) / (VTXCNTZ - 1));
		}
	}

	vertexBuffer->Unlock();

	INDEX*	pIndex = NULL;
	int iTriCnt = 0;
	D3DXVECTOR3	vDest, vSour, vResult;

	indexBuffer->Lock(0, 0, (void**)&pIndex, 0);
	for (int z = 0; z < VTXCNTZ - 1; ++z)
	{
		for (int x = 0; x < VTXCNTX - 1; ++x)
		{
			int iIndex = (z * VTXCNTX) + x;

			//우측 상단
			pIndex[iTriCnt]._1 = VTXCNTX + iIndex;
			pIndex[iTriCnt]._2 = VTXCNTX + 1 + iIndex;
			pIndex[iTriCnt]._3 = iIndex + 1;

			vDest = pVtxTex[pIndex[iTriCnt]._2].position
				- pVtxTex[pIndex[iTriCnt]._1].position;

			vSour = pVtxTex[pIndex[iTriCnt]._3].position
				- pVtxTex[pIndex[iTriCnt]._1].position;
			D3DXVec3Cross(&vResult, &vDest, &vSour);
			D3DXVec3Normalize(&vResult, &vResult);
			pVtxTex[pIndex[iTriCnt]._1].normal += vResult;
			pVtxTex[pIndex[iTriCnt]._2].normal += vResult;
			pVtxTex[pIndex[iTriCnt]._3].normal += vResult;
			++iTriCnt;

			//좌측 하단
			pIndex[iTriCnt]._1 = VTXCNTX + iIndex;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;

			vDest = pVtxTex[pIndex[iTriCnt]._2].position
				- pVtxTex[pIndex[iTriCnt]._1].position;
			vSour = pVtxTex[pIndex[iTriCnt]._3].position
				- pVtxTex[pIndex[iTriCnt]._1].position;
			D3DXVec3Cross(&vResult, &vDest, &vSour);
			D3DXVec3Normalize(&vResult, &vResult);
			pVtxTex[pIndex[iTriCnt]._1].normal += vResult;
			pVtxTex[pIndex[iTriCnt]._2].normal += vResult;
			pVtxTex[pIndex[iTriCnt]._3].normal += vResult;

			++iTriCnt;
		}
	}

	for (int i = 0; i < vertexCnt; ++i){
		D3DXVec3Normalize(&pVtxTex[i].normal, &pVtxTex[i].normal);
	}

	indexBuffer->Unlock();

	// 최소 최대 충돌 지점 설정
	D3DXVECTOR3* vertices = NULL;
	vertexBuffer->Lock(0, 0, (void**)&vertices, NULL);
	D3DXComputeBoundingBox(vertices, vertexCnt, D3DXGetFVFVertexSize(vertexFVF), &min, &max);
	vertexBuffer->Unlock();

	return S_OK;
}

TerrainBuffer::TerrainBuffer()
{
}


TerrainBuffer::~TerrainBuffer()
{
}
