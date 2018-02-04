#include "stdafx.h"
#include "BufferManager.h"


BufferManager::BufferManager()
{
}


BufferManager::~BufferManager()
{
	Release();
}

HRESULT BufferManager::AddBuffer(const TCHAR * mapKey, const TCHAR * bufferKey)
{
	auto iterMap = bufferMap.find(mapKey);
	if (iterMap != bufferMap.end())
		return E_FAIL;

	Buffer* buffer = NULL;
	if (_tcscmp(bufferKey, L"Terrain") == 0) {
		buffer = new TerrainBuffer;
		if (FAILED(buffer->Initialize())) {
			return E_FAIL;
		}
	}
	else if (_tcscmp(bufferKey, L"Box") == 0) {
		buffer = new BoxBuffer;
		if (FAILED(buffer->Initialize())) {
			return E_FAIL;
		}
	}
	else if (_tcscmp(bufferKey, L"Rectangle") == 0) {
		buffer = new RectangleBuffer;
		if (FAILED(buffer->Initialize())) {
			return E_FAIL;
		}
	}

	bufferMap.insert(make_pair(mapKey, buffer));
	return S_OK;
}

void BufferManager::Render(const TCHAR * mapKey)
{
	auto iterMap = bufferMap.find(mapKey);
	if (iterMap == bufferMap.end())
		return;
	iterMap->second->Render();
}

void BufferManager::Release()
{
	for (auto iterMap = bufferMap.begin();
		iterMap != bufferMap.end(); ++iterMap){
		SAFE_DELETE(iterMap->second);
	}
	bufferMap.clear();
}

const D3DXVECTOR3 * BufferManager::GetMin(const TCHAR * mapKey)
{
	auto iterMap = bufferMap.find(mapKey);
	if (iterMap == bufferMap.end())
		return nullptr;
	return iterMap->second->GetMin();
}

const D3DXVECTOR3 * BufferManager::GetMax(const TCHAR * mapKey)
{
	auto iterMap = bufferMap.find(mapKey);
	if (iterMap == bufferMap.end())
		return nullptr;
	return iterMap->second->GetMax();
}

const int & BufferManager::GetVertexCount(const TCHAR * mapKey)
{
	auto iterMap = bufferMap.find(mapKey);
	if (iterMap == bufferMap.end())
		return -1;
	
	return iterMap->second->GetVertexCount();
}

void BufferManager::CopyVertexColorInfo(const TCHAR * mapKey, VTXCOL * pVtxCol)
{
	auto iterMap = bufferMap.find(mapKey);
	if (iterMap == bufferMap.end())
		return;
	return iterMap->second->CopyVertexColorInfo(pVtxCol);
}

void BufferManager::PasteVertexColorInfo(const TCHAR * mapKey, VTXCOL * pVtxCol)
{
	auto iterMap = bufferMap.find(mapKey);
	if (iterMap == bufferMap.end())
		return;
	return iterMap->second->CopyVertexColorInfo(pVtxCol);
}

void BufferManager::CopyVertexTextureInfo(const TCHAR * mapKey, VTXTEX * pVtxtex)
{
	auto iterMap = bufferMap.find(mapKey);
	if (iterMap == bufferMap.end())
		return;
	return iterMap->second->CopyVertexTextureInfo(pVtxtex);
}

void BufferManager::PasteVertexTextureInfo(const TCHAR * mapKey, VTXTEX * pVtxtex)
{
	auto iterMap = bufferMap.find(mapKey);
	if (iterMap == bufferMap.end())
		return;
	return iterMap->second->PasteVertexTextureInfo(pVtxtex);
}

void BufferManager::CopyIndexInfo(const TCHAR * mapKey, INDEX * pIndex, int & triCnt)
{
	auto iterMap = bufferMap.find(mapKey);
	if (iterMap == bufferMap.end())
		return;
	return iterMap->second->CopyIndexInfo(pIndex, triCnt);
}

void BufferManager::PasteIndexInfo(const TCHAR * mapKey, INDEX * pIndex, int triCnt)
{
	auto iterMap = bufferMap.find(mapKey);
	if (iterMap == bufferMap.end())
		return;
	return iterMap->second->PasteIndexInfo(pIndex, triCnt);
}
