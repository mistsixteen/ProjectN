#pragma once
#include "Define.h"
#include "DXFramework.h"

class Buffer
{
protected:
	LPDIRECT3DVERTEXBUFFER9		vertexBuffer;
	int							vertexSize;
	int							vertexCnt;
	DWORD						vertexFVF;
protected:
	LPDIRECT3DINDEXBUFFER9		indexBuffer;
	int							triCnt;
	int							indexSize;
	D3DFORMAT					indexFormat;
protected:
	D3DXVECTOR3					min;
	D3DXVECTOR3					max;
public:
	virtual HRESULT Initialize();
	void Render();
	void Release();
public:
	const int& GetVertexCount(void) const { return vertexCnt; }
	const D3DXVECTOR3* GetMin(void) const { return &min; };
	const D3DXVECTOR3* GetMax(void) const { return &max; };;

	void CopyVertexColorInfo(VTXCOL* pVtxCol);
	void PasteVertexColorInfo(VTXCOL* pVtxCol);

	void CopyVertexTextureInfo(VTXTEX* pVtxtex);
	void PasteVertexTextureInfo(VTXTEX* pVtxtex);

	void CopyIndexInfo(INDEX* pIndex, int& triCnt);
	void PasteIndexInfo(INDEX* pIndex, int triCnt);
public:
	Buffer();
	virtual ~Buffer();
};

