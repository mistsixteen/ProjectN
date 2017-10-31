#include "stdafx.h"
#include "Buffer.h"


HRESULT Buffer::Initialize()
{
	if (FAILED(GET_SINGLE(DXFramework)->GetDevice()->CreateVertexBuffer(vertexSize * vertexCnt,
		0, vertexFVF, D3DPOOL_MANAGED, &vertexBuffer, NULL)))
	{
		MSGBOX(L"버텍스 버퍼 생성 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(DXFramework)->GetDevice()->CreateIndexBuffer(indexSize * triCnt,
		0, indexFormat, D3DPOOL_MANAGED, &indexBuffer, NULL)))
	{
		MSGBOX(L"인덱스 버퍼 생성 실패");
		return E_FAIL;
	}

	return S_OK;
}

void Buffer::Render()
{
	GET_SINGLE(DXFramework)->GetDevice()->SetStreamSource(0, vertexBuffer, 0, vertexSize);
	GET_SINGLE(DXFramework)->GetDevice()->SetFVF(vertexFVF);
	GET_SINGLE(DXFramework)->GetDevice()->SetIndices(indexBuffer);
	GET_SINGLE(DXFramework)->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCnt, 0, triCnt);
}

void Buffer::Release()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}


void Buffer::CopyVertexColorInfo(VTXCOL * pVtxCol)
{
	VTXCOL* oriColorVtx = NULL;

	vertexBuffer->Lock(0, 0, (void**)&oriColorVtx, NULL);
	memcpy(pVtxCol, oriColorVtx, vertexSize * sizeof(VTXCOL));
	vertexBuffer->Unlock();
}

void Buffer::PasteVertexColorInfo(VTXCOL * pVtxCol)
{
	VTXCOL* oriColorVtx = NULL;

	vertexBuffer->Lock(0, 0, (void**)&oriColorVtx, NULL);
	memcpy(oriColorVtx, pVtxCol, vertexSize * sizeof(VTXCOL));
	vertexBuffer->Unlock();
}

void Buffer::CopyVertexTextureInfo(VTXTEX * pVtxtex)
{
	VTXTEX* oriTextureVtx = NULL;

	vertexBuffer->Lock(0, 0, (void**)&oriTextureVtx, NULL);
	memcpy(pVtxtex, oriTextureVtx, vertexSize * vertexCnt);
	vertexBuffer->Unlock();
}

void Buffer::PasteVertexTextureInfo(VTXTEX * pVtxtex)
{
	VTXTEX* oriTextureVtx = NULL;

	vertexBuffer->Lock(0, 0, (void**)&oriTextureVtx, NULL);
	memcpy(oriTextureVtx, pVtxtex, vertexSize * vertexCnt);
	vertexBuffer->Unlock();
}

void Buffer::CopyIndexInfo(INDEX * index, int & triCnt)
{
	INDEX*	oriIndices = NULL;
	this->triCnt = triCnt;

	indexBuffer->Lock(0, 0, (void**)&oriIndices, 0);
	memcpy(oriIndices, index, triCnt * indexSize);
	indexBuffer->Unlock();
}

void Buffer::PasteIndexInfo(INDEX * index, int triCnt)
{
	INDEX*	oriIndices = NULL;
	this->triCnt = triCnt;

	indexBuffer->Lock(0, 0, (void**)&oriIndices, 0);
	memcpy(index, oriIndices, triCnt * indexSize);
	indexBuffer->Unlock();
}

Buffer::Buffer()
{
}


Buffer::~Buffer()
{
	Release();
}
