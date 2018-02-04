#include "stdafx.h"
#include "RectangleBuffer.h"

HRESULT RectangleBuffer::Initialize()
{
	vertexSize	= sizeof(VTXTEX);
	vertexCnt	= 4;
	vertexFVF	= VTXFVF_TEXTURE;
	triCnt		= 2;

	indexSize	= sizeof(INDEX);
	indexFormat = D3DFMT_INDEX32;
	if(FAILED(Buffer::Initialize())){
		MSGBOX(L"사각 텍스쳐 버퍼 생성실패");
		return E_FAIL;
	}

	VTXTEX*	pVtxTex = NULL;
	vertexBuffer->Lock(0, 0, (void**)&pVtxTex, 0);
	pVtxTex[0].position		= D3DXVECTOR3(-1.f, 1.f, 0.f);
	pVtxTex[0].texture		= D3DXVECTOR2(0.f, 0.f);

	pVtxTex[1].position		= D3DXVECTOR3(1.f, 1.f, 0.f);
	pVtxTex[1].texture		= D3DXVECTOR2(1.f, 0.f);

	pVtxTex[2].position		= D3DXVECTOR3(1.f, -1.f, 0.f);
	pVtxTex[2].texture		= D3DXVECTOR2(1.f, 1.f);

	pVtxTex[3].position		= D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVtxTex[3].texture		= D3DXVECTOR2(0.f, 1.f);
	vertexBuffer->Unlock();

	INDEX* pIndex = NULL;
	indexBuffer->Lock(0, 0, (void**)&pIndex, 0);
	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;
	indexBuffer->Unlock();

	return S_OK;
}

RectangleBuffer::RectangleBuffer()
{
}


RectangleBuffer::~RectangleBuffer()
{
}
