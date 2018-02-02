#pragma once

#include "Mesh.h"
#include "TerrainMesh.h"
#include "BoxMesh.h"
#include "SphereMesh.h"

class MeshManager
{
	DECLARE_SINGLETON(MeshManager)
private:
	map<const TCHAR*, Mesh*> meshMap;
public:
	HRESULT AddMesh(const TCHAR* mapKey,
					const TCHAR*  meshKey);
	HRESULT	CloneMesh( const TCHAR* mapKey, LPD3DXMESH* ppMesh);
	void Render(const TCHAR* mapKey);
	void Release();
public:
	const D3DXVECTOR3* GetMin(const TCHAR*	mapKey);
	const D3DXVECTOR3* GetMax(const TCHAR*	mapKey);
public:
	void CopyVertexTextureInfo(const TCHAR*	mapKey, VTXTEX * vtxTex);
	void PasteVertexTextureInfo(const TCHAR* mapKey, VTXTEX * vtxTex);
private:
	MeshManager();
public:
	~MeshManager();
};

