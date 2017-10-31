#include "stdafx.h"
#include "BoxBuffer.h"


HRESULT BoxBuffer::Initialize()
{
	vertexCnt = 8;
	vertexSize = sizeof(VTXCUBE);
	vertexFVF = VTXFVF_CUBE;
	indexFormat = D3DFMT_INDEX32;
	indexSize = sizeof(INDEX);
	triCnt = 12;

	if (FAILED(Buffer::Initialize()))
	{
		MSGBOX(L"큐브 버퍼 생성실패");
		return E_FAIL;
	}

	VTXCUBE* pVtxCol = NULL;
	vertexBuffer->Lock(0, 0, (void**)&pVtxCol, 0);

	//앞면
	pVtxCol[0].position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtxCol[0].texture = pVtxCol[0].position;
	pVtxCol[1].position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtxCol[1].texture = pVtxCol[1].position;
	pVtxCol[2].position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtxCol[2].texture = pVtxCol[2].position;
	pVtxCol[3].position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVtxCol[3].texture = pVtxCol[3].position;

	//뒷면
	pVtxCol[4].position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtxCol[4].texture = pVtxCol[4].position;
	pVtxCol[5].position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtxCol[5].texture = pVtxCol[5].position;
	pVtxCol[6].position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVtxCol[6].texture = pVtxCol[6].position;
	pVtxCol[7].position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVtxCol[7].texture = pVtxCol[7].position;

	vertexBuffer->Unlock();


	INDEX* pIndex = NULL;

	indexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	//+x
	pIndex[0]._1 = 1; pIndex[0]._2 = 5; pIndex[0]._3 = 6;
	pIndex[1]._1 = 1; pIndex[1]._2 = 6; pIndex[1]._3 = 2;

	//-x
	pIndex[2]._1 = 4; pIndex[2]._2 = 0; pIndex[2]._3 = 3;
	pIndex[3]._1 = 4; pIndex[3]._2 = 3; pIndex[3]._3 = 7;

	//+y
	pIndex[4]._1 = 4; pIndex[4]._2 = 5; pIndex[4]._3 = 1;
	pIndex[5]._1 = 4; pIndex[5]._2 = 1; pIndex[5]._3 = 0;

	//-y
	pIndex[6]._1 = 3; pIndex[6]._2 = 2; pIndex[6]._3 = 6;
	pIndex[7]._1 = 3; pIndex[7]._2 = 6; pIndex[7]._3 = 7;

	//+z
	pIndex[8]._1 = 7; pIndex[8]._2 = 6; pIndex[8]._3 = 5;
	pIndex[9]._1 = 7; pIndex[9]._2 = 5; pIndex[9]._3 = 4;

	//-z
	pIndex[10]._1 = 0; pIndex[10]._2 = 1; pIndex[10]._3 = 2;
	pIndex[11]._1 = 0; pIndex[11]._2 = 2; pIndex[11]._3 = 3;

	indexBuffer->Unlock();

	return S_OK;
}

BoxBuffer::BoxBuffer()
{
}


BoxBuffer::~BoxBuffer()
{
}
