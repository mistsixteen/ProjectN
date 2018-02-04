#pragma once
#include "Buffer.h"
#include "BoxBuffer.h"
#include "TerrainBuffer.h"
#include "RectangleBuffer.h"

class BufferManager
{
	DECLARE_SINGLETON(BufferManager)
private:
	map<const TCHAR*, Buffer*> bufferMap;
public:
	HRESULT AddBuffer(const TCHAR* mapKey, const TCHAR* bufferKey);
	void Render(const TCHAR* mapKey);
	void Release(); 
public:
		const D3DXVECTOR3* GetMin(const TCHAR*	mapKey);
		const D3DXVECTOR3* GetMax(const TCHAR*	mapKey);
public:
	const int& GetVertexCount(const TCHAR* mapKey);

	void CopyVertexColorInfo(const TCHAR* mapKey, VTXCOL* pVtxCol);
	void PasteVertexColorInfo(const TCHAR* mapKey, VTXCOL* pVtxCol);

	void CopyVertexTextureInfo(const TCHAR* mapKey, VTXTEX* pVtxtex);
	void PasteVertexTextureInfo(const TCHAR* mapKey, VTXTEX* pVtxtex);

	void CopyIndexInfo(const TCHAR* mapKey, INDEX* pIndex, int& triCnt);
	void PasteIndexInfo(const TCHAR* mapKey, INDEX* pIndex, int triCnt);
private:
	BufferManager();
public:
	~BufferManager();
};

