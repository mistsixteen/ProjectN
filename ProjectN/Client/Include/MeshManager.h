#pragma once
#include "Define.h"

#include "Mesh.h"
#include "TerrainMesh.h"
#include "BoxMesh.h"

class MeshManager
{
	DECLARE_SINGLETON(MeshManager)
private:
	map<const TCHAR*, Mesh*> meshMap;
public:
	/* 오브젝트 메쉬 추가 */
	HRESULT AddMesh(const TCHAR* mapKey,
					const TCHAR*  meshKey);
	HRESULT	CloneMesh( const TCHAR* mapKey, LPD3DXMESH* ppMesh);
	void Mesh_Render(const TCHAR* mapKey);
	void Release();
public:
	const D3DXVECTOR3* GetMin(const TCHAR*	mapKey);
	const D3DXVECTOR3* GetMax(const TCHAR*	mapKey);
private:
	MeshManager();
public:
	~MeshManager();
};

