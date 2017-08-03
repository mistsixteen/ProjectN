#pragma once
#include "Define.h"

#include "Mesh.h"
#include "TerrainMesh.h"

class MeshManager
{
	DECLARE_SINGLETON(MeshManager)
private:
	map<const TCHAR*, Mesh*> meshMap;
public:
	HRESULT AddMesh(const TCHAR* meshKey,
					MESH meshType,
					const TCHAR* pPath = NULL,
					const TCHAR* pFileName = NULL);
	HRESULT	CloneMesh( const TCHAR* meshKey, LPD3DXMESH* ppMesh);
	void Mesh_Render(const TCHAR* meshKey);
	void Release();
public:
	const D3DXVECTOR3* GetMin(const TCHAR*	meshKey);
	const D3DXVECTOR3* GetMax(const TCHAR*	meshKey);
private:
	MeshManager();
public:
	~MeshManager();
};

